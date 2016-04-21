/** Update the cycle state for the summary model
 *
 *  @file
 *  @date  4/21/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/logic/summary/map_cycle_to_read.h"


namespace illumina { namespace interop { namespace logic { namespace summary {

/** Define a member function of cycle_state_summary that sets a cycle state */
typedef void (model::summary::cycle_state_summary::*set_cycle_state_func_t )(const model::run::cycle_range&);
/** Vector of vectors of cycle_range objects */
typedef std::vector<std::vector< model::run::cycle_range> > cycle_range_vector2d_t;

/** Cache errors for all tiles up to a give max cycle
 *
 * This function only includes errors from useable cycles (not the last cycle).
 *
 * @param beg iterator to start of a collection of error metrics
 * @param end iterator to end of a collection of error metrics
 * @param cycle_to_read map that takes a cycle and returns the read-number cycle-in-read pair
 * @param summary_by_lane_read destination cache by read then by lane a collection of errors
 */
template<typename I>
void cache_cycle_state_by_lane_read(I beg, I end,
                                    const read_cycle_vector_t& cycle_to_read,
                                    cycle_range_vector2d_t& summary_by_lane_read)
{
    typedef std::map< size_t, model::run::cycle_range >  cycle_range_tile_t;
    typedef std::vector< cycle_range_tile_t > cycle_range_by_read_tile_t;
    cycle_range_by_read_tile_t tmp (summary_by_lane_read.size());
    for(;beg != end;++beg)
    {
        INTEROP_ASSERT(beg->cycle() > 0);
        INTEROP_ASSERT((beg->cycle()-1) < cycle_to_read.size());
        const read_cycle& read = cycle_to_read[beg->cycle()-1];
        if(read.number == 0) continue;
        INTEROP_ASSERT((read.number-1) < tmp.size());
        const size_t id = static_cast<size_t>(model::metric_base::base_metric::id(beg->lane(), beg->tile()));
        tmp[read.number-1][id].update(beg->cycle());
    }
    for(size_t read=0;read<tmp.size();++read)
    {
        INTEROP_ASSERT(read < summary_by_lane_read.size());
        for(typename cycle_range_tile_t::const_iterator ebeg = tmp[read].begin(), eend = tmp[read].end();ebeg != eend;++ebeg)
        {
            const size_t lane = static_cast<size_t>(model::metric_base::base_metric::lane_from_id(ebeg->first)-1);
            INTEROP_ASSERT(lane < summary_by_lane_read[read].size());
            summary_by_lane_read[read][lane].update(ebeg->second.last_cycle());
        }
    }
}


/** Summarize the cycle state for a particular metric
 *
 * @param beg iterator to start of a collection of error metrics
 * @param end iterator to end of a collection of error metrics
 * @param cycle_to_read map cycle to the read number and cycle within read number
 * @param pointer to function that sets a cycle state
 * @param run destination run summary
 */
template<typename I>
void summarize_cycle_state(I beg,
                           I end,
                           const read_cycle_vector_t& cycle_to_read,
                           set_cycle_state_func_t set_cycle_state_fun,
                           model::summary::run_summary &run) throw( model::index_out_of_bounds_exception )
{


    cycle_range_vector2d_t cycle_range(run.size(), std::vector< model::run::cycle_range>(run.lane_count()));
    cache_cycle_state_by_lane_read(beg, end, cycle_to_read, cycle_range);

    model::run::cycle_range overall_cycle_state;
    for(size_t read=0;read<run.size();++read)
    {
        for (size_t lane = 0; lane < run[read].size(); ++lane)
        {
            (run[read][lane].cycle_state().*set_cycle_state_fun)(
                    cycle_range[read][lane] - (run[read].read().first_cycle()-1));
            overall_cycle_state.update(cycle_range[read][lane]);
        }
    }
    (run.cycle_state().*set_cycle_state_fun)(overall_cycle_state);
}

}}}}
