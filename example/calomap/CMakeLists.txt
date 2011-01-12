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


### SETTINGS ################################################################

# try to load ilcsoft CMakeModules from CMAKE_MODULE_PATH
INCLUDE( ilcsoft_cmake_modules )
IF( NOT ILCSOFT_CMAKE_MODULES_FOUND )
    MESSAGE( FATAL_ERROR "Could not find ilcsoft CMakeModules in CMAKE_MODULE_PATH" )
ENDIF( NOT ILCSOFT_CMAKE_MODULES_FOUND )
ILCSOFT_CMAKE_MODULES_MINIMUM_REQUIRED( 0.11 )
INCLUDE( ilcsoft_default_settings )



### DEPENDENCIES ############################################################

# ------- required packages -------------------
SET( required_packages LCCD GEAR Marlin )

# ------- optional packages -------------------
#SET( optional_packages AIDA )


# ------- do the search -----------------------

# fix for backwards compatibility
FOREACH( pkg ${required_packages} ${optional_packages} )
    IF( NOT ${pkg}_DIR AND DEFINED ${pkg}_HOME )
        SET( ${pkg}_DIR ${${pkg}_HOME} )
    ENDIF()
ENDFOREACH()

FOREACH( pkg ${required_packages} )
    FIND_PACKAGE( ${pkg} REQUIRED )
ENDFOREACH()

FOREACH( pkg ${optional_packages} )
    FIND_PACKAGE( ${pkg} QUIET )
ENDFOREACH()

FOREACH( pkg ${required_packages} ${optional_packages} )
    IF( ${pkg}_FOUND )
        INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
        LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
        ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )
    ELSE()
        MESSAGE( STATUS "${pkg} not found" )
    ENDIF()
ENDFOREACH()



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

