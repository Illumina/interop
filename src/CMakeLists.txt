

add_subdirectory("interop")

if(ENABLE_SWIG)
    add_subdirectory("ext")
endif()

add_subdirectory("apps")
add_subdirectory("examples")
add_subdirectory("tests")
