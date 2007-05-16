#################################
# cmake file for building LCCD
# @author Jan Engels, DESY
#################################

####################################################################################################
### INCLUDES
####################################################################################################

# include directories
INCLUDE_DIRECTORIES( "${${PROJECT_NAME}_SOURCE_DIR}/source/include" )

# install include files
INSTALL( DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/source/include" DESTINATION .
		PATTERN "*~" EXCLUDE
		PATTERN "*CVS*" EXCLUDE )


####################################################################################################
### SOURCES
####################################################################################################

# require proper c++
ADD_DEFINITIONS( "-Wall -ansi -pedantic" )

#----- need long long for int64 for now ------
#FIXME: should establish wether we are on a 32bit or 64 bit machine ....
ADD_DEFINITIONS( "-Wno-long-long" )

ADD_DEFINITIONS( "-DLCCD_DB_INIT_DEFAULT=\\\"${LCCD_DB_INIT}\\\"" )

# get list of all source files in this directory
AUX_SOURCE_DIRECTORY( src lib_srcs )

####################################################################################################
### DEPENDENCIES
####################################################################################################

# load macro
IF( DEFINED ${PROJECT_NAME}_DEPENDS OR DEFINED BUILD_WITH OR DEFINED LINK_WITH )
    # load macro
    IF( NOT EXISTS "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
        MESSAGE( FATAL_ERROR "\nSorry, could not find MacroCheckDeps.cmake...\n"
            "Please set CMAKE_MODULE_PATH correctly with: "
            "cmake -DCMAKE_MODULE_PATH=<path_to_cmake_modules>" )
    ENDIF()
    INCLUDE( "${CMAKE_MODULE_PATH}/MacroCheckDeps.cmake" )
    CHECK_DEPS()
ENDIF()


####################################################################################################
### LIBRARY
####################################################################################################

# create library
ADD_LIBRARY( lib_${PROJECT_NAME} ${lib_srcs} )

# create symbolic lib target for calling target lib_XXX
ADD_CUSTOM_TARGET( lib DEPENDS lib_${PROJECT_NAME} )

# change lib_target properties
SET_TARGET_PROPERTIES( lib_${PROJECT_NAME} PROPERTIES
	# create *nix style library versions + symbolic links
	VERSION ${${PROJECT_NAME}_VERSION}
	SOVERSION ${${PROJECT_NAME}_SOVERSION}
	# allow creating static and shared libs without conflicts
	CLEAN_DIRECT_OUTPUT 1
	# avoid conflicts between library and binary target names
	OUTPUT_NAME lccd )

# install library
INSTALL( TARGETS lib_${PROJECT_NAME} DESTINATION lib PERMISSIONS
		OWNER_READ OWNER_WRITE OWNER_EXECUTE
		GROUP_READ GROUP_EXECUTE
		WORLD_READ WORLD_EXECUTE )


####################################################################################################
### BINS
####################################################################################################

SET( lccd_tests
	
	createcalfile
	multiblefolders
	readcalfile
	readdatafile
	testdbfile
)

SET( conddb_tests
	
	calfilefromdb
	calfiletodb
	createdbfile
	tagdbfolder
)

IF( BUILD_LCCD_TESTS )
	LIST( APPEND test_bins ${lccd_tests} )
ENDIF()
IF( BUILD_CONDDB_TESTS )
	LIST( APPEND test_bins ${conddb_tests} )
ENDIF()


IF( test_bins )
	# create symbolic bin target for calling targets bin_XXX
	ADD_CUSTOM_TARGET( bin )

	# loop through list of binaries
	FOREACH( bin ${test_bins} )
		# create target for binary
		ADD_EXECUTABLE( bin_${bin} EXCLUDE_FROM_ALL
			test/${bin}.cc
			test/CalibrationConstant.cc
		)
		# tell bin target to call this bin_XXX target
		ADD_DEPENDENCIES( bin bin_${bin} )
		# avoid conflicts between library and binary target names
		SET_TARGET_PROPERTIES( bin_${bin} PROPERTIES OUTPUT_NAME ${bin} )
		# link against project library and DLL lib
		TARGET_LINK_LIBRARIES( bin_${bin} lib_${PROJECT_NAME} )
		# install binary commando
		INSTALL( TARGETS bin_${bin} DESTINATION test )
	ENDFOREACH()
ENDIF()

