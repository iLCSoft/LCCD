#ifndef VCollectionStreamer_h
#define VCollectionStreamer_h

#include "lccd.h"

// -- lcio headers
#include "lcio.h"
#include "EVENT/LCCollection.h"

// -- CondDB headers
#include "ConditionsDB/ICondDBStreamable.h"

#include <iostream>

namespace lccd {
  

  /** Base class for LCCollection streamer classes.
   * 
   *  @author F.Gaede, DESY
   *  @version $Id: VCollectionStreamer.hh,v 1.2 2007-04-27 13:21:31 gaede Exp $
   */

  class VCollectionStreamer : public ICondDBStreamable {
    
  public:
    
    VCollectionStreamer() = default ;
    VCollectionStreamer(const VCollectionStreamer&) = delete;
    VCollectionStreamer& operator=(const VCollectionStreamer&) = delete ;

    virtual ~VCollectionStreamer() {} ;


    /** Set the collection that will be stored in the database.
     */
    virtual void setCollection( lcio::LCCollection* col) { _col = col ; } ;

    /** Get the collection retrieved from the database.
     *  It is the callers responsibility to delete the collection.
     */
    virtual lcio::LCCollection* getCollection() { return _col ; }

    /** Covariant factory method. Subclasses need to return 
     *  a new instance of their type.
     */
    virtual VCollectionStreamer* create()=0 ;


  protected:

    /** Write the collection's parameters to the string/stream. 
     */
    virtual void writeParameters( std::string& s ) const ;

    /** Read the collection's parameters from the stream.
     */
    virtual unsigned readParameters( const std::string& s , unsigned index ) ; 

    lcio::LCCollection*  _col = nullptr ;
  };

} //end namespace

#endif // VCollectionStreamer_h
