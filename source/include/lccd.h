#ifndef lccd_h
#define lccd_h 1

#include <bitset>
#include "lcio.h"

/**
 * \mainpage <a href="http://www.desy.de/~gaede/lccd">LCCD</a> (v00-00)
 * A conditions data framework for the ILC - based on LCIO.
 * <p>UNDER DEVELOPMENT !!!!!
 *
 */


// Provides common definitions and typedefs for LCCD.
/** The only namespace in the project.<br>
 */
namespace lccd{
  


  /** The timestamp in LCCD is a 64bit int as defined in LCIO.
   */
  typedef lcio::long64 LCCDTimeStamp  ;

  /** The smallest possible time stamp 
   */
  extern LCCDTimeStamp LCCDMinusInf ; 
  
  /** The largest possible time stamp 
   */
  extern LCCDTimeStamp LCCDPlusInf ;
   


  /** If this doesn't compile than LCCDTimeStamp doesn't have 64bit.
   */
  void ensure_LCCDTimeStamp_is_64bit() ;


};

#endif
