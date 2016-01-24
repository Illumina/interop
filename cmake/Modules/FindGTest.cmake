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

message(WARNING "GTEST_INCLUDE_DIR=${GTEST_INCLUDE_DIR}")
message(WARNING "GTEST_LIBRARY=${GTEST_LIBRARY}")

if(NOT GTEST_INCLUDE_DIR)

    if(MSVC)
        set(USE_OWN_TR1_TUPLE 0)
    else()
        set(USE_OWN_TR1_TUPLE 1)
    endif()
    set(GTEST_PREFIX ${CMAKE_BINARY_DIR}/external/gtest)
    include(ExternalProject)
    ExternalProject_Add(
            googletest
            PREFIX ${GTEST_PREFIX}
            URL https://github.com/google/googletest/archive/release-1.7.0.zip
            CMAKE_ARGS -DCMAKE_CXX_FLAGS=-DGTEST_USE_OWN_TR1_TUPLE=${USE_OWN_TR1_TUPLE}
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${GTEST_PREFIX}/lib64
                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=${GTEST_PREFIX}/lib64
                -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=${GTEST_PREFIX}/lib64
                -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${GTEST_PREFIX}/lib64
                -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG=${GTEST_PREFIX}/lib6
                -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE=${GTEST_PREFIX}/lib64
            INSTALL_COMMAND ""
            LOG_DOWNLOAD ON
            LOG_CONFIGURE ON
            LOG_BUILD ON)
    ExternalProject_Get_Property(googletest source_dir)
    set(GTEST_INCLUDE_DIR ${source_dir}/include)

    ExternalProject_Get_Property(googletest binary_dir)
    set(GTEST_LIBRARY_PATH ${GTEST_PREFIX}/lib64 CACHE INTERNAL "Path to Google Test Library")


    set(GTEST_TARGET googletest)
endif()



set(GTEST_LIBRARIES ${GTEST_LIBRARY} )
set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR} )
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GTest DEFAULT_MSG GTEST_LIBRARY GTEST_INCLUDE_DIR)


set( GTEST_USE_FILE ${CMAKE_CURRENT_LIST_DIR}/UseGTest.cmake )
mark_as_advanced(GTEST_LIBRARIES GTEST_INCLUDE_DIRS)



