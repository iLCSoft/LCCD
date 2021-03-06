#include "lccd/LCConditionsMgr.hh"

#include "lcio.h"
#include <iostream>

namespace lccd{

  LCConditionsMgr* LCConditionsMgr::_me  = 0 ;
  

  LCConditionsMgr* LCConditionsMgr::instance(){
    if( _me == 0) 
      _me = new LCConditionsMgr ;
    return _me ;
  }
  
  LCConditionsMgr::LCConditionsMgr(){
  }
  

  void LCConditionsMgr::registerChangeListener( IConditionsChangeListener* cl, const std::string&  name) {

    HandlerMap::iterator it =  _map.find( name ) ;

    if( it != _map.end() ) {
      it->second->registerChangeListener( cl ) ;
    }      
    else
      throw lccd::LCCDException(" LCConditionsMgr::registerChangeListener: no handler "
			    " for name: " + name ) ; 
  }
  
  void LCConditionsMgr::removeHandler(const std::string&  name ) {

    HandlerMap::iterator it =  _map.find( name ) ;

    if( it != _map.end() ) {
      delete it->second ;
      _map.erase( it )  ;
    }
  }
  
  void LCConditionsMgr::updateEvent( lcio::LCEvent* evt ) {
    
    for( HandlerMap::iterator it =  _map.begin() ; it != _map.end() ; it++ ){
      
      it->second->updateEvent( evt ) ;

    }
  }
  
  void LCConditionsMgr::update( LCCDTimeStamp timestamp ) {

    for( HandlerMap::iterator it =  _map.begin() ; it != _map.end() ; it++ ){
      
      it->second->update( timestamp ) ;

    }

  }

  IConditionsHandler* LCConditionsMgr::getHandler( const std::string&  name ) {

    HandlerMap::iterator it =  _map.find( name ) ;

    if( it != _map.end() ) 
      return it->second ;
    else 
      return 0 ;  
  }
  

  
  void LCConditionsMgr::registerHandler( const std::string&  name, IConditionsHandler* handler) {
    
    HandlerMap::iterator it =  _map.find( name ) ;

    if( it != _map.end() ) {

      throw lccd::LCCDException(" LCConditionsMgr::registerHandler: a handler already exists"
			    " for name " + name ) ; 
    }

    _map[ name ] = handler  ;
  }
  

  void LCConditionsMgr::clear() {

    for( HandlerMap::iterator it =  _map.begin() ; it != _map.end() ; it++ ){

      delete it->second ;
    }

    _map.clear() ;
  }

}

