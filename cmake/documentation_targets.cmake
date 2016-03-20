# Creates a Doxygen documentation target
find_package(Doxygen)
if(DOXYGEN_FOUND)
    if(EXISTS ${PROJECT_SOURCE_DIR}/Doxyfile)
        add_custom_target(documentation ${DOXYGEN_EXECUTABLE} Doxyfile
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generating doxygen project documentation." VERBATIM)
    else(EXISTS ${PROJECT_SOURCE_DIR}/Doxyfile)
        add_custom_target(documentation COMMENT "Doxyfile not found. Please \
        generate a doxygen configuration file to use this target." VERBATIM)
    endif(EXISTS ${PROJECT_SOURCE_DIR}/Doxyfile)
else(DOXYGEN_FOUND)
    add_custom_target(documentation COMMENT "Doxygen not found. Please \
    install doxygen to use this target." VERBATIM)
endif(DOXYGEN_FOUND)
