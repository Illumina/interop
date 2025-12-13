# Get SWIG major version
function(check_swig_version _major_ver)
    execute_process(
            COMMAND ${SWIG_EXECUTABLE} -version
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE
            res
            OUTPUT_VARIABLE
            VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "SWIG version output: ${VERSION}")
    string(REGEX REPLACE "SWIG Version ([0-9]+)\\.[0-9]+\\.[0-9]+.*" "\\1" swig_major_ver "${VERSION}")
    string(STRIP ${swig_major_ver} swig_major_ver)
    set(${_major_ver} ${swig_major_ver} PARENT_SCOPE)
endfunction()
