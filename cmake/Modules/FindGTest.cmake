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

if(NOT DEFINED GTEST_TAG)
    #set(GTEST_TAG "v1.16.0" CACHE PATH "Git tag for automatic download of GTest")
    set(GTEST_TAG "release-1.8.1" CACHE PATH "Git tag for automatic download of GTest")
endif()
string(REGEX REPLACE "release-" "" GTEST_VERSION_NUM ${GTEST_TAG})


# OR NOT GMOCK_INCLUDE_DIR_TEST OR NOT GMOCK_LIBRARY_TEST OR NOT GMOCK_MAIN_LIBRARY_TEST
if(NOT GTEST_INCLUDE_DIR OR NOT GTEST_LIBRARY OR NOT GTEST_MAIN_LIBRARY)
    if(NOT GTEST_INCLUDE_DIR)
        message(STATUS "GTest include directory not found - GTEST_ROOT: ${GTEST_ROOT}")
    elseif(NOT GTEST_LIBRARY)
        message(STATUS "GTest library not found - GTEST_ROOT: ${GTEST_ROOT}")
    elseif(NOT GTEST_MAIN_LIBRARY)
        message(STATUS "GTest main library not found - GTEST_ROOT: ${GTEST_ROOT}")
    endif()

    if(NOT DISABLE_SHARED_CRT)
        message(STATUS "Enable Shared CRT for GTest")
        set(gtest_force_shared_crt ON)
    else()
        set(gtest_force_shared_crt OFF)
    endif()

    set(GTEST_PREFIX ${CMAKE_BINARY_DIR}/external/gtest)

    #if(${GTEST_VERSION_NUM} VERSION_LESS_EQUAL "1.8.1")
    set(GTEST_LIBRARY_PATH ${GTEST_PREFIX}/dist/lib CACHE INTERNAL "Path to Google Test Library")
    set(GTEST_MAIN_LIBRARY_DEBUG ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_maind${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GTEST_LIBRARY_DEBUG ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gtestd${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GMOCK_LIBRARY_DEBUG ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gmockd${CMAKE_STATIC_LIBRARY_SUFFIX})

        #set(GTEST_LIBRARY_PATH ${GTEST_PREFIX}/src/gtest_download-build/lib CACHE INTERNAL "Path to Google Test Library")
        #set(GTEST_MAIN_LIBRARY_DEBUG ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})
        #set(GTEST_LIBRARY_DEBUG ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})
        #set(GMOCK_LIBRARY_DEBUG ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX})

    set(GTEST_LIBRARY_RELEASE ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GMOCK_LIBRARY_RELEASE ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GTEST_MAIN_LIBRARY_RELEASE ${GTEST_LIBRARY_PATH}/${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(GTEST_INCLUDE_DIR ${GTEST_PREFIX}/dist/include)

    if(MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(GMOCK_LIBRARY ${GMOCK_LIBRARY_DEBUG})
        set(GTEST_LIBRARY ${GTEST_LIBRARY_DEBUG})
        set(GTEST_MAIN_LIBRARY ${GTEST_MAIN_LIBRARY_DEBUG})
    else()
        set(GMOCK_LIBRARY ${GMOCK_LIBRARY_RELEASE})
        set(GTEST_LIBRARY ${GTEST_LIBRARY_RELEASE})
        set(GTEST_MAIN_LIBRARY ${GTEST_MAIN_LIBRARY_RELEASE})
    endif()
    string(REPLACE ";" "$<SEMICOLON>" CMAKE_OSX_ARCHITECTURES_NEW "${CMAKE_OSX_ARCHITECTURES}")
    message(STATUS "CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}")
    include(ExternalProject)
    ExternalProject_Add(
            gtest_download
            PREFIX ${GTEST_PREFIX}
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG ${GTEST_TAG}
            CMAKE_ARGS
            "-DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}"
            "-DCMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS}"
            "-DCMAKE_STATIC_LINKER_FLAGS=${CMAKE_STATIC_LINKER_FLAGS}"
            "-DCMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS}"
            "-DCMAKE_MODULE_LINKER_FLAGS=${CMAKE_MODULE_LINKER_FLAGS}"
            -G${CMAKE_GENERATOR}
            -DCMAKE_POLICY_VERSION_MINIMUM=3.5
            -DCMAKE_GENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}
            -DCMAKE_GENERATOR_TOOLSET=${CMAKE_GENERATOR_TOOLSET}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES_NEW}
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${GTEST_LIBRARY_PATH}
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=${GTEST_LIBRARY_PATH}
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=${GTEST_LIBRARY_PATH}
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${GTEST_LIBRARY_PATH}
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG=${GTEST_LIBRARY_PATH}
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE=${GTEST_LIBRARY_PATH}
            -Dgtest_force_shared_crt=${gtest_force_shared_crt}
            -DINSTALL_GTEST=OFF
            INSTALL_COMMAND ""
            UPDATE_DISCONNECTED ${UPDATE_DISCONNECTED}
            LOG_DOWNLOAD OFF
            LOG_CONFIGURE OFF
            LOG_BUILD OFF
            BUILD_BYPRODUCTS ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} ${GMOCK_LIBRARY})

    ExternalProject_Get_Property(gtest_download source_dir)
    message(STATUS "source_dir=${source_dir}")
    set(GTEST_INCLUDE_DIR ${source_dir}/googletest/include)
    set(GMOCK_INCLUDE_DIR ${source_dir}/googlemock/include)
    file(MAKE_DIRECTORY ${GTEST_INCLUDE_DIR})
    file(MAKE_DIRECTORY ${GMOCK_INCLUDE_DIR})

    message(STATUS "Download GTest ${GTEST_TAG}")
    set_target_properties(gtest_download PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()

set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR})
set(GMOCK_INCLUDE_DIRS ${GMOCK_INCLUDE_DIR})
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GTest DEFAULT_MSG GTEST_LIBRARY GTEST_INCLUDE_DIR)
mark_as_advanced(GTEST_LIBRARIES GTEST_INCLUDE_DIRS)

message(STATUS "GTEST_INCLUDE_DIRS=${GTEST_INCLUDE_DIRS}")
message(STATUS "GTEST_LIBRARY=${GTEST_LIBRARY}")
message(STATUS "GTEST_MAIN_LIBRARY=${GTEST_MAIN_LIBRARY}")
message(STATUS "GMOCK_LIBRARY=${GMOCK_LIBRARY}")
message(STATUS "GTEST_HAS_TR1_TUPLE=${GTEST_HAS_TR1_TUPLE}")

if(NOT TARGET gtest)
    add_library(gtest INTERFACE)
    target_link_libraries(gtest INTERFACE
            $<BUILD_INTERFACE:${GTEST_LIBRARY}>
            $<BUILD_INTERFACE:${GMOCK_LIBRARY}>
    )
    target_include_directories(gtest
            SYSTEM INTERFACE ${GTEST_INCLUDE_DIRS}
    )
    target_include_directories(gtest
            SYSTEM INTERFACE ${GMOCK_INCLUDE_DIRS}
    )
    if(TARGET gtest_download)
        add_dependencies(gtest gtest_download)
    endif()

    add_library(gtest_main INTERFACE)
    target_link_libraries(gtest_main INTERFACE
            $<BUILD_INTERFACE:${GTEST_MAIN_LIBRARY}>
    )
    target_include_directories(gtest_main
            SYSTEM INTERFACE ${GTEST_INCLUDE_DIRS}
    )

    if(TARGET gtest_download)
        add_dependencies(gtest_main gtest_download)
    endif()

    message(STATUS "GTEST include dir: ${GTEST_INCLUDE_DIR}")
endif()