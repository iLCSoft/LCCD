#include "lccd/DataFileHandler.hh"

#include "lcio.h"

#include "IO/LCReader.h"

#include <iostream>
#include <sstream>

using namespace lcio ;

namespace lccd {


  DataFileHandler::DataFileHandler(const std::string& name ) : 
    ConditionsHandlerBase(name) {
    
    _col = 0 ;
  }
  
  DataFileHandler::~DataFileHandler() {

    delete _col ;
  }
  
  void DataFileHandler::registerDefaultCollection( lcio::LCCollection* col){
    
    std::stringstream mess ;
    mess << "DataFileHandler::registerDefaultCollection is not implemented for DataFileCondHandler. Name of handler: "  << this->name() << std::ends ;
    throw lcio::Exception( mess.str() ) ; 
    
  }
  
  lcio::LCCollection* DataFileHandler::defaultCollection(){
    
    std::stringstream mess ;
    mess << "DataFileHandler::defaultCollection is not implemented for DataFileCondHandler. Name of handler: " <<  this->name() << std::ends ;
    throw lcio::Exception( mess.str() ) ; 
    
  }
  
  lcio::LCCollection* DataFileHandler::lastValidCollection(){
    
    std::stringstream mess ;
    mess << "DataFileHandler::lastValidCollection is not implemented for DataFileCondHandler. Name of handler: " <<  this->name() << std::ends ;
    throw lcio::Exception( mess.str() ) ; 
    
  }
  
  
  void DataFileHandler::updateEvent( LCEvent* evt ) {
    
    
    try{

       LCCollection* col = evt->takeCollection( name() ) ;

       if( _col != 0 )  delete _col ;
       
       _col = col ;

       _validSince =  evt->getTimeStamp()  ;
       _validTill = LCCDPlusInf ;

       _col->parameters().setValue("CollectionName", this->name() ) ;
       notifyListeners() ;
    }
    catch( DataNotAvailableException ) { // no new data -> simply add current collection

      evt->addCollection( currentCollection() , name()  ) ;
      evt->takeCollection( name() ) ;

    }

  }
  
}
