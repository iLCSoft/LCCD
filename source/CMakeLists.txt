#################################
# cmake file for building LCCD
# @author Jan Engels, DESY
#################################

INCLUDE_DIRECTORIES( ${LCIO_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( BEFORE "${PROJECT_SOURCE_DIR}/source/include" )
INCLUDE_DIRECTORIES( ${PROJECT_BINARY_DIR}/lccd )
INSTALL_DIRECTORY( ./include DESTINATION . )



# omit warning about preprocessor definitions being escaped
IF(COMMAND CMAKE_POLICY)
    CMAKE_POLICY(SET CMP0005 OLD)
ENDIF()

# specific LCCD variable
IF( NOT LCCD_DB_INIT )
    SET( LCCD_DB_INIT "localhost:lccd_test:calvin:hobbes" )
ENDIF()
# write LCCD_DB_INIT to cache
SET( LCCD_DB_INIT "${LCCD_DB_INIT}" CACHE STRING "DB initialization" FORCE )


#----- need long long for int64 for now ------
#FIXME: should establish wether we are on a 32bit or 64 bit machine ....
ADD_DEFINITIONS( "-Wno-long-long" )
ADD_DEFINITIONS( "-DLCCD_DB_INIT_DEFAULT=\\\"${LCCD_DB_INIT}\\\"" )


# add library
SET( libname lccd )
AUX_SOURCE_DIRECTORY( ./src library_sources )

IF( LCCD_CONDDB )
    AUX_SOURCE_DIRECTORY( ./conddb/src library_sources )
    INCLUDE_DIRECTORIES( ./conddb/include )
    INSTALL_DIRECTORY( ./conddb/include DESTINATION . )

    INCLUDE_DIRECTORIES( ${CondDBMySQL_INCLUDE_DIRS} )
    LINK_LIBRARIES( ${CondDBMySQL_LIBRARIES} )
    ADD_DEFINITIONS( ${CondDBMySQL_DEFINITIONS} )
ENDIF()


SET_SOURCE_FILES_PROPERTIES( ${library_sources}
    PROPERTIES COMPILE_FLAGS "-Wall -ansi -pedantic"
)
ADD_SHARED_LIBRARY( ${libname} ${library_sources} )
INSTALL_SHARED_LIBRARY( ${libname} DESTINATION lib )
TARGET_LINK_LIBRARIES( ${libname} ${LCIO_LIBRARIES} )


# add tests
ADD_SUBDIRECTORY( ./test )

