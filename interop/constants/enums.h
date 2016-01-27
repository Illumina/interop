/** Enumeration constants used throughout InterOp
 *
 *  @file
 *  @date 9/27/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include "interop/util/cstdint.h"

namespace illumina {
    namespace interop {
        namespace constants {
            /** Codes for each metric
             */
            enum metric_group
            {
                CorrectedInt,
                Error,
                Extraction,
                Image,
                Index,
                Q,
                Tile,
                MetricCount
            };
            /** Tile naming conventions used in various Illumina Platforms
             */
            enum tile_naming_method
            {
                /** Unknown tile naming method */
                unknown,
                /** Four digit tiles for HiSeq and modern MiSeqs */
                four_digit,
                /** Five digit tiles for NextSeq and ... */
                five_digit,
                /** Absolute naming for tiles GAs and old MiSeqs*/
                absolute,
                /** Number of naming methods */
                naming_method_count
            };
            /** Get enum code from a string
             *
             * @param name naming method
             * @return enum code
             */
            inline tile_naming_method tile_naming_from_string(const std::string& name)
            {
                const char* names[] = {"Unknown", "FourDigit", "FiveDigit", "Absolute"};// TODO: make static?
                for(int32_t i=0;i<naming_method_count;i++)
                    if( name == names[i]) return static_cast<tile_naming_method>(i);
                return unknown;
            }
            /** DNA base constants
             */
            enum dna_base
            {
                /** No calls */
                NC=-1,
                /** DNA base adenine */
                A,
                /** DNA base cytosine */
                C,
                /** DNA base guanine */
                G,
                /** DNA base thymine */
                T,
                /** Number of bases */
                NUM_OF_BASES,
                /** Number of bases including no calls*/
                NUM_OF_BASES_AND_NC
            };
            /** Flow cell surface code */
            enum flowcell_surface // TODO: what else to say here?
            {
                Top=1,
                Bottom=2
            };
            /** Instrument type */
            enum instrument_type // TODO: More info on each type and add new instruments
            {
                UnknownInstrument,
                HiSeq,
                HiScan,
                MiSeq,
                NextSeq
                // ...
            };
            /** Instrument type to string
             *
             * @param type insturment type
             * @return string representation
             */
            inline std::string instrument_type_to_string(instrument_type type)
            {
                const char* names[] = {"UnknownInstrument", "HiSeq", "HiScan", "MiSeq", "NextSeq"}; // TODO: make static?
                if(type < UnknownInstrument) return names[0];
                if(type > NextSeq) return names[0];
                return names[type];
            }
        }
    }
}

