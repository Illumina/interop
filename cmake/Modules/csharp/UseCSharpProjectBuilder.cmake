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
    csharp_add_project("Exe" ${name} ${ARGN})
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
        elseif( ${it} MATCHES "(.*)(nuspec)" )
            list(APPEND deps ${it})
            if(NOT CSBUILD_NUSPEC_FILE)
                set(CSBUILD_NUSPEC_FILE ${it})
            else()
                message(FATAL_ERROR "Nuspec file cannot be specified twice, already using: ${CSBUILD_NUSPEC_FILE}, but trying to add ${it}")
            endif()
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
                if(DotNet_FOUND)
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
        set(CSHARP_LEGACY_PACKAGE_REFERENCES "${legacy_packages}")
    else()
        set(CSHARP_PACKAGE_REFERENCES "")
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
        if(DotNetStandard_FOUND)
            set( ext "dll" )
        else()
            set( ext "exe" )
        endif()
    endif()
    # TODO: <RuntimeIdentifier>osx.10.11-x64</RuntimeIdentifier>
    set(CSBUILD_${name}_BINARY_DIR "${CSHARP_BUILDER_OUTPUT_PATH}")
    if(DotNetStandard_FOUND)
        set(CSBUILD_${name}_BINARY_DIR "${CSHARP_BUILDER_OUTPUT_PATH}/${CSHARP_TARGET_FRAMEWORK}/publish")
    endif()
    set(CSBUILD_${name}_BINARY_FILE "${CSBUILD_OUPUT_PREFIX}${name}${CSBUILD_OUTPUT_SUFFIX}")
    set(CSBUILD_${name}_BINARY_NAME "${CSBUILD_${name}_BINARY_FILE}.${ext}")
    set(CSBUILD_${name}_BINARY "${CSBUILD_${name}_BINARY_DIR}/${CSBUILD_${name}_BINARY_NAME}")
    message(STATUS "CSHARP_BUILDER_OUTPUT_PATH=${CSHARP_BUILDER_OUTPUT_PATH}")
    message(STATUS "CSBUILD_${name}_BINARY_DIR=${CSBUILD_${name}_BINARY_DIR}")
    if(CSHARP_NUGET_SOURCE)
        set(CSHARP_NUGET_SOURCE_CMD -source ${CSHARP_NUGET_SOURCE})
    else()
        set(CSHARP_NUGET_SOURCE_CMD  -source ${CMAKE_CURRENT_BINARY_DIR}/cache)
    endif()
    set(CSBUILD_RESTORE_MORE_FLAGS "")
    if(RESTORE_EXE AND CSHARP_NUGET_SOURCE_CMD)
        set(RESTORE_CMD ${RESTORE_EXE} install ${CSHARP_NUGET_SOURCE_CMD})
    else()
        set(RESTORE_CMD ${CMAKE_COMMAND} -version)
        if(CSHARP_NUGET_SOURCE)
            # Note that for .NET Core 2.1.4 or before, the local repo must follow public repo
            set(CSBUILD_RESTORE_MORE_FLAGS --source https://api.nuget.org/v3/index.json --source ${CSHARP_NUGET_SOURCE} --no-cache)
        endif()
    endif()

    if(CSBUILD_DISABLE_CONFIG)
        set(CSBUILD_CONFIG "")
    endif()

    set(PACK_EXE ${CSBUILD_EXECUTABLE})
    if(DotNetStandard_FOUND AND CSBUILD_NUSPEC_FILE)
        if(NOT CSHARP_NUGET_PREFIX)
            set(CSHARP_NUGET_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/dist)
        endif()
        set(CSBUILD_PACK_FLAGS pack -p:NuspecFile=${CSBUILD_NUSPEC_FILE} --output ${CSHARP_NUGET_PREFIX} ${CSBUILD_CONFIG} ${CSBUILD_${name}_CSPROJ})
        set(PACK_EXE ${CSBUILD_EXECUTABLE})
    elseif(CSBUILD_NUSPEC_FILE)
        set(PACK_EXE ${RESTORE_EXE})
        set(CSBUILD_PACK_FLAGS pack ${CSBUILD_NUSPEC_FILE} -OutputDirectory ${CSHARP_NUGET_PREFIX})
    endif()
    message(STATUS "CSBUILD_PACK_FLAGS=${CSBUILD_PACK_FLAGS}")
    message(STATUS "DotNetStandard_FOUND=${DotNetStandard_FOUND}")
    message(STATUS "CSBUILD_NUSPEC_FILE=${CSBUILD_NUSPEC_FILE}")

    set(IPA_VALID_ARM64_PROCESSORS "aarch64;ARM64;arm64")
    if(CMAKE_SYSTEM_PROCESSOR IN_LIST IPA_VALID_ARM64_PROCESSORS)
        set(DotNetCore_PLATFORM "arm64" CACHE STRING "C# target platform: x86, x64, anycpu, or itanium")
    else()
        set(DotNetCore_PLATFORM "x64" CACHE STRING "C# target platform: x86, x64, anycpu, or itanium")
    endif()

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/cmake/ConfigureFile.cmake "configure_file(\${CONFIG_INPUT_FILE} \${CONFIG_OUTPUT_FILE} @ONLY)")
    set(CSBUILD_${name}_CSPROJ "${name}_${CSBUILD_CSPROJ}")
    file(TO_NATIVE_PATH ${CSHARP_BUILDER_OUTPUT_PATH} CSHARP_BUILDER_OUTPUT_PATH_NATIVE)
    add_custom_target(
            ${name} ALL
            ${CMAKE_COMMAND}
            -DCSHARP_TARGET_FRAMEWORK="${CSHARP_TARGET_FRAMEWORK}"
            -DCSHARP_BUILDER_OUTPUT_TYPE="${type}"
            -DCSHARP_BUILDER_OUTPUT_PATH="${CSHARP_BUILDER_OUTPUT_PATH_NATIVE}"
            -DCSHARP_PLATFORM="${CSHARP_PLATFORM}"
            -DCSHARP_BUILDER_OUTPUT_NAME="${CSBUILD_${name}_BINARY_FILE}"
            -DCSHARP_BUILDER_ADDITIONAL_REFERENCES="${CSHARP_BUILDER_ADDITIONAL_REFERENCES}"
            -DCSHARP_BUILDER_SOURCES="${CSHARP_BUILDER_SOURCES}"
            -DCSHARP_TARGET_FRAMEWORK_VERSION="${CSHARP_TARGET_FRAMEWORK_VERSION}"
            -DCSHARP_PACKAGE_REFERENCES="${CSHARP_PACKAGE_REFERENCES}"
            -DDotNetCore_PLATFORM="${DotNetCore_PLATFORM}"
            -DMSBUILD_TOOLSET="${MSBUILD_TOOLSET}"
            -DCSHARP_IMPORTS="${CSHARP_IMPORTS}"
            -DCONFIG_INPUT_FILE="${CSBUILD_CSPROJ_IN}"
            -DCONFIG_OUTPUT_FILE="${CURRENT_TARGET_BINARY_DIR}/${CSBUILD_${name}_CSPROJ}"
            -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/ConfigureFile.cmake

            COMMAND ${CMAKE_COMMAND}
            -DCSHARP_PACKAGE_REFERENCES="${CSHARP_LEGACY_PACKAGE_REFERENCES}"
            -DCONFIG_INPUT_FILE="${CSBUILD_ROOT_DIR}/packages.config.in"
            -DCONFIG_OUTPUT_FILE="${CURRENT_TARGET_BINARY_DIR}/packages.config"
            -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/ConfigureFile.cmake

            COMMAND ${RESTORE_CMD}

            COMMAND ${CSBUILD_CLEAR_NUGET_CACHE}
            COMMAND ${CSBUILD_EXECUTABLE} ${CSBUILD_RESTORE_FLAGS} ${CSBUILD_RESTORE_MORE_FLAGS} ${CSBUILD_${name}_CSPROJ}
            COMMAND ${CSBUILD_EXECUTABLE} ${CSBUILD_CLEAN_FLAGS} ${CSBUILD_${name}_CSPROJ}
            COMMAND ${CSBUILD_EXECUTABLE} ${CSBUILD_BUILD_FLAGS} ${CSBUILD_${name}_CSPROJ}
            COMMAND ${PACK_EXE} ${CSBUILD_PACK_FLAGS}
            WORKING_DIRECTORY ${CURRENT_TARGET_BINARY_DIR}
            COMMENT "${RESTORE_CMD};${CSBUILD_EXECUTABLE} ${CSBUILD_RESTORE_FLAGS} ${CSBUILD_RESTORE_MORE_FLAGS} ${CSBUILD_${name}_CSPROJ}; ${CSBUILD_EXECUTABLE} ${CSBUILD_BUILD_FLAGS} ${CSBUILD_${name}_CSPROJ}; ${PACK_EXE} ${CSBUILD_PACK_FLAGS} -> ${CURRENT_TARGET_BINARY_DIR}"
            DEPENDS ${sources_dep} ${deps}
    )
    unset(ext)
    unset(CSHARP_BUILDER_SOURCES)
    unset(CSHARP_BUILDER_OUTPUT_PATH_NATIVE)
    unset(sources)
    unset(refs)

endmacro()