#include "lccd.h"

#include <bitset>

/** Provides global constants
 */

namespace lccd{
  
  LCCDTimeStamp LCCDMinusInf = -9223372036854775807LL - 1; // -2^63
  
  LCCDTimeStamp LCCDPlusInf  =  9223372036854775807LL ;   //   2^63-1
  

  /** If this doesn't compile than LCCDTimeStamp doesn't have 64bit.
   */
  void ensure_LCCDTimeStamp_is_64bit(){ 
    std::bitset<64> a(0xcafebabedeadbeafLL)  ; 
    std::bitset< 8*sizeof(LCCDTimeStamp)  > b = a ;
  } 


};
