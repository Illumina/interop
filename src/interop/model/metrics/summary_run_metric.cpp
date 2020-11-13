/** Register format layouts for summary run metrics
 *
 * Each version of the summary run metrics file has a layout defined below.
 *
 *  @file
 *  @date 7/06/2020
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <vector>
#include <algorithm>
#include "interop/model/metrics/summary_run_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/text_format.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina { namespace interop { namespace io
{
#pragma pack(1)

    /** Summary Run Metric Record Layout Version 1
     *
     * This class provides an interface to reading the summary run metric file:
     *  - InterOp/SummaryRun.bin
     *  - InterOp/SummaryRunOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: summary_run_metric
     *      2. Version: 1
     */
    template<>
    struct generic_layout<summary_run_metric, 1> : public default_layout<1>
    {
        /** @page summary_run_v1 SummaryRun Version 1
         *
         * This class provides an interface to reading the SummaryRun metric file:
         *  - InterOp/SummaryRun.bin
         *  - InterOp/SummaryRunOut.bin
         *
         *  The file format for SummaryRun metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::metric_format_stream (Class that parses this information)
         *
         *          byte 0: version number (1)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::generic_layout<summary_run_metric, 1> (Class that parses this information)
         *          2 byte: dummy (int16)
         *          4 byte: occupancy proxy cluster count (float)
         *          4 byte: raw cluster count (float)
         *          4 byte: occupancy cluster count (float)
         *          4 byte: PF cluster count (float)
         */

        /** Metric ID type */
        typedef layout::base_run_metric metric_id_t;

        /** Define a record size type */
        typedef ::uint32_t record_size_t;

        /**Count type */
        typedef summary_run_metric::count_t count_t;

        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @param header metric header layout
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header &/*header*/, const bool)
        {
            std::streamsize count = 0;
            count += stream_map< count_t >(stream, metric.m_occupancy_proxy_cluster_count);
            count += stream_map< count_t >(stream, metric.m_raw_cluster_count);
            count += stream_map< count_t >(stream, metric.m_occupied_cluster_count);
            count += stream_map< count_t >(stream, metric.m_pf_cluster_count);
            return count;
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const summary_run_metric::header_type &/*header*/)
        {
            const size_t metric_id_byte_count = sizeof(metric_id_t);
            return static_cast<record_size_t>(
                    metric_id_byte_count +
                    sizeof(count_t) +
                    sizeof(count_t) +
                    sizeof(count_t) +
                    sizeof(count_t)
            );
        }
        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const summary_run_metric::header_type&)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }
    };

#pragma pack() // DO NOT MOVE
    /** Static Run Metric CSV text format
     *
     * This class provide an interface for writing the summary run metrics to a CSV file:
     *
     *  - SummaryRun.csv
     */
    template<>
    struct text_layout< summary_run_metric, 1 >
    {
        /** Define a header type */
        typedef summary_run_metric::header_type header_type;
        /** Write header to the output stream
         *
         * @param out output stream
         * @param header metric set header
         * @param sep column separator
         * @param eol row separator
         * @return number of column headers
         */
        static size_t write_header(std::ostream& out,
                                   const header_type& /*header*/,
                                   const std::vector<std::string>&,
                                   const char sep,
                                   const char eol)
        {
            const char* column_headers[] =
            {
                "Raw Cluster Count", "Occupied Cluster Count", "PF Cluster Count", "Occupancy Proxy Cluster Count"
            };
            std::vector<std::string> headers;
            headers.reserve(util::length_of(column_headers));
            for(size_t i=0;i<util::length_of(column_headers);++i)
                headers.push_back(column_headers[i]);
            
            out << "# Column Count: " << util::length_of(headers) << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a summary run metric to the output stream
         *
         * @param out output stream
         * @param metric summary run metric
         * @param header metric set header
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const summary_run_metric& metric,
                                   const header_type& /*header*/,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            out << metric.raw_cluster_count() << sep
                << metric.occupied_cluster_count() << sep
                << metric.pf_cluster_count() << sep
                << metric.occupancy_proxy_cluster_count();
            out << eol;
            return 0;
        }
    };

}}}

INTEROP_FORCE_LINK_DEF(summary_run_metric)

INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(summary_run_metric, 1)

// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(summary_run_metric, 1)
