# Add a target to build a C# library or executable
#
# csharp_add_library( name references [files] ) - Define C# library with the given name
#
# Examples:
#   csharp_add_library( MyLibrary "" "File1.cs" )
#   csharp_add_library( MyLibrary "ref1.dll ref2.dll" "File1.cs" )
#
# Note, the while you can reference a NuGet package, this code is untested if the package has multiple framework Dlls.

macro( csharp_add_library name )
    csharp_add_project( "library" ${name} ${ARGN} )
endmacro()

macro( csharp_add_executable name )
    csharp_add_project( "Exe" ${name} ${ARGN} )
endmacro()


macro( csharp_add_project type name )
    if(CSBUILD_PROJECT_DIR)
        set(CURRENT_TARGET_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CSBUILD_PROJECT_DIR}")
    else()
        set(CURRENT_TARGET_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
    endif()
    set(CSBUILD_PROJECT_DIR "")
    file(MAKE_DIRECTORY ${CURRENT_TARGET_BINARY_DIR})
    foreach( it ${ARGN} )
        if( ${it} MATCHES "(.*)(dll)" )
            file(TO_NATIVE_PATH ${it} nit)
            list( APPEND refs "<Reference;Include=\\\"${nit}\\\";/>" )
        elseif( ${it} MATCHES "(.*)(nupkg)" )
            file(TO_NATIVE_PATH ${it} nit)
            list( APPEND pkgs "<package;id=\\\"${nit}\\\";version=;/>" )
        elseif( ${it} MATCHES "(.*)=([0-9]*)([.])([0-9]*)([.]*)([0-9]*)" )
            string(REPLACE "=" ";" PACKAGE_ID "${it}")
            list(GET PACKAGE_ID 0 PACKAGE_NAME )
            list(GET PACKAGE_ID 1 PACKAGE_VERSION )
            list( APPEND packages "<PackageReference;Include=\\\"${PACKAGE_NAME}\\\";Version=\\\"${PACKAGE_VERSION}\\\";/>" )
            list( APPEND legacy_packages "<package;id=\\\"${PACKAGE_NAME}\\\";version=\\\"${PACKAGE_VERSION}\\\";/>" )
            file(TO_NATIVE_PATH "${CURRENT_TARGET_BINARY_DIR}/${PACKAGE_NAME}.${PACKAGE_VERSION}/lib/**/*.dll" hint_path)
            list( APPEND refs "<Reference;Include=\\\"${hint_path}\\\";></Reference>" )

            file(TO_NATIVE_PATH "${CURRENT_TARGET_BINARY_DIR}/${PACKAGE_NAME}.${PACKAGE_VERSION}/build/${PACKAGE_NAME}.targets" target_path)
            list(APPEND imports "<Import;Project=\\\"${target_path}\\\";Condition=\\\"Exists('${target_path}')\\\";/>")
        else( )
            if( EXISTS "${it}" )
                file(TO_NATIVE_PATH ${it} nit)
                list( APPEND sources "<Compile;Include=\\\"${nit}\\\";/>" )
                list( APPEND sources_dep ${it} )
            elseif( EXISTS "${CSBUILD_SOURCE_DIRECTORY}/${it}" )
                file(TO_NATIVE_PATH ${CSHARP_SOURCE_DIRECTORY}/${it} nit)
                list( APPEND sources "<Compile;Include=\\\"${nit}\\\";/>" )
                list( APPEND sources_dep ${CSHARP_SOURCE_DIRECTORY}/${it} )
            elseif( EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${it}" )
                if(DOTNET_FOUND)
                    string(REPLACE "/" "\\" nit "${CMAKE_CURRENT_SOURCE_DIR}/${it}")
                else()
                    file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${it} nit)
                endif()
                list( APPEND sources "<Compile;Include=\\\"${nit}\\\";/>" )
                list( APPEND sources_dep ${CMAKE_CURRENT_SOURCE_DIR}/${it} )
            elseif( ${it} MATCHES "[*]" )
                file(TO_NATIVE_PATH ${it} nit)
                FILE( GLOB it_glob ${it} )
                list( APPEND sources "<Compile;Include=\\\"${nit}\\\";/>" )
                list( APPEND sources_dep ${it_glob} )
            else()
                message(WARNING "not found ${it}")
            endif( )
        endif ( )
    endforeach( )
    list(LENGTH sources SOURCE_FILE_COUNT)
    list(LENGTH refs REFEENCE_COUNT)
    list(LENGTH packages PACKAGE_COUNT)
    list(LENGTH imports IMPORT_COUNT)
    if(SOURCE_FILE_COUNT GREATER 0)
        set(CSHARP_BUILDER_SOURCES "${sources}")
    else()
        messge(FATAL_ERROR "No C# source files for library")
    endif()
    if(REFEENCE_COUNT GREATER 0)
        set(CSHARP_BUILDER_ADDITIONAL_REFERENCES "${refs}")
    else()
        set(CSHARP_BUILDER_ADDITIONAL_REFERENCES "")
    endif()
    if(PACKAGE_COUNT GREATER 0)
        set(CSHARP_PACKAGE_REFERENCES "${packages}")
    else()
        set(CSHARP_PACKAGE_REFERENCES "")
    endif()
    if(PACKAGE_COUNT GREATER 0)
        set(CSHARP_LEGACY_PACKAGE_REFERENCES "${legacy_packages}")
    else()
        set(CSHARP_LEGACY_PACKAGE_REFERENCES "")
    endif()
    if(IMPORT_COUNT GREATER 0)
        set(CSHARP_IMPORTS "${imports}")
    else()
        set(CSHARP_IMPORTS "")
    endif()

    if( ${type} MATCHES "library" )
        set( ext "dll" )
    elseif( ${type} MATCHES "Exe" )
        if(DOTNET_CORE_FOUND)
            set( ext "dll" )
        else()
            set( ext "exe" )
        endif()
    endif()
    # TODO: <RuntimeIdentifier>osx.10.11-x64</RuntimeIdentifier>
    set(CSBUILD_${name}_BINARY "${CSHARP_BUILDER_OUTPUT_PATH}/${CSBUILD_OUPUT_PREFIX}${name}${CSBUILD_OUTPUT_SUFFIX}.${ext}")
    set(CSBUILD_${name}_BINARY_DIR "${CSBUILD_OUPUT_PREFIX}")
    set(CSBUILD_${name}_BINARY_NAME "${name}${CSBUILD_OUTPUT_SUFFIX}.${ext}")
    if(CSHARP_NUGET_SOURCE)
        set(CSHARP_NUGET_SOURCE_CMD -source ${CSHARP_NUGET_SOURCE})
    endif()
    set(CSBUILD_RESTORE_MORE_FLAGS "")
    if(RESTORE_EXE AND CSHARP_NUGET_SOURCE_CMD)
        set(RESTORE_CMD ${RESTORE_EXE} install ${CSHARP_NUGET_SOURCE_CMD})
    else()
        set(RESTORE_CMD ${CMAKE_COMMAND} -version)
        if(CSHARP_NUGET_SOURCE)
            # Note that for .NET Core 2.1.4 or before, the local repo must follow public repo
            set(CSBUILD_RESTORE_MORE_FLAGS --source https://api.nuget.org/v3/index.json --source ${CSHARP_NUGET_SOURCE} )
        endif()
    endif()



    set(CSBUILD_${name}_CSPROJ "${name}_${CSBUILD_CSPROJ}")
    file(TO_NATIVE_PATH ${CSHARP_BUILDER_OUTPUT_PATH} CSHARP_BUILDER_OUTPUT_PATH_NATIVE)
    add_custom_target(
            ${name} ALL
            ${CMAKE_COMMAND}
            -DCSHARP_TARGET_FRAMEWORK="${CSHARP_TARGET_FRAMEWORK}"
            -DCSHARP_BUILDER_OUTPUT_TYPE="${type}"
            -DCSHARP_BUILDER_OUTPUT_PATH="${CSHARP_BUILDER_OUTPUT_PATH_NATIVE}"
            -DCSHARP_PLATFORM="${CSHARP_PLATFORM}"
            -DCSHARP_BUILDER_OUTPUT_NAME="${name}${CSBUILD_OUTPUT_SUFFIX}"
            -DCSHARP_BUILDER_ADDITIONAL_REFERENCES="${CSHARP_BUILDER_ADDITIONAL_REFERENCES}"
            -DCSHARP_BUILDER_SOURCES="${CSHARP_BUILDER_SOURCES}"
            -DCSHARP_TARGET_FRAMEWORK_VERSION="${CSHARP_TARGET_FRAMEWORK_VERSION}"
            -DCSHARP_PACKAGE_REFERENCES="${CSHARP_PACKAGE_REFERENCES}"
            -DMSBUILD_TOOLSET="${MSBUILD_TOOLSET}"
            -DCSHARP_IMPORTS="${CSHARP_IMPORTS}"
            -DCONFIG_INPUT_FILE="${CSBUILD_CSPROJ_IN}"
            -DCONFIG_OUTPUT_FILE="${CURRENT_TARGET_BINARY_DIR}/${CSBUILD_${name}_CSPROJ}"
            -P ${CMAKE_SOURCE_DIR}/cmake/ConfigureFile.cmake

            COMMAND ${CMAKE_COMMAND}
            -DCSHARP_PACKAGE_REFERENCES="${CSHARP_LEGACY_PACKAGE_REFERENCES}"
            -DCONFIG_INPUT_FILE="${CMAKE_SOURCE_DIR}/cmake/Modules/csharp/packages.config.in"
            -DCONFIG_OUTPUT_FILE="${CURRENT_TARGET_BINARY_DIR}/packages.config"
            -P ${CMAKE_SOURCE_DIR}/cmake/ConfigureFile.cmake

            COMMAND ${RESTORE_CMD}

            COMMAND ${CSBUILD_EXECUTABLE} ${CSBUILD_RESTORE_FLAGS} ${CSBUILD_RESTORE_MORE_FLAGS} ${CSBUILD_${name}_CSPROJ}
            COMMAND ${CSBUILD_EXECUTABLE} ${CSBUILD_BUILD_FLAGS} ${CSBUILD_${name}_CSPROJ}
            WORKING_DIRECTORY ${CURRENT_TARGET_BINARY_DIR}
            COMMENT "${RESTORE_CMD};${CSBUILD_EXECUTABLE} ${CSBUILD_RESTORE_FLAGS} ${CSBUILD_RESTORE_MORE_FLAGS} ${CSBUILD_${name}_CSPROJ}; ${CSBUILD_EXECUTABLE} ${CSBUILD_BUILD_FLAGS} ${CSBUILD_${name}_CSPROJ} -> ${CURRENT_TARGET_BINARY_DIR}"
            DEPENDS ${sources_dep}
    )
    unset(ext)
    unset(CSHARP_BUILDER_SOURCES)
    unset(CSHARP_BUILDER_OUTPUT_PATH_NATIVE)
    unset(sources)
    unset(refs)

endmacro()