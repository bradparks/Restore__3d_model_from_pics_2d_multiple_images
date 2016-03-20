# RELEASE Flags
list(APPEND EXTRA_C_FLAGS_RELEASE -Wall)
list(APPEND EXTRA_C_FLAGS_RELEASE -O3)

# DEBUG Flags
## Show all warnings
list(APPEND EXTRA_C_FLAGS -Wall)
## Enable some extra warning flags
list(APPEND EXTRA_C_FLAGS -Wextra)
## Warn if anything depends upn the size of a function or of void
list(APPEND EXTRA_C_FLAGS -Wpointer-arith)
## Avoid int i = i
list(APPEND EXTRA_C_FLAGS -Winit-self)
## Warn if an unitialized identifier is evaluated
list(APPEND EXTRA_C_FLAGS -Wundef)
## Warn whenever a local variable shadows another local/global one
list(APPEND EXTRA_C_FLAGS -Wshadow)
## Warn if a pointer is cast such that alignment of target is increased
list(APPEND EXTRA_C_FLAGS -Wcast-align)
## Give string consts the type const char[length]
list(APPEND EXTRA_C_FLAGS -Wwrite-strings)
## Warn whenever a poinster is cast to remove a type qualifier from target
list(APPEND EXTRA_C_FLAGS -Wcast-qual)
## Warn whenever a switch does not have a default case
list(APPEND EXTRA_C_FLAGS -Wswitch-default)
## Warn whenever a switch has an index of enumerated type and lacks a case
list(APPEND EXTRA_C_FLAGS -Wswitch-enum)
## Warn for implicit conversions that may alter a value
list(APPEND EXTRA_C_FLAGS -Wconversion)
## Warn ifh the compiler detects code that will never be executed
list(APPEND EXTRA_C_FLAGS -Wunreachable-code)
## Issue all warnings demanded by strict ISO C++
if(WARNINGS_ANSI_ISO)
    list(APPEND EXTRA_C_FLAGS -pedantic)
endif()
## Treat all warnings as errors
if(WARNINGS_ARE_ERRORS)
    list(APPEND EXTRA_C_FLAGS -Werror)
endif()
## Enable warnings for constructs that violates Effective C++ guidelines
if(WARNINGS_EFF_CXX)
    list(APPEND EXTRA_C_FLAGS -Weffc++)
endif()

# Avoid duplicate entries when calling cmake again
list(REMOVE_DUPLICATES EXTRA_C_FLAGS)
list(REMOVE_DUPLICATES EXTRA_C_FLAGS_RELEASE)

