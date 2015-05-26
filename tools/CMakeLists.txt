# Project tools binaries dir
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/tools/bin)

FILE(GLOB_RECURSE show_cameras_SRCS
    ${PROJECT_SOURCE_DIR}/tools/show-cameras/*.hpp
    ${PROJECT_SOURCE_DIR}/tools/show-cameras/*.cpp)
ADD_EXECUTABLE(show_cameras ${show_cameras_SRCS} ${PROJECT_SOURCES})
TARGET_LINK_LIBRARIES(show_cameras ${OpenCV_LIBS} ${Boost_LIBRARIES}
    ${VTK_LIBRARIES})
