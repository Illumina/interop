

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(doc
            COMMAND ${CMAKE_COMMAND} -DCURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR} -DINTEROP_VERSION=${INTEROP_VERSION} -DCONFIG_INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in -DCONFIG_OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/Doxyfile -P ${CMAKE_SOURCE_DIR}/cmake/ConfigureFile.cmake
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/theme/bootstrap3 ${CMAKE_CURRENT_BINARY_DIR}/html/bootstrap3
            COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    add_dependencies(doc version)

    if(NOT GIT_FOUND)
        find_package(Git REQUIRED)
    endif()

    if(GIT_FOUND)
        add_custom_target(prepare-docs
                COMMAND ${GIT_EXECUTABLE} checkout gh-pages
                COMMAND ${GIT_EXECUTABLE} reset GH_PAGES_ROOT --hard
                # TODO make system independent
                COMMAND rm -fr *
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/html ${CMAKE_SOURCE_DIR}
                COMMAND ${GIT_EXECUTABLE} add .
                COMMAND ${GIT_EXECUTABLE} commit -am "Added documentation"
                COMMAND ${GIT_EXECUTABLE} checkout @{-1}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                )
        #add_dependencies(prepare-docs doc) # TODO call cmake first
    endif()

else()
    message(WARNING "Doxygen not found, documentation generation is disabled")
endif(DOXYGEN_FOUND)

