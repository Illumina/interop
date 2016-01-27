

add_executable(example1 example1.cpp)
target_link_libraries(example1 ${INTEROP_LIB})

add_executable(example2 example2.cpp)
target_link_libraries(example2 ${INTEROP_LIB})

add_executable(example3 example3.cpp)
target_link_libraries(example3 ${INTEROP_LIB})

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC)
    set_target_properties(example1 PROPERTIES COMPILE_FLAGS "-Wpedantic" )
    set_target_properties(example2 PROPERTIES COMPILE_FLAGS "-Wpedantic" )
    set_target_properties(example3 PROPERTIES COMPILE_FLAGS "-Wpedantic" )
elseif(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(NOT ENABLE_EXAMPLES)
    set_target_properties(example1 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
    set_target_properties(example2 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
    set_target_properties(example3 PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
else()
    install(TARGETS example1 example2 example3
            RUNTIME DESTINATION share/illumina/interop/examples
            )
endif()

if(NOT ENABLE_STATIC)
    add_custom_command(TARGET example1 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(TARGET example2 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(TARGET example3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()
if(ENABLE_SWIG)
    add_subdirectory("csharp")
endif()
