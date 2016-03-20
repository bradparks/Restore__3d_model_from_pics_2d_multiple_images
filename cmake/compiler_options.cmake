if(CMAKE_COMPILER_IS_GNUCXX)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.7)
        message(FATAL_ERROR "${PROJECT_NAME} C++11 requires at least gcc 4.7.")
    endif()

    if(USE_CODE_COVERAGE)
        include(cmake/code_coverage.cmake REQUIRED)
    endif()
    include(cmake/compiler_flags.cmake REQUIRED)
    include(cmake/dynamic_analyzer_options.cmake REQUIRED)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if(NOT EXISTS ${CMAKE_CXX_COMPILER})
        message(FATAL_ERROR "${PROJECT_NAME} Clang++ compiler not found.")
    endif()

    if(USE_CODE_COVERAGE)
        message(WARNING "Code coverage only with GCC available.")
    endif()
    include(cmake/compiler_flags.cmake REQUIRED)
    include(cmake/dynamic_analyzer_options.cmake REQUIRED)
endif()

# Add user supplied extra options
set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS}"
    CACHE INTERNAL "Extra compiler options")
set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE}"
    CACHE INTERNAL "Extra compiler options for Release build")
set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG}"
    CACHE INTERNAL "Extra compiler options for Debug build")

