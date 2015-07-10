IF(CMAKE_COMPILER_IS_GNUCXX)
    IF(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
        MESSAGE(FATAL_ERROR "${PROJECT_NAME} C++11 requires at least gcc 4.7.")
    ENDIF()

    IF(USE_CODE_COVERAGE)
        INCLUDE(cmake/code_coverage.cmake REQUIRED)
    ENDIF()
    INCLUDE(cmake/compiler_flags.cmake REQUIRED)
    INCLUDE(cmake/dynamic_analyzer_options.cmake REQUIRED)
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    IF(NOT EXISTS ${CMAKE_CXX_COMPILER})
        MESSAGE(FATAL_ERROR "${PROJECT_NAME} Clang++ compiler not found.")
    ENDIF()

    IF(USE_CODE_COVERAGE)
        MESSAGE(WARNING "Code coverage only with GCC available.")
    ENDIF()
    INCLUDE(cmake/compiler_flags.cmake REQUIRED)
    INCLUDE(cmake/dynamic_analyzer_options.cmake REQUIRED)
ENDIF()

# Add user supplied extra options
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS}" 
    CACHE INTERNAL "Extra compiler options")
SET(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE}" 
    CACHE INTERNAL "Extra compiler options for Release build")
SET(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG}" 
    CACHE INTERNAL "Extra compiler options for Debug build")

