#ifdef USE_CONDDB

// -- CondDB headers
#include "ConditionsDB/ICondDBMgr.h"
#include "ConditionsDB/CondDBMySQLMgrFactory.h"
#include "ConditionsDB/CondDBObjFactory.h"
//#include "exampleObject.h"

// -- LCIO headers
#include "lcio.h"
#include "lccd/SimpleFileHandler.hh"
#include "IO/LCWriter.h"
#include "IMPL/LCRunHeaderImpl.h"
#include "IMPL/LCEventImpl.h"
//
#include "UTIL/LCTOOLS.h"
#include "UTIL/LCTime.h"


// -- LCCD headers
#include "lccd.h"
#include "lccd/StreamerMgr.hh"
#include "lccd/VCollectionStreamer.hh"
#include "lccd/DBCondHandler.hh"
#include "lccd/DBInterface.hh"
#include "lccd/ConditionsMap.hh"

#include "CalibrationConstant.hh"

// -- C++ headers 
#include <iostream>


using namespace std ;
using namespace lcio;
//using namespace lccd ;


/** Test program that reads some calibration from a data base and writes 
 *  it to an LCIO file
 * 
 * @author F.Gaede, DESY
 * @version $Id: calfilefromdb.cc,v 1.2 2005-02-11 15:36:00 gaede Exp $
 */

int main(int argc, char** argv ) {
  
  // enable LCIO exception handling (usually automatically done when Reader/Writer exists)
  HANDLE_LCIO_EXCEPTIONS ;

  // read file name and collection name from command line 
  if( argc < 4) {
    cout << " usage: calfilefromdb <db collection name> <timestamp> <output-file> [<tag>]" << endl ;
    exit(1) ;
  }

  string colName( argv[1] ) ;
  string timeStr( argv[2] ) ;
  string fileName( argv[3] ) ;

  lccd::LCCDTimeStamp  timeStamp ( atoi( timeStr.c_str() ) )   ;  
  
  string folder( "/lccd/" + colName ) ;

  string tag("") ;

  if( argc > 4 )
    tag = argv[4] ;
  

  // ----- read directly from the db: ----- 
  //   //     lccd::DBInterface db( folder ) ;
  //   lccd::DBInterface db("localhost:lccd_test:calvin:hobbes" , folder , true ) ;
  //   lcio::LCCollection* col = db.findCollection( timeStamp , tag ) ;
  //   if( col == 0 ){ 
  //     std::cout << " No collection found for time stamp: " << timeStamp << std::endl ;
  //     return 1 ;
  //   }
  

  // ---- use the DBCondHandler -----------

  lccd::IConditionsHandler* conData = 
    new lccd::DBCondHandler( "localhost:lccd_test:calvin:hobbes", folder, colName, tag ) ;
  
  // create a calibration map

  typedef lccd::ConditionsMap<int,CalibrationConstant> CalMap ;
  CalMap calMap( &CalibrationConstant::getCellID )   ;

//   typedef lccd::ConditionsMap<float,CalibrationConstant> CalMap ;
//   CalMap calMap( &CalibrationConstant::getGain )   ;


  conData->registerChangeListener(  &calMap )  ;
  
  conData->update( timeStamp ) ;
//   conData->update( timeStamp+100 ) ;
//   conData->update( timeStamp+200 ) ;
//   conData->update( timeStamp+300 ) ;


  calMap.print( std::cout ) ;

  lcio::LCTime t0 ( conData->validSince()  ) ;
  lcio::LCTime t1 ( conData->validTill()  ) ;

  cout << endl 
       << " -- calibration data has been read from data base folder" << folder 
       << endl 
       << " ---- valid from: " <<  t0.getDateString()  
       << " [" <<  conData->validSince()  << "] " 
       << endl 
       << " ----       till: " <<  t1.getDateString()  
       << " [" <<  conData->validTill()  << "] "
       << endl 
       << endl ;
   
  LCCollection* col = conData->currentCollection() ;
  
  //------------------ write constants to file -----------------------
  
  LCWriter* wrt = LCFactory::getInstance()->createLCWriter() ;
  
  wrt->open( fileName , LCIO::WRITE_NEW )  ;
  
  
  // create and add a run header to the file 
  // this isn't needed by LCCD but allows to use the 'dumpevent' tool
  LCRunHeader* rHdr = new LCRunHeaderImpl ;
  wrt->writeRunHeader( rHdr ) ;
  
  LCEvent* evt = new LCEventImpl ;
  evt->addCollection(  col , colName  ) ;  


  evt->takeCollection(  colName  ) ;  // this is needed because the event should not own the collection... 

  
  LCTOOLS::dumpEventDetailed( evt ) ; 
  wrt->writeEvent( evt ) ;
  
  wrt->close() ;
  //----------------------------------------------------------------

  // clean up
  delete evt ; // this deletes the collection as well
  delete wrt ;
  delete rHdr ;
  
  
  delete conData ;

}
#endif
