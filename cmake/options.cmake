# Build type
IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF()

# Project configuration
SET(CMAKE_VERBOSE_MAKEFILE                                            FALSE)

OPTION(USE_CLANG             "Use Clang compiler"                       OFF)
OPTION(USE_CODE_COVERAGE     "GCC: Use gcov for code coverage analysis" OFF)
OPTION(USE_ADDRESS_SANITIZER "Fast memory error detector"               OFF)
OPTION(USE_MEMORY_SANITIZER  "Clang: Detect unitialized reads"          OFF)
OPTION(USE_THREAD_SANITIZER  "Detect data races"                        OFF)
OPTION(WARNINGS_ARE_ERRORS   "Treat all warnings ass errors"            OFF)
OPTION(WARNINGS_ANSI_ISO     "Be standard ISO consistent"                ON)
OPTION(WARNINGS_EFF_CXX      "Use Effective C++ guidelines"             OFF)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}" DESTINATION include)
