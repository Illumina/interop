/** Register format layouts for error metrics
 *
 * Each version of the error metrics file has a layout defined below.
 *
 *  @file
 *  @date 8/19/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/model/metrics/error_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"
#include "interop/io/format/text_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io
{
#pragma pack(1)

    /** Error Metric Record Layout Version 3
     *
     * This class provides an interface to reading the error metric file:
     *  - InterOp/ErrorMetrics.bin
     *  - InterOp/ErrorMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: error_metric
     *      2. Version: 3
     */
    template<>
    struct generic_layout<error_metric, 3> : public default_layout<3>
    {
        /** @page error_v3 Error Version 3
         *
         * This class provides an interface to reading the error metric file:
         *  - InterOp/ErrorMetrics.bin
         *  - InterOp/ErrorMetricsOut.bin
         *
         *  The file format for error metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (3)
         *          byte 1: record size (30)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<error_metric, 3> (Class that parses this information)
         *
         *          4 bytes: error rate (float32)
         *          4 bytes: number of perfect reads (uint32)
         *          4 bytes: number of reads with 1 error (uint32)
         *          4 bytes: number of reads with 2 error (uint32)
         *          4 bytes: number of reads with 3 error (uint32)
         *          4 bytes: number of reads with 4 error (uint32)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Error type */
        typedef float error_t;
        /** Error type */
        typedef ::uint32_t count_t;
        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream& stream, Metric& metric, Header&, const bool)
        {
            std::streamsize count = 0;
            count += stream_map< error_t >(stream, metric.m_error_rate);
            count += stream_map< count_t >(stream, metric.m_mismatch_cluster_count, error_metric::MAX_MISMATCH);
            return count;
        }
        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const error_metric::header_type&)
        {
            return static_cast<record_size_t>(sizeof(metric_id_t)+
                    sizeof(error_t)+                                  // m_error_rate
                    sizeof(count_t)*error_metric::MAX_MISMATCH   // m_mismatch_cluster_count
            );
        }
        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const error_metric::header_type&)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }

    };


#pragma pack()
    /** Error Metric CSV text format
     *
     * This class provide an interface for writing the error metrics to a CSV file:
     *
     *  - ErrorMetrics.csv
     */
    template<>
    struct text_layout< error_metric, 1 >
    {
        /** Define a header type */
        typedef error_metric::header_type header_type;
        /** Write header to the output stream
         *
         * @param out output stream
         * @param sep column separator
         * @param eol row separator
         * @return number of column headers
         */
        static size_t write_header(std::ostream& out,
                                   const header_type&,
                                   const std::vector<std::string>&,
                                   const char sep,
                                   const char eol)
        {
            const char* headers[] =
            {
                "Lane", "Tile", "Cycle", "ErrorRate"
            };
            out << "# Column Count: " << util::length_of(headers) << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a error metric to the output stream
         *
         * @param out output stream
         * @param metric error metric
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const error_metric& metric,
                                   const header_type&,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            out << metric.lane() << sep << metric.tile() << sep << metric.cycle() << sep;
            out << metric.error_rate() << eol;
            return 0;
        }
    };
}}}

INTEROP_FORCE_LINK_DEF(error_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(error_metric, 3 )


// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(error_metric, 1)
