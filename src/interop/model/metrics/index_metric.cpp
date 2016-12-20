/** Register format layouts for index metrics
 *
 * Each version of the index metrics file has a layout defined below.
 *
 *  @file
 *  @date 11/2/15.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <string>
#include "interop/model/metrics/index_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"
#include "interop/io/format/text_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina { namespace interop { namespace io
{
#pragma pack(1)

    /** Index Metric Record Layout Version 1
     *
     * This class provides an interface to reading the index metric file:
     *  - InterOp/IndexMetrics.bin
     *  - InterOp/IndexMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: index_metric
     *      2. Version: 1
     */
    template<>
    struct generic_layout<index_metric, 1> : public default_layout<1, 1 /*Multi record */>
    {
        /** @page index_v1 Index Version 1
         *
         * This class provides an interface to reading the index metric file:
         *  - InterOp/IndexMetrics.bin
         *  - InterOp/IndexMetricsOut.bin
         *
         *  The file format for index metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_read_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: read number (uint16)
         *
         *  illumina::interop::io::generic_layout<index_metric, 1> (Class that parses this information)
         *
         *          2 bytes: index name length (indexNameLength) (uint16)
         *          indexNameLength bytes: index name
         *          4 bytes: index cluster count (uint32)
         *          2 bytes: sample name length (sampleNameLength) (uint16)
         *          sampleNameLength bytes: sample name
         *          2 bytes: project name length (projectNameLength) (uint16)
         *          projectNameLength bytes: project name
         */
        /** Metric ID type */
        typedef layout::base_read_metric< ::uint16_t > metric_id_t;
        /** Defines an empty header */
        typedef void *header_t;
        /** No record size is required for this stream */
        //typedef fixed_record_size<sizeof(metric_id_t)> record_size_t;
        typedef ::uint32_t cluster_count_t;
        enum
        {
            BYTE_COUNT = 1, RECORD_SIZE = sizeof(metric_id_t) + BYTE_COUNT
        };

        /** Read metric from the input stream
         *
         * @param in input stream
         * @param metric destination metric
         * @return sentinel
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(std::istream &in, Metric &metric, Header &, const bool)
        {
            std::string index_name;
            cluster_count_t count;
            std::string sample_name;
            std::string project_name;

            read_binary(in, index_name, "NA");
            if (in.fail())
                INTEROP_THROW(incomplete_file_exception, "No more data after index name");
            read_binary(in, count);
            if (in.fail())
                INTEROP_THROW(incomplete_file_exception, "No more data after count");
            read_binary(in, sample_name, "NA");
            if (in.fail())
                INTEROP_THROW(incomplete_file_exception, "No more data after sample name");
            read_binary(in, project_name, "NA");
            index_metric::index_array_t::iterator beg = metric.m_indices.begin(), end = metric.m_indices.end();
            for (; beg != end; ++beg) if (beg->index_seq() == sample_name) break;
            if (beg == end)
            {
                metric.m_indices.push_back(index_info(index_name, sample_name, project_name, count));
            }
            else beg->m_cluster_count += count;

            return BYTE_COUNT;
        }
        template<class Metric, class Header>
        static std::streamsize map_stream(const char*, Metric&, Header&, const bool)
        {
            INTEROP_THROW(std::runtime_error, "This function should not be called");
        }

        /** Write metric to the output stream
         *
         * @param out output stream
         * @param metric source metric
         * @return sentinel
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(std::ostream &out, Metric &metric, Header &, const bool)
        {
            metric_id_t metric_id;
            metric_id.set(metric);
            for (index_metric::index_array_t::const_iterator beg = metric.indices().begin(),
                         end = metric.indices().end(); beg != end; ++beg)
            {
                if (beg != metric.indices().begin()) write_binary(out, metric_id);
                write_binary(out, beg->index_seq());
                write_binary(out, static_cast< cluster_count_t >(beg->cluster_count()));
                write_binary(out, beg->sample_id());
                write_binary(out, beg->sample_proj());
            }
            return BYTE_COUNT;
        }

        /** Compute the layout size
         *
         * @note The size of the record is not known ahead of time, so we just return 1.
         *
         * @return sentinel
         */
        static record_size_t compute_size(const index_metric::header_type &)
        {
            return static_cast<record_size_t>(RECORD_SIZE);
        }

        /** Compute header size
         *
         * @return header size
         */
        static size_t compute_header_size(const index_metric::header_type &)
        {
            return sizeof(version_t);
        }

        /** Skip reading/writing record size to this file
         *
         * @return sentinel
         */
        template<class Stream>
        static record_size_t map_stream_record_size(Stream &, record_size_t)
        {
            return static_cast<record_size_t>(RECORD_SIZE);
        }
    };

#pragma pack()
    /** Tile Metric CSV text format
     *
     * This class provide an interface for writing the tile metrics to a CSV file:
     *
     *  - TileMetrics.csv
     */
    template<>
    struct text_layout< index_metric, 1 >
    {
        /** Define a header type */
        typedef index_metric::header_type header_type;
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
                "Lane", "Tile", "Read", "Sequence", "Sample", "Project", "ClusterCount"
            };
            out << "# Column Count: " << util::length_of(headers) << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a tile metric to the output stream
         *
         * @param out output stream
         * @param metric tile metric
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const index_metric& metric,
                                   const header_type&,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            typedef index_metric::index_array_t::const_iterator const_iterator;
            for(const_iterator index_beg=metric.indices().begin();index_beg != metric.indices().end();++index_beg)
            {
                out << metric.lane() << sep << metric.tile() << sep << metric.read() << sep;
                out << index_beg->index_seq() << sep << index_beg->sample_id();
                out << sep << index_beg->sample_proj() << sep << index_beg->cluster_count() << eol;
            }
            return 0;
        }
    };
}}}

INTEROP_FORCE_LINK_DEF(index_metric)

INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(index_metric, 1)


// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(index_metric, 1)

