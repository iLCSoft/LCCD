#ifndef IConditionsChangeListener_h
#define IConditionsChangeListener_h

#include "lcio.h"
#include "EVENT/LCCollection.h"

namespace lccd {
  
  class ConditionsHandlerBase;
  class IConditionsHandler;

/** Simple interface that allows notification of implementation classes if
 *  a conditions data set has changed, or the implementation classes are 
 *  registered with a class inheriting from ConditionsHandlerBase.
 *  
 *  Note: friend class ConditionsHandlerBase
 *
 *  @author F.Gaede, S.Aplin,  DESY
 *  @version $Id: IConditionsChangeListener.hh,v 1.2 2007-04-27 13:21:31 gaede Exp $
 */

class IConditionsChangeListener {
  
public:

  virtual ~IConditionsChangeListener() {} 

  friend class ConditionsHandlerBase;

protected:

  /** The callback - use updated conditions data collection as needed, e.g.
   *  repopulate a calibration map. Implementors can access the collection name 
   *  through the collection parameter "CollectionName": <br>
   *  <pre>  
   *    std::string colName = col->getParameters().getStringVal("CollectionName") ;
   *  </pre>
   *  if needed. 
   */ 
  virtual void conditionsChanged( lcio::LCCollection* col ) = 0 ;


  /** Callback called by handler when registered, should not be called directly 
   */ 
  virtual void registeredWithHandler( IConditionsHandler* ch )  { _handlerList.push_back( ch );  } ;


  /** Callback called by handler when deregistered, should not be called directly
   */ 
  virtual void deRegisteredWithHandler( IConditionsHandler* ch ) { _handlerList.remove( ch ); } ;

  /** Get handler list 
   */ 
  const std::list<IConditionsHandler*>& handlerList() { return _handlerList ; } ;

private:  
  /** list of handlers with which the listener is currently registered 
   */
  
  std::list<IConditionsHandler*> _handlerList;
  
};

}  //end namespace

#endif // IConditionsChangeListener_h
