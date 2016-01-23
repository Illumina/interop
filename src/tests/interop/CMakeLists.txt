find_package(GTest)
include(${GTEST_USE_FILE})
find_package(Threads)

add_executable(interop_gtests
        metrics/stream_exception_tests.hpp
        metrics/metric_test_utils.h
        metrics/corrected_intensity_metrics_test.cpp
        metrics/error_metrics_test.cpp
        metrics/extraction_metrics_test.cpp
        metrics/image_metrics_test.cpp
        metrics/index_metrics_test.cpp
        metrics/q_metrics_test.cpp
        metrics/tile_metrics_test.cpp)

target_link_libraries(interop_gtests ${INTEROP_LIB} ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME interop_gtests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/testBin
              COMMAND ${CMAKE_BINARY_DIR}/testBin/${INTEROP_TESTS} )


if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
if(NOT ENABLE_TEST)
    set_target_properties(interop_gtests PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()

if(NOT ENABLE_STATIC)
add_custom_command(TARGET interop_gtests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()

