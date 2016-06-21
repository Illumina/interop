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
#include "interop/util/exception.h"
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/summary_statistics.h"
#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/model/summary/run_summary.h"

namespace illumina { namespace interop { namespace logic { namespace summary {

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

   /** Summarize a collection collapsed quality metrics
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
    * @param beg iterator to start of a collection of collapsed q metrics
    * @param end iterator to end of a collection of collapsed q metrics
    * @param cycle_to_read map cycle to the read number and cycle within read number
    * @param run destination run summary
    */
    template<typename I>
    void summarize_collapsed_quality_metrics(I beg,
                                             I end,
                                             const read_cycle_vector_t& cycle_to_read,
                                             model::summary::run_summary &run)
                                            throw( model::index_out_of_bounds_exception )
    {
        typedef model::summary::lane_summary lane_summary;
        typedef std::vector< size_t > size_vector_t;
        typedef std::vector< size_vector_t > size_vector2d_t;
        typedef std::vector< qval_total > qval_total_vector_t;
        typedef std::vector < qval_total_vector_t > qval_total_vector2d_t;
        typedef std::vector< std::set<size_t> > tile_lookup_t;
        if( beg == end ) return;
        if( run.size()==0 )return;
        qval_total_vector2d_t cache(run.size(), qval_total_vector_t(run.lane_count()));
        size_vector2d_t metrics_in_read(run.size(), size_vector_t(run.lane_count()));
        tile_lookup_t tile_lookup(run.lane_count());

        for(;beg != end;++beg)
        {
            INTEROP_ASSERT(beg->cycle() > 0);
            INTEROP_ASSERT((beg->cycle()-1) < cycle_to_read.size());
            const size_t read_number = cycle_to_read[beg->cycle()-1].number-1;
            if(cycle_to_read[beg->cycle()-1].is_last_cycle_in_read) continue;
            const size_t lane = beg->lane()-1;
            INTEROP_ASSERT(read_number < cache.size());
            INTEROP_ASSERT(lane < cache[read_number].size());
            INTEROP_ASSERT(read_number < metrics_in_read.size());
            INTEROP_ASSERT(lane < metrics_in_read[read_number].size());
            if(lane >= run.lane_count())
                INTEROP_THROW( model::index_out_of_bounds_exception, "Lane exceeds lane count in RunInfo.xml");
            cache[read_number][lane].above_qval+=beg->q30();
            cache[read_number][lane].total += beg->total();
            tile_lookup[lane].insert(beg->tile());
            metrics_in_read[read_number][lane] += 1;
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
            float read_projected_yield_g = 0;
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                INTEROP_ASSERT(lane < run[read].size());
                INTEROP_ASSERT(lane < cache[read].size());
                const float prediction_factor = divide(float(useable_cycles*tile_lookup[lane].size()),float(metrics_in_read[read][lane]));
                if( metrics_in_read[read][lane] > 0)
                {
                    total_useable_calls_by_read += cache[read][lane].total;
                    useable_calls_gt_q30_by_read += cache[read][lane].above_qval;
                    run[read][lane].percent_gt_q30(cache[read][lane].percent_above_qval());
                    run[read][lane].yield_g(cache[read][lane].total / 1e9f);
                    const float projected_yield_g = cache[read][lane].total * prediction_factor;
                    read_projected_yield_g += projected_yield_g/ 1e9f;
                    run[read][lane].projected_yield_g(
                            ::uint64_t(projected_yield_g + 0.5f) / 1e9f);
                }
                else
                {
                    float total = 0;
                    float metric_count = 0;
                    for(size_t r=0, n=read+1;r<n;++r)
                    {
                        if(cache[r][lane].total > 0)
                        {
                            total += run[r][lane].projected_yield_g();
                            metric_count += static_cast<float>(run[r].read().useable_cycles());
                        }
                    }
                    if(metric_count > 0) total /= metric_count;
                    const float projected_yield_g = total*useable_cycles;
                    run[read][lane].projected_yield_g(::uint64_t(projected_yield_g+0.5f));
                    read_projected_yield_g += projected_yield_g;
                }
            }
            run[read].summary().projected_yield_g(read_projected_yield_g);
            run[read].summary().yield_g(std::accumulate(run[read].begin(),
                                                        run[read].end(),
                                                        float(0),
                                                        util::op::const_member_function(&lane_summary::yield_g)));
            run[read].summary().percent_gt_q30(100 * divide(float(useable_calls_gt_q30_by_read), float(total_useable_calls_by_read)));
            total_useable_calls += total_useable_calls_by_read;
            useable_calls_gt_q30 += useable_calls_gt_q30_by_read;
            projected_yield_g += read_projected_yield_g;
            yield_g += run[read].summary().yield_g();
            // Certain metrics can be biased by the index read, e.g. C1 intensity, total yield
            // So, we include totals that skip the index reads
            if(!run[read].read().is_index())
            {
                total_useable_calls_nonindex += total_useable_calls_by_read;
                useable_calls_gt_q30_nonindex += useable_calls_gt_q30_by_read;
                projected_yield_g_nonindex += read_projected_yield_g;
                yield_g_nonindex += run[read].summary().yield_g();
            }
        }
        run.nonindex_summary().projected_yield_g(::uint64_t(projected_yield_g_nonindex)/1e9f);
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
