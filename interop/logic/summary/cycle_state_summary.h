/** Update the cycle state for the summary model
 *
 *  @file
 *  @date  4/21/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/util/map.h"
#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/model/metrics/tile_metric.h"


namespace illumina { namespace interop { namespace logic { namespace summary
{

    /** Define a member function of cycle_state_summary that sets a cycle state */
    typedef void (model::summary::cycle_state_summary::*set_cycle_state_func_t )(const model::run::cycle_range &);

    /** Vector of vectors of cycle_range objects */
    typedef std::vector<std::vector<model::run::cycle_range> > cycle_range_vector2d_t;


    /** Summarize the cycle state for a particular metric
     *
     * @param tile_metrics tile metric set
     * @param cycle_metrics a cycle based metric set
     * @param cycle_to_read map between the current cycle and read information
     * @param set_cycle_state_fun callback to set the cycle state
     * @param run run summary
     */
    template<typename Metric>
    void summarize_cycle_state(const model::metric_base::metric_set <model::metrics::tile_metric> &tile_metrics,
                               const model::metric_base::metric_set <Metric> &cycle_metrics,
                               const read_cycle_vector_t &cycle_to_read,
                               set_cycle_state_func_t set_cycle_state_fun,
                               model::summary::run_summary &run) throw(model::index_out_of_bounds_exception)
    {
        typedef model::run::cycle_range cycle_range;
        typedef typename model::metric_base::metric_set<model::metrics::tile_metric>::const_iterator const_tile_iterator;
        typedef typename model::metric_base::metric_set<Metric>::const_iterator const_metric_iterator;
        cycle_range_vector2d_t summary_by_lane_read(run.size(), std::vector<cycle_range>(run.lane_count()));

        typedef model::metrics::tile_metric::id_t id_t;
        typedef INTEROP_UNORDERED_MAP(id_t, cycle_range) cycle_range_tile_t;
        typedef INTEROP_UNORDERED_MAP(id_t, size_t) max_tile_map_t;
        typedef typename max_tile_map_t::const_iterator const_max_tile_iterator;
        typedef std::vector<cycle_range_tile_t> cycle_range_by_read_tile_t;
        cycle_range_by_read_tile_t tmp(summary_by_lane_read.size());
        max_tile_map_t tmp_by_tile;
        cycle_range overall_cycle_state;
        for (const_metric_iterator cycle_metric_it = cycle_metrics.begin(), cycle_metric_end = cycle_metrics.end();
             cycle_metric_it != cycle_metric_end; ++cycle_metric_it)
        {
            INTEROP_ASSERT(cycle_metric_it->cycle() > 0);
            INTEROP_ASSERT((cycle_metric_it->cycle() - 1) < cycle_to_read.size());

            if ((cycle_metric_it->cycle() - 1) >= cycle_to_read.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Cycle exceeds total cycles from Reads in the RunInfo.xml");
            const read_cycle &read = cycle_to_read[cycle_metric_it->cycle() - 1];
            if (read.number == 0) continue;
            INTEROP_ASSERT((read.number - 1) < tmp.size());

            const id_t id = cycle_metric_it->tile_hash();
            tmp[read.number - 1][id].update(cycle_metric_it->cycle());
            typename max_tile_map_t::iterator it = tmp_by_tile.find(id);
            if (it == tmp_by_tile.end())
                tmp_by_tile[id] = cycle_metric_it->cycle();
            else it->second = std::max(static_cast<size_t>(cycle_metric_it->cycle()), it->second);
        }

        // Tile exists, but nothing was written out for that metric on any cycle
        for (const_tile_iterator tile_it = tile_metrics.begin(), tile_end = tile_metrics.end();
             tile_it != tile_end; ++tile_it)
        {
            size_t cycle_for_tile = 0;
            const id_t id = model::metric_base::base_metric::create_id(tile_it->lane(), tile_it->tile());
            for (size_t read_index = 0; read_index < tmp.size(); ++read_index)
            {
                if (tmp[read_index].find(tile_it->tile_hash()) == tmp[read_index].end())
                {
                    tmp[read_index][id].update(run[read_index].read().first_cycle() - 1);
                    tmp_by_tile[id] = 0;
                }
                else
                {
                    cycle_for_tile = tmp[read_index][id].last_cycle();
                }
            }
            tmp_by_tile[id] = cycle_for_tile;
        }
        for (size_t read = 0; read < tmp.size(); ++read)
        {
            INTEROP_ASSERT(read < summary_by_lane_read.size());
            for (typename cycle_range_tile_t::const_iterator it = tmp[read].begin(), end = tmp[read].end();
                 it != end; ++it)
            {
                const size_t lane = static_cast<size_t>(model::metric_base::base_metric::lane_from_id(it->first) - 1);
                INTEROP_ASSERT(lane < summary_by_lane_read[read].size());
                summary_by_lane_read[read][lane].update(it->second.last_cycle());
            }
        }


        for (size_t read = 0; read < run.size(); ++read)
        {
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                const size_t first_cycle_index_of_read = run[read].read().first_cycle() - 1;
                const cycle_range cycle_range_within_read =
                        summary_by_lane_read[read][lane] - first_cycle_index_of_read;
                (run[read][lane].cycle_state().*set_cycle_state_fun)(cycle_range_within_read);
            }
        }
        for (const_max_tile_iterator range_it = tmp_by_tile.begin(), range_end = tmp_by_tile.end();
             range_it != range_end;
             ++range_it)
            overall_cycle_state.update(range_it->second);
        (run.cycle_state().*set_cycle_state_fun)(overall_cycle_state);
    }

}}}}

