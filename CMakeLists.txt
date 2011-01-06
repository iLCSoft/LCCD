######################################################
# cmake file for building LCCD
# @author Jan Engels, DESY
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
######################################################


# project name
PROJECT( LCCD )


# project version
SET( LCCD_VERSION_MAJOR 1 )
SET( LCCD_VERSION_MINOR 1 )
SET( LCCD_VERSION_PATCH 0 )



### SETTINGS ################################################################

# try to load ilcsoft CMakeModules from CMAKE_MODULE_PATH
INCLUDE( ilcsoft_cmake_modules )
IF( NOT ILCSOFT_CMAKE_MODULES_FOUND )
    MESSAGE( FATAL_ERROR "Could not find ilcsoft CMakeModules in CMAKE_MODULE_PATH" )
ENDIF( NOT ILCSOFT_CMAKE_MODULES_FOUND )
ILCSOFT_CMAKE_MODULES_MINIMUM_REQUIRED( 0.10.1.1 )
INCLUDE( ilcsoft_default_settings )

# option for building LCCD tests
# build target: 'make tests'
OPTION( BUILD_LCCD_TESTS "Set to ON to build LCCD tests" OFF )



### DEPENDENCIES ############################################################

FIND_PACKAGE( LCIO REQUIRED )

# CondDBMySQL
OPTION( LCCD_CONDDB "Set to ON to build with CondDBMySQL" OFF )
IF( LCCD_CONDDB )
    FIND_PACKAGE( CondDBMySQL REQUIRED )
ENDIF()



### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

IF( INSTALL_DOC )
    ADD_SUBDIRECTORY( ./doc )
ENDIF()



### LIBRARY AND TOOLS #######################################################

# -------- generate LCCDConfig.h --------------------------------------------
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/LCCDConfig.h.cmake.in"
    "${PROJECT_BINARY_DIR}/lccd/LCCDConfig.h" @ONLY )

INSTALL( FILES "${PROJECT_BINARY_DIR}/lccd/LCCDConfig.h" DESTINATION "include/lccd/" )
# ---------------------------------------------------------------------------


ADD_SUBDIRECTORY( ./source )


# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

MESSAGE( STATUS "LCCD_CONDDB = ${LCCD_CONDDB}" )
MESSAGE( STATUS "BUILD_LCCD_TESTS = ${BUILD_LCCD_TESTS}" )
MESSAGE( STATUS "LCCD_DB_INIT = ${LCCD_DB_INIT}" )
MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
MESSAGE( STATUS )

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "LCCDLibDeps.cmake" )
INSTALL( FILES "${PROJECT_BINARY_DIR}/LCCDLibDeps.cmake" DESTINATION lib/cmake )

