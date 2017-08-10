/** Summary logic for phasing metrics
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include <interop/model/metrics/dynamic_phasing_metric.h>
#include "interop/util/exception.h"
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/summary_statistics.h"
#include "interop/model/metrics/phasing_metric.h"
#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/model/summary/run_summary.h"


namespace illumina { namespace interop { namespace logic { namespace summary
{
    /** Summarize a collection of phasing metrics
     *
     * @sa model::summary::lane_summary::phasing_slope
     * @sa model::summary::lane_summary::phasing_offset
     * @sa model::summary::lane_summary::prephasing_slope
     * @sa model::summary::lane_summary::prephasing_offset
     *
     *
     * @param beg iterator to start of a collection of dynamic phasing metrics
     * @param end iterator to end of a collection of dynamic phasing metrics
     * @param run destination run summary
     * @param naming_method tile naming convention
     * @param skip_median skip the median calculation
     */
    template<typename I>
    void summarize_phasing_metrics(I beg,
                                   I end,
                                   model::summary::run_summary &run,
                                   const constants::tile_naming_method naming_method,
                                   const bool skip_median=false) throw(model::index_out_of_bounds_exception)
    {
        typedef summary_by_lane_read<float> summary_by_lane_read_t;
        if (beg == end) return;
        if (run.size() == 0)return;
        const size_t surface_count = run.surface_count();
        summary_by_lane_read_t phasing_slope(run, std::distance(beg, end));
        summary_by_lane_read_t phasing_offset(run, std::distance(beg, end));
        summary_by_lane_read_t prephasing_slope(run, std::distance(beg, end));
        summary_by_lane_read_t prephasing_offset(run, std::distance(beg, end));

        summary_by_lane_read_t phasing_slope_surface(run, std::distance(beg, end), surface_count);
        summary_by_lane_read_t phasing_offset_surface(run, std::distance(beg, end), surface_count);
        summary_by_lane_read_t prephasing_slope_surface(run, std::distance(beg, end), surface_count);
        summary_by_lane_read_t prephasing_offset_surface(run, std::distance(beg, end), surface_count);

        for (; beg != end; ++beg)
        {
            const size_t read = beg->read() - 1;
            INTEROP_ASSERT(read < phasing_slope.read_count());
            const size_t lane = beg->lane() - 1;
            INTEROP_BOUNDS_CHECK(lane, phasing_slope.lane_count(), "Lane exceeds number of lanes in RunInfo.xml");
            phasing_slope(read, lane).push_back(beg->phasing_slope());
            phasing_offset(read, lane).push_back(beg->phasing_offset());
            prephasing_slope(read, lane).push_back(beg->prephasing_slope());
            prephasing_offset(read, lane).push_back(beg->prephasing_offset());

            if(surface_count < 2) continue;
            const size_t surface = beg->surface(naming_method);
            phasing_slope_surface(read, lane, surface-1).push_back(beg->phasing_slope());
            phasing_offset_surface(read, lane, surface-1).push_back(beg->phasing_offset());
            prephasing_slope_surface(read, lane, surface-1).push_back(beg->prephasing_slope());
            prephasing_offset_surface(read, lane, surface-1).push_back(beg->prephasing_offset());
        }

        for (size_t read = 0; read < run.size(); ++read)
        {
            INTEROP_ASSERT(read < phasing_slope.read_count());
            INTEROP_ASSERT(read < run.size());
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                INTEROP_ASSERT(lane < phasing_slope.lane_count());
                INTEROP_ASSERT(lane < run[read].size());
                model::summary::metric_stat phasing_stat;
                summarize(phasing_slope(read, lane).begin(), phasing_slope(read, lane).end(), phasing_stat, skip_median);
                run[read][lane].phasing_slope(phasing_stat);
                summarize(phasing_offset(read, lane).begin(), phasing_offset(read, lane).end(), phasing_stat,skip_median);
                run[read][lane].phasing_offset(phasing_stat);
                summarize(prephasing_slope(read, lane).begin(), prephasing_slope(read, lane).end(), phasing_stat,skip_median);
                run[read][lane].prephasing_slope(phasing_stat);
                summarize(prephasing_offset(read, lane).begin(), prephasing_offset(read, lane).end(), phasing_stat,skip_median);
                run[read][lane].prephasing_offset(phasing_stat);
                if(surface_count < 2) continue;
                for(size_t surface=0;surface<surface_count;++surface)
                {
                    summarize(phasing_slope_surface(read, lane,surface).begin(), phasing_slope_surface(read, lane,surface).end(), phasing_stat, skip_median);
                    run[read][lane][surface].phasing_slope(phasing_stat);
                    summarize(phasing_offset_surface(read, lane,surface).begin(), phasing_offset_surface(read, lane,surface).end(), phasing_stat,skip_median);
                    run[read][lane][surface].phasing_offset(phasing_stat);
                    summarize(prephasing_slope_surface(read, lane,surface).begin(), prephasing_slope_surface(read, lane,surface).end(), phasing_stat,skip_median);
                    run[read][lane][surface].prephasing_slope(phasing_stat);
                    summarize(prephasing_offset_surface(read, lane,surface).begin(), prephasing_offset_surface(read, lane,surface).end(), phasing_stat,skip_median);
                    run[read][lane][surface].prephasing_offset(phasing_stat);
                }
            }
        }
    }



}}}}
