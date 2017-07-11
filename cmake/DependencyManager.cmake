# Download
function(update_dependencies dep_url dep_dir output_var)

    string(REGEX REPLACE ".*[^/]+/(.*)\\.[^0-9.]*" "\\1" DEP_FILENAME ${dep_url})

    if(WIN32)
        set(HOME_DIR $ENV{LOCALAPPDATA}\\${dep_dir})
        file(TO_CMAKE_PATH ${HOME_DIR} HOME_DIR)
    else()
        set(HOME_DIR $ENV{HOME}/${dep_dir})
    endif()
    set(${output_var} ${HOME_DIR} PARENT_SCOPE)

    if(EXISTS ${HOME_DIR}/${DEP_FILENAME}.txt)
        message(STATUS "Found dep marker: ${HOME_DIR}/${DEP_FILENAME}.txt")
        return()
    else()
        message(STATUS "Dependencies not found")
    endif()

    file(WRITE ${CMAKE_BINARY_DIR}/deps/CMakeLists.txt
            " cmake_minimum_required(VERSION 3.4)
            include(ExternalProject)
            ExternalProject_Add(
            update_deps
            URL ${dep_url}
            DOWNLOAD_DIR ${HOME_DIR}/download
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
    if(NOT EXISTS ${HOME_DIR}/${DEP_FILENAME}.txt)
        set(${output_var} "")
    endif()
endfunction()

