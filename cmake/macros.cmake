# Macro for enabling c++11 as standard
macro(enable_cxx_11 TARGET_NAME)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD 11)
    set_target_properties(${TARGET_NAME} PROPERTIES CXX_STANDARD_REQUIRED ON)
endmacro()
