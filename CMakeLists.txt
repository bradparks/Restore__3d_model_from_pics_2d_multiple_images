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

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(restore LANGUAGES CXX)

include(cmake/versions.cmake REQUIRED)
include(cmake/options.cmake REQUIRED)
include(cmake/compiler_options.cmake REQUIRED)
include(cmake/documentation_targets.cmake REQUIRED)
include(cmake/macros.cmake REQUIRED)
include(cmake/ide_project_layout.cmake REQUIRED)

# Required packages
find_package(OpenCV REQUIRED core imgproc calib3d highgui)
find_package(Boost COMPONENTS filesystem system REQUIRED)
find_package(VTK REQUIRED NO_MODULE)
find_package(Qt5Widgets REQUIRED QUIET)

# Add build targets
set(RESTORE_LIB ${PROJECT_NAME}_lib)
add_subdirectory(src)        # Restore modules, each with own CMakeLists.txt
add_subdirectory(thirdparty) # googletest
add_subdirectory(test)       # Tests
add_subdirectory(tools)      # Small helper tools, etc.
add_subdirectory(perf)       # benchmarking stuff
