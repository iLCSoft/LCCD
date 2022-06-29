######################################################
# cmake file for building LCCD
# @author Jan Engels, DESY
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
######################################################


# project name
PROJECT( LCCD )


# project version
SET( LCCD_VERSION_MAJOR 1 )
SET( LCCD_VERSION_MINOR 5 )
SET( LCCD_VERSION_PATCH 1 )



### DEPENDENCIES ############################################################

FIND_PACKAGE( ILCUTIL REQUIRED COMPONENTS ILCSOFT_CMAKE_MODULES ILCTEST )

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )


FIND_PACKAGE( LCIO REQUIRED )
# export following variables to LCCDConfig.cmake
SET( LCCD_DEPENDS_INCLUDE_DIRS ${LCIO_INCLUDE_DIRS} )
SET( LCCD_DEPENDS_LIBRARY_DIRS ${LCIO_LIBRARY_DIRS} )
SET( LCCD_DEPENDS_LIBRARIES ${LCIO_LIBRARIES} )

# CondDBMySQL
OPTION( LCCD_CONDDBMYSQL "Set to ON to build with CondDBMySQL" ON )
IF( LCCD_CONDDBMYSQL )
    FIND_PACKAGE( CondDBMySQL REQUIRED )
ENDIF()


### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" OFF )

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


ADD_SUBDIRECTORY( ./source )


# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

MESSAGE( STATUS "LCCD_CONDDBMYSQL = ${LCCD_CONDDBMYSQL}" )
MESSAGE( STATUS "LCCD_DB_INIT = ${LCCD_DB_INIT}" )
MESSAGE( STATUS )

# generate and install following configuration files
GENERATE_PACKAGE_CONFIGURATION_FILES( LCCDConfig.cmake LCCDConfigVersion.cmake)

