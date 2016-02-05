

set(SRCS
        model/metrics/corrected_intensity_metric.cpp
        model/metrics/tile_metric.cpp
        model/metrics/error_metric.cpp
        model/metrics/extraction_metric.cpp
        model/metrics/image_metric.cpp
        model/metrics/q_metric.cpp
        model/metrics/index_metric.cpp
)

set(HEADERS
        ../../interop/model/metric_sets/error_metric_set.h
        ../../interop/model/metric_sets/corrected_intensity_metric_set.h
        ../../interop/model/metrics/tile_metric.h
        ../../interop/model/metrics/corrected_intensity_metric.h
        ../../interop/io/layout/base_metric.h
        ../../interop/model/metric_base/metric_set.h
        ../../interop/model/metric_base/base_metric.h
        ../../interop/model/metric_base/base_cycle_metric.h
        ../../interop/model/metric_base/base_read_metric.h
        ../../interop/util/filesystem.h
        ../../interop/util/unique_ptr.h
        ../../interop/util/lexical_cast.h
        ../../interop/io/stream_exceptions.h
        ../../interop/io/format/abstract_metric_format.h
        ../../interop/io/format/metric_format.h
        ../../interop/io/format/metric_format_factory.h
        ../../interop/io/metric_stream.h
        ../../interop/io/format/generic_layout.h
        ../../interop/model/metrics/error_metric.h
        ../../interop/model/metrics/extraction_metric.h
        ../../interop/model/metrics/image_metric.h
        ../../interop/model/metrics/q_metric.h
        ../../interop/io/format/stream_util.h
        ../../interop/util/static_assert.h
        ../../interop/util/cstdint.h
        ../../interop/util/pstdint.h
        ../../interop/constants/enums.h
        ../../interop/model/metric_sets/extraction_metric_set.h
        ../../interop/model/metric_sets/image_metric_set.h
        ../../interop/model/metric_sets/q_metric_set.h
        ../../interop/model/metric_sets/tile_metric_set.h
        ../../interop/io/metric_file_stream.h
        ../../interop/model/metric_base/metric_exceptions.h
        ../../interop/util/assert.h
        ../../interop/io/format/map_io.h
        ../../interop/util/length_of.h
        ../../interop/model/metrics/index_metric.h
        ../../interop/model/metric_sets/index_metric_set.h
        ../../interop/util/xml_parser.h
        ../../interop/util/xml_exceptions.h
        ../../interop/util/time.h)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR CMAKE_COMPILER_IS_GNUCC)
    foreach(SRC ${SRCS})
        set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${CXX_PEDANTIC_FLAG}" )
    endforeach()
endif()

include(CheckFunctionExists)
check_function_exists("strptime" HAVE_STRPTIME)
if(HAVE_STRPTIME)
    add_definitions(-DHAVE_STRPTIME)
elseif(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_library(${INTEROP_LIB} ${LIBRARY_TYPE} ${SRCS} ${HEADERS})
add_library(${INTEROP_DL_LIB} ${LIBRARY_TYPE} ${SRCS} ${HEADERS})

set_target_properties(${INTEROP_DL_LIB} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)
if(COMPILER_IS_GNUCC_OR_CLANG AND NOT MINGW)
    set_target_properties(${INTEROP_DL_LIB} PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

install(TARGETS ${INTEROP_LIB}
        LIBRARY DESTINATION lib64
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib64
)
