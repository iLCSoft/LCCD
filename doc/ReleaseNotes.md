# v01-04

Marko Petric 2017-04-08
  - Coverity integration

Marko Petric 2017-03-23
  - Add install statement to CI
  - Add CONTRIBUTING.md and PULL_REQUEST_TEMPLATE and fix test script

Frank Gaede 2017-03-21
  - add AUTHORS file

Marko Petric 2017-03-20
  - Fix typo LCDD -> LCCD
  - Enable CI and add LICENCE

# v01-03-01

Frank Gaede 2016-03-07
  -  - made compatible with c++11     - removed -ansi -pedantic -Wno-long-long


# v01-03
     - updated all database test binaries 
         source/test/conddbtests/multiblefolders.cc
         source/test/conddbtests/calfiletodb.cc
         source/test/conddbtests/calfilefromdb.cc
         source/test/conddbtests/createdbfile.cc
       to take the db folder as argument on the command line as part of the DBcol name
       example:
       ./bin/calfiletodb condDB_channelmapping_HEAD_20130604_155014.slcio channelmapping /lctpc/test/channelmapping 0 999999

     - bug fix: wrong declaration logic (made obvious by coverity bugfix):
       move declaration and initialization outside loop in LCGenericObjectStreamer
       
# v01-02-01
			- Ch. Rosemann: coverity bugfixes


# v01-02   

        - added configuration header file LCCDConfig.h
            #defines or #undefines LCCD_CONDDBMYSQL (based on how LCCD was built)

        - removed old makefiles

        - cmake changes:

            - simplified cmake files (cmake macros now live in sub-package ILCSOFT_CMAKE_MODULES from package ILCUTIL)
        
            - added LCCDConfigVersion.cmake to allow checking version of LCCD at cmake time, e.g.:
                FIND_PACKAGE( LCCD 1.2 ) # at least version 1.2

            - export of LCIO dependency done through LCCDConfig.cmake, i.e. other packages get LCIO_INCLUDE_DIRS and
                LCIO_LIBRARIES when building against LCCD, e.g.:
                    
                    FIND_PACKAGE( LCCD 1.2 )
                    INCLUDE_DIRECTORIES( ${LCCD_INCLUDE_DIRS} )       # also includes ${LCIO_INCLUDE_DIRS}
                    TARGET_LINK_LIBRARIES( mylib ${LCCD_LIBRARIES} )  # also includes ${LCIO_LIBRARIES}

            - removed deprecated LCCD_DEFINITIONS (-DUSE_LCCD) from LCCDConfig.cmake
            
            - replaced options BUILD_LCCD_TESTS and BUILD_CONDDB_TESTS with option BUILD_TESTING
                If BUILD_TESTING is set to ON (default) tests are built with 'make'
                Otherwise tests are built separately with target 'make tests'
                (conddb tests are also built if LCCD is built with CondDBMySQL)

            - upgraded CMAKE_MINIMUM_REQUIRED to 2.6

        - fixed lcio::DataNotAvailableException bug in DataFileHandler

        - fixed forward declarations from CondDBMySQL


# v01-01   

        Improvements from R. Diener. Needs update of CondDBMySQL.
        - DBInterface::tagFolder now handles folders previously tagged of whose subfolders are tagged with the tag supplied to tagFolder. 
        - Replaced lcio exceptions with new LCCD exceptions. 
        - Exception throw when try to create a SimpleFile for a timestamp where no collection present 
        - Added DBSINCE, DBTILL and DBQUERYTIME for default collections
      -  Note: due to changes in DBCondHandler, v01-01 is only compatible with CondDBMySQL version CondDBMySQL_ILC-0-9-1

# v01-00   

      -  Changes to the API 
      -  IConditionsChangeListener is no longer a pure abstract base class and now contains the two additional call back methods:	

      	 - virtual void registeredWithHandler( IConditionsHandler* ch ) ;
	 - virtual void deRegisteredWithHandler( IConditionsHandler* ch ) ;

	 These are used to maintain a std::list of pointers to the handlers with which the listener has been registered.

      -  The ConditionsHandlerBase class has been declared a friend class of IConditionsChangeListener and uses these 
         call-back methods when a listener is registered or de-registered respectively, providing a pointer to itself 
	 as the argument.

      -  The IConditionsHandler has been extended to take a default collection which should be used as the current 
         collection in the case that no collection is available for the current time interval. In such case this
	  default collection is the one sent to the registered IConditionsChangeListeners.

	 - Note: this functionality is only implemented in the DBCondHandler and DBFileHandler classes and not in
	         SimpleFileHandler and DataFileHandler classes. For the latter two classes calling the methods: 

		     virtual void registerDefaultCollection( lcio::LCCollection* col);
		     virtual lcio::LCCollection* defaultCollection() ;    
		     virtual lcio::LCCollection* lastValidCollection()  ;
		     
		 will cause an exception to be thrown, otherwise their functionality remains unchanged.

         - Note: if no default collection is registered for a given handler and no data is available an exception 
	         will be thrown by the handler which will not be caught by the LCConditionsMgr.

      -  The IConditionsHandler has also been extended to hold the last valid collection. This is again only implemented 
      	 in the DBCondHandler and DBFileHandler classes and not in SimpleFileHandler and DataFileHandler classes.

	    	     virtual lcio::LCCollection* lastValidCollection() = 0;	      

      -	 The IConditionsHandler has also been extended to check if a given IConditionsChangeListener is register with it 

      	             virtual bool isChangeListenerRegistered( IConditionsChangeListener* cl ) ;

      -  LCConditionsMgr no longer catches exceptions in the update and updateEvent methods.

      -  CMakeLists.txt: OPTION( BUILD_32BIT_COMPATIBLE "Set to ON to build in 32 bit compatibility mode" OFF )

      -  Note: due to changes in DBCondHandler, v01-00 is only compatible with CondDBMySQL version CondDBMySQL_ILC-0-8

# v00-06-01

      -  corrected the version number in CMakeLists.txt, as it had not been incremented from v00-05 for v00-06



# v00-06
      -  added collection name to collection parameter "CollectionName" in 
         IConditionsChangeListener::conditionsChanged(lcio::LCCollection* col)
         so that implementors can acces it, via:
            std::string colName = col->getParameters().getStringVal("CollectionName") ;
      -  A new method added DBInterface::findNextValidCollection which retrieves the next valid 
          collection from the given point in time, searching forward in time - returns 
	  NULL if no data is found.
      -  findNextValidCollection is used in DBCondHandler::update to prevent the DB from being repeatedly 
     	 querried for every event while no collection is available. Using the valid_from_time of the 
	 object returned by findNextValidCollection and the timestamp of the current event, the 
	 start and end times of the period containing no collection are set.  
	 In the case that no collection is found a warning is printed and an exception thrown which is caught
    	 by the LCConditionsManager. 
    	 It should be noted that the ChangeListeners will not be notified. And no further warning will
	 be issued before a new object is found and another period of time without a ConditionsCollection
    	 is encoutered.	 
      -  bug fixed in DBCondHandler::update to ensure that it cannot delete using a NULL pointer.
      	 
      -  this version is only compatible with ConDBMySQL version CondDBMySQL_ILC-0-7-3 and later

# v00-05
      DBinterface extended:  B.Lutz DESY
       - added functions: 
	 void findCollections(ColVec &colVec, LCCDTimeStamp since, LCCDTimeStamp till, 
                              const std::string &tag="") ;
         void dump2File( ) ;
         void findAllCollections( ColVec& colVec ) ;

# v00-04  
    - made gcc4.3 compliant
    - made cmake 2.6 compliant
    - added LCCD_LIBRARY_DIRS
    - added 32 bit compatibility build option
    - added CMakeLists.txt for calomap example
 
# v00-03-06
    - cmake now default build tool - see README for details
    - bug fix:
	  CondDBMySQL is now optional but not required for building LCCD

# v00-03-05

 bug fix release
  - removed ';'s at end of namespace for gcc 3.4 and higher
  - added support for cmake (experimental)
