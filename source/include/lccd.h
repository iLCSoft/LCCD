#ifndef lccd_h
#define lccd_h 1

#include <bitset>
#include "lcio.h"

#ifdef USE_CONDDB
class SimpleTime ;
#endif


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



  /** String constant: DBSince */
  extern std::string DBSINCE ;

  /** String constant: DBTill */
  extern std::string DBTILL ;

  /** String constant: DBQueryTime */
  extern std::string DBQUERYTIME ;

  /** String constant: DBInsertionTime */
  extern std::string DBINSERTIONTIME ;

  /** String constant: DBTag */
  extern std::string DBTAG ;

  /** String constant: DBFolder */
  extern std::string DBFOLDER ;

  /** String constant: DBName */
  extern std::string DBNAME ;

  /** String constant: DBLayer */
  extern std::string DBLAYER ;           

  /** String constant: DBTimeStamp */
  extern std::string DBTIMESTAMP ;

  


#ifdef USE_CONDDB
  /** Converts the 64bit time stamp in SimpleTime objects (based on 1.1.1900) to the LCIO definition
   *  of time stamps: ns since 1.1.1970 UTC.
   */
  LCCDTimeStamp fromSimpleTime( SimpleTime st ) ;
#endif

};

#endif
