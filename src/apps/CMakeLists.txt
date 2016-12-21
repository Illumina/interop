
function(add_application _target _source_files)

    if(MSVC)
        set(SWIG_VERSION_INFO "${CMAKE_CURRENT_BINARY_DIR}/${_target}_version.rc")
        set(LIB_NAME "${_target}") # Used to configure the version info file
        configure_file(${CMAKE_SOURCE_DIR}/cmake/version.rc.in ${SWIG_VERSION_INFO} @ONLY) # Requires: LIB_NAME, VERSION_LIST and VERSION
    endif()

    add_executable(${_target} ${_source_files} inc/application.h inc/plot_options.h ${SWIG_VERSION_INFO})
    target_link_libraries(${_target} ${INTEROP_LIB})

    if(COMPILER_IS_GNUCC_OR_CLANG)
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

set(SWIG_VERSION_INFO "")

add_application(interop2csv interop2csv.cpp)
add_application(dumptext dumptext.cpp)
add_application(cyclesim cyclesim.cpp)
add_application(summary summary.cpp)
add_application(index-summary index_summary.cpp)
add_application(dumpbin dumpbin.cpp)
add_application(plot_qscore_histogram plot_qscore_histogram.cpp)
add_application(plot_by_cycle plot_by_cycle.cpp)
add_application(plot_by_lane plot_by_lane.cpp)
add_application(plot_flowcell plot_flowcell.cpp)
add_application(plot_qscore_heatmap plot_qscore_heatmap.cpp)
add_application(plot_sample_qc plot_sample_qc.cpp)
add_application(imaging_table imaging_table.cpp)
