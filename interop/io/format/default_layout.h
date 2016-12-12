/** Default layout for shared functions
 *
 *  @file
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <ios>
#include "interop/io/format/map_io.h"

namespace illumina { namespace interop { namespace io
{
    /** Define default methods and typedefs for a record layout
     */
    template<int V, int MultiRecord=0>
    struct default_layout
    {
        enum
        {
            /** Version of the format */
            VERSION = V,
            /** Flag indicating whether metric is split into multiple records in the InterOp file */
            MULTI_RECORD=MultiRecord
        };
        /** Define a record size type */
        typedef ::uint8_t record_size_t;
        /** Define a version type */
        typedef ::uint8_t version_t;

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

        /** Map reading/writing the record size to the stream
         *
         * @param stream input/output stream
         * @param record_size size of the record
         * @return size of the record
         */
        template<class Stream, class RecordSize>
        static RecordSize map_stream_record_size(Stream &stream, RecordSize record_size)
        {
            stream_map<RecordSize>(stream, record_size);
            return record_size;
        }
        /** Skip inserting this metric into the metric set
         *
         * This function was originally added to skip control records in tile metrics.
         *
         * @param metric metric to check
         * @return true, if the metric id is 0
         */
        template<class Metric>
        static bool skip_metric(const Metric& metric)
        {
            return metric.id() == 0;
        }
        /** Skip inserting this metric into the metric set
         *
         * This function was originally added to skip control records in tile metrics.
         *
         * @param id metric to check
         * @return true, if the metric id is 0
         */
        template<class LayoutId>
        static bool is_valid(const LayoutId& id)
        {
            return id.is_valid();
        }
    };
}}}


