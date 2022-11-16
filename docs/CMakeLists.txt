# Find Doxygen
find_package(Doxygen)

if(DOXYGEN_FOUND)
    set(DOXYGEN_OUTPUT_DIR ${PROJECT_BINARY_DIR})
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(
        DOXYGEN_INPUT
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/LICENSE.md
        ${PROJECT_SOURCE_DIR}/include
    )

    # Replace variables inside @@ with the current values.
    set(DOXYFILE_IN ${PROJECT_SOURCE_DIR}/docs/doxyfile.in)
    set(DOXYFILE_OUT ${PROJECT_BINARY_DIR}/doxyfile.out)
    configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

    # Copy the files needd by the documentation.
    set(DOXYFILE_CSS ${PROJECT_SOURCE_DIR}/docs/doxygen.css)
    set(DOXYFILE_HEADER ${PROJECT_SOURCE_DIR}/docs/header.html)
    set(DOXYFILE_FOOTER ${PROJECT_SOURCE_DIR}/docs/footer.html)

    # Doxygen won't create this for us.
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

    add_custom_command(
        OUTPUT ${DOXYGEN_INDEX_FILE}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT} ${DOXYFILE_OUT}
        MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_custom_target(
        ${PROJECT_NAME}_documentation
        DEPENDS
        ${DOXYGEN_INDEX_FILE}
        ${DOXYFILE_IN}
        ${DOXYFILE_CSS}
        ${DOXYFILE_HEADER}
        ${DOXYFILE_FOOTER}
    )
endif(DOXYGEN_FOUND)