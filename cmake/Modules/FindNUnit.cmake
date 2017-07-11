# - Find NUnit C# Unit Test Framework
#
#

#
# A CMake Module for finding the NUnit C# Unit Test Framework
#
# The following variables may be used to set the path of the NUnit C# Unit Test Framework:
#   - NUNIT_ROOT: a cmake variable
#   - NUNIT_DIR: an environment variable
#
# If NUnit is not found, an attempt will be made to auto-download and make avaialble.
#
# The following variables are set:
#   - NUNIT_COMMAND - test runner command for the NUnit C# Unit Test Framework
#   - NUNIT_LIBRARY - library path for the NUnit C# Unit Test Framework
#   - NUNIT_FOUND - set to ON if NUnit is found

find_package(PkgConfig)
pkg_check_modules(PC_NUNIT QUIET nunit)

set(NUNIT_ROOT "" CACHE PATH "Set the location of the NUnit library")

set(nunit_search_hints
        ${NUNIT_ROOT}
        $ENV{NUNIT_DIR}
        [HKEY_CURRENT_USER\\Software\\nunit.org\\NUnit\\2.6.4]
        "C:\\Program Files (x86)\\NUnit 2.6.4"
        /usr/local
        /usr/lib/monodevelop/AddIns/NUnit
        /usr/lib/mono/4.0
        /usr/lib/mono/3.5
        /usr/lib/mono/2.0
        /usr/lib/mono/1.0
        /usr/lib/nunit
        )

find_file(NUNIT_LIBRARY nunit.framework.dll
        HINTS ${nunit_search_hints} ${PC_NUNIT_INCLUDEDIR} ${PC_NUNIT_INCLUDE_DIRS}
        PATH_SUFFIXES bin bin/framework lib/mono lib/mono/4.5
        )

find_program(NUNIT64_COMMAND nunit-console nunit-console.exe
        HINTS ${PC_NUNIT_INCLUDEDIR} ${PC_NUNIT_INCLUDE_DIRS} ${nunit_search_hints}
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
        )

find_program(NUNIT64_COMMAND nunit-console nunit-console.exe
        HINTS ${PC_NUNIT_INCLUDEDIR} ${PC_NUNIT_INCLUDE_DIRS} ${nunit_search_hints}
        PATH_SUFFIXES bin
        DOC "NUnit test runner command"
        )

find_program(NUNIT32_COMMAND nunit-console-x86 nunit-console-x86.exe
        HINTS ${PC_NUNIT_INCLUDEDIR} ${PC_NUNIT_INCLUDE_DIRS} ${nunit_search_hints}
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
        )

find_program(NUNIT32_COMMAND nunit-console-x86 nunit-console-x86.exe
        HINTS ${PC_NUNIT_INCLUDEDIR} ${PC_NUNIT_INCLUDE_DIRS} ${nunit_search_hints}
        PATH_SUFFIXES bin
        DOC "NUnit test runner command"
        )

if("${CSHARP_PLATFORM}" STREQUAL "x86")
    set(NUNIT_COMMAND ${NUNIT32_COMMAND})
else()
    set(NUNIT_COMMAND ${NUNIT64_COMMAND})
endif()

if(NOT NUNIT_LIBRARY OR NOT NUNIT_COMMAND)
    if(NUNIT_ROOT)
        if(NOT NUNIT_LIBRARY)
            message(WARNING "NUNIT library not found at root: ${NUNIT_ROOT}")
        else()
            message(WARNING "NUNIT command not found at root: ${NUNIT_ROOT} with library: ${NUNIT_LIBRARY}")
        endif()
    endif()
    include(ExternalProject)
    ExternalProject_Add(
            NUnit
            URL https://github.com/nunit/nunitv2/releases/download/2.6.4/NUnit-2.6.4.zip
            UPDATE_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
    )
    ExternalProject_Get_Property(NUnit SOURCE_DIR)
    set(NUNIT_LIBRARY ${SOURCE_DIR}/bin/nunit.framework.dll)
    set(NUNIT32_COMMAND ${SOURCE_DIR}/bin/nunit-console-x86.exe)
    set(NUNIT64_COMMAND ${SOURCE_DIR}/bin/nunit-console.exe)
    set(NUNIT_TARGET NUnit)
else()
    set(NUNIT_TARGET "")
endif()

if("${CSHARP_PLATFORM}" STREQUAL "x86")
    set(NUNIT_COMMAND ${NUNIT32_COMMAND})
else()
    set(NUNIT_COMMAND ${NUNIT64_COMMAND})
endif()

set(NUNIT_LIBRARIES ${NUNIT_LIBRARY} )
set(NUNIT_INCLUDE_DIR ${NUNIT_LIBRARY} )
set(NUNIT_INCLUDE_DIRS ${NUNIT_LIBRARY} )
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NUnit DEFAULT_MSG NUNIT_LIBRARY NUNIT_COMMAND)
message(STATUS "Found NUnit Runner: ${NUNIT_COMMAND}")
if(NUNIT_COMMAND)
    if(MONO_EXECUTABLE AND NUNIT_COMMAND MATCHES "(.exe)")
        set(NUNIT_COMMAND ${MONO_EXECUTABLE} ${NUNIT_COMMAND})
    endif()
endif()



mark_as_advanced(NUNIT_LIBRARIES NUNIT_INCLUDE_DIR NUNIT_INCLUDE_DIRS)
