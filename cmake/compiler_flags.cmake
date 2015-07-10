# RELEASE Flags
LIST(APPEND EXTRA_C_FLAGS_RELEASE -Wall)
LIST(APPEND EXTRA_C_FLAGS_RELEASE -O3)

# DEBUG Flags
## Show all warnings
LIST(APPEND EXTRA_C_FLAGS -Wall)
## Enable some extra warning flags
LIST(APPEND EXTRA_C_FLAGS -Wextra)
## Warn if anything depends upn the size of a function or of void
LIST(APPEND EXTRA_C_FLAGS -Wpointer-arith)
## Avoid int i = i
LIST(APPEND EXTRA_C_FLAGS -Winit-self)
## Warn if an unitialized identifier is evaluated
LIST(APPEND EXTRA_C_FLAGS -Wundef)
## Warn whenever a local variable shadows another local/global one
LIST(APPEND EXTRA_C_FLAGS -Wshadow)
## Warn if a pointer is cast such that alignment of target is increased
LIST(APPEND EXTRA_C_FLAGS -Wcast-align)
## Give string consts the type const char[length]
LIST(APPEND EXTRA_C_FLAGS -Wwrite-strings)
## Warn whenever a poinster is cast to remove a type qualifier from target
LIST(APPEND EXTRA_C_FLAGS -Wcast-qual)
## Warn whenever a switch does not have a default case
LIST(APPEND EXTRA_C_FLAGS -Wswitch-default)
## Warn whenever a switch has an index of enumerated type and lacks a case
LIST(APPEND EXTRA_C_FLAGS -Wswitch-enum)
## Warn for implicit conversions that may alter a value
LIST(APPEND EXTRA_C_FLAGS -Wconversion)
## Warn ifh the compiler detects code that will never be executed
LIST(APPEND EXTRA_C_FLAGS -Wunreachable-code)
## Issue all warnings demanded by strict ISO C++
IF(WARNINGS_ANSI_ISO)
    LIST(APPEND EXTRA_C_FLAGS -pedantic)
ENDIF()
## Treat all warnings as errors
IF(WARNINGS_ARE_ERRORS)
    LIST(APPEND EXTRA_C_FLAGS -Werror)
ENDIF()
## Enable warnings for constructs that violates Effective C++ guidelines
IF(WARNINGS_EFF_CXX)
    LIST(APPEND EXTRA_C_FLAGS -Weffc++)
ENDIF()

# Avoid duplicate entries when calling cmake again
LIST(REMOVE_DUPLICATES EXTRA_C_FLAGS)
LIST(REMOVE_DUPLICATES EXTRA_C_FLAGS_RELEASE)

