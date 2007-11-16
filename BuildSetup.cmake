#############################################################################
# cmake build setup for LCCD
#
# For building lccd with cmake type:
# (1) $ mkdir build
# (2) $ cd build
# (3) $ cmake -C ../BuildSetup.cmake ..
# (4) $ make install
#
# @author Jan Engels, DESY
#############################################################################


#############################################################################
# Setup path variables
#############################################################################

# ILC_HOME
SET( ILC_HOME "/afs/desy.de/group/it/ilcsoft/v01-03"
    CACHE PATH "Path to ILC Software" FORCE )

# Path to LCIO
SET( LCIO_HOME "${ILC_HOME}/lcio/v01-09"
    CACHE PATH "Path to LCIO" FORCE )

# CMake Modules Path
SET( CMAKE_MODULE_PATH "${ILC_HOME}/CMakeModules/v01-04"
    CACHE PATH "Path to CMake Modules" FORCE )

#############################################################################
# CondDBMySQL
#############################################################################

# Enable this if you want to build LCCD with CondDBMySQL
SET( BUILD_WITH "CondDBMySQL" CACHE STRING "Build LCCD with these optional packages" FORCE )

# Path to CondDBMySQL
SET( CondDBMySQL_HOME "${ILC_HOME}/CondDBMySQL/CondDBMySQL_ILC-0-5-10"
    CACHE PATH "Path to CondDBMySQL" FORCE )

# Enable this if you want to build the CondDB tests
#SET( BUILD_CONDDB_TESTS ON CACHE BOOL "Set to ON to build CondDB tests" FORCE )

#############################################################################
# Project options
#############################################################################

#SET( INSTALL_DOC OFF CACHE BOOL "Set to OFF to skip build/install Documentation" FORCE )

#SET( BUILD_LCCD_TESTS ON CACHE BOOL "Set to ON to build LCCD tests" FORCE )
#SET( LCCD_DB_INIT "localhost:lccd_test:calvin:hobbes" CACHE STRING "DB initialization" FORCE )

# set cmake build type, default value is: RelWithDebInfo
# possible options are: None Debug Release RelWithDebInfo MinSizeRel
#SET( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build" FORCE )

#############################################################################
# Advanced options
#############################################################################

#SET( BUILD_SHARED_LIBS OFF CACHE BOOL "Set to OFF to build static libraries" FORCE )

# installation path for LCCD
#SET( CMAKE_INSTALL_PREFIX "/foo/bar" CACHE STRING "Where to install LCCD" FORCE )
