

add_custom_target(check)
add_custom_target(performance)
set_target_properties(check PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)



add_subdirectory("interop")


add_custom_command(TARGET check POST_BUILD COMMAND $<TARGET_FILE:interop_gtests>)
add_dependencies(check interop_gtests)

if(ENABLE_SWIG)
    add_subdirectory("csharp")
    if(CSHARP_TEST_ENABLED)
        add_custom_command(TARGET check POST_BUILD
                COMMAND ${NUNIT_COMMAND_EXE} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSHARP_UNIT_TEST_EXE})
        add_dependencies(check csharp_unittest)

        add_custom_command(TARGET performance POST_BUILD
                COMMAND ${NUNIT_COMMAND_EXE} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSHARP_PERF_TEST_EXE})
        add_dependencies(check csharp_perftest)
    endif()
endif()
