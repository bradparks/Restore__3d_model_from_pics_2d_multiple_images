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

CMAKE_MINIMUM_REQUIRED(VERSION 3.2 FATAL_ERROR)
PROJECT(restore LANGUAGES CXX)

INCLUDE(cmake/versions.cmake REQUIRED)
INCLUDE(cmake/options.cmake REQUIRED)
INCLUDE(cmake/compiler_options.cmake REQUIRED)
INCLUDE(cmake/documentation_targets.cmake REQUIRED)
INCLUDE(cmake/macros.cmake REQUIRED)

# Required packages
FIND_PACKAGE(OpenCV REQUIRED core imgproc calib3d highgui)
FIND_PACKAGE(Boost COMPONENTS filesystem system REQUIRED)
FIND_PACKAGE(VTK REQUIRED NO_MODULE)

# Add build targets
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/src")
SET(RESTORE_LIB ${PROJECT_NAME}_lib)
ADD_SUBDIRECTORY(src)        # Resore modules, each wit separate CMakeLists.txt
ADD_SUBDIRECTORY(thirdparty) # Gtest, etc.
ADD_SUBDIRECTORY(test)       # Tests
ADD_SUBDIRECTORY(tools)      # Small helper tools, etc.
