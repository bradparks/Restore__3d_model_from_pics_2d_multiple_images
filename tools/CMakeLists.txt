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

# Project tools binaries dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/tools/bin)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(SYSTEM ${VTK_INCLUDE_DIRS})
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${VTK_DEFINITIONS})

set(LDIR_TARGET_NAME "light_directions")
add_executable(${LDIR_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/light-directions/main.cpp)
enable_cxx_11(${LDIR_TARGET_NAME})
target_link_libraries(${LDIR_TARGET_NAME} ${RESTORE_LIB})

set(CAMS_TARGET_NAME "show_cameras")
add_executable(${CAMS_TARGET_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/show-cameras/main.cpp)
enable_cxx_11(${CAMS_TARGET_NAME})
target_link_libraries(${CAMS_TARGET_NAME} ${RESTORE_LIB})
