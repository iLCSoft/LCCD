#ifndef CalibrationConstant_h
#define CalibrationConstant_h 1

#include "lcio.h"
#include "EVENT/LCGenericObject.h"
#include <iostream>

#define NINT 1 
#define NFLOAT 2
#define NDOUBLE 0

using namespace lcio ;

class CalibrationConstant ;

// std::ostream& operator<< (  std::ostream& os, CalibrationConstant& cc ) ;

/** Example for a simple calibration class that can be stored in an LCIO file.
 */
class CalibrationConstant : public LCGenericObject {

  //  friend std::ostream& operator<< (  std::ostream& os, CalibrationConstant& cc ) ;
  
public: 
  
  /** Default c'tor.
   */
  CalibrationConstant():
    _cellID( 0 ) , 
    _offset( 0. ) ,
    _gain( 0. ) {
  }

  /** Convenient c'tor.
   */
  CalibrationConstant(int cellID, float offset, float gain) :
    _cellID( cellID ) , 
    _offset( offset ) ,
    _gain( gain ) {
  }

  /** Copy c'tor to be used for elements of LCObjects read from
   * an LCIO file.
   */
  CalibrationConstant(LCObject* obj){

    LCGenericObject* gObj = dynamic_cast<LCGenericObject*>(obj) ;

    if(gObj==0){

      _cellID = 0 ; 
      _offset = 0. ;
      _gain = 0. ;

    } else {
      _cellID = gObj->getIntVal( 0 ) ; 
      _offset = gObj->getFloatVal( 0 ) ; 
      _gain   = gObj->getFloatVal( 1 ) ; 
    }
  }
  
  virtual ~CalibrationConstant() { /* nop */; }
  
  // --- own interface ------
  int getCellID()   { return _cellID  ; } 
  float getOffset() { return _offset  ; } 
  float getGain()   { return _gain  ; } 
  

  void print(  std::ostream& os ) ;

  // ---- need to implement LCGenericObject interface:

  virtual int getNInt() const    { return NINT ; } 
  virtual int getNFloat() const  { return NFLOAT ; }  
  virtual int getNDouble() const { return NDOUBLE ; } 
  
  virtual int getIntVal(int index) const {
    if( index == 0 )
      return _cellID ;
    else
      return 0 ;
  }
  virtual float getFloatVal(int index) const { 
    
    switch( index ){
    case 0:
      return _offset ;
      break ;
    case 1:
      return _gain ;
      break ;
    default :
      return 0 ;
    }
  }
  
  virtual double getDoubleVal(int index) const {
    return 0.0 ;
  }
  
  virtual bool isFixedSize() const { return true ; }

  virtual const std::string & getTypeName() const { 
    return _typeName ;
  } 
  
  virtual const std::string & getDataDescription() const {
    return _dataDescription ;
  }

  // ---- end of LCGenericObject interface
  
  
protected:
  int _cellID ;
  float _offset ;
  float _gain ;
  
  static std::string _typeName ;
  static std::string _dataDescription ; 

}; // class


#endif 

//=============================================================================


