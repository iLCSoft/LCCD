#ifdef USE_CONDDB

#include "lccd/DBCondHandler.hh"
#include "lccd/DBInterface.hh"

#include "lcio.h"

#include <iostream>
#include <sstream>

using namespace lcio ;

namespace lccd {



  DBCondHandler::DBCondHandler(const std::string& dbInit,  const std::string& folder,
			       const std::string& name,  const std::string& tag  ) :
    
    ConditionsHandlerBase(name) ,
    _dbInit( dbInit ) ,
    _folder( folder ) ,
    _tag( tag )  {
    
    
    _db = new lccd::DBInterface( _dbInit , _folder ) ;
    _col = 0 ;
  }
  

  DBCondHandler::~DBCondHandler() {
    //     std::cout << " in DBCondHandler::~DBCondHandler() " << std::endl ;
    delete _col ;
    delete _db ;
  }
  
  
  //  void DBCondHandler::update( LCCDTimeStamp timeStamp ) throw (lcio::Exception) {
  void DBCondHandler::update( LCCDTimeStamp timeStamp )  {

    if( timeStamp < _validSince  || timeStamp >= _validTill ) { 
      
      
      //       std::cout << "DBCondHandler::update: reading constants from db for time stamp : " 
      // 		<< timeStamp << std::endl ;
            
      LCCollection* col = _db->findCollection( timeStamp , _validSince,  _validTill,  _tag ) ;

      col = _db->findCollection( timeStamp , _validSince,  _validTill,  _tag ) ;
      
      if( col == 0 ) {
	
	std::stringstream mess ;
	mess << "DBCondHandler::update: no collection found for time stamp: " 
	     << timeStamp << " and name " << name() << std::ends ;
	//	throw lcio::Exception( mess.str() ) ;
	std::cout << mess << std::endl;
	
      }

      if( _col != 0 )  delete _col ;
      
      _col = col;
      
      notifyListeners() ;
      
    }
  }
  
}

#endif // USE_CONDDB
