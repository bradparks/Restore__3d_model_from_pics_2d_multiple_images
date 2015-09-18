# Copyright (c) 2015, Kai Wolf
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Aggregate all test sources
FILE(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# Add coverage flags for test executable, if enabled
IF(USE_CODE_COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS 
        "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
ENDIF()

# Build test executables
INCLUDE_DIRECTORIES(SYSTEM "${gtest_SOURCE_DIR}/include")
SET(TEST_BIN "${PROJECT_NAME}-unittests")
ADD_EXECUTABLE(${TEST_BIN} ${TEST_SOURCES})
ENABLE_CXX_11(${TEST_BIN})
INCLUDE_DIRECTORIES(SYSTEM ${VTK_INCLUDE_DIRS})
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${VTK_DEFINITIONS})
TARGET_LINK_LIBRARIES(${TEST_BIN} gtest_main ${RESTORE_LIB})

# Setup unit testing
ADD_CUSTOM_TARGET(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_BIN}
    DEPENDS ${TEST_BIN} COMMENT "Executing unit tests..."
    SOURCES ${TEST_SOURCES})

IF(USE_CODE_COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
    SETUP_TARGET_FOR_COVERAGE(${PROJECT_NAME}-coverage ${TEST_BIN} coverage)
ENDIF()
