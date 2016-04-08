/** Summary logic for quality metrics
 *
 *  @file
 *  @date  3/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include <map>
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/summary_statistics.h"
#include "interop/model/metrics/q_metric.h"
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
     * @param is_first_cycle_of_read map that takes a cycle and returns the read-number
     * @param op unary operator that takes some object and returns a read info
     */
    template<typename I, typename UnaryOp>
    void map_read_to_cycle(I beg, I end, std::vector<size_t>& cycle_to_read, UnaryOp op)
    {
        cycle_to_read.resize(std::accumulate(beg, end, size_t(0), op::total_cycle_sum<UnaryOp>(op))+1);
        std::fill(cycle_to_read.begin(), cycle_to_read.end(), 0);
        for(;beg != end;++beg)
        {
            for(size_t cycle=op(*beg).first_cycle()-1, last_cycle=op(*beg).last_cycle(), cycle_in_read=1;cycle < last_cycle;++cycle_in_read, ++cycle)
            {
                INTEROP_ASSERT(cycle < cycle_to_read.size());
                cycle_to_read[cycle] = op(*beg).number();
            }
            INTEROP_ASSERT((op(*beg).last_cycle()-1) < cycle_to_read.size());
            cycle_to_read[op(*beg).last_cycle()-1] = 0;// Remove last cycle, not usable
        }
    }
    /** Create a map from cycle to read number (only if it is the first cycle of the read)
     *
     * @param beg iterator to start of a collection of read infos
     * @param end iterator to end of a collection of read infos
     * @param is_first_cycle_of_read map that takes a cycle and returns the read-number
     */
    template<typename I>
    void map_read_to_cycle(I beg, I end, std::vector<size_t>& cycle_to_read)
    {
        map_read_to_cycle(beg, end, cycle_to_read, op::default_get_read());
    }
    /** Contains number above Q30 and total number of calls
     */
    struct qval_total
    {
        /** Constructor
         *
         * @param qval calls above given qval
         * @param tot total number of calls
         */
        qval_total(const ::uint64_t qval=0, const ::uint64_t tot=0) : above_qval(qval), total(tot){}
        /** Calculate the percent of calls greater than Qval
         *
         * @return percent of calls greater than Qval
         */
        float percent_above_qval()const{return 100*average();}
        /**  Calculate average number above Qval
         *
         * @return average number above Qval
         */
        float average()const{return divide(static_cast<float>(above_qval),static_cast<float>(total));}
        /** Calls above Q-val */
        ::uint64_t above_qval;
        /** Total number of calls */
        ::uint64_t total;
    };

    /** Summarize a collection quality metrics
     *
     * @sa model::summary::lane_summary::percent_gt_q30
     * @sa model::summary::lane_summary::yield_g
     * @sa model::summary::lane_summary::projected_yield_g
     *
     * @sa model::summary::read_summary::percent_gt_q30
     * @sa model::summary::read_summary::yield_g
     * @sa model::summary::read_summary::projected_yield_g
     *
     * @sa model::summary::run_summary::percent_gt_q30
     * @sa model::summary::run_summary::yield_g
     * @sa model::summary::run_summary::projected_yield_g
     *
     * @param beg iterator to start of a collection of q metrics
     * @param end iterator to end of a collection of q metrics
     * @param qval30_index index of Q30 bin
     * @param run destination run summary
     */
    template<typename I>
    void summarize_quality_metrics(I beg, I end, const size_t qval_index, model::summary::run_summary &run) throw( model::index_out_of_bounds_exception )
    {
        typedef model::metrics::q_metric::uint_t uint_t;
        typedef model::summary::lane_summary lane_summary;
        typedef std::vector< size_t > size_vector_t;
        typedef std::vector< size_vector_t > size_vector2d_t;
        typedef std::vector< qval_total > qval_total_vector_t;
        typedef std::vector < qval_total_vector_t > qval_total_vector2d_t;
        typedef std::set<size_t> tile_lookup_t;
        if(beg == end) return;
        if(run.size()==0)return;
        qval_total_vector2d_t cache(run.size(), qval_total_vector_t(run.lane_count()));
        size_vector2d_t metrics_in_read(run.size(), size_vector_t(run.lane_count()));
        tile_lookup_t tile_lookup;

        std::vector<size_t> cycle_to_read;
        map_read_to_cycle(run.begin(), run.end(), cycle_to_read);

        for(;beg != end;++beg)
        {
            INTEROP_ASSERT(beg->cycle() > 0);
            INTEROP_ASSERT((beg->cycle()-1) < cycle_to_read.size());
            const size_t read_number = cycle_to_read[beg->cycle()-1];
            if(read_number == 0) continue;
            const size_t lane = beg->lane()-1;
            INTEROP_ASSERT((read_number-1) < cache.size());
            INTEROP_ASSERT(lane < cache[read_number-1].size());
            INTEROP_ASSERT((read_number-1) < metrics_in_read.size());
            INTEROP_ASSERT(lane < metrics_in_read[read_number-1].size());
            if(lane >= run.lane_count()) throw model::index_out_of_bounds_exception("Lane exceeds lane count in RunInfo.xml");
            cache[read_number-1][lane].above_qval+=beg->total_over_qscore(static_cast<uint_t>(qval_index));
            cache[read_number-1][lane].total += beg->sum_qscore();
            tile_lookup.insert(beg->tile());
            metrics_in_read[read_number - 1][lane] += 1;
        }
        ::uint64_t total_useable_calls = 0;
        ::uint64_t useable_calls_gt_q30 = 0;
        float projected_yield_g = 0;
        float yield_g = 0;

        ::uint64_t total_useable_calls_nonindex = 0;
        ::uint64_t useable_calls_gt_q30_nonindex = 0;
        float projected_yield_g_nonindex = 0;
        float yield_g_nonindex = 0;
        for(size_t read=0;read<run.size();++read)
        {
            INTEROP_ASSERT(read < run.size());
            INTEROP_ASSERT(read < cache.size());
            const size_t useable_cycles = run[read].read().useable_cycles();
            ::uint64_t total_useable_calls_by_read=0;
            ::uint64_t useable_calls_gt_q30_by_read = 0;
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                INTEROP_ASSERT(lane < run[read].size());
                INTEROP_ASSERT(lane < cache[read].size());
                const float prediction_factor = divide(float(useable_cycles*tile_lookup.size()),float(metrics_in_read[read][lane]));
                if( metrics_in_read[read][lane] > 0)
                {
                    total_useable_calls_by_read += cache[read][lane].total;
                    useable_calls_gt_q30_by_read += cache[read][lane].above_qval;
                    run[read][lane].percent_gt_q30(cache[read][lane].percent_above_qval());
                    run[read][lane].yield_g(cache[read][lane].total / 1e9f);
                    run[read][lane].projected_yield_g(
                            ::uint64_t(cache[read][lane].total * prediction_factor + 0.5f) / 1e9f);
                }
                else
                {
                    run[read][lane].projected_yield_g(::uint64_t(cache[read][lane].average()*useable_cycles)/1e9f);
                }
            }
            run[read].summary().projected_yield_g(std::accumulate(run[read].begin(),
                                                        run[read].end(),
                                                        float(0),
                                                        util::op::const_member_function(&lane_summary::projected_yield_g)));
            run[read].summary().yield_g(std::accumulate(run[read].begin(),
                                              run[read].end(),
                                              float(0),
                                              util::op::const_member_function(&lane_summary::yield_g)));
            run[read].summary().percent_gt_q30(100 * divide(float(useable_calls_gt_q30_by_read), float(total_useable_calls_by_read)));
            total_useable_calls += total_useable_calls_by_read;
            useable_calls_gt_q30 += useable_calls_gt_q30_by_read;
            projected_yield_g += run[read].summary().projected_yield_g();
            yield_g += run[read].summary().yield_g();
            // Certain metrics can be biased by the index read, e.g. C1 intensity, total yield
            // So, we include totals that skip the index reads
            if(!run[read].read().is_index())
            {
                total_useable_calls_nonindex += total_useable_calls_by_read;
                useable_calls_gt_q30_nonindex += useable_calls_gt_q30_by_read;
                projected_yield_g_nonindex += run[read].summary().projected_yield_g();
                yield_g_nonindex += run[read].summary().yield_g();
            }
        }
        run.nonindex_summary().projected_yield_g(projected_yield_g_nonindex);
        run.nonindex_summary().yield_g(yield_g_nonindex);
        run.nonindex_summary().percent_gt_q30(100 * divide(float(useable_calls_gt_q30_nonindex), float(total_useable_calls_nonindex)));

        run.total_summary().projected_yield_g(projected_yield_g);
        run.total_summary().yield_g(yield_g);
        run.total_summary().percent_gt_q30(100 * divide(float(useable_calls_gt_q30), float(total_useable_calls)));
    }

}
}
}
}
