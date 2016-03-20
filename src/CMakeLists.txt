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
set(ASSETS_PATH "${PROJECT_SOURCE_DIR}/assets")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/io/assets_path.hpp.in.cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/io/assets_path.hpp")

# Build lib from Restore sources
set(CALIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/calibration/light_direction_model.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/calibration/ransac.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/calibration/ransac.hpp)
set(COMMON_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera_extrinsics.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera_intrinsics.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/dataset.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/polydata.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/types/triangle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/types/vec3f.hpp)
set(FILTER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/filtering/segmentation.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filtering/segmentation.cpp)
set(IO_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/io/assets_path.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/dataset_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/dataset_reader.hpp)
set(MATH_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/math/dual_quaternion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/dual_quaternion.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/math_utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/quaternion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/quaternion.hpp)
set(RENDER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/bounding_box.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/bounding_box.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/cv_utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/light_dir_estimation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/light_dir_estimation.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/mesh_coloring.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/mesh_coloring.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/voxel_carving.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/voxel_carving.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/mc/basedef.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/mc/lookup.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/mc/lookup.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/mc/marching_cubes.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/mc/marching_cubes.hpp)
add_library(${RESTORE_LIB} SHARED ${CALIB_SOURCES} ${COMMON_SOURCES}
    ${FILTER_SOURCES} ${IO_SOURCES} ${MATH_SOURCES} ${RENDER_SOURCES})
include_directories(SYSTEM ${VTK_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${VTK_DEFINITIONS})
target_link_libraries(${RESTORE_LIB} PUBLIC ${OpenCV_LIBS} ${VTK_LIBRARIES}
    ${Boost_LIBRARIES})
enable_cxx_11(${RESTORE_LIB})

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx)
enable_cxx_11(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${RESTORE_LIB})
install(TARGETS ${PROJECT_BIN} DESTINATION bin)
