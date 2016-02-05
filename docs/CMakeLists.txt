
# TODO: Add version to documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )

    add_custom_target(deploy-docs
            COMMAND git checkout gh-pages
            COMMAND git rm -rf .
            COMMAND cp -r Build/Debug/html/* ./
            COMMAND git add .
            COMMAND git commit -am "Added documentation"
            COMMAND git push origin gh-pages
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

else()
    message(WARNING "Doxygen not found, documentation generation is disabled")
endif(DOXYGEN_FOUND)

