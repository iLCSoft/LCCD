#########################################
# cmake file for building LCCD tests
# @author Jan Engels, DESY
#########################################


# ---------------------------------------------------------------

OPTION( BUILD_LCCD_TESTS "Set to ON to build LCCD tests" OFF )

IF( BUILD_LCCD_TESTS )

    LIST( APPEND test_bins
        createcalfile
        readcalfile
        readdatafile
        testdbfile
    )

ENDIF()



# ---------------------------------------------------------------

OPTION( BUILD_CONDDB_TESTS "Set to ON to build CondDB tests" OFF )

IF( BUILD_CONDDB_TESTS )

    IF( NOT CondDBMySQL_FOUND )
        MESSAGE( FATAL_ERROR "could not find CondDBMySQL required to build CONDDB_TESTS" )
    ENDIF()

    LIST( APPEND test_bins
        calfilefromdb
        calfiletodb
        createdbfile
        tagdbfolder
        multiblefolders
    )

ENDIF()



ADD_CUSTOM_TARGET( tests )

IF( test_bins )

    FOREACH( _test ${test_bins} )

        SET_SOURCE_FILES_PROPERTIES( ${_test}.cc PROPERTIES
            # FIXME -pedantic crashes with gcc 3.4.6
          COMPILE_FLAGS "-Wall -ansi"
        )

        ADD_EXECUTABLE( ${_test} ./${_test}.cc ./CalibrationConstant.cc )
        TARGET_LINK_LIBRARIES( ${_test} lccd )
        ADD_DEPENDENCIES( tests ${_test} )

        INSTALL( TARGETS ${_test} DESTINATION bin/lccd_tests )

    ENDFOREACH()

ENDIF()

