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
#include "interop/util/cstdint.h"

/** Enumeration of each metric type
 *
 * @note This macro requires the macro INTEROP_ENUM_DESCRIPTION to be defined before use
 * @see illumina::interop::constants::metric_type
 */
#define INTEROP_ENUM_METRIC_TYPES \
        INTEROP_ENUM_DESCRIPTION(Intensity, "Intensity", Extraction),\
        INTEROP_ENUM_DESCRIPTION(FWHM, "FWHM", Extraction),\
        INTEROP_ENUM_DESCRIPTION(PercentBase, "% Base", CorrectedInt),\
        INTEROP_ENUM_DESCRIPTION(PercentQ20, "% >=Q20", Q),\
        INTEROP_ENUM_DESCRIPTION(PercentQ30, "% >=Q30", Q),\
        INTEROP_ENUM_DESCRIPTION(AccumPercentQ20, "% >=Q20 (Accum)", Q),\
        INTEROP_ENUM_DESCRIPTION(AccumPercentQ30, "% >=Q30 (Accum)", Q),\
        INTEROP_ENUM_DESCRIPTION(QScore, "Median QScore", Q),\
        INTEROP_ENUM_DESCRIPTION(Density, "Cluster Density (K/mm2)", Tile),\
        /*INTEROP_ENUM_DESCRIPTION(Density, "Density (K/mm2)", Tile),*/ \
        INTEROP_ENUM_DESCRIPTION(DensityPF, "Density PF", Tile),\
        INTEROP_ENUM_DESCRIPTION(ClusterCount, "Clusters", Tile),\
        /*INTEROP_ENUM_DESCRIPTION(ClusterCount, "Cluster Count", Tile),*/\
        INTEROP_ENUM_DESCRIPTION(ClusterCountPF, "Clusters PF", Tile),\
        INTEROP_ENUM_DESCRIPTION(AlignedReadCount, "Aligned Read Count", Tile),\
        INTEROP_ENUM_DESCRIPTION(ErrorRate, "Error Rate", Error),\
        INTEROP_ENUM_DESCRIPTION(PercentPhasing, "% Phasing", Tile),\
        INTEROP_ENUM_DESCRIPTION(PercentPrephasing, "% Prephasing", Tile),\
        INTEROP_ENUM_DESCRIPTION(PercentAligned, "% Aligned", Tile),\
        INTEROP_ENUM_DESCRIPTION(CorrectedIntensity, "Corrected Int", CorrectedInt),\
        INTEROP_ENUM_DESCRIPTION(CalledIntensity, "Called Int", CorrectedInt),\
        INTEROP_ENUM_DESCRIPTION(SignalToNoise, "Signal to Noise", CorrectedInt),\
        INTEROP_ENUM_DESCRIPTION(MetricTypeCount, "!!!!BAD!!!!", UnknownMetricGroup),\
        INTEROP_ENUM_DESCRIPTION(UnknownMetricType, "!!!!BAD!!!!", UnknownMetricGroup)

/** Enumeration of each metric group
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::metric_group
 */
#define INTEROP_ENUM_METRIC_GROUPS \
        INTEROP_ENUM(CorrectedInt),\
        INTEROP_ENUM(Error),\
        INTEROP_ENUM(Extraction),\
        INTEROP_ENUM(Image),\
        INTEROP_ENUM(Index),\
        INTEROP_ENUM(Q),\
        INTEROP_ENUM(Tile),\
        INTEROP_ENUM(QByLane),\
        INTEROP_ENUM(QCollapsed),\
        INTEROP_ENUM(MetricCount),\
        INTEROP_ENUM(UnknownMetricGroup)


/** Enumeration of each metric data type
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::metric_data
 */
#define INTEROP_ENUM_METRIC_DATA_TYPES \
        INTEROP_ENUM(IdType),\
        INTEROP_ENUM(ValueType),\
        INTEROP_ENUM(ChannelArray),\
        INTEROP_ENUM(BaseArray),\
        INTEROP_ENUM(MetricDataCount),\
        INTEROP_ENUM(UnknownMetricData)

/** Enumeration of tile naming methods
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::tile_naming_method
 */
#define INTEROP_ENUM_TILE_NAMING_METHODS \
        /** Four digit tiles for HiSeq and modern MiSeqs */\
        INTEROP_ENUM(FourDigit),\
        /** Five digit tiles for NextSeq and ... */\
        INTEROP_ENUM(FiveDigit),\
        /** Absolute naming for tiles GAs and old MiSeqs*/\
        INTEROP_ENUM(Absolute),\
        /** Number of naming methods */\
        INTEROP_ENUM(TileNamingMethodCount),\
        INTEROP_ENUM(UnknownTileNamingMethod)

/** Enumeration of DNA bases
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @note This macro requires the macro INTEROP_ENUM_VALUE to be defined before use
 * @see illumina::interop::constants::dna_bases
 */
#define INTEROP_ENUM_DNA_BASE_TYPES \
        /** No calls */ \
        INTEROP_ENUM_VALUE(NC, -1), \
        /** DNA base adenine */ \
        INTEROP_ENUM(A), \
        /** DNA base cytosine */ \
        INTEROP_ENUM(C), \
        /** DNA base guanine */ \
        INTEROP_ENUM(G), \
        /** DNA base thymine */ \
        INTEROP_ENUM(T), \
        /** Number of bases */ \
        INTEROP_ENUM(NUM_OF_BASES), \
        /** Number of bases including no calls*/ \
        INTEROP_ENUM(NUM_OF_BASES_AND_NC), \
        /** Unknown base */ \
        INTEROP_ENUM(UnknownBase)

/** Enumeration of surface types
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::surface_type
 */
#define INTEROP_ENUM_SURFACE_TYPES \
        INTEROP_ENUM(SentinelSurface),\
        INTEROP_ENUM(Top),\
        INTEROP_ENUM(Bottom),\
        INTEROP_ENUM(UnknownSurface)


/** Enumeration of instrument types
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::instrument_type
 */
#define INTEROP_ENUM_INSTRUMENT_TYPES \
        INTEROP_ENUM(HiSeq),\
        INTEROP_ENUM(HiScan),\
        INTEROP_ENUM(MiSeq),\
        INTEROP_ENUM(NextSeq),\
        INTEROP_ENUM(MiniSeq), \
        INTEROP_ENUM(InstrumentCount), \
        INTEROP_ENUM(UnknownInstrument)

/** Enumeration of instrument types
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::instrument_type
 */
#define INTEROP_ENUM_METRIC_BASE_TYPES \
        /** Tile base types are written out once for each tile */\
        INTEROP_ENUM(BaseTileType),\
        /** Cycle base types are written out once for each tile and cycle */\
        INTEROP_ENUM(BaseCycleType),\
        /** Read base types are written out once for each tile and read */\
        INTEROP_ENUM(BaseReadType),\
        INTEROP_ENUM(BaseMetricCount),\
        INTEROP_ENUM(UnknownBaseType)


/** Enumeration of channel colors (assuming expected order)
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::plot_colors
 */
#define INTEROP_ENUM_PLOT_COLORS \
        /** Color for red channel (or A) */\
        INTEROP_ENUM(Red),\
        /** Color for green channel or (C) */\
        INTEROP_ENUM(Green),\
        /** Color for G */\
        INTEROP_ENUM(Blue),\
        /** Color for T */\
        INTEROP_ENUM(Black),\
        INTEROP_ENUM(UnknownColor)

/** Enumeration of bar plot options
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::bar_plot_options
 */
#define INTEROP_ENUM_BAR_PLOT_OPTIONS \
        /** Bar centered on x-value */\
        INTEROP_ENUM(Centered),\
        /** Bar shifted to start on x-value */\
        INTEROP_ENUM(Shifted),\
        INTEROP_ENUM(UnknownBarPlotOption)

#define INTEROP_UNKNOWN 0xff
/** This temp macro converts a enum/description pair into the an enum (first element of the pair) */
#define INTEROP_ENUM_DESCRIPTION(X, Y, Z) X
/** This temp macro converts an enum to an enum */
#define INTEROP_ENUM(X) X
/** This temp macro converts an enum/value pair to an enum */
#define INTEROP_ENUM_VALUE(X, V) X=V
namespace illumina {
    namespace interop {
        namespace constants {
            /** Common codes for all enums */
            enum common { Unknown = INTEROP_UNKNOWN};
            /** Codes for each metric type */
            enum metric_type {INTEROP_ENUM_METRIC_TYPES = INTEROP_UNKNOWN};
            /** Codes for each metric group */
            enum metric_group {INTEROP_ENUM_METRIC_GROUPS = INTEROP_UNKNOWN };
            /** Tile naming conventions used in various Illumina Platforms */
            enum tile_naming_method {INTEROP_ENUM_TILE_NAMING_METHODS = INTEROP_UNKNOWN};
            /** DNA base constants */
            enum dna_bases {INTEROP_ENUM_DNA_BASE_TYPES = INTEROP_UNKNOWN};
            /** Flow cell surface code */
            enum surface_type {INTEROP_ENUM_SURFACE_TYPES = INTEROP_UNKNOWN};
            /** Instrument type */
            enum instrument_type {INTEROP_ENUM_INSTRUMENT_TYPES = INTEROP_UNKNOWN};
            /** Base types for each metric */
            enum metric_base_type {INTEROP_ENUM_METRIC_BASE_TYPES = INTEROP_UNKNOWN};
            /** Color codes for plotting */
            enum plot_colors {INTEROP_ENUM_PLOT_COLORS = INTEROP_UNKNOWN};
            /** Options for a bar plot */
            enum bar_plot_options{ INTEROP_ENUM_BAR_PLOT_OPTIONS  = INTEROP_UNKNOWN};
            /** Options for a bar plot */
            enum metric_data{ INTEROP_ENUM_METRIC_DATA_TYPES  = INTEROP_UNKNOWN};
        }
    }
}

#undef INTEROP_ENUM_VALUE
#undef INTEROP_ENUM
#undef INTEROP_ENUM_DESCRIPTION

