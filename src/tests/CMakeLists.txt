

add_custom_target(check)
set_target_properties(check PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)



add_subdirectory("interop")


add_custom_command(TARGET check POST_BUILD COMMAND $<TARGET_FILE:interop_gtests>)
add_dependencies(check interop_gtests)

if(ENABLE_SWIG)
    add_subdirectory("csharp")

    add_custom_command(TARGET check POST_BUILD
            COMMAND ${NUNIT_COMMAND} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSHARP_UNIT_TEST_EXE})
    add_dependencies(check csharp_unittest)
endif()
