#ifndef DBCondHandler_h
#define DBCondHandler_h

#include "lccd.h"
#include "ConditionsHandlerBase.hh"

#include "lcio.h"
#include "Exceptions.h"


namespace lccd {

  class DBInterface ;
  
  /** Implementation of ConditionsHandlerBase that reads the conditions data 
   *  from the database. On every call to update() it checks whether the provided
   *  time stamp exceeds the validity of the current conditions data. 
   *  If so, new constants are retrieved from the data base.
   * 
   *  In the case where no collection is available for a given time range, the default collection will be set 
   *  as the present collection, and will be the one returned to the registered IConditionsChangeListener. The 
   *  previously last valid collection i.e. not the Default Collection, will be held and can be accessed via the 
   *  method lastValidCollection().
   *
   *  The Default Collection should be set via the method registerDefaultCollection( lcio::LCCollection* col)
   *  In the case of no collection found and no Default Collection set an exception will be raised. Only one 
   *  Default Collection may be registered per Conditions Handler. Trying to register additional Default Collections 
   *  will cause an lcio::Exception( std::string ) to be thrown. 
   *
   *  The default collection may be accessed via the method defaultCollection(), and if so desired this method 
   *  should be used by the IConditionsChangeListener to verify if this is the collection provided via the 
   *  call-back method conditionsChanged.
   *
   *  Upon registering an IConditionsChangeListener with the Conditions Handler the call-back method 
   *  registeredWithHandler( IConditionsHandler* ch ) will be called. Likewise calling removeChangeListener( IConditionsChangeListener* cl) 
   *  will call the call-back method deRegisteredWithHandler( IConditionsHandler* ch )
   * 
   *  @author F.Gaede, S.Aplin,  DESY
   *  @version $Id: DBCondHandler.hh,v 1.2 2007-04-27 13:21:31 gaede Exp $
   */
  
  class DBCondHandler : public ConditionsHandlerBase {
    
  public:
    
    /** Default constructor. You have to specify the following parameters:
     * 
     * @param dbInit: initialization string for the database, e.g. "localhost:lccd_test:calvin:hobbes" 
     * @param folder:  name of the database folder
     * @param name:  of this conditions data set - used, e.g. as collection name 
     * in update(LCEvent* evt)
     * @param tag: tag to be used for queries, HEAD if omitted 
     */
    DBCondHandler(const std::string& dbInit,  const std::string& folder,
		  const std::string& name,  const std::string& tag="" ) ;
    
    
    virtual ~DBCondHandler() ;
    
    
    /** Retrieves the conditions data from the data base if time stamp 
     *  exceeds current validity time range.<br>
     *  Notifies all registered listeners if conditions have changed.
     */
    virtual void update( LCCDTimeStamp timeStamp ) ;
//     virtual void update( LCCDTimeStamp timeStamp )  throw (lcio::Exception)  ;


    /** Register a pointer to a default collection which will be passed to the
     *  IConditionsChangeListener if no data can be found for a given time stamp
     */
    virtual void registerDefaultCollection( lcio::LCCollection* col);    
    
    
    /** The default collection registered with the handler 
     */
    virtual lcio::LCCollection* defaultCollection() { return _defaultCollection; } ;    
    
    
   /** The last valid collection held by the handler 
    */
    virtual lcio::LCCollection* lastValidCollection()  { return _lastValidCollection; } ;


  protected :
    
    DBCondHandler() {} 
    
    lccd::DBInterface* _db ;

    // ----  data members ------
    std::string _dbInit ;
    std::string _folder ;
    std::string _tag ;
    
    /** The registered default collection */
    lcio::LCCollection* _defaultCollection ;
    
    /** The last valid collection of conditions data.*/
    lcio::LCCollection* _lastValidCollection ;


  };
}  //end namespace

#endif // DBCondHandler_h
