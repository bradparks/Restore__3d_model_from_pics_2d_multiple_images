# Unset compiler flags for thirdparty dependencies
UNSET(CMAKE_C_FLAGS)
UNSET(CMAKE_CXX_FLAGS)

# GoogleTest for unit testing
ADD_SUBDIRECTORY(gtest-1.7.0)
