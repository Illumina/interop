# This file contains a function, which configures the compiler and linker to values appropriate for this project
#
#


# Adopted from Google Test
# https://code.google.com/p/googletest/source/browse/trunk/cmake/internal_utils.cmake?r=677
#
# Tweaks CMake's default compiler/linker settings to suit Google Test's needs.
#
# This must be a macro(), as inside a function string() can only
# update variables in the function scope.
macro(fix_default_compiler_settings_)
    if (MSVC)
        # For MSVC, CMake sets certain flags to defaults we want to override.
        # This replacement code is taken from sample in the CMake Wiki at
        # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
        foreach (flag_var
                CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
                CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
            if (NOT BUILD_SHARED_LIBS AND NOT gtest_force_shared_crt)
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
macro(config_compiler_and_linker)
    fix_default_compiler_settings_()

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCC)
        set(COMPILER_IS_GNUCC_OR_CLANG ON)
    endif()

    include(CheckIncludeFiles)
    check_include_files("stdint.h" HAVE_STDINT_H)
    if(HAVE_STDINT_H)
        add_definitions(-DHAVE_STDINT_H)
    endif()
    include(CheckStdInt)
    include(CheckCXXCompilerFlag)

    check_int64_long(IS_INT64_LONG)
    if(IS_INT64_LONG)
        add_definitions(-DHAVE_LONG64)
    endif()

    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX11)
        set(CXX_CX11_FLAG "-std=c++11")
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
            set(CMAKE_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} -m32)
            set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -m32)
        else()
            message(FATAL_ERROR "Unsupported compiler")
        endif()
    endif()
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-c++0x-compat -Wno-error=c++0x-compat -Wextra")
    elseif(MSVC)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        if(WIN32)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-c++0x-compat -Wno-error=c++0x-compat -Wextra")
        endif()
    endif()
    if(NOT ENABLE_BACKWARDS_COMPATIBILITY)
        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
        endif()
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedef -Wno-unnamed-type-template-args")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
    elseif(MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
        message(WARNING "ENABLE_BACKWARDS_COMPATIBILITY=ON does not work with MinGW, ignoring")
    endif()
endmacro()

