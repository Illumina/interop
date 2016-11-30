/** Summary logic for extraction metrics
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include "interop/util/exception.h"
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/summary_statistics.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/model/summary/run_summary.h"


namespace illumina { namespace interop { namespace logic { namespace summary
{
    /** Summarize and aggregate the first_cycle_intensity
     *
     * @sa model::summary::lane_summary::first_cycle_intensity
     * @sa model::summary::read_summary::first_cycle_intensity
     * @sa model::summary::run_summary::first_cycle_intensity
     *
     *
     * @param beg iterator to start of a collection of extraction metrics
     * @param end iterator to end of a collection of extraction metrics
     * @param cycle_to_read map cycle to the read number and cycle within read number
     * @param channel channel to use for intensity reporting
     * @param naming_method tile naming convention
     * @param run destination run summary
     * @param skip_median skip the median calculation
     */
    template<typename I>
    void summarize_extraction_metrics(I beg,
                                      I end,
                                      const read_cycle_vector_t &cycle_to_read,
                                      const size_t channel,
                                      const constants::tile_naming_method naming_method,
                                      model::summary::run_summary &run,
                                      const bool skip_median=false) throw(model::index_out_of_bounds_exception)
    {
        typedef typename model::metrics::extraction_metric::ushort_t ushort_t;
        typedef summary_by_lane_read<ushort_t> summary_by_lane_read_t;
        if (beg == end) return;
        if (run.size() == 0)return;
        const size_t surface_count = run.surface_count();
        summary_by_lane_read_t read_lane_cache(run, std::distance(beg, end));
        summary_by_lane_read_t read_lane_surface_cache(run, std::distance(beg, end), surface_count);

        for (; beg != end; ++beg)
        {
            if ((beg->cycle() - 1) >= cycle_to_read.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Cycle exceeds total cycles from Reads in the RunInfo.xml");
            const size_t read = cycle_to_read[beg->cycle() - 1].number - 1;
            if (cycle_to_read[beg->cycle() - 1].cycle_within_read > 1) continue;
            INTEROP_ASSERT(read < read_lane_cache.read_count());
            const size_t lane = beg->lane() - 1;
            if (lane >= read_lane_cache.lane_count())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Lane exceeds lane count in RunInfo.xml");
            read_lane_cache(read, lane).push_back(beg->max_intensity(channel));
            if(surface_count < 2) continue;
            const size_t surface = beg->surface(naming_method);
            INTEROP_ASSERT(surface > 0);
            read_lane_surface_cache(read, lane, surface-1).push_back(beg->max_intensity(channel));
        }

        float first_cycle_intensity = 0;
        size_t total = 0;
        float first_cycle_intensity_nonindex = 0;
        size_t total_nonindex = 0;

        for (size_t read = 0; read < run.size(); ++read)
        {
            INTEROP_ASSERT(read < read_lane_cache.read_count());
            INTEROP_ASSERT(read < run.size());
            float first_cycle_intensity_by_read = 0;
            size_t total_by_read = 0;
            model::summary::metric_stat first_cycle_intensity_stat;
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                INTEROP_ASSERT(lane < read_lane_cache.lane_count());
                INTEROP_ASSERT(lane < run[read].size());
                summarize(read_lane_cache(read, lane).begin(), read_lane_cache(read, lane).end(), first_cycle_intensity_stat, skip_median);
                run[read][lane].first_cycle_intensity(first_cycle_intensity_stat);
                if(surface_count > 1)
                {
                    for (size_t surface = 0; surface < surface_count; ++surface)
                    {
                        first_cycle_intensity_stat.clear();
                        summarize(read_lane_surface_cache(read, lane, surface).begin(),
                                  read_lane_surface_cache(read, lane, surface).end(),
                                  first_cycle_intensity_stat, skip_median);
                        run[read][lane][surface].first_cycle_intensity(first_cycle_intensity_stat);
                    }
                }
                first_cycle_intensity_by_read += std::accumulate(read_lane_cache(read, lane).begin(),
                                                                 read_lane_cache(read, lane).end(),
                                                                 size_t(0));
                total_by_read += read_lane_cache(read, lane).size();
            }
            run[read].summary().first_cycle_intensity(
                    divide(first_cycle_intensity_by_read, static_cast<float>(total_by_read)));
            first_cycle_intensity += first_cycle_intensity_by_read;
            total += total_by_read;

            if (!run[read].read().is_index()) //Only for non-indexed reads
            {
                first_cycle_intensity_nonindex += first_cycle_intensity_by_read;
                total_nonindex += total_by_read;
            }
        }
        run.nonindex_summary().first_cycle_intensity(
                divide(first_cycle_intensity_nonindex, static_cast<float>(total_nonindex)));

        run.total_summary().first_cycle_intensity(divide(first_cycle_intensity, static_cast<float>(total)));
    }

}}}}

