
function(add_application _target _source_files)
    add_executable(${_target} ${_source_files})
    target_link_libraries(${_target} ${INTEROP_LIB})

    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC)
        set_target_properties(${_target} PROPERTIES COMPILE_FLAGS "${CXX_PEDANTIC_FLAG}" )
    endif()

    if(NOT ENABLE_APPS)
        set_target_properties(${_target} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
    else()
        install(TARGETS ${_target}
                RUNTIME DESTINATION bin
                )
    endif()

    if(NOT ENABLE_STATIC)
        add_custom_command(TARGET ${_target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
    endif()
endfunction()

add_application(interop2csv interop2csv.cpp)
add_application(cyclesim cyclesim.cpp)
