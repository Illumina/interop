
find_package(PythonInterp)
if(NOT ENABLE_PYTHON_DYNAMIC_LOAD OR WIN32)
    find_package(PythonLibs EXACT ${PYTHON_VERSION_STRING})
endif()
if(NOT PYTHONLIBS_FOUND)
    execute_process(
            COMMAND ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print(sysconfig.get_python_inc())"
            OUTPUT_VARIABLE
            PYTHON_INCLUDE_PATH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()
message(STATUS "Found PythonIncludePath: ${PYTHON_INCLUDE_PATH}")
find_package(NumPy)
if(NOT NUMPY_FOUND)
    set(PYTHON_TARGET_FOUND OFF CACHE INTERNAL "Python target is available" FORCE)
    message(WARNING "Numpy not found, unable to generate Python Binding, interpreter ${PYTHON_EXECUTABLE}")
    return()
endif()

# Workaround for fun little MinGw/Python "feature" for 64-bit systems
if(MSVC)
    set(PYTHON_GENERATED_COMPILE_FLAGS "/wd4702 /wd4286 /wd4127 /wd4459 /wd4456 /wd4701 /wd4267 /wd4244 /wd4706 /wd4703 ${ENABLE_BIG_OBJ_FLAG}")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(PYTHON_GENERATED_COMPILE_FLAGS "${_WNO_UNUSED_FUNCTION} ${_WNO_UNUSED_PARAMETER}")
elseif(COMPILER_IS_GNUCC_OR_CLANG)
    set(PYTHON_GENERATED_COMPILE_FLAGS "${_WNO_UNINITIALIZED} ${_WNO_UNUSED_FUNCTION} ${_WNO_UNUSED_PARAMETER} ${_WNO_MAYBE_UNINITIALIZED} ${ENABLE_BIG_OBJ_FLAG} ${_WNO_STRICT_ALIASING} ${_FNO_STRICT_ALIASING}")
    if(WIN32 AND MINGW AND "${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
        set(PYTHON_GENERATED_COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS} -DMS_WIN64")
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
    #-flat_namespace -undefined suppress
    set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
    set(CMAKE_SHARED_LIBRARY_CREATE_LINKER_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -undefined dynamic_lookup")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -undefined dynamic_lookup")
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
    add_swig_library(${MODULE} python ${SRC} ${SWIG_VERSION_INFO})
    set(SWIG_MODULE ${SWIG_MODULE_${MODULE}_REAL_NAME})

    if(PYTHON_GENERATED_COMPILE_FLAGS)
        set_target_properties(${SWIG_MODULE} PROPERTIES COMPILE_FLAGS "${PYTHON_GENERATED_COMPILE_FLAGS}")
    endif()

    if(PYTHON_GENERATED_LINK_FLAGS)
        set_target_properties(${SWIG_MODULE} PROPERTIES LINK_FLAGS "${PYTHON_GENERATED_LINK_FLAGS}")
    endif()

    if(PYTHONLIBS_FOUND)
        swig_link_libraries(${MODULE} ${PYTHON_LIBRARIES} ${INTEROP_DL_LIB})
    else()
        swig_link_libraries(${MODULE} ${INTEROP_DL_LIB})
    endif()
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

#

    add_custom_command(
            TARGET ${SWIG_MODULE}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${swig_py_file} $<TARGET_FILE_DIR:${INTEROP_LIB}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${SWIG_MODULE}> $<TARGET_FILE_DIR:${INTEROP_LIB}>
    )
    add_dependencies(python_lib ${SWIG_MODULE})
    install(TARGETS ${SWIG_MODULE} LIBRARY DESTINATION lib64/python)
    install(FILES ${swig_py_file} DESTINATION lib64/python)
endforeach()

string(REPLACE ";" "," PY_MOD_NAME_LIST "${py_mod_names}")
string(REPLACE ";" "','" PY_GEN_LIST "${py_gens}")

set(INTEROP_VERSION ${VERSION_SHORT}${VERSION_DEV})
configure_file(__init__.py.in interop/__init__.py @ONLY)
configure_file(__main__.py.in interop/__main__.py @ONLY)
file(COPY ${CMAKE_SOURCE_DIR}/src/tests/python/CoreTests.py DESTINATION interop)

add_custom_command( TARGET python_lib POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -DINTEROP_VERSION=${VERSION_SHORT}${VERSION_DEV}
        -DINTEROP_LIB_LIST="${PY_GEN_LIST}"
        -DCONFIG_INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in
        -DCONFIG_OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/setup.py
        -P ${CMAKE_SOURCE_DIR}/cmake/ConfigureFile.cmake
)

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DESTINATION share/illumina/interop/src
        FILES_MATCHING
        PATTERN "*.cxx"
        PATTERN "*.py"
        PATTERN "CMakeFiles" EXCLUDE
        PATTERN "${CMAKE_BUILD_TYPE}" EXCLUDE)

set(SWIG_PYTHON_MODULE_TARGET ${SWIG_MODULE} CACHE INTERNAL "Python generator generated by SWIG" FORCE)
set(SWIG_PYTHON_GENERATORS ${py_gens} CACHE INTERNAL "Python generators generated by SWIG" FORCE)
set(SWIG_GEN_PYTHON_LIBS ${py_mods} CACHE INTERNAL "Python libraries generated by SWIG" FORCE)
set(SWIG_GEN_PYTHON_SOURCE_FILES ${py_files} CACHE INTERNAL "Python scripts generated by SWIG" FORCE)


if(NOT PYTHONINTERP_FOUND)
    message(WARNING "Cannot find Python Interpretor, cannot create wheel package")
    return()
endif()

add_custom_target(package_wheel
        COMMENT "Creating Python wheel package"
        COMMAND ${PYTHON_EXECUTABLE} setup.py bdist_wheel -d ${PACKAGE_OUTPUT_FILE_PREFIX}
        )
add_dependencies(package_wheel python_lib)
set_target_properties(package_wheel PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

set(PYTHON_TARGET_FOUND ON CACHE INTERNAL "Python target is available" FORCE)
