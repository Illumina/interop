/** Summary logic for error metrics
 *
 *  @file
 *  @date  3/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include "interop/util/map.h"
#include "interop/util/exception.h"
#include "interop/util/length_of.h"
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/summary_statistics.h"
#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/logic/summary/cycle_state_summary.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/summary/run_summary.h"
#include "interop/logic/metric/tile_metric.h"


namespace illumina { namespace interop { namespace logic { namespace summary
{
    /** Cache data structure for error metrics
     *
     */
     class error_cache_element
     {
     public:
         /** Constructor */
         error_cache_element() : m_error_sum(0.0f), m_error_count(0), m_max_cycle(0) {}
         /** Update the cache element
          *
          * @param error_rate error rate
          */
         void update_error(const float error_rate)
         {
             if(std::isnan(error_rate)) return;
             m_error_sum += error_rate;
             m_error_count++;
         }
         /** Update the cache element
          *
          * @param cycle_within_read cycle within read
          */
         void update_cycle(const size_t cycle_within_read)
         {
             m_max_cycle = std::max(m_max_cycle, cycle_within_read);
         }
         /** Average error rate
          *
          * @return average
          */
         float average()const
         {
             return divide(m_error_sum, static_cast<float>(m_error_count));
         }
         /** Get the maximum cycle apart of the average
          *
          * @return maximum cycle
          */
         size_t max_cycle()const
         {
             return m_max_cycle;
         }
         /** Test if there are no error values
          *
          * @return true if no values have been averaged
          */
         bool is_empty()const
         {
             return m_error_count == 0;
         }

     private:
         float m_error_sum;
         size_t m_error_count;
         size_t m_max_cycle;
     };

    /** Cache errors for all tiles up to a give max cycle
     *
     * This function only includes errors from useable cycles (not the last cycle) to up the given max cycle.
     *
     * @param beg iterator to start of a collection of error metrics
     * @param end iterator to end of a collection of error metrics
     * @param max_cycle maximum cycle to take
     * @param cycle_to_read map that takes a cycle and returns the read-number cycle-in-read pair
     * @param naming_method tile naming convention
     * @param read_lane_cache destination cache by read then by lane a collection of errors
     * @param read_lane_surface_cache source cache by read then by lane then by surface a collection of errors
     */
    template<typename I>
    void cache_error_by_lane_read(I beg,
                                  I end,
                                  const size_t max_cycle,
                                  const std::vector<read_cycle> &cycle_to_read,
                                  const constants::tile_naming_method naming_method,
                                  summary_by_lane_read<float> &read_lane_cache,
                                  summary_by_lane_read<float> &read_lane_surface_cache)
    throw(model::index_out_of_bounds_exception)
    {
        typedef std::pair<size_t, size_t> key_t;
        typedef INTEROP_ORDERED_MAP(key_t, error_cache_element) error_tile_t;
        typedef std::vector<error_tile_t> error_by_read_tile_t;
        error_by_read_tile_t tmp(read_lane_cache.size());
        for (; beg != end; ++beg)
        {
            INTEROP_ASSERT(beg->cycle() > 0);
            INTEROP_ASSERT((beg->cycle() - 1) < cycle_to_read.size());
            if ((beg->cycle() - 1) >= cycle_to_read.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Cycle exceeds total cycles from Reads in the RunInfo.xml");
            const read_cycle &read = cycle_to_read[beg->cycle() - 1];
            const key_t key = std::make_pair(beg->lane(), beg->tile());
            const size_t read_number = read.number - 1;
            INTEROP_ASSERTMSG(read_number < tmp.size(),
                              read.number << " " << read.cycle_within_read << ", " << beg->cycle());
            tmp[read_number][key].update_cycle(read.cycle_within_read);
            if (read.cycle_within_read > max_cycle || read.is_last_cycle_in_read) continue;
            tmp[read_number][key].update_error(beg->error_rate());
        }
        for (size_t read = 0; read < tmp.size(); ++read)
        {
            for (typename error_tile_t::const_iterator ebeg = tmp[read].begin(), eend = tmp[read].end();
                 ebeg != eend; ++ebeg)
            {
                INTEROP_ASSERT(read < read_lane_cache.read_count());
                const size_t lane = ebeg->first.first - 1;
                if (lane >= read_lane_cache.lane_count())
                    INTEROP_THROW(model::index_out_of_bounds_exception, "Lane exceeds number of lanes in RunInfo.xml");
                if(max_cycle < std::numeric_limits<size_t>::max() && ebeg->second.max_cycle() < max_cycle) continue;
                if(ebeg->second.is_empty()) continue;
                const float err_avg = ebeg->second.average();
                read_lane_cache(read, lane).push_back(err_avg);
                if(read_lane_surface_cache.surface_count() < 2) continue;
                const ::uint32_t surface = logic::metric::surface(static_cast< ::uint32_t >(ebeg->first.second), naming_method);
                INTEROP_ASSERT(surface <= read_lane_surface_cache.surface_count());
                INTEROP_ASSERT(surface > 0);
                read_lane_surface_cache(read, lane, surface-1).push_back(err_avg);
            }
        }
    }

    /** Calculate summary statistics for each collection of metrics organized by read and lane
     *
     * @param read_lane_cache source cache by read then by lane a collection of errors
     * @param read_lane_surface_cache source cache by read then by lane then by surface a collection of errors
     * @param run destination run summary
     * @param func member function pointer to metric
     * @param skip_median skip the median calculation
     */
    inline void error_summary_from_cache(summary_by_lane_read<float> &read_lane_cache,
                                         summary_by_lane_read<float> &read_lane_surface_cache,
                                         model::summary::run_summary &run,
                                         void (model::summary::stat_summary::*func )(const model::summary::metric_stat&),
                                         const bool skip_median=false)
    {
        for (size_t read = 0; read < run.size(); ++read)
        {
            INTEROP_ASSERT(read < read_lane_cache.read_count());
            INTEROP_ASSERT(read < run.size());
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                INTEROP_ASSERT(lane < run[read].size());
                INTEROP_ASSERT(lane < read_lane_cache.lane_count());
                model::summary::metric_stat stat;
                summarize(read_lane_cache(read, lane).begin(),
                          read_lane_cache(read, lane).end(),
                          stat,
                          skip_median);
                (run[read][lane].*func)(stat);
                if(read_lane_surface_cache.surface_count() < 2) continue;
                for(size_t surface=0;surface<read_lane_surface_cache.surface_count();++surface)
                {
                    stat.clear();
                    summarize(read_lane_surface_cache(read, lane, surface).begin(),
                              read_lane_surface_cache(read, lane, surface).end(),
                           stat,
                           skip_median);
                    (run[read][lane][surface].*func)(stat);
                }
            }
        }
    }

    /** Summarize a collection error metrics
     *
     * @sa model::summary::stat_summary::error_rate
     * @sa model::summary::stat_summary::error_rate_35
     * @sa model::summary::stat_summary::error_rate_50
     * @sa model::summary::stat_summary::error_rate_75
     * @sa model::summary::stat_summary::error_rate_100
     *
     * @sa model::summary::read_summary::error_rate
     *
     * @sa model::summary::run_summary::error_rate
     *
     * @todo: Refactor this to make simpler
     *
     * @param beg iterator to start of a collection of error metrics
     * @param end iterator to end of a collection of error metrics
     * @param cycle_to_read map cycle to the read number and cycle within read number
     * @param naming_method tile naming convention
     * @param run destination run summary
     * @param skip_median skip the median calculation
     */
    template<typename I>
    void summarize_error_metrics(I beg,
                                 I end,
                                 const read_cycle_vector_t &cycle_to_read,
                                 const constants::tile_naming_method naming_method,
                                 model::summary::run_summary &run,
                                 const bool skip_median=false) throw(model::index_out_of_bounds_exception)
    {
        typedef summary_by_lane_read<float> summary_by_lane_read_t;
        typedef void (model::summary::stat_summary::*error_functor_t )(const model::summary::metric_stat&);
        typedef std::pair<size_t, error_functor_t> cycle_functor_pair_t;

        if (beg == end) return;
        if (run.size() == 0) return;
        const size_t surface_count = run.surface_count();
        summary_by_lane_read_t read_lane_cache(run, std::distance(beg, end));
        summary_by_lane_read_t read_lane_surface_cache(run, std::distance(beg, end), surface_count);

        cycle_functor_pair_t cycle_functor_pairs[] = {
                cycle_functor_pair_t(35u, &model::summary::stat_summary::error_rate_35),
                cycle_functor_pair_t(50u, &model::summary::stat_summary::error_rate_50),
                cycle_functor_pair_t(75u, &model::summary::stat_summary::error_rate_75),
                cycle_functor_pair_t(100u, &model::summary::stat_summary::error_rate_100),
        };
        for (size_t i = 0; i < util::length_of(cycle_functor_pairs); ++i)
        {
            cache_error_by_lane_read(beg,
                                     end,
                                     cycle_functor_pairs[i].first,
                                     cycle_to_read,
                                     naming_method,
                                     read_lane_cache,
                                     read_lane_surface_cache);
            error_summary_from_cache(read_lane_cache,
                                     read_lane_surface_cache,
                                     run,
                                     cycle_functor_pairs[i].second,
                                     skip_median);
            read_lane_cache.clear();
            read_lane_surface_cache.clear();
        }


        cache_error_by_lane_read(beg,
                                 end,
                                 std::numeric_limits<size_t>::max(),
                                 cycle_to_read,
                                 naming_method,
                                 read_lane_cache,
                                 read_lane_surface_cache);

        float error_rate = 0;
        size_t total = 0;
        float error_rate_nonindex = 0;
        size_t total_nonindex = 0;
        for (size_t read = 0; read < run.size(); ++read)
        {
            INTEROP_ASSERT(read < run.size());
            float error_rate_by_read = 0;
            size_t total_by_read = 0;
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                INTEROP_ASSERT(lane < run[read].size());
                model::summary::metric_stat error_stat;
                summarize(read_lane_cache(read, lane).begin(),
                          read_lane_cache(read, lane).end(),
                          error_stat,
                          skip_median);
                run[read][lane].error_rate(error_stat);
                error_rate_by_read += std::accumulate(read_lane_cache(read, lane).begin(),
                                                      read_lane_cache(read, lane).end(),
                                                      float(0));
                total_by_read += read_lane_cache(read, lane).size();
                if(surface_count < 2) continue;
                for(size_t surface=0;surface<surface_count;++surface)
                {
                    error_stat.clear();
                    summarize(read_lane_surface_cache(read, lane, surface).begin(),
                              read_lane_surface_cache(read, lane, surface).end(),
                              error_stat,
                              skip_median);
                    run[read][lane][surface].error_rate(error_stat);
                }
            }
            if (total_by_read > 0)
                run[read].summary().error_rate(divide(error_rate_by_read, static_cast<float>(total_by_read)));
            error_rate += error_rate_by_read;
            total += total_by_read;

            // We keep track of the throughput for non-index reads
            if (!run[read].read().is_index())
            {
                error_rate_nonindex += error_rate_by_read;
                total_nonindex += total_by_read;
            }
        }
        run.nonindex_summary().error_rate(divide(error_rate_nonindex, static_cast<float>(total_nonindex)));
        run.total_summary().error_rate(divide(error_rate, static_cast<float>(total)));
    }

}}}}

