find_package(SWIG)
if(NOT SWIG_FOUND)
    return()
endif()

check_swig_version(swig_major_ver)
if(swig_major_ver LESS "3")
    return()
endif()

find_package(PythonInterp)
find_package(PythonLibs EXACT ${PYTHON_VERSION_STRING})
if(NOT PYTHONLIBS_FOUND)
    return()
endif()

find_package(NumPy)
if(NOT NUMPY_FOUND)
    return()
endif()

if(NOT PYTHONINTERP_FOUND)
    message(WARNING "Cannot find Python Interpretor, unit tests will not be run")
    return()
endif()

set(PYTHON_UNIT_TEST_SUITE CoreTests.py)

set(PYTHON_TEST_ENABLED ON PARENT_SCOPE)
add_custom_target(check_python
        COMMENT "Running Python Unit tests"
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${SWIG_PYTHON_MODULE_TARGET}> ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/CoreTests.py ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${PYTHON_EXECUTABLE} ${PYTHON_UNIT_TEST_SUITE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

add_dependencies(check_python python_lib)
set_target_properties(check_python PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
