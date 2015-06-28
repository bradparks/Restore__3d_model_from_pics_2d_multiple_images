# RELEASE Flags
SET(CMAKE_CXX_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} -Wall -O3")

# DEBUG Flags
## Forcing the compiler to work harder helps finding potential problems
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -O")
## Show all warnings
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall")
## Enable some extra warning flags
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wextra")
## Warn if anything depends upn the size of a function or of void
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wpointer-arith")
## Avoid int i = i
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Winit-self")
## Warn if an unitialized identifier is evaluated
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wundef")
## Warn whenever a local variable shadows another local/global one
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wshadow")
## Warn if a pointer is cast such that alignment of target is increased
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wcast-align")
## Give string consts the type const char[length}
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wwrite-strings")
## Warn whenever a poinster is cast to remove a type qualifier from target
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wcast-qual")
## Warn whenever a switch does not have a default case
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wswitch-default")
## Warn whenever a switch has an index of enumerated type and lacks a case
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wswitch-enum")
## Warn for implicit conversions that may alter a value
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wconversion")
## Warn ifh the compiler detects code that will never be executed
SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wunreachable-code")
## Enable warnings for constructs that violates Effective C++ guidelines
IF(WARNINGS_EFF_CXX)
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Weffc++")
ENDIF()
## Issue all warnings demanded by strict ISO C++
IF(WARNINGS_ANSI_ISO)
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -pedantic")
ENDIF()
## Treat all warnings as errors
IF(WARNINGS_ARE_ERRORS)
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Werror")
ENDIF()


