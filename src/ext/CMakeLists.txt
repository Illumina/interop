set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)
find_package(SWIG)
if(NOT SWIG_FOUND)
    message(WARNING "Swig not found, cannot generate intefaces")
    return()
endif()

############################################################
# Need to enable SWIGWORDSIZE64 in SWIG when long is 64-bit
# This is so stdint.i works properly
############################################################
include(CheckStdInt)
check_int64_long(IS_INT64_LONG)
if(IS_INT64_LONG)
    set(SWIG_WORDSIZE_FLAG ";-DSWIGWORDSIZE64")
else()
    set(SWIG_WORDSIZE_FLAG "")
endif()
include(${SWIG_USE_FILE})
set(CMAKE_SWIG_FLAGS "")

set(SWIG_SRCS_TEMP swig/interop.i)
set(SWIG_DEPS_TEMP
        swig/exceptions/exceptions_impl.i
        swig/exceptions/exceptions_csharp.i
        swig/arrays/arrays_csharp_impl.i
        swig/arrays/arrays_impl.i
        swig/arrays/arrays_numpy_impl.i
        swig/extends/extends_csharp.i
        swig/extends/extends_impl.i)

foreach(SRC ${SWIG_SRCS_TEMP})
    get_filename_component(SRC ${SRC} ABSOLUTE)
    list(APPEND SWIG_SRCS ${SRC})
    set_source_files_properties(${SRC} PROPERTIES CPLUSPLUS ON)
endforeach()
foreach(SRC ${SWIG_DEPS_TEMP})
    get_filename_component(SRC ${SRC} ABSOLUTE)
    list(APPEND SWIG_DEPS ${SRC})
endforeach()
# Language options
# https://github.com/SimpleITK/SimpleITK/blob/master/CMake/sitkLanguageOptions.cmake
add_subdirectory("csharp")

