find_package(GTest)
find_package(GMock)
include(${GTEST_USE_FILE})
include(${GMOCK_USE_FILE})
find_package(Threads)

set(SRCS
        run/info_test.cpp
        run/parameters_test.cpp
        metrics/corrected_intensity_metrics_test.cpp
        metrics/error_metrics_test.cpp
        metrics/extraction_metrics_test.cpp
        metrics/index_metrics_test.cpp
        metrics/image_metrics_test.cpp
        metrics/q_metrics_test.cpp
        metrics/tile_metrics_test.cpp
        metrics/q_by_lane_metric_test.cpp
        metrics/q_collapsed_metrics_test.cpp
        logic/summary_metrics_test.cpp
        metrics/base_metric_tests.cpp
        logic/enum_parsing_test.cpp
        logic/metric_type_ext_test.cpp
        logic/plot_logic_test.cpp
        util/option_parser_test.cpp
        logic/imaging_table_logic_test.cpp
        logic/imaging_table_regression_test.cpp
        metrics/metric_streams_test.cpp
        util/stat_test.cpp
        unit_tests.cpp
        metrics/run_metric_test.cpp)

set(HEADERS
        metrics/inc/corrected_intensity_metrics_test.h
        metrics/inc/error_metrics_test.h
        metrics/inc/metric_test.h
        metrics/inc/extraction_metrics_test.h
        metrics/inc/image_metrics_test.h
        metrics/inc/index_metrics_test.h
        metrics/inc/q_metrics_test.h
        metrics/inc/tile_metrics_test.h
        metrics/inc/q_collapsed_metrics_test.h
        inc/failure_listener.h
        inc/persistent_parameter_generator.h
        inc/generic_fixture.h
        metrics/inc/metric_generator.h
        inc/regression_test_data.h
        inc/proxy_parameter_generator.h
        inc/abstract_regression_test_generator.h
        metrics/inc/metric_format_fixtures.h)


add_executable(interop_gtests ${SRCS} ${HEADERS})

target_link_libraries(interop_gtests ${INTEROP_LIB}  ${GTEST_LIBRARY} ${GMOCK_LIBRARY}  ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME interop_gtests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
              COMMAND ${CMAKE_BINARY_DIR}/testBin/${INTEROP_TESTS} )

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
endif()

if(NOT ENABLE_STATIC)
add_custom_command(TARGET interop_gtests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(MSVC)
    set_target_properties(interop_gtests PROPERTIES COMPILE_FLAGS "${ENABLE_BIG_OBJ_FLAG}")
elseif(MINGW)
    set_target_properties(interop_gtests PROPERTIES COMPILE_FLAGS "${ENABLE_BIG_OBJ_FLAG} ${_WNO_UNUSED_FUNCTION} ${_WNO_UNUSED_PARAMETER}")
elseif(COMPILER_IS_GNUCC_OR_CLANG)
    set_target_properties(interop_gtests PROPERTIES COMPILE_FLAGS "${_WNO_UNUSED_FUNCTION} ${_WNO_UNUSED_PARAMETER} ${_WNO_UNNAMED_TYPE_TEMPLATE_ARGS}")
endif()

