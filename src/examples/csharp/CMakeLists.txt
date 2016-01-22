# Adds the following targets:
#   1. csharp_unittest - Shared library for Unit Testing
#
# Post build of csharp_unittest the unit tests are run
#

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(CSharp)
find_package(SWIG)

if(NOT CSHARP_FOUND)
    return()
endif()

if(NOT SWIG_FOUND)
    return()
endif()

if(NOT SWIG_GEN_CSHARP_SOURCE_FILES)
    message(WARNING "No source files found for unit testing")
    message(WARNING "Please run cmake <path> and cmake <path> --build . again")
    return()
endif()

include(${CSHARP_USE_FILE})

set(EXAMPLE_SRCS
        Example1.cs
        )
set(csharp_files ${SWIG_GEN_CSHARP_SOURCE_FILES})
foreach(SRC ${EXAMPLE_SRCS})
    get_filename_component(CSHARP_TEST_SRCS ${SRC} ABSOLUTE)
    list(APPEND csharp_files ${CSHARP_TEST_SRCS})
endforeach()

csharp_add_executable(csharp_example1 ${csharp_files})
add_dependencies(csharp_example1 csharp_interop)
add_custom_command(TARGET csharp_example1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CSHARP_csharp_example1_BINARY} $<TARGET_FILE_DIR:${INTEROP_LIB}>)

set(EXAMPLE_SRCS
        Example2.cs
        )

set(csharp_files ${SWIG_GEN_CSHARP_SOURCE_FILES})
foreach(SRC ${EXAMPLE_SRCS})
    get_filename_component(CSHARP_TEST_SRCS ${SRC} ABSOLUTE)
    list(APPEND csharp_files ${CSHARP_TEST_SRCS})
endforeach()

csharp_add_executable(csharp_example2 ${csharp_files})
add_dependencies(csharp_example2 csharp_interop)
add_custom_command(TARGET csharp_example2 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CSHARP_csharp_example2_BINARY} $<TARGET_FILE_DIR:${INTEROP_LIB}>)


set(EXAMPLE_SRCS
        Example3.cs
        )

set(csharp_files ${SWIG_GEN_CSHARP_SOURCE_FILES})
foreach(SRC ${EXAMPLE_SRCS})
    get_filename_component(CSHARP_TEST_SRCS ${SRC} ABSOLUTE)
    list(APPEND csharp_files ${CSHARP_TEST_SRCS})
endforeach()

csharp_add_executable(csharp_example3 ${csharp_files})
add_dependencies(csharp_example3 csharp_interop)
add_custom_command(TARGET csharp_example3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CSHARP_csharp_example3_BINARY} $<TARGET_FILE_DIR:${INTEROP_LIB}>)

if(NOT ENABLE_EXAMPLES)
    set_target_properties(csharp_example1 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
    set_target_properties(csharp_example2 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
    set_target_properties(csharp_example3 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
else()
    install(FILES ${CSHARP_csharp_example1_BINARY} ${CSHARP_csharp_example2_BINARY} ${CSHARP_csharp_example3_BINARY}
            DESTINATION share/illumina/interop/examples
            )
endif()

if(NOT ENABLE_STATIC)
    add_custom_command(TARGET csharp_example1 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(TARGET csharp_example2 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(TARGET csharp_example3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()
