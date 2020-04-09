/** Register format layouts for phasing metrics
*
* Each version of the error metrics file has a layout defined below.
*
*  @file
*  @date 10/22/15
*  @version 1.0
*  @copyright GNU Public License.
*/

#include "interop/model/metrics/phasing_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/text_format.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina { namespace interop { namespace io
{
#pragma pack(1)

    /** Phasing Metric Record Layout Version 1
     *
     * This class provides an interface to reading the phasing metric file:
     *  - InterOp/EmpiricalPhasingMetrics.bin
     *  - InterOp/EmpiricalPhasingMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: phasing_metric
     *      2. Version: 1
     */
    template<>
    struct generic_layout<phasing_metric, 1> : public default_layout<1>
    {
        /** @page phasing_v1 Phasing Version 1
         *
         * This class provides an interface to reading the phasing metric file:
         *  - InterOp/EmpiricalPhasingMetrics.bin
         *  - InterOp/EmpiricalPhasingMetricsOut.bin
         *
         *  The file format for phasing metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::metric_format_stream (Class that parses this information)
         *
         *          byte 0: version number (1)
         *          byte 1: record size (14)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<phasing_metric, 1> (Class that parses this information)
         *
         *          4 bytes: phasing weight (float32)
         *          4 bytes: prephasing weight (float32)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t> metric_id_t;
        /** Weight type */
        typedef float weight_t;

        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header &, const bool)
        {
            std::streamsize count = 0;
            count += stream_map<weight_t>(stream, metric.m_phasing_weight);
            count += stream_map<weight_t>(stream, metric.m_prephasing_weight);
            return count;
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const phasing_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(metric_id_t) + sizeof(weight_t) * 2);
        }

        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const phasing_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }
    };

    /** Phasing Metric Record Layout Version 2
     *
     * This class provides an interface to reading the phasing metric file:
     *  - InterOp/EmpiricalPhasingMetrics.bin
     *  - InterOp/EmpiricalPhasingMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: phasing_metric
     *      2. Version: 2
     */
    template<>
    struct generic_layout<phasing_metric, 2> : public default_layout<2>
    {
        /** @page phasing_v2 Phasing Version 2
         *
         * This class provides an interface to reading the phasing metric file:
         *  - InterOp/EmpiricalPhasingMetrics.bin
         *  - InterOp/EmpiricalPhasingMetricsOut.bin
         *
         *  The file format for phasing metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::metric_format_stream (Class that parses this information)
         *
         *          byte 0: version number (2)
         *          byte 1: record size (16)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          4 bytes: tile number (uint32)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<phasing_metric, 2> (Class that parses this information)
         *
         *          4 bytes: phasing weight (float32)
         *          4 bytes: prephasing weight (float32)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint32_t > metric_id_t;
        /** Weight type */
        typedef float weight_t;

        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header &, const bool)
        {
            std::streamsize count = 0;
            count += stream_map<weight_t>(stream, metric.m_phasing_weight);
            count += stream_map<weight_t>(stream, metric.m_prephasing_weight);
            return count;
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const phasing_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(metric_id_t) + sizeof(weight_t) * 2);
        }

        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const phasing_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }
    };

#pragma pack()
    /** Phasing Metric CSV text format
     *
     * This class provide an interface for writing the phasing metrics to a CSV file:
     *
     *  - PhasingMetrics.csv
     */
    template<>
    struct text_layout< phasing_metric, 1 >
    {
        /** Define a header type */
        typedef phasing_metric::header_type header_type;
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
                "Lane", "Tile", "Cycle", "Phasing", "Prephasing"
            };
            out << "# Column Count: " << util::length_of(headers) << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a phasing metric to the output stream
         *
         * @param out output stream
         * @param metric phasing metric
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const phasing_metric& metric,
                                   const header_type&,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            out << metric.lane() << sep << metric.tile() << sep << metric.cycle() << sep
                << metric.phasing_weight() << sep << metric.prephasing_weight();
            out << eol;
            return 0;
        }
    };
}}}

INTEROP_FORCE_LINK_DEF(phasing_metric)

INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(phasing_metric, 1)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(phasing_metric, 2)

// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(phasing_metric, 1)

