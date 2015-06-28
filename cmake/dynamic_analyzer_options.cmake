IF(USE_ADDRESS_SANITIZER)
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize=address")
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fno-omit-frame-pointer")
ENDIF()

IF(USE_MEMORY_SANITIZER)
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize=memory")
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize-memory-track-origins=2")
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fno-omit-frame-pointer")
ENDIF()

IF(USE_THREAD_SANITIZER)
    SET(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fsanitize=thread")
ENDIF()
