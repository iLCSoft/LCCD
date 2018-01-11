#include "lccd/SimpleFileHandler.hh"

#include "lcio.h"

#include "IO/LCReader.h"

#include <iostream>
#include <sstream>

using namespace lcio ;

namespace lccd {


  SimpleFileHandler::SimpleFileHandler(const std::string& fileName, 
				       const std::string& name,
				       const std::string& inputCollection ) : 
    ConditionsHandlerBase(name) ,
    _fileName( fileName ) ,
    _inputCollection( inputCollection ),
    _firstCall( true )  {
    
    // use the same name for the input collection
    // and the data set name 
    if( _inputCollection.size() == 0 ) 
      _inputCollection = name ;

   _col = NULL ;

  }
  
  SimpleFileHandler::~SimpleFileHandler() {
//     std::cout << " in SimpleFileHandler::~SimpleFileHandler() " << std::endl ;
    delete _col ;
  }
  
 void SimpleFileHandler::registerDefaultCollection( lcio::LCCollection*){

    std::stringstream mess ;
    mess << "SimpleFileHandler::registerDefaultCollection is not implemented for SimpleFileCondHandler. Name of handler: "  << this->name() << std::ends ;
    throw lccd::MemberNotImplementedException( mess.str() ) ; 
    
  }


  lcio::LCCollection* SimpleFileHandler::defaultCollection(){

    std::stringstream mess ;
    mess << "SimpleFileHandler::defaultCollection is not implemented for SimpleFileCondHandler. Name of handler: " <<  this->name() << std::ends ;
    throw lccd::MemberNotImplementedException( mess.str() ) ; 
    
  }


  lcio::LCCollection* SimpleFileHandler::lastValidCollection(){

    std::stringstream mess ;
    mess << "SimpleFileHandler::lastValidCollection is not implemented for SimpleFileCondHandler. Name of handler: " <<  this->name() << std::ends ;
    throw lccd::MemberNotImplementedException( mess.str() ) ; 
    
  }

  
  void SimpleFileHandler::update( LCCDTimeStamp ) {
    
    if( _firstCall ) {

      _firstCall = false ;
      

//       std::cout << "SimpleFileHandler::update: reading constants from file for time stamp : " 
// 		<< timestamp << std::endl ;
      
      
      LCReader* lcReader = LCFactory::getInstance()->createLCReader() ;
      
      lcReader->open( _fileName ) ;
      
      LCEvent* evt = lcReader->readNextEvent() ;
      
      if( evt == 0 ) 
	throw lccd::DataNotAvailableException("SimpleFileHandler::update: No event found in file: "+_fileName) ;
      
      if( _col != 0 ) 
	delete _col ;

      _col = evt->takeCollection( _inputCollection ) ;
      
      // set infinite validity range
      _validSince =  LCCDMinusInf ;
      _validTill = LCCDPlusInf ;
      
      _col->parameters().setValue("CollectionName", this->name() ) ;
      notifyListeners() ;
      
      lcReader->close() ;
      delete lcReader ;
      
    }
  }
  
}
