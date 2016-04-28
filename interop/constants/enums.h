/** Enumeration constants used throughout InterOp
 *
 *  @file
 *  @date 9/27/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include "interop/util/cstdint.h"

/** Enumeration of each metric type
 *
 * @note This macro requires the macro INTEROP_ENUM_DESCRIPTION to be defined before use
 * @see illumina::interop::constants::metric_type
 */
#define INTEROP_ENUM_METRIC_TYPES \
        INTEROP_ENUM_DESCRIPTION(Intensity, "Intensity"),\
        INTEROP_ENUM_DESCRIPTION(FWHM, "FWHM"),\
        INTEROP_ENUM_DESCRIPTION(PercentBase, "% Base"),\
        INTEROP_ENUM_DESCRIPTION(PercentQ20, "% >=Q20"),\
        INTEROP_ENUM_DESCRIPTION(PercentQ30, "% >=Q30"),\
        INTEROP_ENUM_DESCRIPTION(QScore, "Median QScore"),\
        INTEROP_ENUM_DESCRIPTION(Density, "Density"),\
        INTEROP_ENUM_DESCRIPTION(DensityPF, "Density PF"),\
        INTEROP_ENUM_DESCRIPTION(ClusterCount, "Clusters"),\
        INTEROP_ENUM_DESCRIPTION(ClusterCountPF, "Clusters PF"),\
        INTEROP_ENUM_DESCRIPTION(AlignedReadCount, "Aligned Read Count"),\
        INTEROP_ENUM_DESCRIPTION(ErrorRate, "Error Rate"),\
        INTEROP_ENUM_DESCRIPTION(PercentPhasing, "% Phasing"),\
        INTEROP_ENUM_DESCRIPTION(PercentPrephasing, "% Prephasing"),\
        INTEROP_ENUM_DESCRIPTION(PercentAligned, "% Aligned"),\
        INTEROP_ENUM_DESCRIPTION(CorrectedIntensity, "Corrected Int"),\
        INTEROP_ENUM_DESCRIPTION(CalledIntensity, "Called Int"),\
        INTEROP_ENUM_DESCRIPTION(SignalToNoise, "Signal To Noise"),\
        INTEROP_ENUM_DESCRIPTION(MetricTypeCount, "!!!!BAD!!!!"),\
        INTEROP_ENUM_DESCRIPTION(UnknownMetricType, "!!!!BAD!!!!")

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
        INTEROP_ENUM(MetricCount),\
        INTEROP_ENUM(UnknownMetricGroup)

/** Enumeration of tile naming methods
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::tile_naming_method
 */
#define INTEROP_ENUM_TILE_NAMING_METHODS \
        INTEROP_ENUM(UnknownTileNamingMethod),\
        /** Four digit tiles for HiSeq and modern MiSeqs */\
        INTEROP_ENUM(FourDigit),\
        /** Five digit tiles for NextSeq and ... */\
        INTEROP_ENUM(FiveDigit),\
        /** Absolute naming for tiles GAs and old MiSeqs*/\
        INTEROP_ENUM(Absolute),\
        /** Number of naming methods */\
        INTEROP_ENUM(TileNamingMethodCount)

/** Enumeration of DNA bases
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @note This macro requires the macro INTEROP_ENUM_VALUE to be defined before use
 * @see illumina::interop::constants::dna_base
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
        INTEROP_ENUM(UnknownSurface),\
        INTEROP_ENUM(Top),\
        INTEROP_ENUM(Bottom)


/** Enumeration of instrument types
 *
 * @note This macro requires the macro INTEROP_ENUM to be defined before use
 * @see illumina::interop::constants::instrument_type
 */
#define INTEROP_ENUM_INSTRUMENT_TYPES \
        INTEROP_ENUM(UnknownInstrument),\
        INTEROP_ENUM(HiSeq),\
        INTEROP_ENUM(HiScan),\
        INTEROP_ENUM(MiSeq),\
        INTEROP_ENUM(NextSeq),\
        INTEROP_ENUM(MiniSeq)

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

/** This temp macro converts a enum/description pair into the an enum (first element of the pair) */
#define INTEROP_ENUM_DESCRIPTION(X, Y) X
/** This temp macro converts an enum to an enum */
#define INTEROP_ENUM(X) X
/** This temp macro converts an enum/value pair to an enum */
#define INTEROP_ENUM_VALUE(X, V) X=V
namespace illumina {
    namespace interop {
        namespace constants {

            /** Codes for each metric type */
            enum metric_type {INTEROP_ENUM_METRIC_TYPES};
            /** Codes for each metric group */
            enum metric_group {INTEROP_ENUM_METRIC_GROUPS };
            /** Tile naming conventions used in various Illumina Platforms */
            enum tile_naming_method {INTEROP_ENUM_TILE_NAMING_METHODS};
            /** DNA base constants */
            enum dna_base {INTEROP_ENUM_DNA_BASE_TYPES};
            /** Flow cell surface code */
            enum surface_type {INTEROP_ENUM_SURFACE_TYPES};
            /** Instrument type */
            enum instrument_type {INTEROP_ENUM_INSTRUMENT_TYPES};
            /** Base types for each metric */
            enum metric_base_type {INTEROP_ENUM_METRIC_BASE_TYPES};
        }
    }
}

#undef INTEROP_ENUM_VALUE
#undef INTEROP_ENUM
#undef INTEROP_ENUM_DESCRIPTION

