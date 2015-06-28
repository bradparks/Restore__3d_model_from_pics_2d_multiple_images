# Code Coverage using gcc features
FIND_PROGRAM(GCOV_PATH gcov)
FIND_PROGRAM(LCOV_PATH lcov)
FIND_PROGRAM(GENHTML_PATH genhtml)
FIND_PROGRAM(GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/test)

IF(NOT GCOV_PATH)
    MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
ENDIF()

IF(NOT CMAKE_COMPILER_IS_GNUCXX)
    IF(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        MESSAGE(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
    ENDIF()
ENDIF() 

IF(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE(WARNING "Code coverage results with an optimized (non-Debug) \
    build may be misleading")
ENDIF() 

# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests.
#                       MUST return ZERO always, even on errors.
#                       If not, no coverage report will be created!
# Param _outputname     lcov output is generated as _outputname.info
#                       HTML report is generated in _outputname/index.html
FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner _outputname)
    IF(NOT LCOV_PATH)
        MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
    ENDIF()

    IF(NOT GENHTML_PATH)
        MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
    ENDIF() 

    # Remove and system headers and sources
    SET(LCOV_REMOVE ${_outputname}.info '/usr/*' 'test/*' 'tools/*')
    # Remove thirdparty software
    SET(LCOV_REMOVE ${LCOV_REMOVE} 'thirdparty/*')

    # Setup target
    ADD_CUSTOM_TARGET(${_targetname}
        # Cleanup lcov
        ${LCOV_PATH} --directory . --zerocounters
        # Run tests
        COMMAND ${_testrunner} ${ARGV3}
        # Capturing lcov counters and generating report
        COMMAND ${LCOV_PATH} --directory . --capture 
            --output-file ${_outputname}.info
        COMMAND ${LCOV_PATH} --remove ${LCOV_REMOVE} 
            --output-file ${_outputname}.info.cleaned
        COMMAND ${GENHTML_PATH} -o ${_outputname} 
            ${_outputname}.info.cleaned

        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Resetting code coverage counters to zero.\nProcessing \
        code coverage counters and generating report."
    )

    # Show info where to find the report
    ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
        COMMAND ;
        COMMENT "Open ./${_outputname}/index.html in your browser to view the \
        coverage report."
    )

ENDFUNCTION() # SETUP_TARGET_FOR_COVERAGE

# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests
# Param _outputname     cobertura output is generated as _outputname.xml
FUNCTION(SETUP_TARGET_FOR_COVERAGE_COBERTURA _targetname _testrunner _outputname)
    IF(NOT PYTHON_EXECUTABLE)
        MESSAGE(FATAL_ERROR "Python not found! Aborting...")
    ENDIF()

    IF(NOT GCOVR_PATH)
        MESSAGE(FATAL_ERROR "gcovr not found! Aborting...")
    ENDIF()

    ADD_CUSTOM_TARGET(${_targetname}
        # Run tests
        ${_testrunner} ${ARGV3}
        # Running gcovr
        COMMAND ${GCOVR_PATH} -x -r ${CMAKE_SOURCE_DIR} -e 
            '${CMAKE_SOURCE_DIR}/test/'  -o ${_outputname}.xml
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running gcovr to produce Cobertura code coverage report."
    )

    # Show info where to find the report
    ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
        COMMAND ;
        COMMENT "Cobertura code coverage report saved in ${_outputname}.xml."
    )
ENDFUNCTION()
