######################################################
# cmake file for building LCCD
# @author Jan Engels, DESY
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
######################################################


# project name
PROJECT( LCCD )


# project version
SET( LCCD_VERSION_MAJOR 1 )
SET( LCCD_VERSION_MINOR 2 )
SET( LCCD_VERSION_PATCH 0 )



### DEPENDENCIES ############################################################

# FindILCSOFT_CMAKE_MODULES.cmake needs to be in CMAKE_MODULE_PATH
FIND_PACKAGE( ILCSOFT_CMAKE_MODULES 0.11 REQUIRED )

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )


FIND_PACKAGE( LCIO REQUIRED )
# export variable LCCD_DEPENDS_INCLUDE_DIRS to LCCDConfig.cmake
LIST( APPEND LCCD_DEPENDS_INCLUDE_DIRS ${LCIO_INCLUDE_DIRS} )

# CondDBMySQL
OPTION( LCCD_CONDDB "Set to ON to build with CondDBMySQL" ON )
IF( LCCD_CONDDB )
    FIND_PACKAGE( CondDBMySQL REQUIRED )
ENDIF()


### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

IF( INSTALL_DOC )

    FIND_PACKAGE( Doxygen )

    IF( DOXYGEN_EXECUTABLE )
        ADD_SUBDIRECTORY( ./doc )
    ELSE()
        MESSAGE( SEND_ERROR "Could not find doxygen required to build documentation" )
        MESSAGE( "Please install doxygen or set INSTALL_DOC to OFF" )
        MESSAGE( "" )
    ENDIF()

ENDIF()



### LIBRARY AND TOOLS #######################################################

# -------- generate LCCDConfig.h --------------------------------------------
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/LCCDConfig.h.cmake.in"
    "${PROJECT_BINARY_DIR}/lccd/LCCDConfig.h" @ONLY )

INSTALL( FILES "${PROJECT_BINARY_DIR}/lccd/LCCDConfig.h" DESTINATION "include/lccd/" )
# ---------------------------------------------------------------------------


# option for building LCCD tests
# build target: 'make tests'
OPTION( BUILD_LCCD_TESTS "Set to ON to build LCCD tests" OFF )


ADD_SUBDIRECTORY( ./source )


# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

MESSAGE( STATUS "LCCD_CONDDB = ${LCCD_CONDDB}" )
MESSAGE( STATUS "BUILD_LCCD_TESTS = ${BUILD_LCCD_TESTS}" )
MESSAGE( STATUS "LCCD_DB_INIT = ${LCCD_DB_INIT}" )
MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
MESSAGE( STATUS )

# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( LCCDConfig.cmake LCCDConfigVersion.cmake LCCDLibDeps.cmake )

