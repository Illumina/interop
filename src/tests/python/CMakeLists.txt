find_package(SWIG)
if(NOT SWIG_FOUND)
    return()
endif()

check_swig_version(swig_major_ver)
if(swig_major_ver LESS "3")
    return()
endif()

find_package(PythonInterp)
if(NOT PYTHON_TARGET_FOUND)
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

set(PYTHON_TEST_ENABLED ON PARENT_SCOPE)
add_custom_target(check_python
        COMMENT "Running Python Unit tests"
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${SWIG_PYTHON_MODULE_TARGET}> ${CMAKE_CURRENT_BINARY_DIR}/interop
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/CoreTests.py ${CMAKE_CURRENT_BINARY_DIR}/interop
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/DepTests.py ${CMAKE_CURRENT_BINARY_DIR}/interop
        COMMAND ${PYTHON_EXECUTABLE} interop/CoreTests.py
        COMMAND ${PYTHON_EXECUTABLE} interop/DepTests.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

add_dependencies(check_python python_lib)
set_target_properties(check_python PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
