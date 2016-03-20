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
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
set(TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/sanity_check.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/calib/light_direction_model_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/calib/ransac_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera_extrinsics_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera_intrinsics_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/dataset_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/polydata_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filtering/segmentation_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/dataset_reader_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/dual_quaternion_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/quaternion_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/utils_test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/voxel_carving_test.cpp)

# Add coverage flags for test executable, if enabled
if(USE_CODE_COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
endif()

# Build test executables
include_directories(SYSTEM "${gtest_SOURCE_DIR}/include")
set(TEST_BIN "${PROJECT_NAME}-unittests")
add_executable(${TEST_BIN} ${TEST_SOURCES})
enable_cxx_11(${TEST_BIN})
include_directories(SYSTEM ${VTK_INCLUDE_DIRS})
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${VTK_DEFINITIONS})
target_link_libraries(${TEST_BIN} gtest_main ${RESTORE_LIB})

# Setup unit testing
add_custom_target(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${TEST_BIN}
    DEPENDS ${TEST_BIN} COMMENT "Executing unit tests..."
    SOURCES ${TEST_SOURCES})

if(USE_CODE_COVERAGE AND CMAKE_COMPILER_IS_GNUCXX)
    setup_target_for_coverage(${PROJECT_NAME}-coverage ${TEST_BIN} coverage)
endif()
