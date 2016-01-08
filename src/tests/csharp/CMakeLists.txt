# Adds the following targets:
#   1. csharp_unittest - Shared library for Unit Testing
#
# Post build of csharp_unittest the unit tests are run
#

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
find_package(CSharp)
find_package(SWIG)
find_package(NUnit)

if(NOT CSHARP_FOUND)
    return()
endif()

if(NOT SWIG_FOUND)
    return()
endif()

if(NOT NUNIT_FOUND)
    message(WARNING "NUnit not found, C# unit testing will be disabled")
    message(WARNING "You may set the location of NUNIT with NUNIT_ROOT or the NUNIT_DIR environment variable")
    return()
endif()

include(${CSHARP_USE_FILE})

set(TEST_SRCS
        metrics/ErrorMetricsTest.cs
        metrics/CorrectedIntensityMetricsTest.cs
        metrics/ExtractionMetricsTest.cs
        metrics/ImageMetricsTest.cs
        metrics/IndexMetricsTest.cs
        metrics/QMetricsTest.cs
        metrics/TileMetricsTest.cs
        )
set(csharp_files ${SWIG_GEN_CSHARP_SOURCE_FILES})
foreach(SRC ${TEST_SRCS})
    get_filename_component(CSHARP_TEST_SRCS ${SRC} ABSOLUTE)
    list(APPEND csharp_files ${CSHARP_TEST_SRCS})
endforeach()

csharp_add_library(csharp_unittest ${NUNIT_LIBRARIES} ${csharp_files})
add_dependencies(csharp_unittest csharp_interop ${NUNIT_TARGET})
add_custom_command(TARGET csharp_unittest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CSHARP_csharp_unittest_BINARY} $<TARGET_FILE_DIR:${INTEROP_LIB}>
        COMMAND ${CMAKE_COMMAND} -E copy ${NUNIT_LIBRARIES} $<TARGET_FILE_DIR:${INTEROP_LIB}>
        COMMAND ${NUNIT_COMMAND} $<TARGET_FILE_DIR:${INTEROP_LIB}>/${CSHARP_csharp_unittest_BINARY_NAME})

if(NOT ENABLE_STATIC)
    add_custom_command(TARGET csharp_unittest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE_DIR:${INTEROP_LIB}> ${CMAKE_CURRENT_BINARY_DIR})
endif()