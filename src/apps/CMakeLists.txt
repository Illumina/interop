

add_executable(interop2csv interop2csv.cpp)
target_link_libraries(interop2csv ${INTEROP_LIB} ${Boost_LIBRARIES})



if(NOT ENABLE_APPS)
    set_target_properties(interop2csv PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()

if(NOT ENABLE_STATIC)
    add_custom_command(TARGET interop2csv POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()