find_package(SWIG 4.0)
if(NOT SWIG_FOUND)
    return()
endif()

check_swig_version(swig_major_ver)
if(swig_major_ver LESS "4")
    return()
endif()

find_package (Python COMPONENTS Interpreter Development.Module NumPy)

if(NOT Python_Interpreter_FOUND)
    message(WARNING "Cannot find Python Interpreter, unit tests will not be run")
    return()
endif()

if(NOT Python_Development.Module_FOUND)
    message(WARNING "Cannot find Python Development, unit tests will not be run")
    return()
endif()

if(NOT Python_NumPy_FOUND)
    message(WARNING "Cannot find Python NumPy, unit tests will not be run")
    return()
endif()

set(PYTHON_TEST_ENABLED ON PARENT_SCOPE)

add_custom_target(check_python
        COMMENT "Running Python Unit tests"
        COMMAND ${CMAKE_COMMAND} -E copy_directory $<TARGET_FILE_DIR:${SWIG_PYTHON_MODULE_TARGET}> ${CMAKE_CURRENT_BINARY_DIR}/interop
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/DepTests.py ${CMAKE_CURRENT_BINARY_DIR}/interop
        COMMAND ${Python_EXECUTABLE} -m interop --test
        COMMAND ${Python_EXECUTABLE} interop/DepTests.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )

add_dependencies(check_python python_lib)
set_target_properties(check_python PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
