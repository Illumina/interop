

find_package(Doxygen)
if(DOXYGEN_FOUND)

    execute_process(
            COMMAND ${DOXYGEN_EXECUTABLE} --version
            RESULT_VARIABLE
            res
            OUTPUT_VARIABLE
            doxygen_ver
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT doxygen_ver VERSION_EQUAL "1.8.10")
        message(WARNING "Only doxygen version 1.8.10 is currently supported - not found version: ${doxygen_ver}\nDocumentation target will be disabled")
        return()
    endif()


    message(STATUS "Build documentation")
    add_custom_target(doc ALL
            COMMAND ${CMAKE_COMMAND} -DCURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DPROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR} -DINTEROP_VERSION=${INTEROP_VERSION} -DCONFIG_INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in -DCONFIG_OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/Doxyfile -P ${CMAKE_SOURCE_DIR}/cmake/ConfigureFile.cmake
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/theme/bootstrap3 ${CMAKE_CURRENT_BINARY_DIR}/html/bootstrap3
            COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
    if(DOCS_OUTPUT_PATH)
        add_custom_command(TARGET doc POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_BINARY_DIR}/html ${DOCS_OUTPUT_PATH}/html
                )
    endif()
    add_dependencies(doc version)

else()
    message(WARNING "Doxygen not found, documentation generation is disabled")
endif(DOXYGEN_FOUND)

