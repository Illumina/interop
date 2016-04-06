/** Summary logic for error metrics
 *
 *  @file
 *  @date  3/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include "interop/util/length_of.h"
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/summary_statistics.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/summary/run_summary.h"


namespace illumina { namespace interop { namespace logic { namespace summary {
/** Read number and cycle in read
 */
struct read_cycle
{
    /** Constructor
     *
     * @param r read number
     * @param c cycle in read
     */
    read_cycle(const size_t r=0, const size_t c=0) : number(r), cycle(c){}
    /** Read number */
    size_t number;
    /** Cycle in read */
    size_t cycle;
};

/** Create a map from cycle to a pair: read number and cycle in read
 *
 * @param beg iterator to start of a collection of read infos
 * @param end iterator to end of a collection of read infos
 * @param cycle_to_read map that takes a cycle and returns the read-number cycle-in-read pair
 * @param op unary operator that takes some object and returns a read info
 */
template<typename I, typename UnaryOp>
void map_read_to_cycle_number(I beg, I end, std::vector<read_cycle>& cycle_to_read, UnaryOp op)
{
    cycle_to_read.resize(std::accumulate(beg, end, size_t(0), op::total_cycle_sum<UnaryOp>(op))+1);
    std::fill(cycle_to_read.begin(), cycle_to_read.end(), 0);
    for(;beg != end;++beg)
    {
        for(size_t cycle=op(*beg).first_cycle()-1, last_cycle=op(*beg).last_cycle(), cycle_in_read=1;
            cycle < last_cycle;++cycle_in_read, ++cycle)
        {
            INTEROP_ASSERT(cycle < cycle_to_read.size());
            cycle_to_read[cycle] = read_cycle(op(*beg).number(), cycle_in_read);
        }
        INTEROP_ASSERT((op(*beg).last_cycle()-1) < cycle_to_read.size());
        cycle_to_read[op(*beg).last_cycle()-1] = read_cycle();// Remove last cycle, not usable
    }
}
/** Create a map from cycle to a pair: read number and cycle in read
 *
 * @param beg iterator to start of a collection of read infos
 * @param end iterator to end of a collection of read infos
 * @param cycle_to_read map that takes a cycle and returns the read-number cycle-in-read pair
 */
template<typename I>
void map_read_to_cycle_number(I beg, I end, std::vector<read_cycle>& cycle_to_read)
{
    map_read_to_cycle_number(beg, end, cycle_to_read, op::default_get_read());
}

/** Cache errors for all tiles up to a give max cycle
 *
 * This function only includes errors from useable cycles (not the last cycle) to up the given max cycle.
 *
 * @param beg iterator to start of a collection of error metrics
 * @param end iterator to end of a collection of error metrics
 * @param max_cycle maximum cycle to take
 * @param cycle_to_read map that takes a cycle and returns the read-number cycle-in-read pair
 * @param summary_by_lane_read destination cache by read then by lane a collection of errors
 */

template<typename I>
void cache_error_by_lane_read(I beg,
                              I end,
                              const size_t max_cycle,
                              const std::vector<read_cycle>& cycle_to_read,
                              summary_by_lane_read<float>& summary_by_lane_read) _INTEROP_MODEL_THROWS
{
    typedef std::pair<size_t, size_t> key_t;
    typedef std::pair<float, float> value_t;
    typedef std::map< key_t, value_t>  error_tile_t;
    typedef std::vector< error_tile_t > error_by_read_tile_t;
    error_by_read_tile_t tmp (summary_by_lane_read.size());
    for(;beg != end;++beg)
    {
        INTEROP_ASSERT(beg->cycle() > 0);
        INTEROP_ASSERT((beg->cycle()-1) < cycle_to_read.size());
        const read_cycle& read = cycle_to_read[beg->cycle()-1];
        if(read.cycle > max_cycle || read.number == 0) continue;
        key_t key = std::make_pair(beg->lane(), beg->tile());
        const size_t read_number = read.number-1;
        INTEROP_ASSERT(read_number < tmp.size());
        if(tmp[read_number].find(key) == tmp[read.number-1].end())
            tmp[read_number][key] = std::make_pair(0.0f, 0.0f);
        tmp[read_number][key].first+=beg->errorRate();
        tmp[read_number][key].second+=1;
    }
    for(size_t read=0;read<tmp.size();++read)
    {
        for(typename error_tile_t::const_iterator ebeg = tmp[read].begin(), eend = tmp[read].end();ebeg != eend;++ebeg)
        {
            INTEROP_ASSERT(read < summary_by_lane_read.read_count());
            const size_t lane = ebeg->first.first-1;
            if(lane >= summary_by_lane_read.lane_count()) throw model::index_out_of_bounds_exception("Lane exceeds number of lanes in RunInfo.xml");
            summary_by_lane_read(read, lane).push_back(divide(ebeg->second.first,ebeg->second.second));
        }
    }
}

/** Calculate summary statistics for each collection of metrics organized by read and lane
 *
 * @param summary_by_lane_read source cache by read then by lane a collection of errors
 * @param run destination run summary
 * @param func member function pointer to metric
 */
inline void error_summary_from_cache(summary_by_lane_read<float>& summary_by_lane_read,
                              model::summary::run_summary& run,
                              model::summary::metric_stat& (model::summary::lane_summary::*func )())
{
    for(size_t read=0;read<run.size();++read)
    {
        INTEROP_ASSERT(read < summary_by_lane_read.read_count());
        INTEROP_ASSERT(read < run.size());
        for (size_t lane = 0; lane < run[read].size(); ++lane)
        {
            INTEROP_ASSERT(lane < run[read].size());
            INTEROP_ASSERT(lane < summary_by_lane_read.lane_count());
            summarize(summary_by_lane_read(read,lane).begin(),
                      summary_by_lane_read(read,lane).end(),
                      (run[read][lane].*func)());
        }
    }
}

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
                                    const std::vector<read_cycle>& cycle_to_read,
                                    std::vector<std::vector< model::run::cycle_range> >& summary_by_lane_read)
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
        size_t id = static_cast<size_t>(model::metric_base::base_metric::id(beg->lane(), beg->tile()));
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

/** Summarize a collection error metrics
 *
 * @sa model::summary::lane_summary::error_rate
 * @sa model::summary::lane_summary::error_rate_35
 * @sa model::summary::lane_summary::error_rate_50
 * @sa model::summary::lane_summary::error_rate_75
 * @sa model::summary::lane_summary::error_rate_100
 *
 * @sa model::summary::read_summary::error_rate
 *
 * @sa model::summary::run_summary::error_rate
 *
 * @todo: Refactor this to make simpler
 *
 * @param beg iterator to start of a collection of error metrics
 * @param end iterator to end of a collection of error metrics
 * @param run destination run summary
 */
template<typename I>
void summarize_error_metrics(I beg, I end, model::summary::run_summary &run) _INTEROP_MODEL_THROWS
{
    typedef summary_by_lane_read<float> summary_by_lane_read_t;
    typedef std::vector<std::vector< model::run::cycle_range> > cycle_range_read_lane_t;
    typedef model::summary::metric_stat& (model::summary::lane_summary::*error_functor_t )();
    typedef std::pair<size_t, error_functor_t > cycle_functor_pair_t;

    if(beg == end) return;
    if(run.size() == 0) return;
    summary_by_lane_read_t temp(run, std::distance(beg, end));

    std::vector<read_cycle> cycle_to_read;
    map_read_to_cycle_number(run.begin(), run.end(), cycle_to_read);

    cycle_functor_pair_t cycle_functor_pairs[] = {
            cycle_functor_pair_t(35u, &model::summary::lane_summary::error_rate_35),
            cycle_functor_pair_t(50u, &model::summary::lane_summary::error_rate_50),
            cycle_functor_pair_t(75u, &model::summary::lane_summary::error_rate_75),
            cycle_functor_pair_t(100u, &model::summary::lane_summary::error_rate_100),
    };
    for(size_t i=0;i<util::length_of(cycle_functor_pairs);++i)
    {
        cache_error_by_lane_read(beg, end, cycle_functor_pairs[i].first, cycle_to_read, temp);
        error_summary_from_cache(temp, run, cycle_functor_pairs[i].second);
        temp.clear();
    }


    cache_error_by_lane_read(beg, end, std::numeric_limits<size_t>::max(), cycle_to_read, temp);

    cycle_range_read_lane_t cycle_range(run.size(), std::vector< model::run::cycle_range>(run.lane_count()));
    cache_cycle_state_by_lane_read(beg, end, cycle_to_read, cycle_range);

    float error_rate = 0;
    size_t total = 0;
    float error_rate_nonindex = 0;
    size_t total_nonindex = 0;
    model::run::cycle_range overall_cycle_state;
    for(size_t read=0;read<run.size();++read)
    {
        INTEROP_ASSERT(read < run.size());
        float error_rate_by_read = 0;
        size_t total_by_read = 0;
        for (size_t lane = 0; lane < run[read].size(); ++lane)
        {
            INTEROP_ASSERT(lane < run[read].size());
            summarize(temp(read,lane).begin(),
                      temp(read,lane).end(),
                      run[read][lane].error_rate());
            error_rate_by_read += std::accumulate(temp(read,lane).begin(),
                                                  temp(read,lane).end(),
                                                  float(0));
            total_by_read += temp(read,lane).size();
            run[read][lane].cycle_state().error_cycle_range(cycle_range[read][lane] - (run[read].read().first_cycle()-1));
            overall_cycle_state.update(cycle_range[read][lane]);
        }
        if(total_by_read>0) run[read].summary().error_rate(divide(error_rate_by_read,static_cast<float>(total_by_read)));
        error_rate += error_rate_by_read;
        total += total_by_read;

        // We keep track of the throughput for non-index reads
        if(!run[read].read().is_index())
        {
            error_rate_nonindex += error_rate_by_read;
            total_nonindex += total_by_read;
        }
    }
    run.nonindex_summary().error_rate(divide(error_rate_nonindex,static_cast<float>(total_nonindex)));
    run.total_summary().error_rate(divide(error_rate,static_cast<float>(total)));
    run.cycle_state().error_cycle_range(overall_cycle_state);
}

}
}
}
}
