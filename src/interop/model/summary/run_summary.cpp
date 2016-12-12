/** Register format layout for run_summary
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
#include "interop/model/summary/run_summary.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/default_layout.h"

using namespace illumina::interop::model::summary;
using namespace illumina::interop;

namespace illumina{ namespace interop{ namespace io {
    /** Run summary Record Layout Version 1
     *
     * This class provides an interface to reading the image metric file:
     *  - InterOp/RunSummary.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: run_summary
     *      2. Version: 1
     */
    template<>
    struct generic_layout<run_summary, 1> : public default_layout<1>
    {
      /** @page summary_v1 Run Summary v1
       *
       * This class provides an interface to reading/writing a binary summary, and is only supported
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

            count += map_cycle_state(stream, summary.cycle_state());
            count += map_metric_summary(stream, summary.nonindex_summary(), summary.channel_count());
            count += map_metric_summary(stream, summary.total_summary(), summary.channel_count());
            count += map_read_summary(stream, summary.begin(), summary.end(), summary.channel_count());
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
        template<class Stream, class Header>
        static std::streamsize map_header(Stream& stream, Header& header)
        {
            std::streamsize count = 0;
            count += stream_map< size_t >(stream, header.m_surface_count);
            count += stream_map< size_t >(stream, header.m_read_count);
            count += stream_map< size_t >(stream, header.m_lane_count);
            count += stream_map< ::uint8_t  >(stream, header.m_channel_count);
            return count;
        }
    private:
        template<class Stream, class ReadSummaryIterator>
        static std::streamsize map_read_summary(Stream& stream,
                                                ReadSummaryIterator read_beg,
                                                ReadSummaryIterator read_end,
                                                const size_t channel_count)
        {
            std::streamsize count = 0;
            for(;read_beg != read_end;++read_beg)
            {
                count += map_read(stream, read_beg->m_read);
                count += map_metric_summary(stream, read_beg->m_metric_summary, channel_count);
                count += map_lane_summary(stream, read_beg->begin(), read_beg->end(), channel_count);
            }
            return count;
        }
        template<class Stream, class LaneSummaryIterator>
        static std::streamsize map_lane_summary(Stream& stream,
                                                LaneSummaryIterator lane_beg,
                                                LaneSummaryIterator lane_end,
                                                const size_t channel_count)
        {
            std::streamsize count = 0;
            for(;lane_beg != lane_end;++lane_beg)
            {
                count += stream_map< size_t >(stream, lane_beg->m_lane);
                count += stream_map< size_t >(stream, lane_beg->m_tile_count);
                count += map_summary_stat(stream, *lane_beg, channel_count);
                count += map_cycle_state(stream, lane_beg->m_cycle_state);
                count += map_surface_summary(stream, lane_beg->begin(), lane_beg->end(), channel_count);
            }
            return count;
        }
        template<class Stream, class SurfaceSummaryIterator>
        static std::streamsize map_surface_summary(Stream& stream,
                                                   SurfaceSummaryIterator surface_beg,
                                                   SurfaceSummaryIterator surface_end,
                                                   const size_t channel_count)
        {
            std::streamsize count = 0;
            for(;surface_beg != surface_end;++surface_beg)
            {
                count += stream_map< size_t >(stream, surface_beg->m_surface);
                count += stream_map< size_t >(stream, surface_beg->m_tile_count);
                count += map_summary_stat(stream, *surface_beg, channel_count);
            }
            return count;
        }
        template<class Stream, class SummaryStat>
        static std::streamsize map_summary_stat(Stream& stream, SummaryStat& stat, const size_t /*channel_count*/)
        {
            std::streamsize count = 0;

            count += stream_map< float >(stream, stat.m_percent_gt_q30);
            count += stream_map< float >(stream, stat.m_yield_g);
            count += stream_map< float >(stream, stat.m_projected_yield_g);
            count += stream_map< float >(stream, stat.m_reads);
            count += stream_map< float >(stream, stat.m_reads_pf);

            count += map_metric_stat(stream, stat.m_density);
            count += map_metric_stat(stream, stat.m_density_pf);
            count += map_metric_stat(stream, stat.m_cluster_count);
            count += map_metric_stat(stream, stat.m_cluster_count_pf);
            count += map_metric_stat(stream, stat.m_percent_pf);
            count += map_metric_stat(stream, stat.m_phasing);
            count += map_metric_stat(stream, stat.m_prephasing);
            count += map_metric_stat(stream, stat.m_percent_aligned);
            count += map_metric_stat(stream, stat.m_error_rate);
            count += map_metric_stat(stream, stat.m_error_rate_35);
            count += map_metric_stat(stream, stat.m_error_rate_50);
            count += map_metric_stat(stream, stat.m_error_rate_75);
            count += map_metric_stat(stream, stat.m_error_rate_100);
            count += map_metric_stat(stream, stat.m_first_cycle_intensity);

            return count;
        }

        template<class Stream, class MetricStat>
        static std::streamsize map_metric_stat(Stream& stream, MetricStat& stat)
        {
            std::streamsize count = 0;
            count += stream_map< float >(stream, stat.m_mean);
            count += stream_map< float >(stream, stat.m_stddev);
            count += stream_map< float >(stream, stat.m_median);
            return count;
        }
        template<class Stream, class Read>
        static std::streamsize map_read(Stream& stream, Read& read)
        {
            std::streamsize count = 0;
            count += stream_map< size_t >(stream, read.m_number);
            count += stream_map< bool >(stream, read.m_is_index);
            count += map_cycle_range(stream, read);
            return count;
        }
        template<class Stream, class CycleState>
        static std::streamsize map_cycle_state(Stream& stream, CycleState& cycle_state)
        {
            std::streamsize count = 0;
            count += map_cycle_range(stream, cycle_state.m_extracted_cycle_range);
            count += map_cycle_range(stream, cycle_state.m_called_cycle_range);
            count += map_cycle_range(stream, cycle_state.m_qscored_cycle_range);
            count += map_cycle_range(stream, cycle_state.m_error_cycle_range);
            return count;
        }
        template<class Stream, class CycleRange>
        static std::streamsize map_cycle_range(Stream& stream, CycleRange& cycle_range)
        {
            std::streamsize count = 0;
            count += stream_map< size_t >(stream, cycle_range.m_first_cycle);
            count += stream_map< size_t >(stream, cycle_range.m_last_cycle);
            return count;
        }
        template<class Stream, class MetricSummary>
        static std::streamsize map_metric_summary(Stream& stream,
                                                  MetricSummary& metric_summary,
                                                  const size_t /*channel_count*/)
        {
            std::streamsize count = 0;

            count += stream_map< float >(stream, metric_summary.m_error_rate);
            count += stream_map< float >(stream, metric_summary.m_percent_aligned);
            count += stream_map< float >(stream, metric_summary.m_first_cycle_intensity);
            count += stream_map< float >(stream, metric_summary.m_percent_gt_q30);
            count += stream_map< float >(stream, metric_summary.m_yield_g);
            count += stream_map< float >(stream, metric_summary.m_projected_yield_g);

            return count;
        }
    };

}}}
namespace illumina { namespace interop { namespace model { namespace summary
{
    std::ostream &operator<<(std::ostream &out, const run_summary &summary)
    {
        typedef io::generic_layout<run_summary, 1> layout_t;
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

    std::istream &operator>>(std::istream &in, run_summary &summary)
    {
        typedef io::generic_layout<run_summary, 1> layout_t;


        if (!in.good()) INTEROP_THROW(io::incomplete_file_exception, "Empty file found");
        const int version = in.get();
        if (version == -1) INTEROP_THROW(io::incomplete_file_exception, "Empty file found");
        if (version != layout_t::VERSION) INTEROP_THROW(io::bad_format_exception, "Version mismatch");

        layout_t::map_header(in, summary);
        summary.resize();
        layout_t::map_records(in, summary);
        return in;
    }
}}}}

