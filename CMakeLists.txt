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
