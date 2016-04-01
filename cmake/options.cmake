# Build type
#if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
#endif()

# Project configuration
set(CMAKE_VERBOSE_MAKEFILE                                            FALSE)
set(CMAKE_AUTOMOC                                                        ON)
set(CMAKE_INCLUDE_CURRENT_DIR                                            ON)

option(USE_CLANG             "Use Clang compiler"                       OFF)
option(USE_CODE_COVERAGE     "GCC: Use gcov for code coverage analysis" OFF)
option(USE_ADDRESS_SANITIZER "Fast memory error detector"               OFF)
option(USE_MEMORY_SANITIZER  "Clang: Detect unitialized reads"          OFF)
option(USE_THREAD_SANITIZER  "Detect data races"                        OFF)
option(WARNINGS_ARE_ERRORS   "Treat all warnings ass errors"            OFF)
option(WARNINGS_ANSI_ISO     "Be standard ISO consistent"                ON)
option(WARNINGS_EFF_CXX      "Use Effective C++ guidelines"             OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}"
    DESTINATION include)
