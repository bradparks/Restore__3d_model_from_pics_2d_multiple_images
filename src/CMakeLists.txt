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

# Configure CMake generated files
set(ASSETS_PATH "${PROJECT_SOURCE_DIR}/assets")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/io/assets_path.hpp.in.cmake"
               "${CMAKE_CURRENT_SOURCE_DIR}/io/assets_path.hpp")

# Build lib from Restore sources
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/calibration/light_direction_model.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/calibration/ransac.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/calibration/ransac.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera_extrinsics.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera_intrinsics.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/dataset.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/polydata.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/types/triangle.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/types/vec3f.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filtering/segmentation.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/filtering/segmentation.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/assets_path.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/dataset_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/io/dataset_reader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/dual_quaternion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/dual_quaternion.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/math_utils.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/quaternion.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/math/quaternion.hpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}/rendering/mc/marching_cubes.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/main_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/main_window.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/dataset_list_widget_item.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/dataset_list_widget_item.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/model_widget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/model_widget.hpp)

add_library(${RESTORE_LIB} SHARED ${SOURCE_FILES})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(SYSTEM ${VTK_INCLUDE_DIRS})
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
include_directories(SYSTEM ${Qt5Widgets_INCLUDE_DIRS})
set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${VTK_DEFINITIONS})
target_link_libraries(${RESTORE_LIB} PUBLIC ${OpenCV_LIBS} ${VTK_LIBRARIES}
                      ${Boost_LIBRARIES})
enable_cxx_11(${RESTORE_LIB})

if (USE_IWYU AND iwyu_path)
    set_property(TARGET ${RESTORE_LIB} PROPERTY CXX_INCLUDE_WHAT_YOU_USE
                 ${iwyu_path} -Xiwyu --no_comments)
endif ()

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/main.cxx)
enable_cxx_11(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${RESTORE_LIB} Qt5::Widgets)
install(TARGETS ${PROJECT_BIN} DESTINATION bin)
