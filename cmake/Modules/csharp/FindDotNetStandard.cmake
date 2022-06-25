# Find the msbuild tool
#
# DotNetStandard_FOUND             System has msbuild
# DotNetStandard_EXECUTABLE        Where to find csc
# DotNetStandard_VERSION           The version number of the DotNet framework

set(DotNetStandard_ROOT "" CACHE PATH "Set the location of the .NET root directory")
set(DotNetStandard_VERSION "" CACHE STRING "C# .NET compiler version" )

if(DotNetStandard_ROOT AND EXISTS "${DotNetStandard_ROOT}")
    find_program(DotNetStandard_EXECUTABLE dotnet dotnet.exe
            PATHS ${DotNetStandard_ROOT}
            PATH_SUFFIXES . bin
            NO_DEFAULT_PATH)
endif()


find_program(DotNetStandard_EXECUTABLE dotnet dotnet.exe)

if(EXISTS "${DotNetStandard_EXECUTABLE}")
    execute_process(
        COMMAND ${DotNetStandard_EXECUTABLE} --version
        OUTPUT_VARIABLE DotNetStandard_version_string
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string( REGEX MATCH "([0-9]*)([.])([0-9]*)([.]*)([0-9]*)" DotNetStandard_version_string "${DotNetStandard_version_string}" )
    set(DotNetStandard_VERSION ${DotNetStandard_version_string} CACHE STRING ".NET standard clr version" FORCE )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DotNetStandard DEFAULT_MSG DotNetStandard_EXECUTABLE)
mark_as_advanced(DotNetStandard_EXECUTABLE DotNetStandard_VERSION)

