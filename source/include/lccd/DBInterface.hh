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


// LCIO headers
#include "lcio.h"
#include "EVENT/LCCollection.h"


#include <iostream>

namespace lccd {
  

  /** Provides a simple interface to the conditions data base for lccd.
   *  The functionality corresponds to a subset of the ConditionsDB API
   *  for user's ( and lccd developer's) convenience.
   * 
   *  @author F.Gaede, DESY
   *  @version $Id: DBInterface.hh,v 1.2 2005-02-11 15:35:30 gaede Exp $
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
    
    /** The db manager object
     */
    ICondDBMgr* condDBMgr() { return _condDBmgr ; }
    
    /** The db access object.
     */
    ICondDBDataAccess* condDataAccess() {  return _condDataAccess ; }
    
    /** The folder manager.
     */
    ICondDBFolderMgr*  condFolderMgr() { return _condFolderMgr ; }
    
    /** The tag manager 
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
      

    /** Retrieves the collection for the given point in time - returns NULL if it 
     *  no data is found.<br>
     *  This expects the LCIO collection type to be decoded in the object description:<br>
     *  "LCIO_COL_TYPE: some description" -  storeCollection() does this automatically.
     * 
     *  @param timeStamp: the point in time for which the conditions data is valid
     *  @param since:  the begin of the validity interval
     *  @param end:  the end of the validity interval
     *  @param tag: the tag to be used for the query, if omitted HEAD is used
     */
    lcio::LCCollection* findCollection( LCCDTimeStamp timeStamp, 
					LCCDTimeStamp& since, LCCDTimeStamp& till,
					const std::string& tag="" ) ; 

    /** Retrieves the collection for the given point in time - returns NULL if it 
     *  no data is found.<br>
     *  No information on the validity interval is given.
     */
    lcio::LCCollection* findCollection( LCCDTimeStamp timeStamp, 
					const std::string& tag="" ) ; 
    


    /** Tag the current HEAD of the database folder as tag.
     */
    void tagFolder( const std::string& tag ,  const std::string& description="" ) ;


  protected:
    
    void init() ;
    
    
    // data members
 
    ICondDBMgr* _condDBmgr ;
    ICondDBDataAccess* _condDataAccess ;
    ICondDBFolderMgr* _condFolderMgr ;
    ICondDBTagMgr * _condTagMgr ; 


  private:

    std::string _dbInit ;
    std::string _folder ;
    bool _update ;

  };

} ; //end namespace

#endif // USE_CONDDB
#endif // DBInterface_h
