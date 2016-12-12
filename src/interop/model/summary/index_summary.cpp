/** Register format layout for index_flowcell_summary
 *
 * Each version of the image metrics file has a layout defined below.
 *
 *  @file
 *  @date 6/24/16.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <vector>
#include <algorithm>
#include "interop/model/summary/index_flowcell_summary.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/default_layout.h"

using namespace illumina::interop::model::summary;
using namespace illumina::interop;

namespace illumina{ namespace interop{ namespace io {

    /** Index summary Record Layout Version 1
     *
     * This class provides an interface to reading the image metric file:
     *  - InterOp/IndexSummary.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: index_flowcell_summary
     *      2. Version: 1
     */
    template<>
    struct generic_layout<index_flowcell_summary, 1> : public default_layout<1>
    {
      /** @page index_summary_v1 Index Summary v1
       *
       * This class provides an interface to reading/writing a binary index flowcell summary, and is only supported
       * for regression testing.
       */

        /** Map reading/writing a summary to a stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param summary source/destination summary
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Summary>
        static std::streamsize map_records(Stream& stream, Summary& summary)
        {
            std::streamsize count = 0;
            count += map_lane_summary(stream, summary.begin(), summary.end());
            return count;
        }

        /** Map reading/writing a summary to a stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param header source/destination header
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Summary>
        static std::streamsize map_header(Stream& stream, Summary& header)
        {
            std::streamsize count = 0;
            size_t lane_count = header.size();
            count += stream_map< size_t >(stream, lane_count);
            map_resize(header, lane_count);
            return count;
        }
    private:
        template<class Stream, class LaneSummaryIterator>
        static std::streamsize map_lane_summary(Stream& stream,
                                                LaneSummaryIterator lane_beg,
                                                LaneSummaryIterator lane_end)
        {
            typedef index_lane_summary::read_count_t read_count_t;
            std::streamsize count = 0;
            for(;lane_beg != lane_end;++lane_beg)
            {
                count += stream_map< read_count_t >(stream, lane_beg->m_total_reads);
                count += stream_map< read_count_t >(stream, lane_beg->m_total_pf_reads);
                count += stream_map< float >(stream, lane_beg->m_total_fraction_mapped_reads);
                count += stream_map< float >(stream, lane_beg->m_mapped_reads_cv);
                count += stream_map< float >(stream, lane_beg->m_min_mapped_reads);
                count += stream_map< float >(stream, lane_beg->m_max_mapped_reads);

                size_t index_count = lane_beg->size();
                count += stream_map< size_t >(stream, index_count);
                map_resize(*lane_beg, index_count);
                map_count_summary(stream, lane_beg->begin(), lane_beg->end());
            }
            return count;
        }
        template<class Stream, class IndexCountIterator>
        static std::streamsize map_count_summary(Stream& stream,
                                                IndexCountIterator count_beg,
                                                IndexCountIterator count_end)
        {
            std::streamsize count = 0;
            for(;count_beg != count_end;++count_beg)
            {
                count += stream_map< size_t >(stream, count_beg->m_id);
                count += stream_map(stream, count_beg->m_index1);
                count += stream_map(stream, count_beg->m_index2);
                count += stream_map< float >(stream, count_beg->m_fraction_mapped);
                count += stream_map< ::uint64_t >(stream, count_beg->m_cluster_count);
                count += stream_map(stream, count_beg->m_sample_id);
                count += stream_map(stream, count_beg->m_project_name);
            }
            return count;
        }
    };

}}}
namespace illumina { namespace interop { namespace model { namespace summary
{
    std::ostream &operator<<(std::ostream &out, const index_flowcell_summary &summary)
    {
        typedef io::generic_layout<index_flowcell_summary, 1> layout_t;
        typedef layout_t::version_t version_t;
        const version_t version = static_cast<version_t>(layout_t::VERSION);
        io::write_binary(out, version);
        if (!out.good()) INTEROP_THROW(io::bad_format_exception, "Cannot write to disk");
        layout_t::map_header(out, summary);
        if (!out.good()) INTEROP_THROW(io::bad_format_exception, "Cannot write to disk");
        layout_t::map_records(out, summary);
        if (!out.good()) INTEROP_THROW(io::bad_format_exception, "Cannot write to disk");
        return out;
    }

    std::istream &operator>>(std::istream &in, index_flowcell_summary &summary)
    {
        typedef io::generic_layout<index_flowcell_summary, 1> layout_t;


        if (!in.good()) INTEROP_THROW(io::incomplete_file_exception, "Empty file found");
        const int version = in.get();
        if (version == -1) INTEROP_THROW(io::incomplete_file_exception, "Empty file found");
        if (version != layout_t::VERSION) INTEROP_THROW(io::bad_format_exception, "Version mismatch");

        layout_t::map_header(in, summary);
        layout_t::map_records(in, summary);
        return in;
    }
}}}}

