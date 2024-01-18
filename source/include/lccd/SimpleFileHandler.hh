#ifndef SimpleFileHandler_h
#define SimpleFileHandler_h

//#include "lccd.h"
#include "ConditionsHandlerBase.hh"

namespace lccd {
  
  /** Implementation of ConditionsHandlerBase that reads the conditions data 
   *  from an LCIO file. The conditions data has to be in the first event.
   *  The data will get an infinite validity time intervall assigned.<br>
   *  This can be used, e.g. to read a set of calibration constants that 
   *  will be used unaltered throughout the job.<br>
   *
   *  Upon registering an IConditionsChangeListener with this Conditions Handler the call-back method 
   *  registeredWithHandler( IConditionsHandler* ch ) will be called. Likewise calling removeChangeListener( IConditionsChangeListener* cl) 
   *  will call the call-back method deRegisteredWithHandler( IConditionsHandler* ch )

   *  Note: The data isn't read from the file until update() is called.
   *  Note: The use of default Collections is not implemented for this Class.
   *
   *  @author F.Gaede, S.Aplin,  DESY
   *  @version $Id: SimpleFileHandler.hh,v 1.3 2007-04-27 13:21:31 gaede Exp $
   */
  
  class SimpleFileHandler : public ConditionsHandlerBase {
    
  public:
    
    /** Default constructor. You have to specify the following parameters:
     * 
     * @param fileName: the LCIO file that holds the conditions data.
     * @param name:  of this conditions data set - used, e.g. as collection name 
     * in update(LCEvent* evt)
     * @param inputCollection: name of collection in input file - if not given the name
     * of this conditions data set is used
     */
    SimpleFileHandler(const std::string& fileName, 
		      const std::string& name,
		      const std::string& inputCollection="" ) ;

    
    virtual ~SimpleFileHandler() ;
    

    /** Retrieves the conditions data on first call. Subsequent calls
     *  have no effect.<br>
     *  Notifies all registered listeners on first call.
     */
    virtual void update( LCCDTimeStamp timestamp ) ;
    

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
    
    SimpleFileHandler() {} 

    // ----  data members ------

    /** Name of LCIO input file with conditions data */
    std::string _fileName{} ;

    /** Name of LCCollection in first event */
    std::string _inputCollection{} ;


    bool  _firstCall{} ;

  };
}  //end namespace

#endif // SimpleFileHandler_h
