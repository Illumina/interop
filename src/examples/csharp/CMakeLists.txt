# Adds the following targets:
#   1. csharp_unittest - Shared library for Unit Testing
#
# Post build of csharp_unittest the unit tests are run
#

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(CSharp)

if(NOT CSHARP_FOUND)
    return()
endif()

find_package(SWIG)
if(NOT SWIG_FOUND)
    return()
endif()

check_swig_version(swig_major_ver)
if(swig_major_ver LESS "3")
    return()
endif()

if(NOT SWIG_GEN_CSHARP_SOURCE_FILES)
    message(WARNING "No source files found for unit testing")
    message(WARNING "Please run cmake <path> and cmake <path> --build . again")
    return()
endif()

include(${CSHARP_USE_FILE})



function(add_csharp_example _target _source_files)
    set(_abs_source_files)
    foreach(SRC ${_source_files})
        list(APPEND _abs_source_files "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
    endforeach()
    csharp_add_executable(${_target} ${_abs_source_files} ${SWIG_CSHARP_LIBRARY})
    add_dependencies(${_target} csharp_interop)
    add_custom_command(TARGET ${_target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CSHARP_${_target}_BINARY} $<TARGET_FILE_DIR:${INTEROP_LIB}>)
    if(NOT ENABLE_EXAMPLES)
        set_target_properties(${_target} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
    else()
        install(FILES ${CSHARP_${_target}_BINARY} ${CSHARP_INTEROP_LIBRARY}
                DESTINATION share/illumina/interop/examples
                )
    endif()
    if(NOT ENABLE_STATIC)
        add_custom_command(TARGET ${_target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
    endif()
endfunction()

add_csharp_example(csharp_example1 Example1.cs)
add_csharp_example(csharp_example2 Example2.cs)
add_csharp_example(csharp_example3 Example3.cs)
add_csharp_example(csharp_summary SummaryExample.cs)

