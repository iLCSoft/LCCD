#########################################
# cmake file for building LCCD tests
# @author Jan Engels, DESY
#########################################

INCLUDE_DIRECTORIES( . )

ADD_CUSTOM_TARGET( tests )
MACRO( ADD_LCCD_TEST _testname )

        #ADD_EXECUTABLE( ${_testname} ./${_test}.cc ./CalibrationConstant.cc )
        ADD_EXECUTABLE( ${_testname} ${ARGN} )
        TARGET_LINK_LIBRARIES( ${_testname} lccd )
        ADD_DEPENDENCIES( tests ${_testname} )

        #INSTALL( TARGETS ${_testname} DESTINATION bin/lccd_tests )

ENDMACRO( ADD_LCCD_TEST )


# ---------------------------------------------------------------

OPTION( BUILD_LCCD_TESTS "Set to ON to build LCCD tests" OFF )

IF( BUILD_LCCD_TESTS )
    
    ADD_LCCD_TEST( createcalfile ./createcalfile.cc ./CalibrationConstant.cc )
    ADD_LCCD_TEST( readcalfile ./readcalfile ./CalibrationConstant.cc )
    ADD_LCCD_TEST( readdatafile ./readdatafile ./CalibrationConstant.cc )
    ADD_LCCD_TEST( testdbfile ./testdbfile ./CalibrationConstant.cc )

ENDIF()



# ---------------------------------------------------------------

OPTION( BUILD_CONDDB_TESTS "Set to ON to build CondDB tests" OFF )

IF( BUILD_CONDDB_TESTS )

    IF( NOT CondDBMySQL_FOUND )
        MESSAGE( FATAL_ERROR "could not find CondDBMySQL required to build CONDDB_TESTS" )
    ENDIF()

    ADD_LCCD_TEST( calfilefromdb ./conddbtests/calfilefromdb ./CalibrationConstant.cc )
    ADD_LCCD_TEST( calfiletodb ./conddbtests/calfiletodb ./CalibrationConstant.cc )
    ADD_LCCD_TEST( createdbfile ./conddbtests/createdbfile ./CalibrationConstant.cc )
    ADD_LCCD_TEST( tagdbfolder ./conddbtests/tagdbfolder ./CalibrationConstant.cc )
    ADD_LCCD_TEST( multiblefolders ./conddbtests/multiblefolders ./CalibrationConstant.cc )

ENDIF()
