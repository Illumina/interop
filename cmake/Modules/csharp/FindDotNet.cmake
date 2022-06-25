# Find the msbuild tool
#
# DotNet_FOUND             System has msbuild
# DotNet_EXECUTABLE        Where to find csc
# MSBUILD_EXECUTABLE      Where to find 'msbuild'
# DotNet_EXECUTABLE_VERSION           The version number of the DotNet framework

if(NOT WIN32)
    return()
endif()

set(DotNet_ROOT "" CACHE PATH "Set the location of the .NET root directory")
set(DotNet_EXECUTABLE_VERSION "" CACHE STRING "C# .NET compiler version" )

if(DotNet_ROOT AND EXISTS "${DotNet_ROOT}")
    find_program(DotNet_EXECUTABLE csc.exe
            PATHS ${DotNet_ROOT}
            PATH_SUFFIXES bin
            NO_DEFAULT_PATH)
    find_program(MSBUILD_EXECUTABLE msbuild.exe
            PATHS ${DotNet_ROOT}
            PATH_SUFFIXES bin
            NO_DEFAULT_PATH)
endif()

if( ${CSHARP_PLATFORM} MATCHES "x64|itanium" )
    set( csharp_DotNet_bin_hints "$ENV{windir}\\Microsoft.NET\\Framework64" )
else( )
    set( csharp_DotNet_bin_hints "$ENV{windir}\\Microsoft.NET\\Framework" )
endif( )
file(TO_CMAKE_PATH "${csharp_DotNet_bin_hints}" csharp_DotNet_bin_hints)

if(DotNet_EXECUTABLE_VERSION)
    set(csharp_DotNet_bin_hints "${csharp_DotNet_bin_hints}/${DotNet_EXECUTABLE_VERSION}")
else()
    file(GLOB_RECURSE csharp_DotNet_executables ${csharp_DotNet_bin_hints}/csc.exe)
    list(SORT csharp_DotNet_executables )
    list(REVERSE csharp_DotNet_executables )
    list(GET csharp_DotNet_executables 0 DotNet_executable)
    string( REPLACE "${csharp_DotNet_bin_hints}/" "" csharp_DotNet_version_temp ${DotNet_executable} )
    string( REPLACE "/csc.exe" "" csharp_DotNet_version_temp ${csharp_DotNet_version_temp} )
    set(DotNet_EXECUTABLE_VERSION "${csharp_DotNet_version_temp}" CACHE STRING "C# .NET compiler version" FORCE )
    set(csharp_DotNet_bin_hints "${csharp_DotNet_bin_hints}/${DotNet_EXECUTABLE_VERSION}")
endif()
find_program(DotNet_EXECUTABLE csc.exe
        HINTS ${csharp_DotNet_bin_hints}
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
        )

find_program(MSBUILD_EXECUTABLE msbuild.exe
        HINTS ${csharp_DotNet_bin_hints}
        PATH_SUFFIXES bin
        NO_DEFAULT_PATH
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DotNet DEFAULT_MSG MSBUILD_EXECUTABLE DotNet_EXECUTABLE)
mark_as_advanced(DotNet_EXECUTABLE MSBUILD_EXECUTABLE DotNet_EXECUTABLE_VERSION)