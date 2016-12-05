#
# A CMake Module for finding and using the Google Mock Library
#
# The following variables may be used to set the path of the Google Mock library:
#   - GMOCK_ROOT: a cmake variable
#   - GMOCK_DIR: an environment variable
#
#
# If GMock is not found, an attempt will be made to auto-download and make avaialble.
#
# The following variables are set:
#   - GMOCK_INCLUDE_DIR - include path for Google Mock library
#   - GMOCK_LIBRARY - library path for Google Mock library
#   - GMOCK_USE_FILE - path to GMock Use File
#   - GMOCK_FOUND - set to ON if GMock is found

find_package(PkgConfig)
pkg_check_modules(PC_GMOCK QUIET gmock)

set(GMOCK_ROOT "" CACHE PATH "Set the location of the Google Mock library and header")

find_path(GMOCK_INCLUDE_DIR
        NAMES gmock/gmock.h
        HINTS ${PC_GMOCK_INCLUDEDIR} ${PC_GMOCK_INCLUDE_DIRS} ${GMOCK_ROOT} $ENV{GMOCK_DIR}
        PATH_SUFFIXES . include
        )

find_library(GMOCK_LIBRARY
        NAMES gmock
        HINTS ${PC_GMOCK_INCLUDEDIR} ${PC_GMOCK_INCLUDE_DIRS} ${GMOCK_ROOT} $ENV{GMOCK_DIR}
        PATH_SUFFIXES . lib lib64
        )
find_library(GMOCK_MAIN_LIBRARY
        NAMES gmock_main
        HINTS ${PC_GMOCK_INCLUDEDIR} ${PC_GMOCK_INCLUDE_DIRS} ${GMOCK_ROOT} $ENV{GMOCK_DIR}
        PATH_SUFFIXES . lib lib64
        )


find_library(GMOCK_LIBRARY_DEBUG
        NAMES gmock-d
        HINTS ${PC_GMOCK_INCLUDEDIR} ${PC_GMOCK_INCLUDE_DIRS} ${GMOCK_ROOT} $ENV{GMOCK_DIR}
        PATH_SUFFIXES . lib lib64
)
find_library(GMOCK_MAIN_LIBRARY_DEBUG
        NAMES gmock_main-d
        HINTS ${PC_GMOCK_INCLUDEDIR} ${PC_GMOCK_INCLUDE_DIRS} ${GMOCK_ROOT} $ENV{GMOCK_DIR}
        PATH_SUFFIXES . lib lib64
)


if(NOT GMOCK_INCLUDE_DIR OR NOT GMOCK_LIBRARY OR NOT GMOCK_MAIN_LIBRARY)

    if(NOT GMOCK_INCLUDE_DIR)
        message(STATUS "GMock include directory not found")
    elseif(NOT GMOCK_LIBRARY)
        message(STATUS "GMock library not found")
    elseif(NOT GMOCK_MAIN_LIBRARY)
        message(STATUS "GMock main library not found")
    endif()
    if(MSVC)
        set(USE_OWN_TR1_TUPLE 0)
    else()
        set(USE_OWN_TR1_TUPLE 1)
    endif()
    if(FORCE_SHARED_CRT)
        set(gtest_force_shared_crt ON)
    else()
        set(gtest_force_shared_crt OFF)
    endif()
    if(FORCE_X86)
        set(EXTRA_FLAGS " -m32")
    endif()
    set(GMOCK_PREFIX ${CMAKE_BINARY_DIR}/external/gtest)
    include(ExternalProject)
    ExternalProject_Add(
            googlemock
            PREFIX ${GMOCK_PREFIX}
            GIT_REPOSITORY https://github.com/google/googlemock.git
            GIT_TAG release-1.7.0
            CMAKE_ARGS "-DCMAKE_CXX_FLAGS=-DGMOCK_USE_OWN_TR1_TUPLE=${USE_OWN_TR1_TUPLE}${EXTRA_FLAGS}"
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${GMOCK_PREFIX}/lib64
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=${GMOCK_PREFIX}/lib64
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=${GMOCK_PREFIX}/lib64
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${GMOCK_PREFIX}/lib64
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG=${GMOCK_PREFIX}/lib64
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE=${GMOCK_PREFIX}/lib64
            -Dgtest_force_shared_crt=${gtest_force_shared_crt}
            INSTALL_COMMAND ""
            LOG_DOWNLOAD ON
            LOG_CONFIGURE ON
            LOG_BUILD ON)
    add_dependencies(googlemock libgtest)
    ExternalProject_Get_Property(googlemock source_dir)
    set(GMOCK_INCLUDE_DIR ${source_dir}/include)
    set(GMOCK_LIBRARY OFF)
    set(GMOCK_MAIN_LIBRARY OFF)

    ExternalProject_Get_Property(googlemock binary_dir)
    set(GMOCK_LIBRARY_PATH ${GMOCK_PREFIX}/lib64 CACHE INTERNAL "Path to Google Mock Library")
    set(GMOCK_LIBRARY_FILE ${GMOCK_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GMOCK_MAIN_LIBRARY_FILE ${GMOCK_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GMOCK_TARGET googlemock)
endif()

if(GMOCK_LIBRARY AND GMOCK_LIBRARY_DEBUG)
    get_filename_component(GMOCK_RELEASE_EXT ${GMOCK_LIBRARY} EXT)
    get_filename_component(GMOCK_DEBUG_EXT ${GMOCK_LIBRARY_DEBUG} EXT)
    if(GMOCK_RELEASE_EXT STREQUAL GMOCK_DEBUG_EXT)
        set(GMOCK_LIBRARY optimized ${GMOCK_LIBRARY} debug ${GMOCK_LIBRARY_DEBUG})
    endif()
endif()
if(GMOCK_MAIN_LIBRARY AND GMOCK_MAIN_LIBRARY_DEBUG)
    get_filename_component(GMOCK_RELEASE_EXT ${GMOCK_MAIN_LIBRARY} EXT)
    get_filename_component(GMOCK_DEBUG_EXT ${GMOCK_MAIN_LIBRARY_DEBUG} EXT)
    if(GMOCK_RELEASE_EXT STREQUAL GMOCK_DEBUG_EXT)
        set(GMOCK_MAIN_LIBRARY optimized ${GMOCK_MAIN_LIBRARY} debug ${GMOCK_MAIN_LIBRARY_DEBUG})
    endif()
endif()

set(GMOCK_LIBRARIES ${GMOCK_LIBRARY} )
set(GMOCK_INCLUDE_DIRS ${GMOCK_INCLUDE_DIR} )
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMock DEFAULT_MSG GMOCK_LIBRARY GMOCK_INCLUDE_DIR)


set( GMOCK_USE_FILE ${CMAKE_CURRENT_LIST_DIR}/UseGMock.cmake )
mark_as_advanced(GMOCK_LIBRARIES GMOCK_INCLUDE_DIRS)



