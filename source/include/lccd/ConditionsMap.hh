#ifndef ConditionsMap_h
#define ConditionsMap_h

#include "lcio.h"
#include "EVENT/LCCollection.h"

#include "lccd/IConditionsChangeListener.hh"

#include <map>

namespace lccd {
  

  /** Teplate class for maps of conditions data.
   * 
   *  @author F.Gaede, DESY
   *  @version $Id: ConditionsMap.hh,v 1.1 2005-02-11 12:31:52 gaede Exp $
   */
  template <class KEY, class LCCONDOBJECT>
  class ConditionsMap : public lccd::IConditionsChangeListener {

  public:
    
    typedef typename std::map< KEY, LCCONDOBJECT >::iterator MapIter ;

    virtual ~ConditionsMap() {} 


    /** The map.*/
    const std::map< KEY, LCCONDOBJECT> & map() { return _map ; }
    
    
    /** Repopulate the map with new conditions data.
     */ 
    void conditionsChanged( lcio::LCCollection* col ) {
      
      _map.clear() ;
      
      // add all elements in the collection to the map
      for( int i=0; i< col->getNumberOfElements() ; i++ ){
	
	LCCONDOBJECT obj( col->getElementAt( i ) ) ;
	
	// FIXME: cannot assume obj.getCellID() exists -> member function pointer in c'tor ...
	_map.insert( std::make_pair( obj.getCellID() , obj)  ) ;

      }

      print( std::cout ) ;
    }

    void print(  std::ostream& os ) {
      
      os << "ConditionsMap"  
//  	 << typeid(KEY).name() << "," << typeid( LCCONDOBJECT ) << ">" 
	 << std::endl ;
      
      //   for( std::map< KEY, LCCONDOBJECT >::iterator it = _map.begin() ;
      for( MapIter it = _map.begin() ;
	   it != _map.end() ;
	   it++ ){
	
	os << "  key: " <<  it->first  << " - " << it->second.getGain() << std::endl ;
      }
      
    }
    

  protected:
    
    std::map< KEY, LCCONDOBJECT > _map ;

};

} ; //end namespace

#endif // ConditionsMap_h
