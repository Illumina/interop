set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(SWIG)
if(NOT SWIG_FOUND)
    message(WARNING "Swig not found. Skipping interface generation.")
    return()
endif()
include(${PROJECT_SOURCE_DIR}/cmake/CheckSWIGVersion.cmake)

check_swig_version(swig_major_ver)
if(swig_major_ver GREATER "4")
    message(FATAL_ERROR "Swig version ${swig_major_ver} found, but version 3.x or earlier is required. Skipping interface generation.")
    return()
endif()
if(swig_major_ver LESS "3")
    message(FATAL_ERROR "Swig version ${swig_major_ver} found, but version 3.x or later is required. Skipping interface generation.")
    return()
endif()

# Support earlier and later versions of CMake
if(CMAKE_VERSION VERSION_LESS 3.8)
    macro(swig_add_library name)
        set(options "")
        set(oneValueArgs LANGUAGE
                TYPE)
        set(multiValueArgs SOURCES)
        cmake_parse_arguments(_SAM "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

        swig_add_module(${name}
                ${_SAM_LANGUAGE}
                ${_SAM_SOURCES})
    endmacro()
endif()

############################################################
# Need to enable SWIGWORDSIZE64 in SWIG when long is 64-bit
# This is so stdint.i works properly
############################################################
if(FORCE_X86)
    set(SWIG_WORDSIZE_FLAG ";-DSWIGWORDSIZE32")
elseif(IS_INT64_LONG)
    set(SWIG_WORDSIZE_FLAG ";-DSWIGWORDSIZE64")
endif()
if("${SIZE_T}" EQUAL 8)
    set(SWIG_WORDSIZE_FLAG "${SWIG_WORDSIZE_FLAG};-DHAVE_UINT64_SIZE_T")
endif()

if(CMAKE_VERSION VERSION_LESS 3.8)
    include(${CMAKE_SOURCE_DIR}/cmake/Modules/UseSWIG.cmake)
else()
    include(${SWIG_USE_FILE})
endif()
set(CMAKE_SWIG_FLAGS "")

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/include)
set(SWIG_SRCS_TEMP swig/run.i swig/metrics.i swig/run_metrics.i swig/comm.i swig/table.i swig/plot.i swig/summary.i swig/sample_sheet.i)
set(SWIG_DEPS_TEMP
        swig/util/operator_overload.i
        swig/exceptions/exceptions_impl.i
        swig/exceptions/exceptions_csharp.i
        swig/arrays/arrays_csharp_impl.i
        swig/arrays/arrays_java_impl.i
        swig/arrays/arrays_impl.i
        swig/arrays/arrays_numpy_impl.i)

foreach(SRC ${SWIG_SRCS_TEMP})
    get_filename_component(SRC ${SRC} ABSOLUTE)
    list(APPEND SWIG_SRCS ${SRC})
    #set_source_files_properties(${SRC} PROPERTIES CPLUSPLUS ON)
endforeach()

# Collect all the *.i files
foreach(SRC ${SWIG_DEPS_TEMP})
    get_filename_component(SRC ${SRC} ABSOLUTE)
    list(APPEND SWIG_DEPS ${SRC})
endforeach()

# Collect all the *.h files in interop (these can change, but the library will not change)
foreach(HEADER ${INTEROP_HEADERS})
    get_filename_component(HEADER ${HEADER} ABSOLUTE)
    list(APPEND SWIG_DEPS ${HEADER})
endforeach()

# Language options
# https://github.com/SimpleITK/SimpleITK/blob/master/CMake/sitkLanguageOptions.cmake


# Pass compiler definitions to SWIG
get_directory_property(DirDefs COMPILE_DEFINITIONS)
if(DirDefs)
    string(REPLACE ";" ";-D" DirDefs "${DirDefs}")
    set(DirDefs "-D${DirDefs}")
endif()
set(CMAKE_SWIG_FLAGS "${CMAKE_SWIG_FLAGS};${DirDefs}")

if(ENABLE_CSHARP)
    add_subdirectory("csharp")
endif()
if(ENABLE_PYTHON)
    add_subdirectory("python")
endif()


