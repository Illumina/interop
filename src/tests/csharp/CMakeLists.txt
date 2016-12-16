# Adds the following targets:
#   1. csharp_unittest - Shared library for Unit Testing
#
# Post build of csharp_unittest the unit tests are run
#

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(CSharp)

if(NOT CSHARP_FOUND)
    return()
endif()

find_package(NUnit)
if(NOT NUNIT_FOUND)
    message(WARNING "NUnit not found, C# unit testing will be disabled")
    message(WARNING "You may set the location of NUNIT with NUNIT_ROOT or the NUNIT_DIR environment variable")
    return()
endif()
set(CSHARP_TEST_ENABLED ON PARENT_SCOPE)

include(${CSHARP_USE_FILE})

set(TEST_SRCS
        CoreTests.cs
        logic/ImagingTableLogic.cs
        logic/ExceptionTest.cs
        logic/IndexSummaryLogic.cs
        logic/PlotSampleQC.cs
        logic/PlotFlowcellMap.cs
        logic/PlotQScoreHeatmap.cs
        logic/PlotQScoreHistogram.cs
        logic/PlotDataByLaneTest.cs
        logic/PlotDataByCycleTest.cs
        metrics/RunMetricsTest.cs
        metrics/ErrorMetricsTest.cs
        metrics/CorrectedIntensityMetricsTest.cs
        metrics/ExtractionMetricsTest.cs
        metrics/ImageMetricsTest.cs
        metrics/IndexMetricsTest.cs
        metrics/QMetricsTest.cs
        metrics/TileMetricsTest.cs
        run/RunInfoTest.cs
        run/RunParametersTest.cs
        )
set(csharp_files)
foreach(SRC ${TEST_SRCS})
    list(APPEND csharp_files "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
endforeach()

csharp_add_library(csharp_unittest ${NUNIT_LIBRARIES} ${csharp_files} ${SWIG_CSHARP_LIBRARY})
add_dependencies(csharp_unittest csharp_interop ${NUNIT_TARGET})

add_custom_command(TARGET csharp_unittest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CSHARP_csharp_unittest_BINARY} $<TARGET_FILE_DIR:${INTEROP_LIB}>
        COMMAND ${CMAKE_COMMAND} -E copy ${NUNIT_LIBRARIES} $<TARGET_FILE_DIR:${INTEROP_LIB}>)


if(NOT ENABLE_STATIC)
    add_custom_command(TARGET csharp_unittest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()



set(PERF_TEST_SRCS
        metrics/PerformanceTest.cs
        )
set(csharp_perf_files)
foreach(SRC ${PERF_TEST_SRCS})
    list(APPEND csharp_perf_files "${CMAKE_CURRENT_SOURCE_DIR}/${SRC}")
endforeach()

csharp_add_library(csharp_perftest ${NUNIT_LIBRARIES} ${csharp_perf_files} ${SWIG_CSHARP_LIBRARY})
add_dependencies(csharp_perftest csharp_interop ${NUNIT_TARGET})

add_custom_command(TARGET csharp_perftest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CSHARP_csharp_perftest_BINARY} $<TARGET_FILE_DIR:${INTEROP_LIB}>
        COMMAND ${CMAKE_COMMAND} -E copy ${NUNIT_LIBRARIES} $<TARGET_FILE_DIR:${INTEROP_LIB}>)

if(NOT ENABLE_STATIC)
    add_custom_command(TARGET csharp_perftest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_custom_target(check_csharp
        COMMENT "Running C# unit tests: ${NUNIT_COMMAND} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSHARP_csharp_unittest_BINARY_NAME}}"
        COMMAND ${NUNIT_COMMAND} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSHARP_csharp_unittest_BINARY_NAME}
        )

add_dependencies(check_csharp csharp_unittest)
set_target_properties(check_csharp PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)


set(NUNIT_COMMAND_EXE "${NUNIT_COMMAND}" PARENT_SCOPE)
set(CSHARP_PERF_TEST_EXE "${CSHARP_csharp_perftest_BINARY_NAME}" PARENT_SCOPE)
