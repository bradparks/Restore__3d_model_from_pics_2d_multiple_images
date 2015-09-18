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

# Configure CMake generated files
SET(ASSETS_PATH "${PROJECT_SOURCE_DIR}/assets")
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/io/assets_path.hpp.in.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/io/assets_path.hpp")

# Build lib from Restore sources
ADD_SOURCES(CALIB_SOURCES calibration)
ADD_SOURCES(COMMON_SOURCES common)
ADD_SOURCES(FILTER_SOURCES filtering)
ADD_SOURCES(IO_SOURCES io)
ADD_SOURCES(MATH_SOURCES math)
ADD_SOURCES(RENDER_SOURCES rendering)
ADD_LIBRARY(${RESTORE_LIB} SHARED ${CALIB_SOURCES} ${COMMON_SOURCES} 
    ${FILTER_SOURCES} ${IO_SOURCES} ${MATH_SOURCES} ${RENDER_SOURCES})
INCLUDE_DIRECTORIES(SYSTEM ${VTK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${VTK_DEFINITIONS})
TARGET_LINK_LIBRARIES(${RESTORE_LIB} PUBLIC ${OpenCV_LIBS} ${VTK_LIBRARIES}
    ${Boost_LIBRARIES})
ENABLE_CXX_11(${RESTORE_LIB})

ADD_EXECUTABLE(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx)
ENABLE_CXX_11(${PROJECT_NAME})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${RESTORE_LIB})
INSTALL(TARGETS ${PROJECT_BIN} DESTINATION bin)
