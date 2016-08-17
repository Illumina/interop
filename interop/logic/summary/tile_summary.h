/** Summary logic for tile metrics
 *
 *  @file
 *  @date  3/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include "interop/util/exception.h"
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/summary_statistics.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/summary/run_summary.h"

namespace illumina { namespace interop { namespace logic { namespace summary
{
    /** Summarize a collection tile metrics
    *
    * @sa model::summary::lane_summary::density
    * @sa model::summary::lane_summary::density_pf
    * @sa model::summary::lane_summary::cluster_count
    * @sa model::summary::lane_summary::cluster_count_pf
    * @sa model::summary::lane_summary::percent_pf
    * @sa model::summary::lane_summary::reads
    * @sa model::summary::lane_summary::reads_pf
    * @sa model::summary::lane_summary::percent_aligned
    * @sa model::summary::lane_summary::prephasing
    * @sa model::summary::lane_summary::phasing
    *
    * @sa model::summary::read_summary::percent_aligned
    * @sa model::summary::run_summary::percent_aligned
    *
    * @param beg iterator to start of a collection of tile metrics
    * @param end iterator to end of a collection of tile metrics
    * @param run destination run summary
    */
    template<typename I>
    void summarize_tile_metrics(I beg, I end, model::summary::run_summary &run)
                                    throw(model::index_out_of_bounds_exception)
    {
        typedef typename model::metrics::tile_metric::read_metric_vector read_metric_vector_t;
        typedef typename read_metric_vector_t::const_iterator const_read_metric_iterator;
        typedef std::vector<model::metrics::tile_metric> tile_vector_t;
        typedef std::vector<tile_vector_t> tile_by_lane_vector_t;

        if (beg == end) return;
        if (run.size() == 0)return;
        const ptrdiff_t n = std::distance(beg, end);

        tile_by_lane_vector_t tile_data_by_lane(run.lane_count());
        reserve(tile_data_by_lane.begin(), tile_data_by_lane.end(), n);

        summary_by_lane_read<model::metrics::read_metric> read_data_by_lane_read(run, n);

        for (; beg != end; ++beg)
        {
            const size_t lane = beg->lane() - 1;
            if (lane >= tile_data_by_lane.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Lane exceeds lane count in RunInfo.xml");
            tile_data_by_lane[beg->lane() - 1].push_back(*beg);// TODO: make more efficient by copying only tile data
            for (const_read_metric_iterator rb = beg->read_metrics().begin(), re = beg->read_metrics().end();
                 rb != re; ++rb)
            {
                const size_t read = rb->read() - 1;
                if (read >= read_data_by_lane_read.read_count())
                    INTEROP_THROW(model::index_out_of_bounds_exception, "Read exceeds read count in RunInfo.xml");
                read_data_by_lane_read(rb->read() - 1, beg->lane() - 1).push_back(*rb);
            }
        }


        //reads and reads pf
        // percent pf
        INTEROP_ASSERT(run.size() > 0);
        model::summary::metric_stat stat;
        for (size_t lane = 0; lane < run[0].size(); ++lane)
        {
            INTEROP_ASSERT(lane < tile_data_by_lane.size());
            INTEROP_ASSERT(lane < run[0].size());
            summarize(tile_data_by_lane[lane].begin(),
                      tile_data_by_lane[lane].end(),
                      stat,
                      util::op::const_member_function(&model::metrics::tile_metric::cluster_density),
                      util::op::const_member_function_less(&model::metrics::tile_metric::cluster_density));
            run[0][lane].density(stat);
            summarize(tile_data_by_lane[lane].begin(),
                      tile_data_by_lane[lane].end(),
                      stat,
                      util::op::const_member_function(&model::metrics::tile_metric::cluster_density_pf),
                      util::op::const_member_function_less(&model::metrics::tile_metric::cluster_density_pf));
            run[0][lane].density_pf(stat);
            summarize(tile_data_by_lane[lane].begin(),
                      tile_data_by_lane[lane].end(),
                      stat,
                      util::op::const_member_function(&model::metrics::tile_metric::cluster_count),
                      util::op::const_member_function_less(&model::metrics::tile_metric::cluster_count));
            run[0][lane].cluster_count(stat);
            summarize(tile_data_by_lane[lane].begin(),
                      tile_data_by_lane[lane].end(),
                      stat,
                      util::op::const_member_function(&model::metrics::tile_metric::cluster_count_pf),
                      util::op::const_member_function_less(&model::metrics::tile_metric::cluster_count_pf));
            run[0][lane].cluster_count_pf(stat);
            summarize(tile_data_by_lane[lane].begin(),
                      tile_data_by_lane[lane].end(),
                      stat,
                      util::op::const_member_function(&model::metrics::tile_metric::percent_pf),
                      util::op::const_member_function_less(&model::metrics::tile_metric::percent_pf));
            run[0][lane].percent_pf(stat);
            run[0][lane].reads(std::accumulate(tile_data_by_lane[lane].begin(),
                                               tile_data_by_lane[lane].end(),
                                               float(0),
                                               util::op::const_member_function(
                                                       &model::metrics::tile_metric::cluster_count)));
            run[0][lane].reads_pf(std::accumulate(tile_data_by_lane[lane].begin(),
                                                  tile_data_by_lane[lane].end(),
                                                  float(0),
                                                  util::op::const_member_function(
                                                          &model::metrics::tile_metric::cluster_count_pf)));

            for (size_t read = 1; read < run.size(); ++read)
            {
                INTEROP_ASSERT(read < run.size());
                run[read][lane].density(run[0][lane].density());
                run[read][lane].density_pf(run[0][lane].density_pf());
                run[read][lane].cluster_count(run[0][lane].cluster_count());
                run[read][lane].cluster_count_pf(run[0][lane].cluster_count_pf());
                run[read][lane].percent_pf(run[0][lane].percent_pf());
                run[read][lane].reads(run[0][lane].reads());
                run[read][lane].reads_pf(run[0][lane].reads_pf());
            }
        }
        float percent_aligned = 0;
        size_t total = 0;
        float percent_aligned_nonindex = 0;
        size_t total_nonindex = 0;
        for (size_t read = 0; read < run.size(); ++read)
        {
            INTEROP_ASSERT(read < run.size());
            float percent_aligned_by_read = 0;
            size_t total_by_read = 0;
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                INTEROP_ASSERT(lane < run[0].size());

                const size_t non_nan = nan_summarize(read_data_by_lane_read(read, lane).begin(),
                                                     read_data_by_lane_read(read, lane).end(),
                                                     stat,
                                                     util::op::const_member_function(
                                                             &model::metrics::read_metric::percent_aligned),
                                                     util::op::const_member_function_less(
                                                             &model::metrics::read_metric::percent_aligned));
                run[read][lane].percent_aligned(stat);
                nan_summarize(read_data_by_lane_read(read, lane).begin(),
                              read_data_by_lane_read(read, lane).end(),
                              stat,
                              util::op::const_member_function(&model::metrics::read_metric::percent_prephasing),
                              util::op::const_member_function_less(&model::metrics::read_metric::percent_prephasing));
                run[read][lane].prephasing(stat);
                nan_summarize(read_data_by_lane_read(read, lane).begin(),
                              read_data_by_lane_read(read, lane).end(),
                              stat,
                              util::op::const_member_function(&model::metrics::read_metric::percent_phasing),
                              util::op::const_member_function_less(&model::metrics::read_metric::percent_phasing));
                run[read][lane].phasing(stat);
                INTEROP_ASSERT(!std::isnan(run[read][lane].percent_aligned().mean()));
                percent_aligned_by_read += run[read][lane].percent_aligned().mean() * non_nan;
                total_by_read += non_nan;
            }
            run[read].summary().percent_aligned(divide(percent_aligned_by_read, float(total_by_read)));
            percent_aligned += percent_aligned_by_read;
            total += total_by_read;
            if (!run[read].read().is_index())
            {
                percent_aligned_nonindex += percent_aligned_by_read;
                total_nonindex += total_by_read;
            }
        }
        run.nonindex_summary().percent_aligned(divide(percent_aligned_nonindex, static_cast<float>(total_nonindex)));
        run.total_summary().percent_aligned(divide(percent_aligned, static_cast<float>(total)));

    }

}}}}
