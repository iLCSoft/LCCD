#################################
# cmake file for building LCCD
# @author Jan Engels, DESY
#################################


########################################################
# CMake compatibility issues: don't modify this, please!
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.6 )
#SET( CMAKE_BACKWARDS_COMPATIBILITY 2.4.6 )
MARK_AS_ADVANCED(CMAKE_BACKWARDS_COMPATIBILITY)
# allow more human readable "if then else" constructs
SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )
########################################################

# project name
PROJECT( LCCD )

# project version
SET( ${PROJECT_NAME}_MAJOR_VERSION 0 )
SET( ${PROJECT_NAME}_MINOR_VERSION 3 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 6 )

# project options
OPTION( BUILD_SHARED_LIBS "Set to OFF to build static libraries" ON )
OPTION( BUILD_LCCD_TESTS "Set to ON to build LCCD tests" OFF )
OPTION( BUILD_CONDDB_TESTS "Set to ON to build CondDB tests" OFF )
OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" ON )

# project dependencies
SET( ${PROJECT_NAME}_DEPENDS "LCIO" )

# set default cmake build type to RelWithDebInfo
IF( NOT CMAKE_BUILD_TYPE )
    SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )
ENDIF()

# set default install prefix to project root directory
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${${PROJECT_NAME}_SOURCE_DIR}" )
ENDIF()

# specific LCCD variable
IF( NOT LCCD_DB_INIT )
    SET( LCCD_DB_INIT "localhost:lccd_test:calvin:hobbes" )
ENDIF()

#----- need long long for int64 for now ------
#FIXME: should establish wether we are on a 32bit or 64 bit machine ....
ADD_DEFINITIONS( "-Wno-long-long" )
LIST( APPEND ${PROJECT_NAME}_DEFINITIONS "-Wno-long-long" )
                                                                                                                                                           
ADD_DEFINITIONS( "-DLCCD_DB_INIT_DEFAULT=\\\"${LCCD_DB_INIT}\\\"" )

# library *nix style versioning
SET( ${PROJECT_NAME}_SOVERSION
    "${${PROJECT_NAME}_MAJOR_VERSION}.${${PROJECT_NAME}_MINOR_VERSION}" )
SET( ${PROJECT_NAME}_VERSION
    "${${PROJECT_NAME}_SOVERSION}.${${PROJECT_NAME}_PATCH_LEVEL}" )

# DOCUMENTATION
# code for *nix only!
IF( UNIX )
    # find shell
    FIND_PROGRAM( SH
      sh
      ${CYGWIN_INSTALL_PATH}/bin
      /bin
      /usr/bin
      /usr/local/bin
      /sbin
      # FIXME add path to MacOS here
    )
    MARK_AS_ADVANCED( SH )

    IF( SH )
        # generate shell script for setting environment before running a command
        MESSAGE( STATUS "Generating shell environment script..." )
        FILE( WRITE "${PROJECT_BINARY_DIR}/script.sh"
                    "export LCCDVERSION=${${PROJECT_NAME}_VERSION}\n"
                    "$1 $2" )

        FIND_PACKAGE( Doxygen )
        FIND_PACKAGE( LATEX )
        IF( DOXYGEN_FOUND AND LATEX_COMPILER AND MAKEINDEX_COMPILER )
            
            ADD_CUSTOM_COMMAND(
                OUTPUT  "${${PROJECT_NAME}_SOURCE_DIR}/doc/html"
                COMMAND ${SH} "${PROJECT_BINARY_DIR}/script.sh" "${DOXYGEN_EXECUTABLE}"
                WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/doc"
                COMMENT "Building API Documentation..."
                VERBATIM )
            
            ADD_CUSTOM_TARGET( doc DEPENDS
                "${${PROJECT_NAME}_SOURCE_DIR}/doc/html" )
            
        ELSE()
            IF( NOT DOXYGEN_FOUND )
                MESSAGE( STATUS "Doxygen not found in your system!!" )
            ENDIF()
            IF( NOT LATEX_COMPILER OR NOT MAKEINDEX_COMPILER )
                MESSAGE( STATUS "Latex not found in your system!!" )
            ENDIF()
            IF( INSTALL_DOC )
                MESSAGE( STATUS "INSTALL_DOC forced to OFF" )
                SET( INSTALL_DOC OFF )
            ENDIF()
        ENDIF()
    ELSE()
        IF( INSTALL_DOC )
            MESSAGE( STATUS "Unix Shell not found - INSTALL_DOC forced to OFF and doc target disabled!" )
            SET( INSTALL_DOC OFF )
        ENDIF()
    ENDIF()
ENDIF()

IF( INSTALL_DOC )
    # make sure doxygen is executed before make install
    INSTALL( CODE "EXEC_PROGRAM(${CMAKE_BUILD_TOOL} ${${PROJECT_NAME}_BINARY_DIR} ARGS doc)" )
    
    # install documentation
    INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/doc"
            DESTINATION .
            PATTERN "*CVS*" EXCLUDE )
ENDIF()

##########################################################################################

# add install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# add install path to the rpath list (apple)
IF( APPLE )
    SET( CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib" )
    MARK_AS_ADVANCED( CMAKE_INSTALL_NAME_DIR )
ENDIF()

# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )

# output directories
SET( EXECUTABLE_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${${PROJECT_NAME}_BINARY_DIR}/lib" CACHE PATH
    "LIBRARY_OUTPUT_PATH" FORCE )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH )

# DEPENDENCIES: this code has to be placed before adding any library or
# executable so that these are linked properly against the dependencies
IF( DEFINED ${PROJECT_NAME}_DEPENDS OR DEFINED BUILD_WITH OR DEFINED LINK_WITH )
    # load macro
    IF( NOT EXISTS "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
        MESSAGE( FATAL_ERROR
            "\nSorry, could not find MacroCheckDeps.cmake...\n"
            "Please set CMAKE_MODULE_PATH correctly with: "
            "cmake -DCMAKE_MODULE_PATH=<path_to_cmake_modules>" )
    ENDIF()
    INCLUDE( "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
    CHECK_DEPS()
ENDIF()

# input directories
ADD_SUBDIRECTORY( source )

# create uninstall configuration file 
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake"
                IMMEDIATE @ONLY )

# create uninstall target
ADD_CUSTOM_TARGET( uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" )

# create configuration file from .in file
CONFIGURE_FILE( "${${PROJECT_NAME}_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
                "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY )

# install configuration file
INSTALL( FILES "${${PROJECT_NAME}_BINARY_DIR}/${PROJECT_NAME}Config.cmake" DESTINATION . )

# display status message for important variables
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "BUILD_SHARED_LIBS = ${BUILD_SHARED_LIBS}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}" )
MESSAGE( STATUS "CMAKE_MODULE_PATH = ${CMAKE_MODULE_PATH}" )
MESSAGE( STATUS "BUILD_LCCD_TESTS = ${BUILD_LCCD_TESTS}" )
MESSAGE( STATUS "BUILD_CONDDB_TESTS = ${BUILD_CONDDB_TESTS}" )
MESSAGE( STATUS "LCCD_DB_INIT = ${LCCD_DB_INIT}" )
MESSAGE( STATUS "INSTALL_DOC = ${INSTALL_DOC}" )
MESSAGE( STATUS "Change a value with: cmake -D<Variable>=<Value>" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS )

# force some variables that could be defined in the command line
# to be written to cache
SET( BUILD_SHARED_LIBS "${BUILD_SHARED_LIBS}" CACHE BOOL
    "Set to OFF to build static libraries" FORCE )
SET( CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE PATH
    "Where to install ${PROJECT_NAME}" FORCE )
SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )
SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE PATH
    "Path to custom CMake Modules" FORCE )
SET( BUILD_LCCD_TESTS "${BUILD_LCCD_TESTS}" CACHE BOOL
    "Set to ON to build LCCD tests" FORCE )
SET( BUILD_CONDDB_TESTS "${BUILD_CONDDB_TESTS}" CACHE BOOL
    "Set to ON to build CONDDB tests" FORCE )
SET( LCCD_DB_INIT "${LCCD_DB_INIT}" CACHE STRING
    "DB initialization" FORCE )
SET( INSTALL_DOC "${BUILD_DOC}" CACHE BOOL
    "Set to OFF to skip build/install Documentation" FORCE )

# export build settings
INCLUDE( CMakeExportBuildSettings )
CMAKE_EXPORT_BUILD_SETTINGS( "${PROJECT_NAME}BuildSettings.cmake" )

# export library dependencies (keep this as the last line in the file)
EXPORT_LIBRARY_DEPENDENCIES( "${PROJECT_NAME}LibDeps.cmake" )
