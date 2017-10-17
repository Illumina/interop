# Adds the following targets:
#   1. csharp_unittest - Shared library for Unit Testing
#
# Post build of csharp_unittest the unit tests are run
#

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(CSBuild)

if(NOT CSBUILD_FOUND)
    return()
endif()

if(NOT DOTNET_CORE_FOUND)
    find_package(NUnit)
    if(NOT NUNIT_FOUND)
        message(WARNING "NUnit not found, C# unit testing will be disabled")
        message(WARNING "You may set the location of NUNIT with NUNIT_ROOT or the NUNIT_DIR environment variable")
        return()
    endif()
    set(CSHARP_INTERPRETER "${NUNIT_COMMAND}")
    set(PROJ_TYPE "library")
else()
    set(NUNIT_LIBRARIES "NUnit=3.7.1;NunitLite=3.7.1;System.dll")
    set(NUNIT_TARGET "")
    set(PROJ_TYPE "Exe")
endif()

set(CSHARP_TEST_ENABLED ON PARENT_SCOPE)

include(${CSBUILD_USE_FILE})

set(TEST_SRCS
        CoreTests.cs
        logic/ImagingTableLogic.cs
        logic/IndexSummaryLogic.cs
        logic/PlotSampleQC.cs
        logic/PlotFlowcellMap.cs
        logic/PlotQScoreHeatmap.cs
        logic/PlotQScoreHistogram.cs
        logic/PlotDataByLaneTest.cs
        logic/PlotDataByCycleTest.cs
        logic/DependencyTest.cs
        metrics/RunMetricsTest.cs
        metrics/ErrorMetricsTest.cs
        metrics/CorrectedIntensityMetricsTest.cs
        metrics/ExtractionMetricsTest.cs
        metrics/ImageMetricsTest.cs
        metrics/IndexMetricsTest.cs
        metrics/QMetricsTest.cs
        metrics/TileMetricsTest.cs
        metrics/ExtendedTileMetricsTest.cs
        run/RunInfoTest.cs
        run/RunParametersTest.cs
        )
if(DOTNET_CORE_FOUND)
    set(TEST_SRCS ${TEST_SRCS} Program.cs)
endif()


find_program(NUGET_EXE nuget)
if(NUGET_EXE)
    # Test the NuGet Package
    set(CSHARP_NUGET_SOURCE "${SWIG_CSHARP_PACKAGE_PATH}")
    csharp_add_project(${PROJ_TYPE} csharp_unittest ${NUNIT_LIBRARIES} ${TEST_SRCS} ${NUGET_PACKAGE_ID}=${NUGET_VERSION_STR})
    add_dependencies(csharp_unittest nupack ${NUNIT_TARGET})
else()
    # Test the Library
    csharp_add_project(${PROJ_TYPE} csharp_unittest ${NUNIT_LIBRARIES} ${TEST_SRCS} ${SWIG_CSHARP_LIBRARY})
    add_dependencies(csharp_unittest csharp_interop ${NUNIT_TARGET})
endif()

get_filename_component(CSBUILD_csharp_unittest_BINARY_DIR "${CSBUILD_csharp_unittest_BINARY}" DIRECTORY)
add_custom_command(TARGET csharp_unittest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CSBUILD_csharp_unittest_BINARY_DIR} $<TARGET_FILE_DIR:${INTEROP_LIB}>
        )


if(NOT ENABLE_STATIC)
    add_custom_command(TARGET csharp_unittest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR}
            )
endif()



set(PERF_TEST_SRCS
        metrics/PerformanceTest.cs
        )

csharp_add_library(csharp_perftest ${NUNIT_LIBRARIES} ${PERF_TEST_SRCS} ${SWIG_CSHARP_LIBRARY})
add_dependencies(csharp_perftest csharp_interop ${NUNIT_TARGET})

add_custom_command(TARGET csharp_perftest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CSBUILD_csharp_perftest_BINARY} $<TARGET_FILE_DIR:${INTEROP_LIB}>
        #COMMAND ${CMAKE_COMMAND} -E copy ${NUNIT_LIBRARIES} $<TARGET_FILE_DIR:${INTEROP_LIB}>
        )

if(NOT ENABLE_STATIC)
    add_custom_command(TARGET csharp_perftest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_custom_target(check_csharp
        COMMENT "Running C# unit tests: ${CSHARP_INTERPRETER} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSBUILD_csharp_unittest_BINARY_NAME}"
        COMMAND ${CSHARP_INTERPRETER} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSBUILD_csharp_unittest_BINARY_NAME}
        )

add_dependencies(check_csharp csharp_unittest)
set_target_properties(check_csharp PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)


add_custom_target(csharp_perf
        COMMAND ${NUNIT_COMMAND} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSBUILD__csharp_perftest_BINARY_NAME})
set_target_properties(csharp_perf PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
set_target_properties(csharp_perftest PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

if(ENABLE_BUNDLE_TEST)
    csharp_add_project(${PROJ_TYPE} csharp_unittest_nuget ${NUNIT_LIBRARIES} ${TEST_SRCS} ${SWIG_CSHARP_NUGET_PACKAGE})
    set_target_properties(csharp_unittest_nuget PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
endif()