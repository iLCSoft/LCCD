#include "lccd/ConditionsHandlerBase.hh"

#include <algorithm>
#include <functional> 

using namespace lcio ;

namespace lccd {


  void ConditionsHandlerBase::updateEvent( LCEvent* evt ) {
    
    update( LCCDTimeStamp( evt->getTimeStamp() ) ) ;
    
    evt->addCollection( currentCollection() , name()  ) ;

    // take away ownership of collection
    evt->takeCollection( name() ) ;

  }
  
  
  void ConditionsHandlerBase::registerChangeListener( IConditionsChangeListener* cl ) {
    
    _changeListeners.push_back( cl ) ;
  }
  

  void ConditionsHandlerBase::removeChangeListener( IConditionsChangeListener* cl){
    
    // remove all occurences of cl :
    // remove only puts things to remove at the end and we need to erase them
    _changeListeners.erase ( std::remove( _changeListeners.begin() , 
					  _changeListeners.end() , 
					  cl ) ,
			     _changeListeners.end() ) ;
  }
  
  
  void ConditionsHandlerBase::notifyListeners() {
    
    
    std::for_each( _changeListeners.begin() , 
		   _changeListeners.end() , 
		   std::bind2nd( std::mem_fun
                      ( &IConditionsChangeListener::conditionsChanged  ) , _col ) 
		   ) ;

    // or in old fashioned way - simply loop ....      
    //     typedef std::vector<IConditionsChangeListener*>::iterator IT ;
    //     for(IT iter = _changeListeners.begin() ; 
    // 	       iter != _changeListeners.end() ; iter ++){
    //       (*iter)->conditionsChanged( _col ) ;
    //     }

  }

};
