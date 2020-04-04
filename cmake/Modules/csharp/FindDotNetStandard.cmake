# Find the msbuild tool
#
# DOTNET_STANDARD_FOUND             System has msbuild
# DOTNET_STANDARD_EXECUTABLE        Where to find csc
# DOTNET_STANDARD_VERSION           The version number of the DotNet framework

set(DOTNET_STANDARD_ROOT "" CACHE PATH "Set the location of the .NET root directory")
set(DOTNET_STANDARD_VERSION "" CACHE STRING "C# .NET compiler version" )

if(DOTNET_STANDARD_ROOT AND EXISTS "${DOTNET_STANDARD_ROOT}")
    find_program(DOTNET_STANDARD_EXECUTABLE dotnet dotnet.exe
            PATHS ${DOTNET_STANDARD_ROOT}
            PATH_SUFFIXES . bin
            NO_DEFAULT_PATH)
endif()


find_program(DOTNET_STANDARD_EXECUTABLE dotnet dotnet.exe)

if(EXISTS "${DOTNET_STANDARD_EXECUTABLE}")
    execute_process(
        COMMAND ${DOTNET_STANDARD_EXECUTABLE} --version
        OUTPUT_VARIABLE dotnet_standard_version_string
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string( REGEX MATCH "([0-9]*)([.])([0-9]*)([.]*)([0-9]*)" dotnet_standard_version_string "${dotnet_standard_version_string}" )
    set(DOTNET_STANDARD_VERSION ${dotnet_standard_version_string} CACHE STRING ".NET standard clr version" FORCE )
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DOTNET_STANDARD DEFAULT_MSG DOTNET_STANDARD_EXECUTABLE)
mark_as_advanced(DOTNET_STANDARD_EXECUTABLE DOTNET_STANDARD_VERSION)

