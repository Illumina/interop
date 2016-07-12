function(add_example _target _source_files)
    add_executable(${_target} ${_source_files})
    target_link_libraries(${_target} ${INTEROP_LIB})

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC)
        set_target_properties(${_target} PROPERTIES COMPILE_FLAGS "${CXX_PEDANTIC_FLAG}" )
    elseif(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS  -D_SCL_SECURE_NO_WARNINGS)
    endif()

    if(NOT ENABLE_EXAMPLES)
        set_target_properties(${_target} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
    else()
        install(TARGETS ${_target}
                RUNTIME DESTINATION share/illumina/interop/examples
                )
    endif()

    if(NOT ENABLE_STATIC)
        add_custom_command(TARGET ${_target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
    endif()
endfunction()

add_example(example1 example1.cpp)
add_example(example2 example2.cpp)
add_example(example3 example3.cpp)
add_example(example4 example4.cpp)
add_example(example_q_metric example_q_metric.cpp)

if(ENABLE_SWIG)
    add_subdirectory("csharp")
endif()
