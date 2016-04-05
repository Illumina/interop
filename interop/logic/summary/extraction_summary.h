/** Summary logic for extraction metrics
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/summary_statistics.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/summary/run_summary.h"


namespace illumina
{
namespace interop
{
namespace logic
{
namespace summary
{
    /** Create a map from cycle to read number (only if it is the first cycle of the read)
     *
     * @param beg iterator to start of a collection of read infos
     * @param end iterator to end of a collection of read infos
     * @param is_first_cycle_of_read map that takes a cycle and returns the read-number if it is the first cycle in the read
     * @param op unary operator that takes some object and returns a read info
     */
    template<typename I, typename UnaryOp>
    void map_is_first_cycle_of_read(I beg, I end, std::vector<size_t>& is_first_cycle_of_read, UnaryOp op)
    {
        is_first_cycle_of_read.resize(std::accumulate(beg, end, size_t(0), op::total_cycle_sum<UnaryOp>(op))+1);
        std::fill(is_first_cycle_of_read.begin(), is_first_cycle_of_read.end(), 0);
        for(;beg != end;++beg) is_first_cycle_of_read[op(*beg).first_cycle()] = op(*beg).number();
    }
    /** Create a map from cycle to read number (only if it is the first cycle of the read)
     *
     * @param beg iterator to start of a collection of read infos
     * @param end iterator to end of a collection of read infos
     * @param is_first_cycle_of_read map that takes a cycle and returns the read-number if it is the first cycle in the read
     */
    template<typename I>
    void map_is_first_cycle_of_read(I beg, I end, std::vector<size_t>& is_first_cycle_of_read)
    {
        map_is_first_cycle_of_read(beg, end, is_first_cycle_of_read, op::default_get_read());
    }


    /** Summarize a collection extraction metrics
     *
     * @sa model::summary::lane_summary::first_cycle_intensity
     * @sa model::summary::read_summary::first_cycle_intensity
     * @sa model::summary::run_summary::first_cycle_intensity
     *
     *
     * @param beg iterator to start of a collection of extraction metrics
     * @param end iterator to end of a collection of extraction metrics
     * @param channel channel to use for intensity reporting
     * @param run destination run summary
     */
    template<typename I>
    void summarize_extraction_metrics(I beg, I end, const size_t channel, model::summary::run_summary &run) _INTEROP_MODEL_THROWS
    {
        typedef typename model::metrics::extraction_metric::ushort_t ushort_t;
        typedef summary_by_lane_read<ushort_t> summary_by_lane_read_t;
        if(beg == end) return;
        if(run.size()==0)return;

        summary_by_lane_read_t temp(run, std::distance(beg, end));
        std::vector<size_t> is_first_cycle_of_read;
        logic::summary::map_is_first_cycle_of_read(run.begin(), run.end(), is_first_cycle_of_read);

        for(;beg != end;++beg)
        {
            const size_t read = is_first_cycle_of_read[beg->cycle()];
            if(read == 0) continue; // If read is zero, then the cycle is invalid (not the first cycle of read)
            INTEROP_ASSERT((read-1) < temp.read_count());
            const size_t lane = beg->lane()-1;
            if(lane >= temp.lane_count()) throw model::index_out_of_bounds_exception("Lane exceeds lane count in RunInfo.xml");
            temp(read-1, lane).push_back(beg->max_intensity(channel));
        }

        float first_cycle_intensity = 0;
        size_t total = 0;
        float first_cycle_intensity_nonindex = 0;
        size_t total_nonindex = 0;
        for(size_t read=0;read<run.size();++read)
        {
            INTEROP_ASSERT(read < temp.read_count());
            INTEROP_ASSERT(read < run.size());
            float first_cycle_intensity_by_read = 0;
            size_t total_by_read = 0;
            for(size_t lane=0;lane<run[read].size();++lane)
            {
                INTEROP_ASSERT(lane < temp.lane_count());
                INTEROP_ASSERT(lane < run[read].size());
                summarize(temp(read,lane).begin(), temp(read,lane).end(), run[read][lane].first_cycle_intensity());
                first_cycle_intensity_by_read += std::accumulate(temp(read,lane).begin(),
                                                      temp(read,lane).end(),
                                                      size_t(0));
                total_by_read += temp(read,lane).size();
            }
            run[read].summary().first_cycle_intensity(divide(first_cycle_intensity_by_read, static_cast<float>(total_by_read)));
            first_cycle_intensity += first_cycle_intensity_by_read;
            total += total_by_read;
            if(!run[read].read().is_index())
            {
                first_cycle_intensity_nonindex += first_cycle_intensity_by_read;
                total_nonindex += total_by_read;
            }
        }
        run.nonindex_summary().first_cycle_intensity(divide(first_cycle_intensity_nonindex, static_cast<float>(total_nonindex)));
        run.total_summary().first_cycle_intensity(divide(first_cycle_intensity, static_cast<float>(total)));
    }

}
}
}
}
