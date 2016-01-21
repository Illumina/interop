/** Register format layouts for index metrics
 *
 * Each version of the index metrics file has a layout defined below.
 *
 *  @file
 *
 *  @date 11/2/15.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <string>
#include "interop/model/metrics/index_metric.h"
#include "interop/io/format/metric_format_factory.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io {
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
struct generic_layout<index_metric, 1> : public default_layout<1>
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
     *          4 bytes: index count (uint32)
     *          2 bytes: sample name length (sampleNameLength) (uint16)
     *          sampleNameLength bytes: sample name
     *          2 bytes: project name length (projectNameLength) (uint16)
     *          projectNameLength bytes: project name
     */
    /** Metric ID type */
    typedef layout::base_read_metric metric_id_t;
    /** Defines an empty header */
    typedef void* header_t;
    /** No record size is required for this stream */
    typedef no_value record_size_t;

    /** Read metric from the input stream
     *
     * @param in input stream
     * @param metric destination metric
     * @return number of bytes read or total number of bytes written
     */
    template<class Metric, class Header>
    static std::streamsize map_stream(std::istream& in, Metric& metric, Header&, const bool)
    {
        std::string index_name;
        ::uint32_t count;
        std::string sample_name;
        std::string project_name;

        read_binary(in, index_name);
        if(in.fail()) throw incomplete_file_exception("No more data after index name");
        read_binary(in, count);
        if(in.fail()) throw incomplete_file_exception("No more data after count");
        read_binary(in, sample_name);
        if(in.fail()) throw incomplete_file_exception("No more data after sample name");
        read_binary(in, project_name);
        index_metric::index_array_t::iterator beg = metric.m_indices.begin(), end = metric.m_indices.end();
        for(;beg != end;++beg) if(beg->index_seq() == sample_name) break;
        if( beg == end )
        {
            metric.m_indices.push_back(index_info(index_name, sample_name, project_name, count));
        }
        else beg->m_count += count;

        return 1;
    }
    /** Write metric to the output stream
     *
     * @param out output stream
     * @param metric source metric
     * @return number of bytes read or total number of bytes written
     */
    template<class Metric, class Header>
    static std::streamsize map_stream(std::ostream& out, Metric& metric, Header&, const bool)
    {
        metric_id_t metric_id;
        metric_id.set(metric);
        for(index_metric::index_array_t::const_iterator beg = metric.indices().begin(),
                    end = metric.indices().end();beg != end;++beg)
        {
            if(beg != metric.indices().begin()) write_binary(out, metric_id);
            write_binary(out, beg->index_seq());
            write_binary(out, static_cast< ::uint32_t >(beg->count()));
            write_binary(out, beg->sample_id());
            write_binary(out, beg->sample_proj());
        }
        return 1;
    }
    /** Compute the layout size
     *
     * @note The size of the record is not known ahead of time, so we just return 1.
     *
     * @return 1
     */
    static record_size_t computeSize(const index_metric::header_type&)
    {
        return record_size_t();
    }
};

#pragma pack()
}}}

INTEROP_FORCE_LINK_DEF(index_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(index_metric, 1 )
