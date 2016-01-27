/** Generic layout defined for a specific metric and version
 *
 * This file defines a generic layout for all metric formats.  This class will be specialized for a specific InterOp
 * interface class.
 *
 *  @file
 *
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <ios>
#include "interop/io/format/map_io.h"

namespace illumina {
    namespace interop {
        namespace io {
            /** A POD-struct that defines the layout of a binary InterOp record
             *
             * The MetricType is the class that holds the actual record data.
             * The Version is the version of the format.
             */
            template<class MetricType, int Version>
            struct generic_layout;

            /** Define default methods and typedefs for a record layout
             *
             */
            template<int V>
            struct default_layout
            {
                /** Version of the format */
                enum
                {
                    VERSION = V
                };
                /** Define a record size type */
                typedef ::uint8_t record_size_t;

                /** Map reading/writing a header to a stream
                 *
                 * Does nothing
                 *
                 * @return 0
                 */
                template<class Stream, class Header>
                static std::streamsize map_stream_for_header(Stream &, Header &)
                {
                    return 0;
                }
            };
        }
    }
}



