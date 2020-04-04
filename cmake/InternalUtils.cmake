# This file contains a function, which configures the compiler and linker to values appropriate for this project
#
#

macro(set_msvc_mt)
    foreach (flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "-MD" "-MT" ${flag_var} "${${flag_var}}")
    endforeach()
endmacro()

macro(set_msvc_md)
    foreach (flag_var
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        string(REPLACE "/MT" "-MD" ${flag_var} "${${flag_var}}")
    endforeach()
endmacro()
# Adopted from Google Test
# https://code.google.com/p/googletest/source/browse/trunk/cmake/internal_utils.cmake?r=677
#
# Tweaks CMake's default compiler/linker settings to suit Google Test's needs.
#
# This must be a macro(), as inside a function string() can only
# update variables in the function scope.
macro(fix_default_compiler_settings_)
    if (MSVC)
        if (NOT BUILD_SHARED_LIBS AND NOT FORCE_SHARED_CRT)
            message(STATUS "Replacing MD with MT")
        endif()
        # For MSVC, CMake sets certain flags to defaults we want to override.
        # This replacement code is taken from sample in the CMake Wiki at
        # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
        foreach (flag_var
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if (NOT BUILD_SHARED_LIBS AND NOT FORCE_SHARED_CRT)
                # When Google Test is built as a shared library, it should also use
                # shared runtime libraries.  Otherwise, it may end up with multiple
                # copies of runtime library data in different modules, resulting in
                # hard-to-find crashes. When it is built as a static library, it is
                # preferable to use CRT as static libraries, as we don't have to rely
                # on CRT DLLs being available. CMake always defaults to using shared
                # CRT libraries, so we override that default here.
                string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
            endif()

            # We prefer more strict warning checking for building Google Test.
            # Replaces /W3 with /W4 in defaults.
            string(REPLACE "/W3" "-W4" ${flag_var} "${${flag_var}}")
        endforeach()
    endif()
endmacro()

# Configure the compiler and linker with more appropirate default values
macro(interop_config_compiler_and_linker)
    fix_default_compiler_settings_()

    include (TestBigEndian)
    TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
    if(IS_BIG_ENDIAN)
        message(FATAL_ERROR "This code does not support big endian systems")
    endif()

    include(CheckIsNaN)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(COMPILER_IS_GNUCC_OR_CLANG ON)
    elseif(NOT WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        set(COMPILER_IS_GNUCC_OR_CLANG ON)
    else()
        set(COMPILER_IS_GNUCC_OR_CLANG OFF)
    endif()

    if(CMAKE_SIZEOF_VOID_P EQUAL "4")
        if(MSVC OR MINGW)
            add_definitions(-D_INTEROP_WIN32)
        endif()
    endif()

    include(CheckTypeSize)
    check_type_size("float" SIZEOF_FLOAT)
    if( "${SIZEOF_FLOAT}" STREQUAL "" )
        message(FATAL_ERROR "Type float is not supported on this system and/or compiler: ${CMAKE_SYSTEM}/${CMAKE_CXX_COMPILER_ID}")
    elseif( NOT ${SIZEOF_FLOAT} EQUAL "4" )
        message(FATAL_ERROR "Type float is ${SIZEOF_FLOAT}-byte - This system and/or compiler is not supported: ${CMAKE_SYSTEM}/${CMAKE_CXX_COMPILER_ID}")
    endif()

    include(CheckIncludeFiles)
    check_include_files("stdint.h" HAVE_STDINT_H)
    if(NOT HAVE_STDINT_H)
        check_include_files("sys/types.h" HAVE_SYS_TYPES_H)
    endif()
    include(CheckStdInt)
    include(CheckCXXCompilerFlag)

    check_int64_long(IS_INT64_LONG)
    if(IS_INT64_LONG)
        add_definitions(-DHAVE_LONG64)
    endif()
    check_type_size(size_t SIZE_T)
    if(MINGW)
        set(CXX11_FLAG_ "-std=gnu++11")
    else()
        set(CXX11_FLAG_ "-std=c++11")
    endif()

    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles("#include <unordered_map>
                               int main() {
                                 std::unordered_map<int, int> map;
                                 return 0;
                               }"
            HAVE_UNORDERED_MAP_IN_STD_NAMESPACE)
    if(HAVE_UNORDERED_MAP_IN_STD_NAMESPACE)
        message(STATUS "Found unordered map")
    endif()

    set(ANSI_FLAG "-std=c++98")
    check_cxx_compiler_flag("${ANSI_FLAG}" COMPILER_SUPPORTS_ANSI) #-ansi (does not work on CLang 3.4)
    check_cxx_compiler_flag("${CXX11_FLAG_}" COMPILER_SUPPORTS_CXX11)

    # This breaks GTest with GCC 4.4.x
    #check_cxx_compiler_flag("${CXX0x_FLAG_}" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        set(CXX_CX11_FLAG "${CXX11_FLAG_}")
    #elseif(COMPILER_SUPPORTS_CXX0X)
        #set(CXX_CX11_FLAG "${CXX0x_FLAG_}")
    endif()

    check_cxx_compiler_flag("-Wpedantic" COMPILER_SUPPORTS_PEDANTIC)
    if(COMPILER_SUPPORTS_PEDANTIC)
        set(CXX_PEDANTIC_FLAG "-Wpedantic")
    endif()

    if(FORCE_X86)
        if(MSVC)
            messge(WARNING "FORCE_X86 has not effect with MSVC")
        elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
            set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -m32")
            set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m32")
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
        else()
            message(FATAL_ERROR "Unsupported compiler")
        endif()
    endif()

    set(ENABLE_BIG_OBJ_FLAG "")
    if(MSVC)
        set(ENABLE_BIG_OBJ_FLAG "/bigobj")
    elseif(MINGW)
        set(ENABLE_BIG_OBJ_FLAG "-Wa,-mbig-obj")
    endif()


    set(flags_to_check "-Wno-uninitialized;-Wno-non-template-friend;-Wno-eof-newline;-Wno-maybe-uninitialized;-Wno-strict-aliasing;-fno-strict-aliasing;-Wno-unused-function;-Wno-unused-parameter;-Wno-unnamed-type-template-args;-Wno-c++0x-compat;-Wno-error=c++0x-compat")
    foreach(flag ${flags_to_check})
        string(TOUPPER ${flag} FLAG_NAME)
        string(REPLACE "-" "_" FLAG_NAME ${FLAG_NAME})
        string(REPLACE "=" "_EQ_" FLAG_NAME ${FLAG_NAME})
        string(REPLACE "C++" "CPP" FLAG_NAME ${FLAG_NAME})
        set(${FLAG_NAME} ${flag})
        check_cxx_compiler_flag(${flag} IS_${FLAG_NAME}_SUPPORTED)
        if(NOT IS_${FLAG_NAME}_SUPPORTED)
            set(${FLAG_NAME} "")
        endif()
    endforeach()

    if(COMPILER_IS_GNUCC_OR_CLANG)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror ${_WNO_CPP0X_COMPAT} ${_WNO_ERROR_EQ_CPP0X_COMPAT} ${_WNO_EOF_NEWLINE} ${_WNO_NON_TEMPLATE_FRIEND}")
    elseif(MSVC)
        # Visual Studio Complains about not being able to create an assignment operator and copy constructor
        # -wd4511 and -wd4512 disable these pointless warnings
        # -wd4127: conditional expression is constant
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX -wd4511 -wd4512 -wd4127")
        # TODO: remove all C4127 in headers
        add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        if(WIN32)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror ${_WNO_CPP0X_COMPAT} ${_WNO_ERROR_EQ_CPP0X_COMPAT} ${_WNO_NON_TEMPLATE_FRIEND}")
        endif()
    endif()
    if(NOT ENABLE_BACKWARDS_COMPATIBILITY)
        if(COMPILER_IS_GNUCC_OR_CLANG)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
            message(STATUS "Using C++11: Yes")
        elseif(MSVC)
            message(STATUS "Using C++11: Yes")
        else()
            message(STATUS "Using C++11: Compiler default")
        endif()
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
        message(STATUS "Using C++11: Yes")
    elseif(MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
        message(STATUS "Using C++11: Yes")
    elseif(COMPILER_SUPPORTS_ANSI)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ANSI_FLAG}")
        message(STATUS "Using C++11: No")
    endif()

    get_property(cxx_features GLOBAL PROPERTY CMAKE_CXX_KNOWN_FEATURES)
    list(FIND cxx_features "cxx_rvalue_references" HAS_CXX_RVALUE_REFERENCES)
    if(HAS_CXX_RVALUE_REFERENCES GREATER -1 AND NOT ENABLE_BACKWARDS_COMPATIBILITY)
        add_definitions(-DHAS_CXX_RVALUE_REFERENCES=1)
    endif()
    if(ENABLE_FLOAT_COMPRESSION)
        add_definitions(-DUSED_COMPRESSED_FLOAT=1)
    endif()
    if(NOT DISABLE_DYNAMIC_ARRAY)
        add_definitions(-DUSE_DYNAMIC_ARRAY=1)
    endif()
    if(ENABLE_LEGACY_METRICS)
        add_definitions(-DUSE_LEGACY_METRICS=1)
    endif()
endmacro()

