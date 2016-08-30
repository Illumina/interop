/** Concrete metric layout formats for the abstract metric format interface.
 *
 *
 *  @file
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/util/exception.h"
#include "interop/io/format/abstract_metric_format.h"
#include "interop/io/format/generic_layout.h"
#include "interop/io/format/stream_util.h"

namespace illumina { namespace interop { namespace io
{
    /** Shared functionality for reading and writing binary InterOp metrics
     *
     * This format assumes that the binary file as the following format:
     *   1. First byte is the version number
     *   2. Second byte is the record size
     *   3. Optional header
     *   4. A series of records (as defined by Layout)
     */
    template<class Metric, class Layout>
    struct metric_format : public abstract_metric_format<Metric>
    {
        /** Define the metric type */
        typedef Metric metric_t;
        /** Define the metric header type */
        typedef typename Metric::header_type header_t;
        /** Define a layout ID type */
        typedef typename Layout::metric_id_t metric_id_t;
        /** Define the record size type */
        typedef typename Layout::record_size_t record_size_t;

        /** Write the header of a metric InterOp to the given output stream
         *
         * @param out output stream to write the binary InterOp file data
         * @param header header of a metric set
         */
        void write_metric_header(std::ostream &out, const header_t &header)
        {
            const ::uint8_t version = static_cast< ::uint8_t >(Layout::VERSION);
            write_binary(out, version);
            Layout::map_stream_record_size(out, Layout::compute_size(header));
            Layout::map_stream_for_header(out, header);
        }

        /** Write a metric record to the given output stream
         *
         * @param out output stream
         * @param metric a metric to write
         * @param header metric set header
         */
        void write_metric(std::ostream &out, const metric_t &metric, const header_t &header)
        {
            metric_id_t metric_id;
            metric_id.set(metric);
            write_binary(out, metric_id);
            Layout::map_stream(out, metric, header, false);
        }

        /** Read a metric set from the given input stream
         *
         * @param in input stream containing binary InterOp file data
         * @param header metric set header
         * @return number of bytes in the record
         */
        std::streamsize read_header(std::istream &in, header_t &header)
        {
            if (in.fail())
                INTEROP_THROW(incomplete_file_exception, "Insufficient header data read from the file");

            //if we're not actually reading the record size from the stream (the stream position is the same before and after),
            // then don't compare the layout size against the record size
            const ::int64_t stream_position_pre_record_check = in.tellg();
            const std::streamsize record_size = Layout::map_stream_record_size(in,
                                                                               static_cast<record_size_t>(0));
            const ::int64_t stream_position_post_record_check = in.tellg();
            Layout::map_stream_for_header(in, header);

            if (in.fail())
                INTEROP_THROW(incomplete_file_exception, "Insufficient extended header data read from the file");
            const std::streamsize layout_size = Layout::compute_size(header);
            if (stream_position_pre_record_check != stream_position_post_record_check && record_size != layout_size)
                INTEROP_THROW(bad_format_exception, "Record size does not match layout size, record size: " <<
                                           record_size << " != layout size: " <<
                                           layout_size << " for "  <<
                                           Metric::prefix() <<  " "  << Metric::suffix()  <<  " v"  <<
                                           Layout::VERSION);
            return layout_size;
        }

        /** Read a metric set from the given input stream
         *
         * @param in input stream containing binary InterOp file data
         * @param metric metric
         * @return number of bytes in the record
         */
        std::streamsize read_metric_id(std::istream &in, metric_t &metric)
        {
            metric_id_t id;
            read_binary(in, id);
            metric.set_base(id);
            return in.gcount();
        }

        /** Calculate the size of a record
         *
         * @param header metric set header
         * @return size of record in bytes
         */
        size_t record_size(const header_t &header) const
        {
            return static_cast<size_t>(Layout::compute_size(header));
        }

        /** Calculate the size of a record
         *
         * @param header metric set header
         * @return size of header in bytes
         */
        size_t header_size(const header_t &header) const
        {
            return static_cast<size_t>(Layout::compute_header_size(header));
        }

        /** Read a metric set from the given input stream
         *
         * @param in input stream containing binary InterOp file data
         * @param metric metric
         * @param header metric header
         * @param is_new the metric does not already exist in the set
         * @return number of bytes in the record
         */
        std::streamsize read_metric(std::istream &in, metric_t &metric, const header_t &header, const bool is_new)
        {
            return Layout::map_stream(in, metric, header, is_new);
        }

        /** Get the version of this metric format
         *
         * @return version number
         */
        ::int16_t version() const
        {
            return static_cast< ::int16_t >(Layout::VERSION);
        }
    };
}}}

