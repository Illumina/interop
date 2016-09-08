
add_subdirectory("interop")

if(ENABLE_SWIG)
    add_subdirectory("ext")
endif()

add_subdirectory("apps")
add_subdirectory("examples")

if(ENABLE_TEST)
    add_subdirectory("tests")
endif()
