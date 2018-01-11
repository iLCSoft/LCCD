#include "lccd.h"
#include "lccd/LCCDConfig.h"

#ifdef LCCD_CONDDBMYSQL
#include "ConditionsDB/SimpleTime.h"
#endif

#include <bitset>
#include <cstdlib>

/** Provides global constants
 */

namespace lccd{
  
  LCCDTimeStamp LCCDMinusInf = -9223372036854775807LL - 1; // -2^63
  
  LCCDTimeStamp LCCDPlusInf  =  9223372036854775807LL ;   //   2^63-1
  

  std::string DBFOLDER        = "DBFolder" ;
  std::string DBSINCE         = "DBSince" ;
  std::string DBTILL          = "DBTill" ;
  std::string DBQUERYTIME     = "DBQueryTime" ;
  std::string DBINSERTIONTIME = "DBInsertionTime" ;
  std::string DBTAG           = "DBTag" ;
  std::string DBNAME          = "DBName" ;
  std::string DBLAYER         = "DBLayer" ;
  std::string DBTIMESTAMP     = "DBTimeStamp" ;




  std::string getDBInitString() {
    
    const char* lccd_db_init = std::getenv("LCCD_DB_INIT");
    
    if (lccd_db_init)
      
      return lccd_db_init ;
    
    else 

      return LCCD_DB_INIT_DEFAULT ;

  }
  



  LCCDTimeStamp fromSimpleTime( const SimpleTime& st ) {

#ifdef LCCD_CONDDBMYSQL
    // subtract the ns from 1.1.1900 to 1.1.1970 
    return TimeT(st) - 2208988800000000000LL ;
#else
    throw lccd::LCCDException(" LCCDTimeStamp fromSimpleTime( const SimpleTime& st ) is only availible when lccd is build against CONDDBMYSQL" ) ;
    return -1 ;
#endif

  }





  /** If this doesn't compile than LCCDTimeStamp doesn't have 64bit.
   */
  void ensure_LCCDTimeStamp_is_64bit(){ 
    std::bitset<64> a(0xcafebabedeadbeafLL)  ; 
    std::bitset< 8*sizeof(LCCDTimeStamp)  > b = a ; b[0]=0 ;
  } 

}
