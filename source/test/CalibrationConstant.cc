#include "CalibrationConstant.hh"



std::string CalibrationConstant::_typeName = "CalibrationConstant" ;
std::string CalibrationConstant::_dataDescription = "i:cellID,f:offset,f:gain" ;


void CalibrationConstant::print(  std::ostream& os ){

  os << " cellId: ["  << std::hex  << getCellID() << "]" <<  std::dec 
     << ", offset: " <<  getOffset() 
     << ", gain: "   <<  getGain() 
     << std::endl ;
  
}

// std::ostream& operator<< (  std::ostream& os, CalibrationConstant& cc ){


//   os << " cellId: ["  << std::hex  << cc.getCellID() << "]" <<  std::dec 
//      << ", offset: " <<  cc.getOffset() 
//      << ", gain: "   <<  cc.getGain() 
//      << std::endl ;

//   return os ;
// }
