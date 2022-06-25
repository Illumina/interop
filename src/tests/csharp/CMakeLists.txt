# Adds the following targets:
#   1. csharp_unittest - Shared library for Unit Testing
#
# Post build of csharp_unittest the unit tests are run
#

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(CSBuild)
if(NOT DEFINED CSHARP_TEST_FRAMEWORK)
    set(CSHARP_TEST_FRAMEWORK "netcoreapp2.0")
endif()
set(CSHARP_TARGET_FRAMEWORK "${CSHARP_TEST_FRAMEWORK}")
set(CSHARP_NUGET_SOURCE "${CSHARP_NUGET_PREFIX}")

if(NOT CSBUILD_FOUND)
    return()
endif()

set(CSHARP_TEST_FRAMEWORK "netcoreapp2.0" CACHE STRING "Framework for unit tests")
set(CSHARP_TARGET_FRAMEWORK "${CSHARP_TEST_FRAMEWORK}")

message(STATUS "CSHARP_TEST_FRAMEWORK=${CSHARP_TEST_FRAMEWORK}")

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

if(NOT DotNetStandard_FOUND)
    include(ExternalProject)
    ExternalProject_Add(
            NUnit
            DOWNLOAD_DIR ${CSHARP_NUGET_SOURCE}
            URL https://github.com/nunit/nunit/releases/download/3.7.1/NUnit.3.7.1.nupkg
            DOWNLOAD_NAME NUnit.3.7.1.nupkg
            UPDATE_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
            INSTALL_COMMAND ""
            DOWNLOAD_NO_EXTRACT 1
    )
    ExternalProject_Add(
            NUnitLite
            DOWNLOAD_DIR ${CSHARP_NUGET_SOURCE}
            URL https://github.com/nunit/nunit/releases/download/3.7.1/NUnitLite.3.7.1.nupkg
            DOWNLOAD_NAME NUnitLite.3.7.1.nupkg
            UPDATE_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
            DOWNLOAD_NO_EXTRACT 1
    )
    set(NUNIT_LIBRARIES "NUnit=3.7.1;NUnitLite=3.7.1")
    set(TEST_SRCS ${TEST_SRCS} TestSuite.cs)
else()
    set(TEST_SRCS ${TEST_SRCS} TestSuite.cs)
    add_custom_target(NUnit)
    add_custom_target(NUnitLite)
    set(NUNIT_LIBRARIES "NUnit=3.7.1;NUnitLite=3.7.1;System.dll")
endif()



csharp_add_project(Exe csharp_unittest ${NUNIT_LIBRARIES} ${TEST_SRCS} ${NUGET_PACKAGE_ID}=${NUGET_VERSION_STR})
add_custom_command(TARGET csharp_unittest
        POST_BUILD
        COMMENT "Remove directory: ${CMAKE_CURRENT_BINARY_DIR}/obj"
        COMMAND ${CMAKE_COMMAND} -E echo "Remove directory: ${CMAKE_CURRENT_BINARY_DIR}/obj"
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/obj
        )
add_dependencies(csharp_unittest csharp_interop NUnit NUnitLite)
# Clear old packages
# dotnet nuget locals all --clear


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
set(CSHARP_TARGET_FRAMEWORK "netstandard2.0" CACHE STRING "C# .NET framework for dotnet" )

add_custom_target(check_csharp
        COMMENT "Running C# unit tests: ${CSHARP_INTERPRETER} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSBUILD_csharp_unittest_BINARY_NAME}"
        COMMAND ${CHECKER} ${CSHARP_INTERPRETER} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSBUILD_csharp_unittest_BINARY_NAME}
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