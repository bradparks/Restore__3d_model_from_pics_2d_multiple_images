# Copyright (c) 2015-2016, Kai Wolf
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

# Aggregate all benchmark sources
set(PERF_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/voxel_carving_perf.cpp)

# Build perf executable
include_directories(${PROJECT_SOURCE_DIR}/src)
set(PERF_BIN "${PROJECT_NAME}-perf")
add_executable(${PERF_BIN} ${PERF_SOURCES})
enable_cxx_11(${PERF_BIN})
include_directories(SYSTEM ${VTK_INCLUDE_DIRS})
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${VTK_DEFINITIONS})
target_link_libraries(${PERF_BIN} benchmark ${RESTORE_LIB})
