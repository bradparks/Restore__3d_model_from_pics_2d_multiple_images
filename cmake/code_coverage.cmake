# Code Coverage using gcc features
find_program(GCOV_PATH gcov)
find_program(LCOV_PATH lcov)
find_program(GENHTML_PATH genhtml)
find_program(GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/test)

if(NOT GCOV_PATH)
    message(FATAL_ERROR "gcov not found! Aborting...")
endif()

if(NOT CMAKE_COMPILER_IS_GNUCXX)
    if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        message(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
    endif()
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(WARNING "Code coverage results with an optimized (non-Debug) \
    build may be misleading")
endif()

# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests.
#                       MUST return ZERO always, even on errors.
#                       If not, no coverage report will be created!
# Param _outputname     lcov output is generated as _outputname.info
#                       HTML report is generated in _outputname/index.html
function(setup_target_for_coverage _targetname _testrunner _outputname)
    if(NOT LCOV_PATH)
        message(FATAL_ERROR "lcov not found! Aborting...")
    endif()

    if(NOT GENHTML_PATH)
        message(FATAL_ERROR "genhtml not found! Aborting...")
    endif()

    # Remove and system headers and sources
    set(LCOV_REMOVE ${_outputname}.info '/usr/*' 'test/*' 'tools/*')
    # Remove thirdparty software
    set(LCOV_REMOVE ${LCOV_REMOVE} 'thirdparty/*')

    # Setup target
    add_custom_target(${_targetname}
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
    add_custom_command(TARGET ${_targetname} POST_BUILD
        COMMAND ;
        COMMENT "Open ./${_outputname}/index.html in your browser to view the \
        coverage report."
    )

endfunction() # setup_target_for_coverage

# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests
# Param _outputname     cobertura output is generated as _outputname.xml
function(setup_target_for_coverage_cobertura _targetname _testrunner _outputname)
    if(NOT PYTHON_EXECUTABLE)
        message(FATAL_ERROR "Python not found! Aborting...")
    endif()

    if(NOT GCOVR_PATH)
        message(FATAL_ERROR "gcovr not found! Aborting...")
    endif()

    add_custom_target(${_targetname}
        # Run tests
        ${_testrunner} ${ARGV3}
        # Running gcovr
        COMMAND ${GCOVR_PATH} -x -r ${CMAKE_SOURCE_DIR} -e 
            '${CMAKE_SOURCE_DIR}/test/'  -o ${_outputname}.xml
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Running gcovr to produce Cobertura code coverage report."
    )

    # Show info where to find the report
    add_custom_command(TARGET ${_targetname} POST_BUILD
        COMMAND ;
        COMMENT "Cobertura code coverage report saved in ${_outputname}.xml."
    )
endfunction()
