cmake_minimum_required(VERSION 3.2)
project(InterOp)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
enable_testing()
include(${PROJECT_SOURCE_DIR}/cmake/Modules/UseGitVersion.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/InternalUtils.cmake)

config_compiler_and_linker()
# TODO: https://help.github.com/articles/creating-project-pages-manually/

# TODO: Handle Windows Shared libs: https://cmake.org/Wiki/BuildingWinDLL

set(INTEROP_TESTS interop_tests)
set(INTEROP_LIB interop_lib)
set(INTEROP_DL_LIB interop_fpic_lib)

option(ENABLE_BACKWARDS_COMPATIBILITY "Compile code for c++98" ON)
option(ENABLE_DOCS "Build documentation with Doxygen" ON)
option(ENABLE_SWIG "Build third-party language bindings, e.g. C#" ON)
option(ENABLE_TEST "Build unit tests (depends on Boost)" ON)
option(ENABLE_APPS "Build command line programs" ON)
option(ENABLE_EXAMPLES "Build example programs" ON)
option(ENABLE_STATIC "Build static libraries instead of dynamic" ON)
option(FORCE_X86 "Force 32-bit libraries instead of platform default (Does nothing for Visual Studio)" OFF)

if(ENABLE_STATIC)
    set(LIBRARY_TYPE STATIC)
else()
    set(LIBRARY_TYPE SHARED)
endif()

if(WIN32 AND ${LIBRARY_TYPE} STREQUAL "SHARED")
    message(FATAL_ERROR "Shared libraries on Windows are not supported")
endif()

include_directories(.)
add_version_target(version ${CMAKE_SOURCE_DIR}/interop/version.h INTEROP_VERSION)
add_subdirectory("src")
if(ENABLE_DOCS)
    add_subdirectory("docs")
endif()
install(DIRECTORY interop DESTINATION include)