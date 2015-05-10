# Set compiler flags to enable coverage
IF(COVERAGE_AVAILABLE)
    SET(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
ENDIF()

# Build test executables
INCLUDE_DIRECTORIES("${gtest_SOURCE_DIR}/include")
SET(TEST_BIN "${PROJECT_NAME}-unittests")
ADD_EXECUTABLE(${TEST_BIN} ${TEST_SOURCES} ${PROJECT_SOURCES} 
    ${RESTORE_HEADERS})
TARGET_LINK_LIBRARIES(${TEST_BIN} gtest_main ${OpenCV_LIBS} ${Boost_LIBRARIES})

## Setup unit testing
ADD_CUSTOM_TARGET(tests ${EXECUTABLE_OUTPUT_PATH}/${TEST_BIN}
    DEPENDS ${TEST_BIN} COMMENT "Executing unit tests..." 
    SOURCES ${TEST_SRCS})

## Setup code coverage if available
IF(COVERAGE_AVAILABLE)
    SETUP_TARGET_FOR_COVERAGE(${PROJECT_NAME}-coverage ${TEST_BIN} coverage)
ENDIF()

