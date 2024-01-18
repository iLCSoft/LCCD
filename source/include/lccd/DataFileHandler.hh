#ifndef DataFileHandler_h
#define DataFileHandler_h

//#include "lccd.h"
#include "ConditionsHandlerBase.hh"

namespace lccd {
  
  /** Implementation of ConditionsHandlerBase that handles conditions data 
   *  that is stored in a data LCIO file. The condition collection is kept 
   *  until an event is presented that has a new version of the particular collection.
   *  The time stamps are ignored and the validity of the conditions data is determined 
   *  by the sequence with which they appear in the data stream.<br>
   *  This can for example be used in cases where the DAQ system writes slow control data
   *  into the same data stream as the raw data.
   *
   *  Upon registering an IConditionsChangeListener with this Conditions Handler the call-back method 
   *  registeredWithHandler( IConditionsHandler* ch ) will be called. Likewise calling removeChangeListener( IConditionsChangeListener* cl) 
   *  will call the call-back method deRegisteredWithHandler( IConditionsHandler* ch )

   *  Note: The use of default Collections is not implemented for this Class.
   * 
   *  @author F.Gaede, S.Aplin,  DESY
   *  @version $Id: DataFileHandler.hh,v 1.4 2007-04-27 13:21:31 gaede Exp $
   */
  
  class DataFileHandler : public ConditionsHandlerBase {
    
  public:
    
    DataFileHandler( const DataFileHandler& ) = delete ;
    DataFileHandler& operator=( const DataFileHandler& ) = delete ;


    /** Default constructor. 
     *  @param name:  of this conditions data set - used as collection name 
     */
    DataFileHandler( const std::string& name ) ;

    
    virtual ~DataFileHandler() ;
    
    /** Checks wether the collection is present in the event, if so it keeps it
     *  until a new version is found otherwise it adds the current collection to the event.<br> 
     *  The events time stamp is ignored.<br>
     *  Notifies all registered listeners.
     */
    virtual void updateEvent( lcio::LCEvent* evt ) ;
    

    /* The timestamp is ignored, use update(LCEvent *evt) instead. 
     */
    virtual void update( LCCDTimeStamp /*timestamp*/ ) {} 
    

    /** Register a pointer to a default collection which will be passed to the
     *  IConditionsChangeListener if no data can be found for a given time stamp
     * @warning Not implemented, use will cause exception to be thrown. 
     */
    virtual void registerDefaultCollection( lcio::LCCollection* col);


    /** The default collection registered with the handler 
     * @warning Not implemented, use will cause exception to be thrown.
     */   
    virtual lcio::LCCollection* defaultCollection() ;    
    
    
    /** The last valid collection held by the handler 
     * @warning Not implemented, use will cause exception to be thrown.
     */
    virtual lcio::LCCollection* lastValidCollection()  ;


  protected :
    
    DataFileHandler() {} 

  };
}  //end namespace

#endif // DataFileHandler_h
