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
pkg_check_modules(PC_RAPIDXML QUIET rapidxml)

set(NUNIT_ROOT "" CACHE PATH "Set the location of the RapidXML library and header")

set(nunit_search_hints
        [HKEY_CURRENT_USER\\Software\\nunit.org\\NUnit\\2.6.4]
        "C:\\Program Files (x86)\\NUnit 2.6.4"
        /usr/local//Cellar/mono/4.0.1/lib/mono/4.5/
        /usr/lib/monodevelop/AddIns/NUnit
        /usr/lib/mono/4.0
        /usr/lib/mono/3.5
        /usr/lib/mono/2.0
        /usr/lib/mono/1.0
        /usr/lib/nunit
        ${NUNIT_ROOT}
        $ENV{NUNIT_DIR}
        )

find_file(NUNIT_LIBRARY nunit.framework.dll
        HINTS ${PC_NUNIT_INCLUDEDIR} ${PC_NUNIT_INCLUDE_DIRS} ${nunit_search_hints}
        PATH_SUFFIXES bin bin/framework
        )

find_program(NUNIT_COMMAND nunit-console.exe
        HINTS ${PC_NUNIT_INCLUDEDIR} ${PC_NUNIT_INCLUDE_DIRS} ${nunit_search_hints}
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
        )

find_program(NUNIT_COMMAND nunit-console.exe
        HINTS ${PC_NUNIT_INCLUDEDIR} ${PC_NUNIT_INCLUDE_DIRS} ${nunit_search_hints}
        PATH_SUFFIXES bin tools
        DOC "NUnit test runner command"
        )

message(WARNING "${NUNIT_ROOT} -- ${NUNIT_LIBRARY} -- ${NUNIT_COMMAND}")
#if(NOT NUNIT_LIBRARY OR NOT NUNIT_COMMAND)
if(NOT NUNIT_LIBRARY AND NOT NUNIT_COMMAND)
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
    if("${CSHARP_PLATFORM}" STREQUAL "x86") # TODO: Add AnyCpu support
        set(NUNIT_COMMAND ${SOURCE_DIR}/bin/nunit-console-x86.exe)
    else()
        set(NUNIT_COMMAND ${SOURCE_DIR}/bin/nunit-console.exe)
    endif()
    set(NUNIT_TARGET NUnit)
else()
    set(NUNIT_TARGET "")
endif()

set(NUNIT_LIBRARIES ${NUNIT_LIBRARY} )
set(NUNIT_INCLUDE_DIR ${NUNIT_LIBRARY} )
set(NUNIT_INCLUDE_DIRS ${NUNIT_LIBRARY} )
include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(NUnit DEFAULT_MSG NUNIT_LIBRARY NUNIT_COMMAND)
find_package_handle_standard_args(NUnit DEFAULT_MSG NUNIT_COMMAND)

if(CSHARP_MONO_FOUND AND CSHARP_INTERPRETER AND NUNIT_COMMAND)
    set(NUNIT_COMMAND ${CSHARP_INTERPRETER} ${NUNIT_COMMAND})
endif()

mark_as_advanced(NUNIT_LIBRARIES NUNIT_INCLUDE_DIR NUNIT_INCLUDE_DIRS)