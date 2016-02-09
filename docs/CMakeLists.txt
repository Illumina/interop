
# TODO: Add version to documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )

    if(NOT GIT_FOUND)
        find_package(Git REQUIRED)
    endif()

    if(GIT_FOUND AND NOT WIN32) # Don't think this will work on Windows (cp command), so disable for now
        add_custom_target(deploy-docs
                COMMAND ${GIT_EXECUTABLE} checkout gh-pages
                COMMAND ${GIT_EXECUTABLE} rm reset `git rev-list --max-parents=0 --abbrev-commit HEAD` --hard
                COMMAND cp -r Build/Debug/html/* ./
                COMMAND ${GIT_EXECUTABLE} add .
                COMMAND ${GIT_EXECUTABLE} commit -am "Added documentation"
                COMMAND ${GIT_EXECUTABLE} push origin gh-pages
                COMMAND ${GIT_EXECUTABLE} checkout @{-1}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
    endif()

else()
    message(WARNING "Doxygen not found, documentation generation is disabled")
endif(DOXYGEN_FOUND)

