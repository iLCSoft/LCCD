#ifndef DBInterface_h
#define DBInterface_h

#ifdef USE_CONDDB

// 
#include "lccd.h"


// forward declarations
class ICondDBMgr ;
class ICondDBDataAccess;
class ICondDBFolderMgr;
class ICondDBTagMgr;
class ICondDBObject;


// LCIO headers
#include "lcio.h"
#include "EVENT/LCCollection.h"


#include <iostream>

namespace lccd {
  
  /** Convenience typedef for vector of LCCollection pointers */
  typedef std::vector< lcio::LCCollection* > ColVec ;
 

  /** Provides a simple interface to the conditions data base for lccd.
   *  The functionality corresponds to a subset of the ConditionsDB API
   *  for user's ( and lccd developer's) convenience.
   * 
   *  @author F.Gaede, DESY
   *  @version $Id: DBInterface.hh,v 1.5 2005-02-17 15:45:53 gaede Exp $
   */

  class DBInterface {
    
  public:
    
    /** Default c'tor, uses the default database as defined in ConitionsDB, i.e. 
     *  as defined at compile time if not superseeded by $COND_DB_INIT.
     *  In update mode the folder is created if it doesn't exist.
     *
     *  @param folder: the folder used for this db access
     *  @param update: open db in update mode, default: false
     */
    DBInterface( const std::string& folder, bool update=false ) ;
    
    /** Open the database specified in dbInit.
     *  In update mode the folder is created if it doesn't exist.
     *
     *  @param folder: the folder used for this db access
     *  @param update: open db in update mode, default: false
     */
    DBInterface( const std::string& dbInit, const std::string& folder, bool update=false) ;
    
    /** D'tor*/
    virtual ~DBInterface() ;
    
    /** The db manager object - use for direct access to the conditions database.
     */
    ICondDBMgr* condDBMgr() { return _condDBmgr ; }
    
    /** The db access object - use for direct access to the conditions database..
     */
    ICondDBDataAccess* condDataAccess() {  return _condDataAccess ; }
    
    /** The folder manager - use for direct access to the conditions database..
     */
    ICondDBFolderMgr*  condFolderMgr() { return _condFolderMgr ; }
    
    /** The tag manager  - use for direct access to the conditions database.
     */
    ICondDBTagMgr * condTagMgr() { return _condTagMgr ; }


    /** The folder used for this DB access.
     */
    const std::string& folderName() {  return _folder ; } 


    /** Stores the given collection in the database 
     */
    void storeCollection( LCCDTimeStamp since, LCCDTimeStamp till,
			  lcio::LCCollection* col,
			  const std::string& description ) ;
      

    /** Retrieves the collection for the given point in time - returns NULL if 
     *  no data is found.<br>
     *  This expects the LCIO collection type to be decoded in the object description:<br>
     *  "LCIO_COL_TYPE: some description" -  storeCollection() does this automatically.<br>
     *  Some parameters are added to the collection:<br>
     *  <ul>
     *  <li>DBName:  name of the (MySQL) database</li>
     *  <li>DBFolder: data base folder </li>
     *  <li>DBTag: the tag used for the query - possibly HEAD at that time. </li>
     *  <li>DBQueryTime: the time the conditions data was extracted from the data base. </li>
     *  <li>DBInsertionTime: the time the conditions data was inserted into the data base. </li>
     *  <li>DBSince: the start of the vaidity intervall</li>
     *  <li>DBTill: the end of the validity intervall</li>
     *  <li>DBLayer: the layer number of this data set</li>
     *  </ul> 
     *  DBQueryTime, DBInsertionTime, DBSince and DBTill are string vectors with two elements: 
     *  the 64bit time stamp and the date in human readable format.
     * 
     *  @param timeStamp: the point in time for which the conditions data is valid
     *  @param since:  return the begin of the validity interval
     *  @param end:  return the end of the validity interval
     *  @param tag: the tag to be used for the query, if omitted HEAD is used
     */
    lcio::LCCollection* findCollection( LCCDTimeStamp timeStamp, 
					LCCDTimeStamp& since, LCCDTimeStamp& till,
					const std::string& tag="" ) ; 
    
    /** Retrieves the collection for the given point in time - returns NULL if it 
     *  no data is found.<br>
     *  Same as above, except that no information on the validity interval is returned.
     */
    lcio::LCCollection* findCollection( LCCDTimeStamp timeStamp, 
					const std::string& tag="" ) ; 
    
    /** Creates collections for all conditions data for the given tag and 
     *  adds them to the colvec. The collections are ordered w.r.t. to their validity 
     *  time.<br>
     *  Description parameters are added to the collections as in  findCollection().
     */
    void findCollections( ColVec& colVec, const std::string& tag="" ) ; 


    /** Tag the current HEAD of the database folder as tagName. If the argument usingTagName is provided
     *  and refers to an existing tag this tag will be duplicated as tagName.<br>
     *  This can be used to provide a common tag with the same name for several folders of conditions data
     *  that have been tagged before with different names. 
     */
    void tagFolder( const std::string& tagName, const std::string& description="", std::string usingTagName = "");



    /** Creates an LCIO file with the all conditions data in the folder for the given tag.
     *  The collections are sorted w.r.t. their validity time intervall and attached to 
     *  consecutive events. The run header holds a map of validity time intervalls to events.
     *  in the parameters DBSince and DBTill.<br>
     *  The name of the file will be of the form "conddb_COLNAME_TAG_YYYYMMDD_HHMMSS.slcio"
     *  where the specified time is the creation time.
     *  This file can be used by the DBFileHandler.<br>
     */
    void createDBFile( const std::string& tag="" ) ;


  protected:
    /** Initializes the database  access */
    void init() ;

    /** Creates an LCColleciton from the data in the ICondDBObject. Adds the db parameters 
     *  as described in findCollection() to the collection. 
     */
    lcio::LCCollection* collectionFromCondDBObject( ICondDBObject* object, const std::string& tag ) ;
    

    // data members
    ICondDBMgr* _condDBmgr ;
    ICondDBDataAccess* _condDataAccess ;
    ICondDBFolderMgr* _condFolderMgr ;
    ICondDBTagMgr * _condTagMgr ; 


  private:

    std::string _dbInit ;
    std::string _folder ;
    bool _update ;
    std::string _dbName ;
  };

} ; //end namespace

#endif // USE_CONDDB
#endif // DBInterface_h
