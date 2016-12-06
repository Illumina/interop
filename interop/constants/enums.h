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
#define INTEROP_UNKNOWN 0x100


/** Enumeration of specific features that can belong to a metric
 *
 * @note This macro requires the macro INTEROP_TUPLE2 to be defined before use
 * @see illumina::interop::constants::metric_feature_type
 */
#define INTEROP_ENUM_METRIC_FEATURE_TYPE \
        INTEROP_TUPLE2(TileFeature, 0x01), \
        INTEROP_TUPLE2(CycleFeature, 0x02), \
        INTEROP_TUPLE2(ReadFeature, 0x04), \
        INTEROP_TUPLE2(BaseFeature, 0x08), \
        INTEROP_TUPLE2(ChannelFeature, 0x10), \
        INTEROP_TUPLE1(UnknownMetricFeature)


/** Enumeration of each metric type
 *
 * @note This macro requires the macro INTEROP_TUPLE3 to be defined before use
 * @see illumina::interop::constants::metric_type
 */
#define INTEROP_ENUM_METRIC_TYPES \
        INTEROP_TUPLE4(Intensity, "Intensity", Extraction, CycleFeature|ChannelFeature),\
        INTEROP_TUPLE4(FWHM, "FWHM", Extraction, CycleFeature|ChannelFeature),\
        /*INTEROP_TUPLE4(PercentBase, "% Base", CorrectedInt, CycleFeature|BaseFeature), */\
        INTEROP_TUPLE4(BasePercent, "% Base", CorrectedInt, CycleFeature|BaseFeature),\
        INTEROP_TUPLE4(PercentNoCall, "% NoCall", CorrectedInt, CycleFeature),\
        /*INTEROP_TUPLE4(PercentQ20, "% >=Q20", Q, CycleFeature),*/\
        INTEROP_TUPLE4(Q20Percent, "% >=Q20", Q, CycleFeature),\
        /*INTEROP_TUPLE4(PercentQ30, "% >=Q30", Q, CycleFeature),*/\
        INTEROP_TUPLE4(Q30Percent, "% >=Q30", Q, CycleFeature),\
        INTEROP_TUPLE4(AccumPercentQ20, "% >=Q20 (Accumulated)", Q, CycleFeature),\
        INTEROP_TUPLE4(AccumPercentQ30, "% >=Q30 (Accumulated)", Q, CycleFeature),\
        INTEROP_TUPLE4(QScore, "Median QScore", Q, CycleFeature),\
        INTEROP_TUPLE4(Clusters, "Density", Tile, TileFeature),\
        /*INTEROP_TUPLE4(Density, "Density (K/mm2)", Tile, TileFeature),*/ \
        INTEROP_TUPLE4(ClustersPF, "Density PF", Tile, TileFeature),\
        /*INTEROP_TUPLE4(DensityPF, "Density PF", Tile, TileFeature),*/\
        INTEROP_TUPLE4(ClusterCount, "Cluster Count", Tile, TileFeature),\
        /*INTEROP_TUPLE4(ClusterCount, "Cluster Count", Tile, TileFeature),*/\
        INTEROP_TUPLE4(ClusterCountPF, "Clusters PF", Tile, TileFeature),\
        INTEROP_TUPLE4(ErrorRate, "Error Rate", Error, CycleFeature),\
        INTEROP_TUPLE4(PercentPhasing, "% Phasing", Tile, ReadFeature),\
        INTEROP_TUPLE4(PercentPrephasing, "% Prephasing", Tile, ReadFeature),\
        INTEROP_TUPLE4(PercentAligned, "% Aligned", Tile, ReadFeature),\
        INTEROP_TUPLE4(CorrectedIntensity, "Corrected Int", CorrectedInt, CycleFeature|BaseFeature),\
        INTEROP_TUPLE4(CalledIntensity, "Called Int", CorrectedInt, CycleFeature|BaseFeature),\
        INTEROP_TUPLE4(SignalToNoise, "Signal to Noise", CorrectedInt, CycleFeature),\
        INTEROP_TUPLE4(MetricTypeCount, "!!!!BAD!!!!", UnknownMetricGroup, UnknownMetricFeature),\
        INTEROP_TUPLE4(UnknownMetricType, "!!!!BAD!!!!", UnknownMetricGroup, UnknownMetricFeature)

/** Enumeration of each metric group
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::metric_group
 */
#define INTEROP_ENUM_METRIC_GROUPS \
        INTEROP_TUPLE1(CorrectedInt),\
        INTEROP_TUPLE1(Error),\
        INTEROP_TUPLE1(Extraction),\
        INTEROP_TUPLE1(Image),\
        INTEROP_TUPLE1(Index),\
        INTEROP_TUPLE1(Q),\
        INTEROP_TUPLE1(Tile),\
        INTEROP_TUPLE1(QByLane),\
        INTEROP_TUPLE1(QCollapsed),\
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
        INTEROP_TUPLE1(FourDigit),\
        /** Five digit tiles for NextSeq and ... */\
        INTEROP_TUPLE1(FiveDigit),\
        /** Absolute naming for tiles GAs and old MiSeqs*/\
        INTEROP_TUPLE1(Absolute),\
        /** Number of naming methods */\
        INTEROP_TUPLE1(TileNamingMethodCount),\
        INTEROP_TUPLE1(UnknownTileNamingMethod)

/** Enumeration of DNA bases
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @note This macro requires the macro INTEROP_TUPLE2 to be defined before use
 * @see illumina::interop::constants::dna_bases
 */
#define INTEROP_ENUM_DNA_BASE_TYPES \
        /** No calls */ \
        INTEROP_TUPLE2(NC, -1), \
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
        INTEROP_TUPLE1(SentinelSurface),\
        INTEROP_TUPLE1(Top),\
        INTEROP_TUPLE1(Bottom),\
        INTEROP_TUPLE1(UnknownSurface)


/** Enumeration of instrument types
 *
 * @note This macro requires the macro INTEROP_TUPLE1 to be defined before use
 * @see illumina::interop::constants::instrument_type
 */
#define INTEROP_ENUM_INSTRUMENT_TYPES \
        INTEROP_TUPLE1(HiSeq),\
        INTEROP_TUPLE1(HiScan),\
        INTEROP_TUPLE1(MiSeq),\
        INTEROP_TUPLE1(NextSeq),\
        INTEROP_TUPLE1(MiniSeq), \
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
        INTEROP_TUPLE1(Red),\
        /** Color for green channel or (C) */\
        INTEROP_TUPLE1(Green),\
        /** Color for G */\
        INTEROP_TUPLE1(Blue),\
        /** Color for T */\
        INTEROP_TUPLE1(Black),\
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
        INTEROP_TUPLE1(Centered),\
        /** Bar shifted to start on x-value */\
        INTEROP_TUPLE1(Shifted),\
        INTEROP_TUPLE1(UnknownBarPlotOption)

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Define x-macros
/////////////////////////////////////////////////////////////////////////////////////////////////////


/** This temp macro converts a enum/description pair into the an enum (first element of the pair) */
#define INTEROP_TUPLE4(X, Y, Z, A) X
/** This temp macro converts a enum/description pair into the an enum (first element of the pair) */
#define INTEROP_TUPLE3(X, Y, Z) X
/** This temp macro converts an enum to an enum */
#define INTEROP_TUPLE1(X) X
/** This temp macro converts an enum/value pair to an enum */
#define INTEROP_TUPLE2(X, V) X=V



namespace illumina { namespace interop { namespace constants
{
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

}}}

#undef INTEROP_TUPLE1
#undef INTEROP_TUPLE2
#undef INTEROP_TUPLE3
#undef INTEROP_TUPLE4


