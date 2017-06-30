
set(SRCS
        logic/metric/q_metric.cpp
        logic/metric/extraction_metric.cpp
        model/run/info.cpp
        model/run/parameters.cpp
        model/metrics/corrected_intensity_metric.cpp
        model/metrics/tile_metric.cpp
        model/metrics/error_metric.cpp
        model/metrics/extraction_metric.cpp
        model/metrics/image_metric.cpp
        model/metrics/q_metric.cpp
        model/metrics/index_metric.cpp
        model/metrics/q_collapsed_metric.cpp
        model/summary/run_summary.cpp
        logic/plot/plot_by_cycle.cpp
        logic/plot/plot_by_lane.cpp
        logic/plot/plot_flowcell_map.cpp
        logic/plot/plot_qscore_heatmap.cpp
        logic/plot/plot_sample_qc.cpp
        logic/plot/plot_qscore_histogram.cpp
        model/run_metrics.cpp
        model/run_metrics_helper.cpp
        logic/summary/run_summary.cpp
        logic/summary/index_summary.cpp
        logic/table/create_imaging_table_columns.cpp
        logic/table/create_imaging_table.cpp
        util/time.cpp
        util/filesystem.cpp
        logic/utils/metrics_to_load.cpp
        model/summary/index_summary.cpp
        model/metrics/phasing_metric.cpp
        model/metrics/dynamic_phasing_metric.cpp
        logic/metric/dynamic_phasing_metric.cpp
        logic/plot/plot_metric_list.cpp
        logic/metric/index_metric.cpp)

set(HEADERS
        ../../interop/io/paths.h
        ../../interop/model/summary/cycle_state_summary.h
        ../../interop/model/summary/lane_summary.h
        ../../interop/model/summary/metric_stat.h
        ../../interop/model/summary/metric_summary.h
        ../../interop/model/summary/read_summary.h
        ../../interop/model/summary/run_summary.h
        ../../interop/logic/summary/error_summary.h
        ../../interop/logic/summary/extraction_summary.h
        ../../interop/logic/summary/quality_summary.h
        ../../interop/logic/summary/run_summary.h
        ../../interop/logic/summary/summary_statistics.h
        ../../interop/logic/summary/tile_summary.h
        ../../interop/model/run/cycle_range.h
        ../../interop/model/run/flowcell_layout.h
        ../../interop/model/run/image_dimensions.h
        ../../interop/model/run/info.h
        ../../interop/model/run/parameters.h
        ../../interop/model/run/read_info.h
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
        ../../interop/io/metric_file_stream.h
        ../../interop/model/model_exceptions.h
        ../../interop/util/assert.h
        ../../interop/io/format/map_io.h
        ../../interop/util/length_of.h
        ../../interop/model/metrics/index_metric.h
        ../../interop/util/xml_parser.h
        ../../interop/util/xml_exceptions.h
        ../../interop/util/time.h
        ../../interop/util/statistics.h
        ../../interop/logic/metric/q_metric.h
        ../../interop/logic/utils/channel.h
        ../../interop/model/run_metrics.h
        ../../interop/util/type_traits.h
        ../../interop/util/linear_hierarchy.h
        ../../interop/util/object_list.h
        ../../interop/logic/summary/map_cycle_to_read.h
        ../../interop/logic/summary/cycle_state_summary.h
        ../../interop/util/math.h
        ../../interop/model/plot/filter_options.h
        ../../interop/logic/utils/enums.h
        ../../interop/logic/utils/metric_type_ext.h
        ../../interop/logic/plot/plot_by_cycle.h
        ../../interop/logic/metric/metric_value.h
        ../../interop/logic/plot/plot_by_lane.h
        ../../interop/logic/plot/plot_point.h
        ../../interop/model/plot/data_point.h
        ../../interop/model/plot/series.h
        ../../interop/model/plot/data_point_collection.h
        ../../interop/model/plot/candle_stick_point.h
        ../../interop/model/plot/plot_data.h
        ../../interop/logic/plot/plot_data.h
        ../../interop/model/plot/axes.h
        ../../interop/logic/plot/plot_qscore_histogram.h
        ../../interop/logic/plot/plot_qscore_heatmap.h
        ../../interop/logic/plot/plot_flowcell_map.h
        ../../interop/model/plot/bar_point.h
        ../../interop/model/plot/heatmap_data.h
        ../../interop/model/plot/chart_data.h
        ../../interop/model/plot/flowcell_data.h
        ../../interop/constants/typedefs.h
        ../../interop/logic/plot/plot_sample_qc.h
        ../../interop/model/summary/index_lane_summary.h
        ../../interop/model/summary/index_flowcell_summary.h
        ../../interop/model/summary/index_count_summary.h
        ../../interop/logic/summary/index_summary.h
        ../../interop/model/table/imaging_table.h
        ../../interop/util/string.h
        ../../interop/model/metrics/q_collapsed_metric.h
        ../../interop/model/metrics/q_by_lane_metric.h
        ../../interop/util/constant_mapping.h
        ../../interop/io/plot/gnuplot.h
        ../../interop/logic/metric/tile_metric.h
        ../../interop/logic/logic.h
        ../../interop/interop.h
        ../../interop/util/exception.h
        ../../interop/io/table/csv_format.h
        ../../interop/util/option_parser.h
        ../../interop/logic/metric/extraction_metric.h
        ../../interop/logic/table/create_imaging_table.h
        ../../interop/model/table/imaging_column.h
        ../../interop/logic/table/create_imaging_table_columns.h
        ../../interop/logic/table/table_util.h
        ../../interop/io/table/imaging_table_csv.h
        ../../interop/logic/table/check_imaging_table_column.h
        ../../interop/logic/table/table_populator.h
        ../../interop/logic/utils/metrics_to_load.h
        ../../interop/io/format/default_layout.h
        ../../interop/io/format/stream_membuf.h
        ../../interop/model/summary/surface_summary.h
        ../../interop/model/summary/stat_summary.h
        ../../interop/util/indirect_range_iterator.h
        ../../interop/util/map.h
        ../../interop/util/timer.h
        ../../interop/constants/enum_description.h
        ../../interop/io/format/abstract_text_format.h
        ../../interop/io/format/text_format.h
        ../../interop/util/self_registration.h
        ../../interop/io/format/text_format_factory.h
        ../../interop/model/summary/metric_average.h
        ../../interop/model/metrics/phasing_metric.h
        ../../interop/logic/summary/phasing_summary.h
        ../../interop/model/metrics/dynamic_phasing_metric.h
        ../../interop/logic/metric/dynamic_phasing_metric.h
        ../../interop/util/base_exception.h
        ../../interop/model/run/run_exceptions.h
        ../../interop/model/metric_base/metric_exceptions.h
        ../../interop/model/table/table_exceptions.h
        ../../interop/model/plot/plot_exceptions.h
        ../../interop/logic/plot/plot_metric_proxy.h
        ../../interop/logic/plot/plot_metric_list.h
        ../../interop/logic/metric/index_metric.h
        )

set(INTEROP_HEADERS ${HEADERS} PARENT_SCOPE)
if(COMPILER_IS_GNUCC_OR_CLANG)
    foreach(SRC ${SRCS})
        set_source_files_properties(${SRC} PROPERTIES COMPILE_FLAGS "${CXX_PEDANTIC_FLAG}" )
    endforeach()
endif()

include(CheckFunctionExists)
check_function_exists("strptime" HAVE_STRPTIME)
if(HAVE_STRPTIME)
    add_definitions(-DHAVE_STRPTIME)
elseif(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS  -D_SCL_SECURE_NO_WARNINGS)
endif()

# Shared libraries on windows are currently not supported, but we may in the future.
set(SWIG_VERSION_INFO "")
if(MSVC AND "${LIBRARY_TYPE}" STREQUAL "SHARED")
    set(SWIG_VERSION_INFO ${CMAKE_CURRENT_BINARY_DIR}/interop_lib_version.rc)
    set(LIB_NAME "Core C++ InterOp Library") # Used to configure the version info file
    configure_file(${CMAKE_SOURCE_DIR}/cmake/version.rc.in ${SWIG_VERSION_INFO} @ONLY) # Requires: LIB_NAME, VERSION_LIST and VERSION
endif()

add_library(${INTEROP_LIB} ${LIBRARY_TYPE} ${SRCS} ${HEADERS} ${SWIG_VERSION_INFO})
add_dependencies(${INTEROP_LIB} version)
if(NOT "${INTEROP_DL_LIB}" STREQUAL "${INTEROP_LIB}")
    add_library(${INTEROP_DL_LIB} ${LIBRARY_TYPE} ${SRCS} ${HEADERS}  ${SWIG_VERSION_INFO} )
    set_target_properties(${INTEROP_DL_LIB} PROPERTIES COMPILE_FLAGS "-fPIC")
    add_dependencies(${INTEROP_DL_LIB} version)
    install(TARGETS ${INTEROP_DL_LIB}
            LIBRARY DESTINATION lib64
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib64
            )
endif()

install(TARGETS ${INTEROP_LIB}
        LIBRARY DESTINATION lib64
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib64
)

if(MSVC)


    if (NOT BUILD_SHARED_LIBS AND NOT FORCE_SHARED_CRT)
        set_msvc_md()
        set(INTEROP_MSVC interop-md)
        set_target_properties(${INTEROP_LIB} PROPERTIES OUTPUT_NAME "interop-mt")
    else()
        set_msvc_mt()
        set(INTEROP_MSVC interop-mt)
        set_target_properties(${INTEROP_LIB} PROPERTIES OUTPUT_NAME "interop-md")
    endif()
    add_library(${INTEROP_MSVC} ${LIBRARY_TYPE} ${SRCS} ${HEADERS}  ${SWIG_VERSION_INFO} )
    add_dependencies(${INTEROP_MSVC} version)
    install(TARGETS ${INTEROP_MSVC}
            LIBRARY DESTINATION lib64
            RUNTIME DESTINATION bin
            ARCHIVE DESTINATION lib64
            )
    if (NOT BUILD_SHARED_LIBS AND NOT FORCE_SHARED_CRT)
        set_msvc_mt()
    else()
        set_msvc_md()
    endif()

endif()