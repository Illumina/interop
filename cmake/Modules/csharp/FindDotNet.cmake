# Find the msbuild tool
#
# DOTNET_FOUND             System has msbuild
# DOTNET_EXECUTABLE        Where to find csc
# MSBUILD_EXECUTABLE      Where to find 'msbuild'
# DOTNET_EXECUTABLE_VERSION           The version number of the DotNet framework

if(NOT WIN32)
    return()
endif()

set(DOTNET_ROOT "" CACHE PATH "Set the location of the .NET root directory")
set(DOTNET_EXECUTABLE_VERSION "" CACHE STRING "C# .NET compiler version" )

if(DOTNET_ROOT AND EXISTS ${DOTNET_ROOT})
    find_program(DOTNET_EXECUTABLE csc.exe
            PATHS ${DOTNET_ROOT}
            PATH_SUFFIXES bin
            NO_DEFAULT_PATH)
    find_program(MSBUILD_EXECUTABLE msbuild.exe
            PATHS ${DOTNET_ROOT
            PATH_SUFFIXES bin
            NO_DEFAULT_PATH)
endif()

if( ${CSHARP_PLATFORM} MATCHES "x64|itanium" )
    set( csharp_DOTNET_bin_hints "$ENV{windir}\\Microsoft.NET\\Framework64" )
else( )
    set( csharp_DOTNET_bin_hints "$ENV{windir}\\Microsoft.NET\\Framework" )
endif( )
file(TO_CMAKE_PATH "${csharp_DOTNET_bin_hints}" csharp_DOTNET_bin_hints)

if(DOTNET_EXECUTABLE_VERSION)
    set(csharp_DOTNET_bin_hints "${csharp_DOTNET_bin_hints}/${DOTNET_EXECUTABLE_VERSION}")
else()
    file(GLOB_RECURSE csharp_dotnet_executables ${csharp_DOTNET_bin_hints}/csc.exe)
    list(SORT csharp_dotnet_executables )
    list(REVERSE csharp_dotnet_executables )
    list(GET csharp_dotnet_executables 0 dotnet_executable)
    string( REPLACE "${csharp_DOTNET_bin_hints}/" "" csharp_dotnet_version_temp ${dotnet_executable} )
    string( REPLACE "/csc.exe" "" csharp_dotnet_version_temp ${csharp_dotnet_version_temp} )
    set(DOTNET_EXECUTABLE_VERSION "${csharp_dotnet_version_temp}" CACHE STRING "C# .NET compiler version" FORCE )
    set(csharp_DOTNET_bin_hints "${csharp_DOTNET_bin_hints}/${DOTNET_EXECUTABLE_VERSION}")
endif()
find_program(DOTNET_EXECUTABLE csc.exe
        HINTS ${csharp_DOTNET_bin_hints}
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
        )

find_program(MSBUILD_EXECUTABLE msbuild.exe
        HINTS ${csharp_DOTNET_bin_hints}
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DOTNET DEFAULT_MSG MSBUILD_EXECUTABLE DOTNET_EXECUTABLE)
mark_as_advanced(DOTNET_EXECUTABLE MSBUILD_EXECUTABLE DOTNET_EXECUTABLE_VERSION)