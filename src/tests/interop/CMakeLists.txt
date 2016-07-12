find_package(GTest)
find_package(GMock)
include(${GTEST_USE_FILE})
include(${GMOCK_USE_FILE})
find_package(Threads)

add_executable(interop_gtests
        run/info_test.cpp
        run/parameters_test.cpp
        metrics/corrected_intensity_metrics_test.cpp
        metrics/error_metrics_test.cpp
        metrics/extraction_metrics_test.cpp
        metrics/image_metrics_test.cpp
        metrics/index_metrics_test.cpp
        metrics/q_metrics_test.cpp
        metrics/tile_metrics_test.cpp
        metrics/q_by_lane_metric_test.cpp
        metrics/q_collapsed_metrics_test.cpp
        metrics/inc/corrected_intensity_metrics_test.h
        metrics/inc/error_metrics_test.h
        metrics/inc/metric_test.h
        metrics/inc/stream_tests.hpp
        metrics/summary_metrics_test.cpp
        metrics/inc/summary_fixture.h
        metrics/inc/extraction_metrics_test.h
        metrics/inc/image_metrics_test.h
        metrics/inc/index_metrics_test.h
        metrics/inc/q_metrics_test.h
        metrics/inc/tile_metrics_test.h
        metrics/inc/q_collapsed_metrics_test.h
        metrics/base_metric_tests.cpp
        logic/enum_parsing_test.cpp
        logic/metric_type_ext_test.cpp
        logic/plot_logic_test.cpp
        logic/image_table_logic_test.cpp
        util/option_parser_test.cpp
        inc/regression_fixture.h
        unit_tests.cpp
        inc/failure_listener.h
        inc/persistent_parameter_generator.h)

target_link_libraries(interop_gtests ${INTEROP_LIB}  ${GTEST_LIBRARY} ${GMOCK_LIBRARY}  ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME interop_gtests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
              COMMAND ${CMAKE_BINARY_DIR}/testBin/${INTEROP_TESTS} )

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
endif()
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set_target_properties(interop_gtests PROPERTIES COMPILE_FLAGS "-Wno-unnamed-type-template-args")
endif()

if(NOT ENABLE_TEST)
    set_target_properties(interop_gtests PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()

if(NOT ENABLE_STATIC)
add_custom_command(TARGET interop_gtests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()

if(MSVC)
    set_target_properties(interop_gtests PROPERTIES COMPILE_FLAGS "/bigobj")
elseif(MINGW)
    set_target_properties(interop_gtests PROPERTIES COMPILE_FLAGS "-Wa,-mbig-obj -Wno-unused-function")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCC)
    set_target_properties(interop_gtests PROPERTIES COMPILE_FLAGS "-Wno-unused-function")
endif()

