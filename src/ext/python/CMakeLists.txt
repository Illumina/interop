add_library(Python::ModuleInterface INTERFACE IMPORTED)
message(STATUS "Python_ROOT_DIR=${Python_ROOT_DIR}")
message(STATUS "Python_EXECUTABLE=${Python_EXECUTABLE}")
if(NOT ENABLE_PYTHON_DYNAMIC_LOAD OR WIN32)
    find_package (Python COMPONENTS Interpreter Development.Module NumPy)
    target_link_libraries(Python::ModuleInterface INTERFACE Python::Module)
else()
    if(CMAKE_VERSION VERSION_LESS 3.19.2)
        find_package (Python COMPONENTS Interpreter NumPy)
    else()
        # Development required for CMake 3.19.2 or higher
        find_package (Python COMPONENTS Interpreter Development.Module NumPy)
    endif()
    target_include_directories(Python::ModuleInterface SYSTEM INTERFACE
            ${Python_INCLUDE_DIRS})
    target_compile_options(Python::ModuleInterface INTERFACE
            -Wno-deprecated-declarations)
    target_link_options(Python::ModuleInterface INTERFACE
            $<$<CXX_COMPILER_ID:AppleClang>:-undefined dynamic_lookup> -DUNUSED
            )
endif()

if(NOT Python_FOUND)
    if(NOT ENABLE_PYTHON_DYNAMIC_LOAD OR WIN32)
        if(NOT Python_Development.Module_FOUND)
            message(WARNING "No Python development libs or headers found")
        else()
            message(STATUS "Found Python headers at ${Python_INCLUDE_DIRS}")
            message(STATUS "Found Python libs at ${Python_LIBRARIES}")
            message(STATUS "Found Python linker options at ${Python_LINK_OPTIONS}")
        endif()
    else()
        message(STATUS "Found Python headers at ${Python_INCLUDE_DIRS}")
        message(STATUS "Found Python libs at ${Python_LIBRARIES}")
        message(STATUS "Found Python linker options at ${Python_LINK_OPTIONS}")
    endif()
    if(NOT Python_Interpreter_FOUND)
        message(WARNING "No Python 3.x interpreter at ${Python_EXECUTABLE}")
    else()
        message(STATUS "Found Python interpreter at ${Python_EXECUTABLE}")
    endif()
    if(NOT Python_NumPy_FOUND)
        execute_process(
                COMMAND ${Python_EXECUTABLE} -c "import numpy; print(numpy.get_include())"
                OUTPUT_VARIABLE
                NUMPY_INCLUDE_PATH
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(NOT Python_NumPy_INCLUDE_DIRS)
            message(WARNING "Numpy include directory missing: (2nd try: ${NUMPY_INCLUDE_PATH})")
        elseif(NOT Python_NumPy_VERSION)
            message(WARNING "Numpy version could not be determined")
        elseif(NOT Python_Development.Module_FOUND)
            message(WARNING "No Python development libs or headers found")
        else()
            message(WARNING "NumPy not found - reason unknown")
        endif()
    else()
        message(STATUS "Found Python numpy headers at ${Python_NumPy_INCLUDE_DIRS}")
        message(STATUS "Found Python numpy version ${Python_NumPy_VERSION}")
    endif()
    if(REQUIRE_PYTHON)
        message(FATAL_ERROR "Insufficient Python requirements found")
    else()
        message(WARNING "Insufficient Python requirements found")
    endif()
    return()
else()
    message(STATUS "Found Python interpreter at ${Python_EXECUTABLE}")
    message(STATUS "Found Python headers at ${Python_INCLUDE_DIRS}")
    message(STATUS "Found Python libs at ${Python_LIBRARIES}")
    message(STATUS "Found Python link options at ${Python_LINK_OPTIONS}")
    message(STATUS "Found Python numpy headers at ${Python_NumPy_INCLUDE_DIRS}")
    message(STATUS "Found Python numpy version ${Python_NumPy_VERSION}")
endif()

# Workaround for fun little MinGw/Python "feature" for 64-bit systems
if(MSVC)
    set(PYTHON_GENERATED_COMPILE_FLAGS "/wd4702 /wd4286 /wd4127 /wd4459 /wd4456 /wd4701 /wd4267 /wd4244 /wd4706 /wd4703 /wd4100 ${ENABLE_BIG_OBJ_FLAG}")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(PYTHON_GENERATED_COMPILE_FLAGS "${_WNO_UNUSED_FUNCTION} ${_WNO_UNUSED_PARAMETER}")

    # TODO: Need to upgrade SWIG FOR Python 3.8 and later and remove this warning suppression
    include(CheckCXXSourceCompiles)
    check_cxx_compiler_flag("-Wno-deprecated-declarations" IS_SUPPORTED_FLAG)
    message(STATUS "Deprecated declarations disable warning: ${IS_SUPPORTED_FLAG}")
    if(IS_SUPPORTED_FLAG)
        message(STATUS "Add flag -Wno-deprecated-declarations")
        set(PYTHON_GENERATED_COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS} -Wno-deprecated-declarations")
    endif()
    check_cxx_compiler_flag("-Wno-deprecated-register" IS_SUPPORTED_FLAG)
    message(STATUS "Deprecated register disable warning: ${IS_SUPPORTED_FLAG}")
    if(IS_SUPPORTED_FLAG)
        message(STATUS "Add flag -Wno-deprecated-register")
        set(PYTHON_GENERATED_COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS} -Wno-deprecated-register")
    endif()
elseif(COMPILER_IS_GNUCC_OR_CLANG)
    set(PYTHON_GENERATED_COMPILE_FLAGS "${_WNO_UNINITIALIZED} ${_WNO_UNUSED_FUNCTION} ${_WNO_UNUSED_PARAMETER} ${_WNO_MAYBE_UNINITIALIZED} ${ENABLE_BIG_OBJ_FLAG} ${_WNO_STRICT_ALIASING} ${_FNO_STRICT_ALIASING}")
    if(WIN32 AND MINGW AND "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(PYTHON_GENERATED_COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS} -DMS_WIN64")
    endif()
    if(WIN32 AND MINGW)
        set(PYTHON_GENERATED_COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS} -D_hypot=hypot")
    endif()


    # TODO: Need to upgrade SWIG FOR Python 3.8 and later and remove this warning suppression
    include(CheckCXXSourceCompiles)
    check_cxx_compiler_flag("-Wno-deprecated-declarations" IS_SUPPORTED_FLAG)
    message(STATUS "Deprecated declarations disable warning: ${IS_SUPPORTED_FLAG}")
    if(IS_SUPPORTED_FLAG)
        message(STATUS "Add flag -Wno-deprecated-declarations")
        set(PYTHON_GENERATED_COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS} -Wno-deprecated-declarations")
    endif()
endif()

if(MSVC)
    string(REGEX MATCH "python[0-9]?[0-9]?_d.lib" HAS_DEBUG_PYTHON "${PYTHON_LIBRARIES}")
    if(NOT HAS_DEBUG_PYTHON)
        message(STATUS "Debug Python library for MSVC: Found")
        set(PYTHON_GENERATED_COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS} -DSWIG_PYTHON_INTERPRETER_NO_DEBUG")
    else()
        message(STATUS "Debug Python library for MSVC: Not Found")
    endif()
endif()

if(CMAKE_COMPILER_IS_GNUCC AND ENABLE_PORTABLE)
    set (CMAKE_SHARED_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(PYTHON_GENERATED_LINK_FLAGS "-undefined dynamic_lookup")
endif()

add_custom_target(python_lib)
set_target_properties(python_lib PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

include_directories(${PYTHON_INCLUDE_PATH} ${NUMPY_INCLUDE_DIRS})
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/interop)
set(SWIG_VERSION_INFO "")


foreach(SRC ${SWIG_SRCS})
    get_filename_component(MODULE ${SRC} NAME_WE)
    set(MODULE "py_interop_${MODULE}")
    # Has a memory leak prior to swig 3.1 in https://github.com/swig/swig/blob/87f14a96b5eb4b7c2e23b212ef38e20b9601d0b2/Lib/python/std_pair.i (Line: 68)
    #set_source_files_properties(${SRC} PROPERTIES SWIG_FLAGS "-modern;-fastunpack;-module;${MODULE}${SWIG_WORDSIZE_FLAG}")
    # Coverity reports a null pointer deference, but this is a false positive
    set_source_files_properties(${SRC} PROPERTIES SWIG_FLAGS "-module;${MODULE}${SWIG_WORDSIZE_FLAG}")
    set_source_files_properties(${SRC} PROPERTIES CPLUSPLUS ON)
    set_property(SOURCE ${SRC} PROPERTY SWIG_MODULE_NAME ${MODULE})
    if(MSVC)
        set(SWIG_VERSION_INFO ${CMAKE_CURRENT_BINARY_DIR}/${MODULE}_version.rc)
        set(LIB_NAME "${MODULE}") # Used to configure the version info file
        configure_file(${CMAKE_SOURCE_DIR}/cmake/version.rc.in ${SWIG_VERSION_INFO} @ONLY) # Requires: LIB_NAME, VERSION_LIST and VERSION
    endif()
    set(SWIG_MODULE_${MODULE}_EXTRA_DEPS ${SWIG_DEPS} ${SWIG_VERSION_INFO})
    swig_add_library(${MODULE} LANGUAGE python SOURCES ${SRC} ${SWIG_VERSION_INFO})
    set(SWIG_MODULE ${SWIG_MODULE_${MODULE}_REAL_NAME})

    if(PYTHON_GENERATED_COMPILE_FLAGS)
        set_target_properties(${SWIG_MODULE} PROPERTIES COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS}")
    endif()

    if(PYTHON_GENERATED_LINK_FLAGS)
        set_target_properties(${SWIG_MODULE} PROPERTIES LINK_FLAGS "${PYTHON_GENERATED_LINK_FLAGS}")
    endif()

    swig_link_libraries(${MODULE} ${INTEROP_DL_LIB}
            Python::ModuleInterface
            Python::NumPy)
    add_dependencies(${SWIG_MODULE} ${INTEROP_DL_LIB})
    set(swig_py_file "${CMAKE_SWIG_OUTDIR}/${MODULE}.py")
    list(APPEND py_gens "$<TARGET_FILE:${SWIG_MODULE}>")
    list(APPEND py_mods ${SWIG_MODULE})
    list(APPEND py_files ${swig_py_file})
    list(APPEND py_mod_names ${MODULE})
    #set_target_properties(${SWIG_MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set_target_properties(${SWIG_MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SWIG_OUTDIR})
    if(MSVC)
        set_target_properties(${SWIG_MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SWIG_OUTDIR}")
        set_target_properties(${SWIG_MODULE} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SWIG_OUTDIR}")
    endif()

    add_custom_command(
            TARGET ${SWIG_MODULE}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${swig_py_file} $<TARGET_FILE_DIR:${INTEROP_LIB}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${SWIG_MODULE}> $<TARGET_FILE_DIR:${INTEROP_LIB}>
    )
    add_dependencies(python_lib ${SWIG_MODULE})
    install(TARGETS ${SWIG_MODULE} LIBRARY DESTINATION lib64/python)
    install(FILES ${swig_py_file} DESTINATION lib64/python)

    message(STATUS "Python swig module: ${SWIG_MODULE}")
endforeach()

string(REPLACE ";" "," PY_MOD_NAME_LIST "${py_mod_names}")
string(REPLACE ";" "','" PY_GEN_LIST "${py_gens}")

set(INTEROP_VERSION ${VERSION_SHORT}${VERSION_DEV})
configure_file(__init__.py.in interop/__init__.py @ONLY)
configure_file(__main__.py.in interop/__main__.py @ONLY)


add_custom_command( TARGET python_lib POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -DINTEROP_VERSION=${VERSION_SHORT}${VERSION_DEV}
        -DINTEROP_LIB_LIST="${PY_GEN_LIST}"
        -DCONFIG_INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
        -DCONFIG_OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/setup.py
        -P ${CMAKE_SOURCE_DIR}/cmake/ConfigureFile.cmake
        COMMAND ${CMAKE_COMMAND}
        -DINTEROP_VERSION=${VERSION_SHORT}${VERSION_DEV}
        -DINTEROP_LIB_LIST="${PY_GEN_LIST}"
        -DCONFIG_INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/__main__.py.in
        -DCONFIG_OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/__main__.py
        -P ${CMAKE_SOURCE_DIR}/cmake/ConfigureFile.cmake
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/core.py
        ${CMAKE_CURRENT_BINARY_DIR}/interop/core.py
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/tests/python/CoreTests.py
        ${CMAKE_CURRENT_BINARY_DIR}/interop/CoreTests.py
)


install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DESTINATION share/illumina/interop/src
        FILES_MATCHING
        PATTERN "*.cxx"
        PATTERN "*.py"
        PATTERN "CMakeFiles" EXCLUDE
        PATTERN "${CMAKE_BUILD_TYPE}" EXCLUDE)

set(SWIG_PYTHON_MODULE_BINARY_DIR ${CMAKE_SWIG_OUTDIR} CACHE INTERNAL "Python generator generated by SWIG" FORCE)
set(SWIG_PYTHON_MODULE_TARGET ${SWIG_MODULE} CACHE INTERNAL "Python generator generated by SWIG" FORCE)
set(SWIG_PYTHON_GENERATORS ${py_gens} CACHE INTERNAL "Python generators generated by SWIG" FORCE)
set(SWIG_GEN_PYTHON_LIBS ${py_mods} CACHE INTERNAL "Python libraries generated by SWIG" FORCE)
set(SWIG_GEN_PYTHON_SOURCE_FILES ${py_files} CACHE INTERNAL "Python scripts generated by SWIG" FORCE)


if(NOT Python_FOUND)
    message(WARNING "Cannot find Python Interpreter, cannot create wheel package")
    return()
endif()

if(NOT PYTHON_WHEEL_PREFIX)
    set(PYTHON_WHEEL_PREFIX "${PACKAGE_OUTPUT_FILE_PREFIX}")
else()
    get_filename_component(PYTHON_WHEEL_PREFIX ${PYTHON_WHEEL_PREFIX} ABSOLUTE)
endif()
add_custom_target(package_wheel
        COMMENT "Creating Python wheel package in ${PYTHON_WHEEL_PREFIX}"
        COMMAND ${Python_EXECUTABLE} setup.py bdist_wheel -d ${PYTHON_WHEEL_PREFIX}
        )
add_dependencies(package_wheel python_lib)
set_target_properties(package_wheel PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
if(NOT SKIP_PACKAGE_ALL_WHEEL)
    add_dependencies(bundle package_wheel)
endif()


set(PYTHON_BUILD_AVAILABLE "Python ${PYTHON_VERSION_STRING}" CACHE INTERNAL "All dependencies are satisfied for the Java Build" FORCE)
