

add_custom_target(check)
set_target_properties(check PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

add_subdirectory("interop")


add_custom_target(check_gtest)
set_target_properties(check_gtest PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

add_custom_command(TARGET check_gtest POST_BUILD COMMAND $<TARGET_FILE:interop_gtests>)
add_dependencies(check check_gtest)

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

if(CSHARP_TEST_ENABLED AND ENABLE_CSHARP)
    add_subdirectory("csharp")
    add_dependencies(check check_csharp)
endif()

if(ENABLE_PYTHON)
    add_subdirectory("python")
    if(PYTHON_TEST_ENABLED)
        add_dependencies(check check_python)
    endif()
endif()
