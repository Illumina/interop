/** Enumeration constants used throughout InterOp
 *
 *  @file
 *  @date 9/27/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <climits>
#include <limits>
#include <string>
#include "interop/util/cstdint.h"

/** Sentinel for an unknown enum type */
#define INTEROP_UNKNOWN 0x400


/** Enumeration of specific features that can belong to a metric
 *
 * @note This macro requires the macro INTEROP_TUPLE_ASSIGN to be defined before use
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::metric_feature_type
 */
#define INTEROP_ENUM_METRIC_FEATURE_TYPE \
        INTEROP_TUPLE_ASSIGN(TileFeature, 0x01), \
        INTEROP_TUPLE_ASSIGN(CycleFeature, 0x02), \
        INTEROP_TUPLE_ASSIGN(ReadFeature, 0x04), \
        INTEROP_TUPLE_ASSIGN(BaseFeature, 0x08), \
        INTEROP_TUPLE_ASSIGN(ChannelFeature, 0x10), \
        INTEROP_TUPLE_ASSIGN(LaneFeature, 0x20), \
        INTEROP_TUPLE1(UnknownMetricFeature)
//NOTE: if we add any more features above, we should update the unknown metric value below

/** Enumeration of each metric type
 *
 * @note This macro requires the macro INTEROP_TUPLE1, INTEROP_TUPLE4, INTEROP_TUPLE7 to be defined before use
 * @see illumina::interop::constants::metric_type
 */
#define INTEROP_ENUM_METRIC_TYPES \
        INTEROP_TUPLE7(Intensity, "Intensity", Extraction, CycleFeature|ChannelFeature, extraction_metric, max_intensity, channel)\
        INTEROP_TUPLE7(FWHM, "FWHM", Extraction, CycleFeature|ChannelFeature, extraction_metric, focus_score, channel)\
        INTEROP_TUPLE7(BasePercent, "% Base", CorrectedInt, CycleFeature|BaseFeature, corrected_intensity_metric, percent_base, base)\
        INTEROP_TUPLE7(PercentNoCall, "% NoCall", CorrectedInt, CycleFeature, corrected_intensity_metric, percent_nocall, Void)\
        INTEROP_TUPLE7(Q20Percent, "% >=Q20", Q, CycleFeature, q_collapsed_metric, percent_over_q20, Void)\
        INTEROP_TUPLE7(Q30Percent, "% >=Q30", Q, CycleFeature, q_collapsed_metric, percent_over_q30, Void)\
        INTEROP_TUPLE7(AccumPercentQ20, "% >=Q20 (Accumulated)", Q, CycleFeature, q_collapsed_metric, cumulative_percent_over_q20, Void)\
        INTEROP_TUPLE7(AccumPercentQ30, "% >=Q30 (Accumulated)", Q, CycleFeature, q_collapsed_metric, cumulative_percent_over_q30, Void)\
        INTEROP_TUPLE7(QScore, "Median QScore", Q, CycleFeature, q_collapsed_metric, median_qscore_flt, Void)\
        INTEROP_TUPLE7(Clusters, "Density", Tile, TileFeature, tile_metric, cluster_density_k, Void)\
        INTEROP_TUPLE7(ClustersPF, "Density PF", Tile, TileFeature, tile_metric, cluster_density_pf_k, Void)\
        INTEROP_TUPLE7(ClusterCount, "Cluster Count", Tile, TileFeature, tile_metric, cluster_count_m, Void)\
        INTEROP_TUPLE7(ClusterCountPF, "Clusters PF", Tile, TileFeature, tile_metric, cluster_count_pf_m, Void)\
        INTEROP_TUPLE7(ErrorRate, "Error Rate", Error, CycleFeature, error_metric, error_rate, Void)\
        INTEROP_TUPLE7(PercentPhasing, "Legacy Phasing Rate", Tile, ReadFeature, tile_metric, percent_phasing_at, read)\
        INTEROP_TUPLE7(PercentPrephasing, "Legacy Prephasing Rate", Tile, ReadFeature, tile_metric, percent_prephasing_at, read)\
        INTEROP_TUPLE7(PercentAligned, "% Aligned", Tile, ReadFeature, tile_metric, percent_aligned_at, read)\
        INTEROP_TUPLE7(Phasing, "Phasing Weight", EmpiricalPhasing, CycleFeature, phasing_metric, phasing_weight, Void)\
        INTEROP_TUPLE7(PrePhasing, "Prephasing Weight", EmpiricalPhasing, CycleFeature, phasing_metric, prephasing_weight, Void)\
        INTEROP_TUPLE7(CorrectedIntensity, "Corrected Int", CorrectedInt, CycleFeature|BaseFeature, corrected_intensity_metric, corrected_int_all_float, base)\
        INTEROP_TUPLE7(CalledIntensity, "Called Int", CorrectedInt, CycleFeature|BaseFeature, corrected_intensity_metric, corrected_int_called, base)\
        INTEROP_TUPLE7(SignalToNoise, "Signal to Noise", CorrectedInt, CycleFeature, corrected_intensity_metric, signal_to_noise, Void)\
        INTEROP_TUPLE7(OccupiedCountK, "Occupied Count (K)", ExtendedTile, TileFeature, extended_tile_metric, cluster_count_occupied_k, Void)\
        INTEROP_TUPLE7(PercentOccupied, "% Occupied", ExtendedTile, TileFeature, extended_tile_metric, percent_occupied, Void)\
        INTEROP_TUPLE7(PercentPF, "% PF", Tile, TileFeature, tile_metric, percent_pf, Void)\
        INTEROP_TUPLE4(MetricTypeCount, "!!!!BAD!!!!", UnknownMetricGroup, UnknownMetricFeature)\
        INTEROP_TUPLE1(UnknownMetricType)

/** Enumeration of each metric group
 *
 * @note This macro requires the macro INTEROP_TUPLE2 to be defined before use
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::metric_group
 */
#define INTEROP_ENUM_METRIC_GROUPS \
        INTEROP_TUPLE2(CorrectedInt, CycleFeature|BaseFeature), \
        INTEROP_TUPLE2(Error, CycleFeature), \
        INTEROP_TUPLE2(Extraction, CycleFeature|ChannelFeature), \
        INTEROP_TUPLE2(Image, CycleFeature|ChannelFeature), \
        INTEROP_TUPLE2(Index, ReadFeature), \
        INTEROP_TUPLE2(Q, CycleFeature), \
        INTEROP_TUPLE2(Tile, TileFeature), \
        INTEROP_TUPLE2(QByLane, LaneFeature), \
        INTEROP_TUPLE2(QCollapsed, CycleFeature), \
        INTEROP_TUPLE2(EmpiricalPhasing, CycleFeature), \
        INTEROP_TUPLE2(DynamicPhasing, CycleFeature), \
        INTEROP_TUPLE2(ExtendedTile, TileFeature), \
        INTEROP_TUPLE1(MetricCount),\
        INTEROP_TUPLE1(UnknownMetricGroup)


/** Enumeration of each metric data type
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::metric_data
 */
#define INTEROP_ENUM_METRIC_DATA_TYPES \
        INTEROP_TUPLE1(IdType),\
        INTEROP_TUPLE1(ValueType),\
        INTEROP_TUPLE1(StructType),\
        INTEROP_TUPLE1(ChannelArray),\
        INTEROP_TUPLE1(BaseArray),\
        INTEROP_TUPLE1(MetricDataCount),\
        INTEROP_TUPLE1(UnknownMetricData)

/** Enumeration of tile naming methods
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::tile_naming_method
 */
#define INTEROP_ENUM_TILE_NAMING_METHODS \
        /** Four digit tiles for HiSeq and modern MiSeqs */\
        INTEROP_TUPLE1(FourDigit), \
        /** Five digit tiles for NextSeq and ... */\
        INTEROP_TUPLE1(FiveDigit), \
        /** Absolute naming for tiles GAs and old MiSeqs*/\
        INTEROP_TUPLE1(Absolute), \
        /** Number of naming methods */\
        INTEROP_TUPLE1(TileNamingMethodCount), \
        INTEROP_TUPLE1(UnknownTileNamingMethod)

/** Enumeration of DNA bases
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @note This macro requires the macro INTEROP_TUPLE_ASSIGN to be defined before use
 * @see illumina::interop::constants::dna_bases
 */
#define INTEROP_ENUM_DNA_BASE_TYPES \
        /** No calls */ \
        INTEROP_TUPLE_ASSIGN(NC, -1), \
        /** DNA base adenine */ \
        INTEROP_TUPLE1(A), \
        /** DNA base cytosine */ \
        INTEROP_TUPLE1(C), \
        /** DNA base guanine */ \
        INTEROP_TUPLE1(G), \
        /** DNA base thymine */ \
        INTEROP_TUPLE1(T), \
        /** Number of bases */ \
        INTEROP_TUPLE1(NUM_OF_BASES), \
        /** Number of bases including no calls*/ \
        INTEROP_TUPLE1(NUM_OF_BASES_AND_NC), \
        /** Unknown base */ \
        INTEROP_TUPLE1(UnknownBase)

/** Enumeration of surface types
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::surface_type
 */
#define INTEROP_ENUM_SURFACE_TYPES \
        INTEROP_TUPLE1(SentinelSurface), \
        INTEROP_TUPLE1(Top), \
        INTEROP_TUPLE1(Bottom), \
        INTEROP_TUPLE1(UnknownSurface)


/** Enumeration of instrument types
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::instrument_type
 */
#define INTEROP_ENUM_INSTRUMENT_TYPES \
        INTEROP_TUPLE1(HiSeq), \
        INTEROP_TUPLE1(HiScan), \
        INTEROP_TUPLE1(MiSeq), \
        INTEROP_TUPLE1(NextSeq), \
        INTEROP_TUPLE1(MiniSeq), \
        INTEROP_TUPLE1(NovaSeq), \
        INTEROP_TUPLE1(iSeq), \
        INTEROP_TUPLE1(InstrumentCount), \
        INTEROP_TUPLE1(UnknownInstrument)

/** Enumeration of metric types
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::metric_base_type
 */
#define INTEROP_ENUM_METRIC_BASE_TYPES \
        /** Tile base types are written out once for each tile */\
        INTEROP_TUPLE1(BaseTileType),\
        /** Cycle base types are written out once for each tile and cycle */\
        INTEROP_TUPLE1(BaseCycleType),\
        /** Read base types are written out once for each tile and read */\
        INTEROP_TUPLE1(BaseReadType),\
        /** Lane base types are written out once for each lane and cycle */\
        INTEROP_TUPLE1(BaseLaneType),\
        INTEROP_TUPLE1(BaseMetricCount),\
        INTEROP_TUPLE1(UnknownBaseType)


/** Enumeration of channel colors (assuming expected order)
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::plot_colors
 */
#define INTEROP_ENUM_PLOT_COLORS \
        /** Color for red channel (or A) */\
        INTEROP_TUPLE1(Red), /*#FF0000*/\
        /** Color for green channel or (C) */\
        INTEROP_TUPLE1(Green), /*#00FF00*/\
        /** Color for G */\
        INTEROP_TUPLE1(Blue), /*#0000FF*/\
        /** Color for T */\
        INTEROP_TUPLE1(Black), /*#000000*/\
        INTEROP_TUPLE1(PlotColorCount), \
        INTEROP_TUPLE1(UnknownColor)

/** Enumeration of plot types
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::plot_types
 */
#define INTEROP_ENUM_PLOT_TYPES \
        /** Spatial plot by tile */\
        INTEROP_TUPLE1(FlowcellPlot),\
        /** Plot data aggrergated by cycle */\
        INTEROP_TUPLE1(ByCyclePlot),\
        /** Spatial plot aggrergated by lane */\
        INTEROP_TUPLE1(ByLanePlot),\
        /** Histogram of q-scores */\
        INTEROP_TUPLE1(QHistogramPlot),\
        /** Heatmap of q-scores by cycle */\
        INTEROP_TUPLE1(QHeatmapPlot),\
        /** Indexing tab plot */\
        INTEROP_TUPLE1(SampleQCPlot),\
        /** Unknown plot type */\
        INTEROP_TUPLE1(UnknownPlotType)

/** Enumeration of bar plot options
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::bar_plot_options
 */
#define INTEROP_ENUM_BAR_PLOT_OPTIONS \
        /** Bar centered on x-value */\
        INTEROP_TUPLE1(Centered), \
        /** Bar shifted to start on x-value */\
        INTEROP_TUPLE1(Shifted), \
        INTEROP_TUPLE1(UnknownBarPlotOption)

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Define x-macros
/////////////////////////////////////////////////////////////////////////////////////////////////////


/** This temp macro converts a enum/description pair into the an enum (first element of the pair) */
#define INTEROP_TUPLE7(X, Unused1, Unused2, Unused3, Unused4, Unused5, Unused6) X,
/** This temp macro converts a enum/description pair into the an enum (first element of the pair) */
#define INTEROP_TUPLE4(X, Y, Z, A) X,
/** This temp macro converts a enum/description pair into the an enum (first element of the pair) */
#define INTEROP_TUPLE3(X, Y, Z) X
/** This temp macro converts an enum to an enum */
#define INTEROP_TUPLE1(X) X
/** This temp macro converts an enum/value pair to an enum */
#define INTEROP_TUPLE2(X, V) X
/** This temp macro converts an enum/value pair to an enum */
#define INTEROP_TUPLE_ASSIGN(X, V) X=V



namespace illumina { namespace interop { namespace constants
{

    /** @defgroup constants Enumerated constants
     *
     * Enumerated constants
     *
     * @ref illumina::interop::model::run::info "See full class description"
     * @{
     */
    /** Common codes for all enums */
    enum common
    {
        Unknown = INTEROP_UNKNOWN
    };
    /** Codes for each metric type */
    enum metric_type
    {
        INTEROP_ENUM_METRIC_TYPES = INTEROP_UNKNOWN
    };
    /** Codes for each metric group */
    enum metric_group
    {
        INTEROP_ENUM_METRIC_GROUPS = INTEROP_UNKNOWN
    };
    /** Tile naming conventions used in various Illumina Platforms */
    enum tile_naming_method
    {
        INTEROP_ENUM_TILE_NAMING_METHODS = INTEROP_UNKNOWN
    };
    /** DNA base constants */
    enum dna_bases
    {
        INTEROP_ENUM_DNA_BASE_TYPES = INTEROP_UNKNOWN
    };
    /** Flow cell surface code */
    enum surface_type
    {
        INTEROP_ENUM_SURFACE_TYPES = INTEROP_UNKNOWN
    };
    /** Instrument type */
    enum instrument_type
    {
        INTEROP_ENUM_INSTRUMENT_TYPES = INTEROP_UNKNOWN
    };
    /** Base types for each metric */
    enum metric_base_type
    {
        INTEROP_ENUM_METRIC_BASE_TYPES = INTEROP_UNKNOWN
    };
    /** Color codes for plotting */
    enum plot_colors
    {
        INTEROP_ENUM_PLOT_COLORS = INTEROP_UNKNOWN
    };
    /** Options for a bar plot */
    enum bar_plot_options
    {
        INTEROP_ENUM_BAR_PLOT_OPTIONS = INTEROP_UNKNOWN
    };
    /** Options for a bar plot */
    enum metric_data
    {
        INTEROP_ENUM_METRIC_DATA_TYPES = INTEROP_UNKNOWN
    };
    /** Options describing metric features */
    enum metric_feature_type
    {
        INTEROP_ENUM_METRIC_FEATURE_TYPE = INTEROP_UNKNOWN
    };
    /** Options for a bar plot */
    enum plot_types
    {
        INTEROP_ENUM_PLOT_TYPES = INTEROP_UNKNOWN
    };
    /** @} */

}}}

#undef INTEROP_TUPLE1
#undef INTEROP_TUPLE2
#undef INTEROP_TUPLE3
#undef INTEROP_TUPLE4
#undef INTEROP_TUPLE7
#undef INTEROP_TUPLE_ASSIGN


