#
# A CMake Module for finding and using the Google Test Library
#
# The following variables may be used to set the path of the Google Test library:
#   - GTEST_ROOT: a cmake variable
#   - GTEST_DIR: an environment variable
#
#
# If GTest is not found, an attempt will be made to auto-download and make avaialble.
#
# The following variables are set:
#   - GTEST_INCLUDE_DIR - include path for Google Test library
#   - GTEST_LIBRARY - library path for Google Test library
#   - GTEST_USE_FILE - path to GTest Use File
#   - GTEST_FOUND - set to ON if GTest is found

find_package(PkgConfig)
pkg_check_modules(PC_GTEST QUIET gtest)

set(GTEST_ROOT "" CACHE PATH "Set the location of the Google Test library and header")

find_path(GTEST_INCLUDE_DIR
        NAMES gtest/gtest.h
        HINTS ${PC_GTEST_INCLUDEDIR} ${PC_GTEST_INCLUDE_DIRS} ${GTEST_ROOT} $ENV{GTEST_DIR}
        PATH_SUFFIXES . include
        )

find_library(GTEST_LIBRARY
        NAMES gtest
        HINTS ${PC_GTEST_INCLUDEDIR} ${PC_GTEST_INCLUDE_DIRS} ${GTEST_ROOT} $ENV{GTEST_DIR}
        PATH_SUFFIXES . lib lib64
)
find_library(GTEST_MAIN_LIBRARY
        NAMES gtest_main
        HINTS ${PC_GTEST_INCLUDEDIR} ${PC_GTEST_INCLUDE_DIRS} ${GTEST_ROOT} $ENV{GTEST_DIR}
        PATH_SUFFIXES . lib lib64
        )

find_library(GTEST_LIBRARY_DEBUG
        NAMES gtest-d
        HINTS ${PC_GTEST_INCLUDEDIR} ${PC_GTEST_INCLUDE_DIRS} ${GTEST_ROOT} $ENV{GTEST_DIR}
        PATH_SUFFIXES . lib lib64
        )
find_library(GTEST_MAIN_LIBRARY_DEBUG
        NAMES gtest_main-d
        HINTS ${PC_GTEST_INCLUDEDIR} ${PC_GTEST_INCLUDE_DIRS} ${GTEST_ROOT} $ENV{GTEST_DIR}
        PATH_SUFFIXES . lib lib64
        )

find_path(GMOCK_INCLUDE_DIR_TEST
        NAMES gmock/gmock.h
        HINTS ${PC_GMOCK_INCLUDEDIR} ${PC_GMOCK_INCLUDE_DIRS} ${GMOCK_ROOT} $ENV{GMOCK_DIR}
        PATH_SUFFIXES . include
        )

find_library(GMOCK_LIBRARY_TEST
        NAMES gmock
        HINTS ${PC_GMOCK_INCLUDEDIR} ${PC_GMOCK_INCLUDE_DIRS} ${GMOCK_ROOT} $ENV{GMOCK_DIR}
        PATH_SUFFIXES . lib lib64
        )
find_library(GMOCK_MAIN_LIBRARY_TEST
        NAMES gmock_main
        HINTS ${PC_GMOCK_INCLUDEDIR} ${PC_GMOCK_INCLUDE_DIRS} ${GMOCK_ROOT} $ENV{GMOCK_DIR}
        PATH_SUFFIXES . lib lib64
        )

if(NOT GTEST_INCLUDE_DIR OR NOT GTEST_LIBRARY OR NOT GTEST_MAIN_LIBRARY OR NOT GMOCK_INCLUDE_DIR_TEST OR NOT GMOCK_LIBRARY_TEST OR NOT GMOCK_MAIN_LIBRARY_TEST)

    if(NOT GTEST_INCLUDE_DIR)
        message(STATUS "GTest include directory not found")
    elseif(NOT GTEST_LIBRARY)
        message(STATUS "GTest library not found")
    elseif(NOT GTEST_MAIN_LIBRARY)
        message(STATUS "GTest main library not found")
    endif()
    if(MSVC)
        set(USE_OWN_TR1_TUPLE 0)
    else()
        set(USE_OWN_TR1_TUPLE 1)
    endif()
    if(FORCE_SHARED_CRT)
        message(STATUS "Enable Shared CRT for GTest")
        set(gtest_force_shared_crt ON)
    else()
        set(gtest_force_shared_crt OFF)
    endif()
    set(GTEST_PREFIX ${CMAKE_BINARY_DIR}/external/gtest)
    if(FORCE_X86)
        set(EXTRA_FLAGS " -m32")
    endif()
    include(ExternalProject)
    ExternalProject_Add(
            gtest
            PREFIX ${GTEST_PREFIX}
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.7.0
            CMAKE_ARGS "-DCMAKE_CXX_FLAGS=-DGTEST_USE_OWN_TR1_TUPLE=${USE_OWN_TR1_TUPLE}${EXTRA_FLAGS}"
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${GTEST_PREFIX}/lib64
                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=${GTEST_PREFIX}/lib64
                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=${GTEST_PREFIX}/lib64
                -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${GTEST_PREFIX}/lib64
                -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG=${GTEST_PREFIX}/lib64
                -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE=${GTEST_PREFIX}/lib64
                -Dgtest_force_shared_crt=${gtest_force_shared_crt}
            INSTALL_COMMAND ""
            #SOURCE_DIR ${GTEST_PREFIX}/src/gtest
            LOG_DOWNLOAD ON
            LOG_CONFIGURE ON
            LOG_BUILD ON)
    ExternalProject_Get_Property(gtest source_dir)
    set(GTEST_INCLUDE_DIR ${source_dir}/include)
    set(GTEST_LIBRARY OFF)
    set(GTEST_MAIN_LIBRARY OFF)

    ExternalProject_Get_Property(gtest binary_dir)
    set(GTEST_LIBRARY_PATH ${GTEST_PREFIX}/lib64 CACHE INTERNAL "Path to Google Test Library")
    set(GTEST_LIBRARY_FILE ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GTEST_MAIN_LIBRARY_FILE ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GTEST_TARGET gtest)
endif()

if(GTEST_LIBRARY AND GTEST_LIBRARY_DEBUG)
    get_filename_component(GTEST_RELEASE_EXT ${GTEST_LIBRARY} EXT)
    get_filename_component(GTEST_DEBUG_EXT ${GTEST_LIBRARY_DEBUG} EXT)
    if(GTEST_RELEASE_EXT STREQUAL GTEST_DEBUG_EXT)
        set(GTEST_LIBRARY optimized ${GTEST_LIBRARY} debug ${GTEST_LIBRARY_DEBUG})
    endif()
endif()
if(GTEST_MAIN_LIBRARY AND GTEST_MAIN_LIBRARY_DEBUG)
    get_filename_component(GTEST_RELEASE_EXT ${GTEST_MAIN_LIBRARY} EXT)
    get_filename_component(GTEST_DEBUG_EXT ${GTEST_MAIN_LIBRARY_DEBUG} EXT)
    if(GTEST_RELEASE_EXT STREQUAL GTEST_DEBUG_EXT)
        set(GTEST_MAIN_LIBRARY optimized ${GTEST_MAIN_LIBRARY} debug ${GTEST_MAIN_LIBRARY_DEBUG})
    endif()
endif()

set(GTEST_LIBRARIES ${GTEST_LIBRARY} )
set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR} )
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GTest DEFAULT_MSG GTEST_LIBRARY GTEST_INCLUDE_DIR)


set( GTEST_USE_FILE ${CMAKE_CURRENT_LIST_DIR}/UseGTest.cmake )
mark_as_advanced(GTEST_LIBRARIES GTEST_INCLUDE_DIRS)



