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


// -- LCCD headers
#include "lccd.h"
#include "lccd/StreamerMgr.hh"
#include "lccd/VCollectionStreamer.hh"
#include "lccd/DBCondHandler.hh"
#include "lccd/DBInterface.hh"

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
 * @version $Id: calfilefromdb.cc,v 1.1.1.1 2005-02-10 12:17:54 gaede Exp $
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
  
  conData->update( timeStamp ) ;

  cout << endl 
       << " -- calibration data has been read from data base folder" << folder 
       << endl 
       << " ---- valid from: " <<  conData->validSince() 
       << " ----       till: " <<  conData->validTill() 
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
  
  //   LCTOOLS::dumpEventDetailed( evt ) ; 
  wrt->writeEvent( evt ) ;
  
  wrt->close() ;
  // clean up
  delete evt ; // this deletes the collection as well
  delete wrt ;
  delete rHdr ;
  
  //----------------------------------------------------------------

}
#endif
