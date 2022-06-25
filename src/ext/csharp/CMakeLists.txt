# Build a C# Shared library using SWIG


if(NOT MSVC)
    set(CSBUILD_DISABLE_CONFIG ON)
endif()
find_package(CSBuild)

set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/csharp_lib/interop)

add_custom_target(csharp_lib)
set_target_properties(csharp_lib PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
set(SWIG_VERSION_INFO "")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(SWIG_SHARED_LIBRARY_SUFFIX ".dylib")
    set(SWIG_SHARED_LIBRARY_PREFIX "lib")
else()
    set(SWIG_SHARED_LIBRARY_SUFFIX "${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(SWIG_SHARED_LIBRARY_PREFIX "${CMAKE_SHARED_LIBRARY_PREFIX}")
endif()

set(SWIG_ADDITIONAL_FLAGS "")
set(CSBUILD_PROJECT_DIR csharp_lib)
set(CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/${CSBUILD_PROJECT_DIR})
foreach(SRC ${SWIG_SRCS})
    get_filename_component(MODULE ${SRC} NAME_WE)
    set(MODULE_NAME ${MODULE})
    string(REPLACE "_" ";" MOD_LIST ${MODULE_NAME})
    list(LENGTH MOD_LIST MOD_LIST_LEN)
    if(MOD_LIST_LEN EQUAL 2)
        list(GET MOD_LIST 0 MOD1)
        list(GET MOD_LIST 1 MOD2)
        string(SUBSTRING ${MOD2} 0 1 FIRST_LETTER)
        string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
        string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" MOD2 "${MOD2}")
        set(MODULE_NAME "${MOD1}${MOD2}")
    endif()
    string(SUBSTRING ${MODULE_NAME} 0 1 FIRST_LETTER)
    string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
    string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" NAMESPACE "${MODULE_NAME}")
    # The preview of .NET Core requires explicit shared library name
    # This workaround does not work with UseSWIG.cmake (2017-06-01), had to modify it UseSWIG2.cmake
    if(DotNetStandard_FOUND AND DotNetStandard_VERSION VERSION_LESS "2.1.0")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(SWIG_ADDITIONAL_FLAGS ";-dllimport;${CMAKE_SHARED_LIBRARY_PREFIX}c_csharp_${MODULE}.so")
        else()
            set(SWIG_ADDITIONAL_FLAGS ";-dllimport;${CMAKE_SHARED_LIBRARY_PREFIX}c_csharp_${MODULE}${CMAKE_SHARED_LIBRARY_SUFFIX}")
        endif()
    endif()
    if(SWIG_WORDSIZE_FLAG)
        if(SWIG_ADDITIONAL_FLAGS)
            set(SWIG_ADDITIONAL_FLAGS "${SWIG_ADDITIONAL_FLAGS};${SWIG_WORDSIZE_FLAG}")
        else()
            set(SWIG_ADDITIONAL_FLAGS ";${SWIG_WORDSIZE_FLAG}")
        endif()
    endif()
    #set_source_files_properties(${SRC} PROPERTIES SWIG_MODULE_NAME c_csharp_${MODULE})
    set_source_files_properties(${SRC} PROPERTIES SWIG_FLAGS "-namespace;Illumina.InterOp.${NAMESPACE};-module;c_csharp_${MODULE}${SWIG_ADDITIONAL_FLAGS}")
    set_source_files_properties(${SRC} PROPERTIES CPLUSPLUS ON)

    if(MSVC)
        set(SWIG_VERSION_INFO ${CURRENT_BINARY_DIR}/${MODULE}_version.rc)
        set(LIB_NAME "${MODULE}") # Used to configure the version info file
        configure_file(${CMAKE_SOURCE_DIR}/cmake/version.rc.in ${SWIG_VERSION_INFO} @ONLY) # Requires: LIB_NAME, VERSION_LIST and VERSION
    endif()
    set(SWIG_MODULE_c_csharp_${MODULE}_EXTRA_DEPS ${SWIG_DEPS} ${SWIG_VERSION_INFO})
    swig_add_library(c_csharp_${MODULE} TYPE SHARED LANGUAGE csharp SOURCES ${SRC} ${SWIG_VERSION_INFO})
    set(SWIG_MODULE ${SWIG_MODULE_c_csharp_${MODULE}_REAL_NAME})
    set(EXTRA_LINKER_FLAGS)
    set_target_properties(${SWIG_MODULE} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_BINARY_DIR}/Release"
            LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_BINARY_DIR}/Debug"
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Debug"
            )
    set_target_properties(${SWIG_MODULE} PROPERTIES PREFIX "${SWIG_SHARED_LIBRARY_PREFIX}")
    set_target_properties(${SWIG_MODULE} PROPERTIES SUFFIX "${SWIG_SHARED_LIBRARY_SUFFIX}")
    if(MINGW)
        set(EXTRA_LINKER_FLAGS "-Wl,-add-stdcall-alias")
        get_target_property(existing_link_flags ${SWIG_MODULE} LINK_FLAGS)
        if(existing_link_flags)
            set(EXTRA_LINKER_FLAGS "${existing_link_flags} ${EXTRA_LINKER_FLAGS}")
        endif()
        set_target_properties(${SWIG_MODULE} PROPERTIES LINK_FLAGS "${EXTRA_LINKER_FLAGS}")
    endif()
    if(MSVC)
        set_target_properties(${SWIG_MODULE} PROPERTIES COMPILE_FLAGS "${ENABLE_BIG_OBJ_FLAG} /wd4100 /wd4702")
    elseif(MINGW)
        set_target_properties(${SWIG_MODULE} PROPERTIES COMPILE_FLAGS "${ENABLE_BIG_OBJ_FLAG} ${_WNO_UNUSED_FUNCTION} ${_WNO_UNUSED_PARAMETER}")
    elseif(COMPILER_IS_GNUCC_OR_CLANG)
        set_target_properties(${SWIG_MODULE} PROPERTIES COMPILE_FLAGS "${_WNO_UNUSED_FUNCTION} ${_WNO_UNUSED_PARAMETER}")
    endif()

    swig_link_libraries(${SWIG_MODULE} ${INTEROP_DL_LIB})

    if(WIN32)
        set_target_properties(${SWIG_MODULE} PROPERTIES PREFIX "")
    endif()

    list(APPEND SWIG_MODULE_LIST ${SWIG_MODULE})
    add_dependencies(csharp_lib ${SWIG_MODULE})
endforeach()


if(NOT CSBUILD_FOUND)
    message(WARNING "C# compiler not found, unable to generate C# binding library")
    return()
endif()

include(${CSBUILD_USE_FILE})

message(STATUS "AssemblyInfo VERSION_SHORT=${VERSION_SHORT}")
set(SWIG_GEN_CSHARP_SOURCE_FILES ${CURRENT_BINARY_DIR}/interop/*.cs CACHE INTERNAL "C# source files generated by SWIG" FORCE)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/AssemblyInfo.cs.in  ${CURRENT_BINARY_DIR}/interop/AssemblyInfo.cs @ONLY)

if(NOT CSHARP_NUGET_PREFIX)
    if(PACKAGE_OUTPUT_FILE_PREFIX)
        message(STATUS "CPACK_OUTPUT_FILE_PREFIX=${PACKAGE_OUTPUT_FILE_PREFIX}")
        set(CSHARP_NUGET_PREFIX "${PACKAGE_OUTPUT_FILE_PREFIX}")
    elseif(CPACK_OUTPUT_FILE_PREFIX)
        message(STATUS "CPACK_OUTPUT_FILE_PREFIX=${CPACK_OUTPUT_FILE_PREFIX}")
        set(CSHARP_NUGET_PREFIX "${CPACK_OUTPUT_FILE_PREFIX}")
    else()
        message(STATUS "CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR}")
        set(CSHARP_NUGET_PREFIX "${CMAKE_BINARY_DIR}/dist")
    endif()
endif()
message(STATUS "CSHARP_NUGET_PREFIX=${CSHARP_NUGET_PREFIX}")
get_filename_component(CSHARP_NUGET_PREFIX ${CSHARP_NUGET_PREFIX} ABSOLUTE)

set(CSHARP_NUGET_PREFIX "${CSHARP_NUGET_PREFIX}" CACHE INTERNAL "")
message(STATUS "CPACK_OUTPUT_FILE_PREFIX=${CPACK_OUTPUT_FILE_PREFIX}")
message(STATUS "CSHARP_NUGET_PREFIX=${v}")
if(NOT EXISTS CSHARP_NUGET_PREFIX)
    make_directory(${CSHARP_NUGET_PREFIX})
endif()

csharp_add_library(csharp_interop "${CURRENT_BINARY_DIR}/interop/*.cs" ${CMAKE_CURRENT_BINARY_DIR}/package.nuspec)
add_dependencies(csharp_interop ${SWIG_MODULE_LIST})
add_custom_target(nupack)
add_dependencies(nupack csharp_interop)

if(TARGET bundle)
    add_dependencies(bundle nupack)
endif()

add_custom_command(TARGET csharp_interop
        POST_BUILD
        COMMENT "Remove directory: ${CURRENT_BINARY_DIR}/obj"
        COMMAND ${CMAKE_COMMAND} -E echo "Remove directory: ${CURRENT_BINARY_DIR}/obj"
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CURRENT_BINARY_DIR}/obj
        )

foreach(SRC ${SWIG_SRCS})
    get_filename_component(MODULE ${SRC} NAME_WE)
    add_custom_command(
            TARGET csharp_interop
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${SWIG_MODULE_c_csharp_${MODULE}_REAL_NAME}> $<TARGET_FILE_DIR:${INTEROP_DL_LIB}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CSBUILD_csharp_interop_BINARY} $<TARGET_FILE_DIR:${INTEROP_DL_LIB}>
            COMMENT "copy ${CSBUILD_csharp_interop_BINARY} $<TARGET_FILE_DIR:${INTEROP_DL_LIB}>"
    )
endforeach()
set(SWIG_CSHARP_LIBRARY ${CSBUILD_csharp_interop_BINARY} CACHE INTERNAL "C# library generated by SWIG" FORCE)

if(ENABLE_EXAMPLES)
    foreach(SRC ${SWIG_SRCS})
        get_filename_component(MODULE ${SRC} NAME_WE)
        install(TARGETS ${SWIG_MODULE_c_csharp_${MODULE}_REAL_NAME}
                LIBRARY DESTINATION share/illumina/interop/examples
                ARCHIVE DESTINATION lib64)
    endforeach()
    install(FILES ${CSBUILD_csharp_interop_BINARY} DESTINATION share/illumina/interop/examples)
endif()

foreach(SRC ${SWIG_SRCS})
    get_filename_component(MODULE ${SRC} NAME_WE)
    install(TARGETS ${SWIG_MODULE_c_csharp_${MODULE}_REAL_NAME}
            LIBRARY DESTINATION lib64
            ARCHIVE DESTINATION lib64)
endforeach()
install(FILES ${CSBUILD_csharp_interop_BINARY}
        DESTINATION lib64
        )
install(DIRECTORY ${CURRENT_BINARY_DIR}/interop
        DESTINATION share/illumina/interop/src
        FILES_MATCHING
        PATTERN "*.cs"
        PATTERN "*.cxx"
        PATTERN "CMakeFiles" EXCLUDE
        PATTERN "${CMAKE_BUILD_TYPE}" EXCLUDE)

########################################################################################################################
# Create NuGet nuspec package file
########################################################################################################################
if(MSVC)
    set(CONFIG_DIR "$<CONFIG>")
else()
    if(CMAKE_BUILD_TYPE)
        set(CONFIG_DIR "${CMAKE_BUILD_TYPE}")
    else()
        set(CONFIG_DIR "Debug")
   endif()
endif()

set(NUGET_SYS_ID "${CMAKE_SYSTEM_NAME}${PACKAGE_SUFFIX}")
set(DOT_NET_FRAMEWORK "net40")
set(DOT_NET_FRAMEWORK "${CSHARP_TYPE}")
set(DOT_NET_FRAMEWORK "netstandard")
get_filename_component(DLL_NAME ${CSBUILD_csharp_interop_BINARY} NAME)
if(BUILD_NUMBER)
    MATH(EXPR VERSION_PATCHN "${VERSION_PATCH}+1")
    set(NUGET_VERSION_STR ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCHN}-alpha${BUILD_NUMBER} CACHE INTERNAL "")
else()
    set(NUGET_VERSION_STR ${VERSION_SHORT} CACHE INTERNAL "")
endif()
message(STATUS "NUGET version for package: ${NUGET_VERSION_STR}")
if("${CMAKE_HOST_SYSTEM}" MATCHES ".*Windows.*")
    set(SHARED_LIB_SEARCH_STR "\\*${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(NUGET_PLATFORM "lib\\${DOT_NET_FRAMEWORK}")
else()
    set(SHARED_LIB_SEARCH_STR "/*${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(NUGET_PLATFORM "lib/${DOT_NET_FRAMEWORK}")
endif()
set(NUGET_PACKAGE_ID illumina_interop_${CSHARP_TYPE}_${NUGET_SYS_ID} CACHE INTERNAL "")
# This creates the nuspec file in the proper directory

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${NUGET_PACKAGE_ID}.targets ${CMAKE_CURRENT_BINARY_DIR}/package.nuspec
        COMMENT "Creating package.nuspec -> ${CMAKE_CURRENT_BINARY_DIR}/package.nuspec from ${CSBUILD_csharp_interop_BINARY_DIR}${CSBUILD_csharp_interop_BINARY_NAME}"
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/package.nuspec.in

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/package.targets
        ${CMAKE_CURRENT_BINARY_DIR}/${NUGET_PACKAGE_ID}.targets

        COMMAND ${CMAKE_COMMAND}
        -DCOMPILER_INFO="${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}"
        -DCMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
        -DCSHARP_TYPE=${CSHARP_TYPE}
        -DCSHARP_VERSION=${CSHARP_VERSION}
        -DNUGET_PLATFORM=${NUGET_PLATFORM}
        -DNUGET_PACKAGE_ID=${NUGET_PACKAGE_ID}
        -DINTEROP_VERSION=${NUGET_VERSION_STR}
        -DPLATFORM=${CMAKE_SYSTEM_NAME}${PACKAGE_SUFFIX}
        -DCONFIG="${CONFIG_DIR}"
        -DCSHARP_LIBRARY="${CSBUILD_csharp_interop_BINARY}"
        -DSHARED_LIB_SEARCH="${SHARED_LIB_SEARCH_STR}"
        -DCONFIG_INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/package.nuspec.in
        -DCONFIG_OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/package.nuspec
        -P ${CMAKE_CURRENT_BINARY_DIR}/cmake/ConfigureFile.cmake
        )
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/LICENSE.txt "GPL v3")


set(CSHARP_BUILD_AVAILABLE ${CSHARP_BUILDER_VERSION} CACHE INTERNAL "All dependencies are satisfied for the C# Build" FORCE)
