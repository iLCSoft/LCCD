#include "lccd/DBFileHandler.hh"

#include "lcio.h"

#include "IO/LCReader.h"
#include "EVENT/LCRunHeader.h"
#include "EVENT/LCParameters.h"
#include "UTIL/LCTime.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>

using namespace lcio ;

namespace lccd {


  /** Helper predicate to find validity ranges */
  class contains_timestamp : public std::unary_function<ValidityInterval,bool>{
  public:
    contains_timestamp(LCCDTimeStamp  t) : _t( t) {}  
    bool operator()(const ValidityInterval& v) {
      return ( v.first <= _t && v.second > _t  ) ; 
    }
  protected:
    LCCDTimeStamp  _t ;
  };
  

  DBFileHandler::DBFileHandler(const std::string& fileName, 
				       const std::string& name,
				       const std::string& inputCollection ) : 
    ConditionsHandlerBase(name) ,
    _fileName( fileName ) ,
    _inputCollection( inputCollection ) {
    
    // use the same name for the input collection
    // and the data set name 
    if( _inputCollection.size() == 0 ) 
      _inputCollection = name ;

    _col = NULL;
    _defaultCollection = NULL;
    _lastValidCollection = NULL;
    init() ;

  }
  
  DBFileHandler::~DBFileHandler() {
    if( _col != _defaultCollection ) delete _col ;
    delete _lastValidCollection;
    _lcReader->close() ;
    delete _lcReader ;
  }
  
  void DBFileHandler::registerDefaultCollection( lcio::LCCollection* col){
    if( _defaultCollection == 0 ) {
      _defaultCollection = col;
      //std::cout << "DBFileHandler::registerDefaultCollection registering Default collection for CondHandler: "  << this->name() << "  " << _defaultCollection << std::endl ;
    } else {
      std::stringstream mess ;
      mess << "DBFileHandler::registerDefaultCollection Default collection already set for CondHandler: "  << this->name() << "  " << _defaultCollection << std::ends ;
      throw lcio::Exception( mess.str() ) ; 
    }
  }

  void DBFileHandler::update( LCCDTimeStamp timestamp ) {
    
    //std::cout << "DBFileHandler::update: reading constants from file for time stamp : " 
    //	      << timestamp << std::endl ;

    static bool firstEvent = true ;

    if( timestamp < _validSince  || timestamp >= _validTill ) { // true for first call  ! 

      int evtNum = 0;
      LCCollection* col = 0;

      if( timestamp < _validSince && ! firstEvent ) { 
	
	std::cout << " DBFileHandler::update: events not ordered w.r.t. time stamps \n"
		  << "    -> need to close and reopen file !" << std::endl ; 

	_lcReader->close() ;
	_lcReader->open( _fileName ) ;
      }


      try{

	evtNum = findEventNumber( timestamp ) ;
	LCEvent* evt = _lcReader->readEvent( 0 , evtNum ) ;
	
	col = evt->takeCollection( _inputCollection ) ;
	// set current validity range
	_validSince =  _valVec[ evtNum ].first  ;
	_validTill = _valVec[ evtNum ].second  ;    
	//std::cout << "DBFileHandler::update: setting validity range:" << _validSince << " " << _validTill << std::endl ;		
	
      } catch(Exception) {
	
	//std::cout << "DBFileHandler::update: evtNum not found" << std::endl ;	
	
	if (_defaultCollection) {
	  col = _defaultCollection; 
	  // set the valid's here to only this timestamp, this ensures that a new search will be made for any other time stamp
	  _validSince = timestamp;
	  _validTill = timestamp + 1LL ; // + 1 nsec
	  //std::cout << "DBFileHandler:: default collection being used for Conditions Hander: " << this->name() << std::endl;
	} 
	else {
	  std::stringstream mess ;
	  mess << "DBFileHandler::update: No default collection set for Conditions Hander:" <<  this->name() << std::ends ;
	  throw lcio::Exception( mess.str() ) ;
	}
      }
      
      if( _col != _defaultCollection ){	  
	delete _lastValidCollection ;
	_lastValidCollection = _col ;
      }

      _col = col; 

      if( _col!=_defaultCollection ) _col->parameters().setValue("CollectionName", this->name() ) ;

      notifyListeners() ;
      
      firstEvent = false ; 

    }

  }
  
  void  DBFileHandler::init() {
    
    _lcReader = LCFactory::getInstance()->createLCReader( LCReader::directAccess ) ;
    
    _lcReader->open( _fileName ) ;
    
    LCRunHeader* rHdr = _lcReader->readNextRunHeader() ;
    
    StringVec sinceVec ;
    rHdr->parameters().getStringVals( "DBSince" , sinceVec) ;
    StringVec tillVec ;
    rHdr->parameters().getStringVals( "DBTill" , tillVec ) ;

    if( sinceVec.size() != tillVec.size()   || sinceVec.size() == 0 ) 
      throw Exception(" Wrong format of validity time intervall"
		      " specification in DBSince and DBTill " ) ;
    

    // fill the vector with validity time intervalls 
    int nEvt = sinceVec.size() ;
    for( int i=0 ; i < nEvt ; i++) {
      
      LCCDTimeStamp since = std::atoll( sinceVec[i].c_str() ) ; 
      LCCDTimeStamp till  = std::atoll(  tillVec[i].c_str()  ) ; 
      
      _valVec.push_back(   std::make_pair( since, till )  ) ;
    }
  }


  int DBFileHandler::findEventNumber( LCCDTimeStamp timestamp ) {


    contains_timestamp contains( timestamp ) ;

    int evtNum = -1 ;

    // This could probably be made more efficient - good for now ...
    int nVal = _valVec.size() ;
    for( int i=0 ; i< nVal ; i++ ){

      if ( contains( _valVec[i] ) ){
	evtNum = i ;
	break ;
      }  
    }
    if( evtNum < 0 ) {

      UTIL::LCTime ts(timestamp);

      std::stringstream err ;
      err << "DBFileHandler::findEventNumber: no conditions data in file for time stamp: " 
	  << ts.getDateString() << " (" << timestamp << ") in file " << _fileName;

      throw Exception( err.str() ) ;
    }

    return evtNum ;
     
  }

}
