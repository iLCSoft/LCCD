########################################################
# cmake file for building Marlin example Package
# @author Jan Engels, Desy IT
CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)
########################################################


# project name
PROJECT( calomap )


# project version
SET( ${PROJECT_NAME}_VERSION_MAJOR 0 )
SET( ${PROJECT_NAME}_VERSION_MINOR 1 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )



### DEPENDENCIES ############################################################

# FindILCSOFT_CMAKE_MODULES.cmake needs to be in CMAKE_MODULE_PATH
FIND_PACKAGE( ILCSOFT_CMAKE_MODULES 0.11 REQUIRED )

# load default settings from ILCSOFT_CMAKE_MODULES
INCLUDE( ilcsoft_default_settings )


FIND_PACKAGE( Marlin 0.14 REQUIRED )
INCLUDE_DIRECTORIES( ${Marlin_INCLUDE_DIRS} )
LINK_LIBRARIES( ${Marlin_LIBRARIES} )


FIND_PACKAGE( LCCD 1.2 REQUIRED )
INCLUDE_DIRECTORIES( ${LCCD_INCLUDE_DIRS} )
LINK_LIBRARIES( ${LCCD_LIBRARIES} )



### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

IF( INSTALL_DOC AND EXISTS "${PROJECT_SOURCE_DIR}/doc/CMakeLists.txt" )
    ADD_SUBDIRECTORY( doc )
ELSE()
    MESSAGE( STATUS "INSTALL_DOC set to OFF" )
    SET( INSTALL_DOC OFF )
ENDIF()



### LIBRARY #################################################################

# include directories
INCLUDE_DIRECTORIES( ./include )

INSTALL_DIRECTORY( ./include DESTINATION . FILES_MATCHING PATTERN "*.h" )

# definitions to pass to the compiler
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )
ADD_DEFINITIONS( "-Wno-long-long" )

# add library
ADD_SHARED_LIBRARY( CaloMapProcessor ./src/CaloMapProcessor.cc )
INSTALL_SHARED_LIBRARY( CaloMapProcessor DESTINATION lib )

# createmaps example
ADD_EXECUTABLE( createmaps ./src/createmaps.cc )
INSTALL( TARGETS createmaps DESTINATION bin )

# display some variables and write them to cache
DISPLAY_STD_VARIABLES()

