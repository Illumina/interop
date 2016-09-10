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

    include(CheckIsNaN)

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
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
    if(HAVE_STDINT_H)
        add_definitions(-DHAVE_STDINT_H)
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

    check_cxx_compiler_flag("-ansi" COMPILER_SUPPORTS_ANSI)
    check_cxx_compiler_flag("${CXX11_FLAG_}" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX11)
        set(CXX_CX11_FLAG "${CXX11_FLAG_}")
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
    if(COMPILER_IS_GNUCC_OR_CLANG)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-c++0x-compat -Wno-error=c++0x-compat -Wextra")
        if(WIN32 AND MINGW)
            # Add big object support to Windows Compilers
            SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj")
        endif()
    elseif(MSVC)
        # Visual Studio Complains about not being able to create an assignment operator and copy constructor
        # -wd4511 and -wd4512 disable these pointless warnings
        # Add big object support to Windows Compilers
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX -wd4511 -wd4512 /bigobj")
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
        if(WIN32)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-c++0x-compat -Wno-error=c++0x-compat -Wextra")
        endif()
    endif()
    if(NOT ENABLE_BACKWARDS_COMPATIBILITY)
        if(COMPILER_IS_GNUCC_OR_CLANG)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
        endif()
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-local-typedef -Wno-unnamed-type-template-args")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
    elseif(MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_CX11_FLAG}")
        message(WARNING "ENABLE_BACKWARDS_COMPATIBILITY=ON does not work with MinGW, ignoring")
    elseif(COMPILER_SUPPORTS_ANSI)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi")
    endif()
endmacro()

