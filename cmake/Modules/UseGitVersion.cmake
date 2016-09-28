#
# Provides a function that determines the version from tags in the Git repository. A target is added, which
# generates a version header file. This version header file defines a single macro.
#
# Input to the function
#   - _target - name of the target
#   - _version_file - name of the version header file
#   - _macro_name - name of the macro in the header file
#

function(add_version_target _target _version_file _macro_name _default)
    if(NOT GIT_FOUND)
        find_package(Git REQUIRED)
    endif()
    get_filename_component(version_base ${_version_file} NAME)
    file(WRITE ${CMAKE_BINARY_DIR}/${version_base}.in "\#define ${_macro_name} \"@VERSION@\"\n")
    file(WRITE ${CMAKE_BINARY_DIR}/version.cmake
    "execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --tags --dirty=-dirty
        WORKING_DIRECTORY \"${CMAKE_SOURCE_DIR}\"
        RESULT_VARIABLE
        res
        OUTPUT_VARIABLE
        VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT res EQUAL 0)
        set(VERSION \"\${BKUP}\")
    endif()
    configure_file(\${SRC} \${DST} @ONLY)
    "
    )
    add_custom_command(OUTPUT ${_version_file}
            COMMAND ${CMAKE_COMMAND} -D SRC=${CMAKE_BINARY_DIR}/${version_base}.in
            -D DST=${_version_file}
            -D BKUP=${_default}
            -P ${CMAKE_BINARY_DIR}/version.cmake)
    add_custom_target(${_target}
        DEPENDS ${_version_file}
    )
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --dirty=-dirty
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE
            res
            OUTPUT_VARIABLE
            VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NOT res EQUAL 0)
        set(VERSION "${_default}")
    endif()
    set(${_macro_name} ${VERSION} PARENT_SCOPE)
endfunction()
