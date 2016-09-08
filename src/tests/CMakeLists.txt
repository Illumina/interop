

add_custom_target(check)
add_custom_target(performance)
set_target_properties(check performance PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

add_subdirectory("interop")

add_custom_command(TARGET check POST_BUILD COMMAND $<TARGET_FILE:interop_gtests>)
add_dependencies(check interop_gtests)

if(NOT ENABLE_SWIG)
    return()
endif()

find_package(SWIG)
if(NOT SWIG_FOUND)
    return()
endif()
include(${PROJECT_SOURCE_DIR}/cmake/CheckSWIGVersion.cmake)

check_swig_version(swig_major_ver)
if(swig_major_ver LESS "3")
    return()
endif()

if(NOT ENABLE_SWIG)
    return()
endif()

if(ENABLE_CSHARP)
    add_subdirectory("csharp")
    if(CSHARP_TEST_ENABLED)
        add_dependencies(check check_csharp)

        add_custom_command(TARGET performance POST_BUILD
                COMMAND ${NUNIT_COMMAND_EXE} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSHARP_PERF_TEST_EXE})
        add_dependencies(check csharp_perftest)
    endif()
endif()
