#########################################
# cmake file for building LCCD tests
# @author Jan Engels, DESY
#########################################

INCLUDE_DIRECTORIES( . )

# helper macro for adding tests
MACRO( ADD_LCCD_TEST _testname )

        IF( BUILD_TESTING )
            ADD_EXECUTABLE( ${_testname} ${ARGN} )
        ELSE()
            ADD_EXECUTABLE( ${_testname} EXCLUDE_FROM_ALL ${ARGN} )
        ENDIF()

        TARGET_LINK_LIBRARIES( ${_testname} lccd )

        #INSTALL( TARGETS ${_testname} DESTINATION bin/lccd_tests )

ENDMACRO( ADD_LCCD_TEST )


# ---------------------------------------------------------------

ADD_LCCD_TEST( createcalfile ./createcalfile.cc )
ADD_LCCD_TEST( readcalfile ./readcalfile.cc ./CalibrationConstant.cc )
ADD_LCCD_TEST( readdatafile ./readdatafile.cc )
ADD_LCCD_TEST( testdbfile ./testdbfile.cc )

IF( LCCD_CONDDBMYSQL )

    ADD_LCCD_TEST( calfilefromdb ./conddbtests/calfilefromdb )
    ADD_LCCD_TEST( calfiletodb ./conddbtests/calfiletodb )
    ADD_LCCD_TEST( createdbfile ./conddbtests/createdbfile )
    ADD_LCCD_TEST( tagdbfolder ./conddbtests/tagdbfolder )
    ADD_LCCD_TEST( multiblefolders ./conddbtests/multiblefolders )

ENDIF()

# ---------------------------------------------------------------

