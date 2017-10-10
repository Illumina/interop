# Download
function(update_dependencies dep_url dep_dir output_var)

    string(REGEX REPLACE ".*[^/]+/(.*)\\.[^0-9.]*" "\\1" DEP_FILENAME ${dep_url})

    if(INTEROP_CACHE_DIR)
        set(HOME_DIR "${INTEROP_CACHE_DIR}/${dep_dir}")
    else()
        set(HOME_DIR ${CMAKE_SOURCE_DIR}/../${dep_dir})
    endif()
    get_filename_component(HOME_DIR "${HOME_DIR}" ABSOLUTE )
    set(${output_var} ${HOME_DIR} PARENT_SCOPE)

    if(EXISTS "${HOME_DIR}/${DEP_FILENAME}.txt")
        file(READ "${HOME_DIR}/${DEP_FILENAME}.txt" CONTENTS)
        if(CONTENTS)
            message(STATUS "Found dep marker: ${HOME_DIR}/${DEP_FILENAME}.txt -- ${CONTENTS}")
            return()
        endif()
    endif()
    message(STATUS "Dependencies not found")

    file(WRITE ${CMAKE_BINARY_DIR}/deps/CMakeLists.txt
            " cmake_minimum_required(VERSION 3.4)
            include(ExternalProject)
            ExternalProject_Add(
            update_deps
            URL ${dep_url}
            SOURCE_DIR ${HOME_DIR}
            UPDATE_COMMAND \"\"
            CONFIGURE_COMMAND \"\"
            BUILD_COMMAND \"\"
            INSTALL_COMMAND \"\")
            "
            )
    execute_process(
            COMMAND ${CMAKE_COMMAND} .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps
    )
    execute_process(
            COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/deps
    )
    if(NOT EXISTS "${HOME_DIR}/${DEP_FILENAME}.txt")
        set(${output_var} "")
        message(WARNING "Dependencies failed to download - no version file at ${HOME_DIR}/${DEP_FILENAME}.txt")
    endif()
endfunction()

