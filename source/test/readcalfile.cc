#include <iostream>

// LCIO headers
#include "lcio.h"
#include "UTIL/LCTOOLS.h"
#include "UTIL/LCTime.h"

// LCCD headers
#include "lccd.h"
#include "lccd/SimpleFileHandler.hh"


#include "CalibrationConstant.hh"


using namespace std ;
using namespace lcio;
//using namespace lccd ;

/** Test program that reads some calibration from a file. 
 * 
 * @author F.Gaede, DESY
 * @version $Id: readcalfile.cc,v 1.2 2005-02-11 11:03:14 gaede Exp $
 */

int main(int argc, char** argv ) {
  
  // read file name and collection name from command line 
  if( argc < 3) {
    cout << " usage: readcalfile <input-file> <collection name>" << endl ;
    exit(1) ;
  }
  
  string fileName( argv[1] ) ;
  string colName( argv[2] ) ;
  


  // create a conditions handler
  // NB: the rest of the code uses the abstract interface !
 
  lccd::IConditionsHandler* conData = new lccd::SimpleFileHandler( fileName, colName ) ;
  


  // read data from file - timestamp is ignored for SimpleFileHandler
  conData->update( lccd::LCCDTimeStamp(   123456789   )   ) ;


  cout << endl 
       << " -- calibration data has been read from the file " 
       << fileName << " in collection " << colName 
       << endl 
       << endl ;


  LCCollection* col = conData->currentCollection() ;
  
  // we can simply dump the collection as LCGenericObjects using LCTOOLS
  LCTOOLS::printLCGenericObjects( col ) ;
  

  // or use the class CalibrationConstant to print, i.e. access the data
  for(int i=0 ; i< col->getNumberOfElements() ; i++) {

    CalibrationConstant cc( col->getElementAt(i) ) ;

    cc.print( std::cout ) ; 
    
  }


  lcio::LCTime t0 ( conData->validSince()  ) ;
  lcio::LCTime t1 ( conData->validTill()  ) ;

  cout << endl 
       << " valid since: " <<  t0.getDateString()  
       << " [" <<  conData->validSince()  << "] " 
       << endl
       << "        till: " <<  t1.getDateString()  
       << " [" <<  conData->validTill()  << "] "
       << endl 
       << endl ;
  
  // clean up
  delete conData ;

}
