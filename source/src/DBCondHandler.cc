
#include "lccd/DBCondHandler.hh"
#include "lccd/DBInterface.hh"

#include "lcio.h"
#include <IMPL/LCCollectionVec.h>
#include <EVENT/LCGenericObject.h>
#include "UTIL/LCTime.h"

#include <iostream>
#include <sstream>

using namespace lcio ;

namespace lccd {



#ifdef USE_CONDDB

  DBCondHandler::DBCondHandler(const std::string& dbInit,  const std::string& folder,
			       const std::string& name,  const std::string& tag  ) :
    
    ConditionsHandlerBase(name) ,
    _dbInit( dbInit ) ,
    _folder( folder ) ,
    _tag( tag )  {
    
    
    _db = new lccd::DBInterface( _dbInit , _folder ) ;
    _col = NULL ;
    _defaultCollection = NULL;
    _lastValidCollection = NULL;
}
  

  DBCondHandler::~DBCondHandler() {
    //     std::cout << " in DBCondHandler::~DBCondHandler() " << std::endl ;
    if( _col != _defaultCollection ) delete _col ;
    delete _lastValidCollection;
    delete _db ;
  }
  
 
  void DBCondHandler::registerDefaultCollection( lcio::LCCollection* col){
    if( _defaultCollection == 0 ) {
      _defaultCollection = col;
      //std::cout << "DBCondHandler::registerDefaultCollection registering Default collection for CondHandler: "  << this->name() << "  " << _defaultCollection << std::endl ;
    } else {
      std::stringstream mess ;
      mess << "DBCondHandler::registerDefaultCollection Default collection already set for CondHandler: "  << this->name() << "  " << _defaultCollection << std::ends ;
      throw lccd::LCCDException( mess.str() ) ; 
    }
  }

 
  void DBCondHandler::update( LCCDTimeStamp timeStamp )  {

    if( timeStamp < _validSince  || timeStamp >= _validTill ) { 
      
      
      //std::cout << "DBCondHandler::update: reading constants from db for time stamp : " 
      // 		<< timeStamp << std::endl ;
            
      LCCollection* col = _db->findCollection( timeStamp , _validSince,  _validTill,  _tag ) ;

      if( col == 0 ) {

	// no valid collection is available
//	// record this time as being the start of validity of NULL return
//	_validSince = timeStamp ;

	// std::cout << "DBCondHandler:: no valid collection is available" << std::endl ;

	// try to get next valid collection 
	LCCDTimeStamp nextObectsValidSince = LCCDPlusInf ;
	LCCDTimeStamp nextObectsValidTill = LCCDMinusInf ;
	LCCollection* nextCol =  _db->findNextValidCollection( timeStamp , nextObectsValidSince, nextObectsValidTill,  _tag ) ;

	if( nextCol != 0 ) { // object found so set the end of validity to the start of validity of the object found

	  //std::cout << "DBCondHandler:: found valid collection starting at: " << nextObectsValidSince << std::endl ;
	  _validTill = nextObectsValidSince ;

	} else { // as no more objects exist for this point in time set the end of validity to +inf

	  //std::cout << "DBCondHandler:: did not find a valid collection in the future" << std::endl ;
	  _validTill = LCCDPlusInf ;
	  //	  std::cout << "DBCondHandler::update: Warning: No further collection avialible from this point in time on: time stamp: " 
	  //		    << timeStamp << " and name " << name() << std::endl;
	} 

	// try to get last valid collection 
	LCCDTimeStamp lastObectsValidSince = LCCDPlusInf ;
	LCCDTimeStamp lastObectsValidTill = LCCDMinusInf ;
	LCCollection* lastCol =  _db->findLastValidCollection( timeStamp , lastObectsValidSince, lastObectsValidTill,  _tag ) ;

	if( lastCol != 0 ) { // ojbect found - so set the beginning of validity to the end of the validity of the object found

	  //std::cout << "DBCondHandler:: found valid collection ending at: " << lastObectsValidTill << std::endl ;
	  _validSince = lastObectsValidTill ; 

	} else { // as no objects exist before this point in time -- set the begining of validity to -inf
	  //std::cout << "DBCondHandler:: did not find a valid collection in the past" << std::endl ;
	  _validSince =  LCCDMinusInf;
	  //	  std::cout << "DBCondHandler::update: Warning: No further collection avialible before this point in time: time stamp: " 
	  //		    << timeStamp << " and name " << name() << std::endl;
	} 


	std::stringstream mess ;
	mess << "DBCondHandler::update: No default collection set for Conditions Hander: " <<  this->name() << std::ends ;
	
	// here don't throw exception - but send the default collection
	if (_defaultCollection) {
	  //std::cout << "DBCondHandler:: default collection being used for Conditions Hander: " << this->name() << std::endl;
	  col = _defaultCollection; }
	else {
	  throw lccd::LCCDException( mess.str() ) ;
	}
	
      }

      if( _col != _defaultCollection )  {
	delete _lastValidCollection ;
	_lastValidCollection = _col;
      }
      
      _col = col;

      if( _col!=_defaultCollection ) 
	{
	  _col->parameters().setValue("CollectionName", this->name() ) ;
	}
      else 
	{

	  lcio::StringVec strVec ;
	  
	  std::stringstream sinceStr ;
	  sinceStr << _validSince ;
	  strVec.push_back(  sinceStr.str() ) ;
	  strVec.push_back( lcio::LCTime( _validSince ).getDateString() ) ;
	  _col->parameters().setValues( lccd::DBSINCE ,  strVec ) ;
	  strVec.clear() ;
	  
	  std::stringstream tillStr ;
	  tillStr << _validTill ;
	  strVec.push_back(  tillStr.str() ) ;
	  strVec.push_back( lcio::LCTime( _validTill ).getDateString() ) ;
	  _col->parameters().setValues( lccd::DBTILL ,  strVec ) ;
	  strVec.clear() ;
	  	  
	  std::stringstream nowStr ;
	  lcio::LCTime now ;
	  nowStr << now.timeStamp() ;
	  strVec.push_back(  nowStr.str() ) ;
	  strVec.push_back( now.getDateString() ) ;
	  _col->parameters().setValues( lccd::DBQUERYTIME ,  strVec ) ;
	  strVec.clear() ;

	}      
      notifyListeners() ;
    }
  }
  



#else // not linked with CondDBMysql ========================================================================

  DBCondHandler::DBCondHandler(const std::string& dbInit,  const std::string& folder,
			       const std::string& name,  const std::string& tag  ) {

    throw lccd::DatabaseException("DBCondHandler: cannot instantiate - not linked with CondDBMysql ...." ) ; 
  }

  DBCondHandler::~DBCondHandler() {}
  void DBCondHandler::registerDefaultCollection( lcio::LCCollection* col){}

  void DBCondHandler::update( LCCDTimeStamp timeStamp )  {}


#endif // USE_CONDDB

}
