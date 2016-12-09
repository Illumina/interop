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

namespace illumina { namespace interop { namespace logic { namespace summary
{
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
    /** Cache the q-value by read/lane or read/lane/surface
     *
     */
    class qval_cache
    {
        typedef std::vector< qval_total > qval_total_vector_t;
        typedef std::vector < qval_total_vector_t > qval_total_vector2d_t;
    private:
        typedef std::vector< size_t > size_vector_t;
        typedef std::vector< size_vector_t > size_vector2d_t;
    private:
        typedef std::vector< std::set<size_t> > tile_lookup_t;

    public:
        /** Constructor
         *
         * @param run run summary
         * @param surface_count number of surfaces
         */
        qval_cache(const model::summary::run_summary &run, const size_t surface_count=1) :
                m_read_lane_cache(run.size(), qval_total_vector_t(run.lane_count()*surface_count)),
                m_metrics_in_read(run.size(), size_vector_t(run.lane_count()*surface_count)),
                m_tile_lookup(run.lane_count()*surface_count),
                m_surface_count(surface_count)
        {}
        /** Accumulate a metric into the cache
         *
         * @param metric collapsed q-metric
         * @param read_number index of the read
         * @param lane index of the lane
         * @param surface surface index
         */
        void add(const model::metrics::q_collapsed_metric& metric,
                 const size_t read_number,
                 const size_t lane,
                 const size_t surface=0)
        {
            INTEROP_ASSERT(read_number < m_read_lane_cache.size());
            const size_t lane_surface_index = index_of(lane, surface);
            INTEROP_ASSERT(lane_surface_index < m_read_lane_cache[read_number].size());
            m_read_lane_cache[read_number][lane_surface_index].above_qval+=metric.q30();
            m_read_lane_cache[read_number][lane_surface_index].total += metric.total();

            INTEROP_ASSERT(lane_surface_index < m_tile_lookup.size());
            m_tile_lookup[lane_surface_index].insert(metric.tile());


            INTEROP_ASSERT(read_number < m_metrics_in_read.size());
            INTEROP_ASSERT(lane_surface_index < m_metrics_in_read[read_number].size());
            m_metrics_in_read[read_number][lane_surface_index] += 1;
        }
        /** Get qval_total struct for given read/lane/surface
         *
         * @param read_number read index
         * @param lane lane index
         * @param surface surface index
         * @return qval_total struct
         */
        const qval_total& total_for(const size_t read_number, const size_t lane, const size_t surface=0)const
        {
            INTEROP_ASSERT(read_number < m_read_lane_cache.size());
            const size_t index = index_of(lane, surface);
            INTEROP_ASSERT(index < m_read_lane_cache[read_number].size());
            return m_read_lane_cache[read_number][index];
        }
        /** Get number of metrics in read/lane/surface
         *
         * @param read_number read index
         * @param lane lane index
         * @param surface surface index
         * @return number of metrics
         */
        size_t metric_count(const size_t read_number, const size_t lane, const size_t surface=0)const
        {
            INTEROP_ASSERT(read_number < m_metrics_in_read.size());
            const size_t index = index_of(lane, surface);
            INTEROP_ASSERT(index < m_metrics_in_read[read_number].size());
            return m_metrics_in_read[read_number][index];
        }
        /** Get number of tiles for lane/surface
         *
         * @param lane lane index
         * @param surface surface index
         * @return number of tiles
         */
        size_t tile_count(const size_t lane, const size_t surface=0)const
        {
            const size_t index = index_of(lane, surface);
            INTEROP_ASSERT(lane < m_tile_lookup.size());
            return m_tile_lookup[index].size();
        }

    private:
        size_t index_of(const size_t lane, const size_t surface)const
        {
            return lane*m_surface_count+surface;
        }

    private:
        qval_total_vector2d_t m_read_lane_cache;
        size_vector2d_t m_metrics_in_read;
        tile_lookup_t m_tile_lookup;
        size_t m_surface_count;
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
    * @param naming_method tile naming convention
    * @param run destination run summary
    */
    template<typename I>
    void summarize_collapsed_quality_metrics(I beg,
                                             I end,
                                             const read_cycle_vector_t& cycle_to_read,
                                             const constants::tile_naming_method naming_method,
                                             model::summary::run_summary &run)
                                             throw( model::index_out_of_bounds_exception )
    {
        typedef model::summary::lane_summary lane_summary;
        if( beg == end ) return;
        if( run.size()==0 )return;
        const size_t surface_count = run.surface_count();
        qval_cache read_lane_cache(run);
        qval_cache read_lane_surface_cache(run, surface_count);

        for(;beg != end;++beg)
        {
            INTEROP_ASSERT(beg->cycle() > 0);
            INTEROP_ASSERT((beg->cycle()-1) < cycle_to_read.size());
            if((beg->cycle()-1) >= cycle_to_read.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Cycle exceeds total cycles from Reads in the RunInfo.xml");
            const size_t read_number = cycle_to_read[beg->cycle()-1].number-1;
            if(cycle_to_read[beg->cycle()-1].is_last_cycle_in_read) continue;
            const size_t lane = beg->lane()-1;
            if(lane >= run.lane_count())
                INTEROP_THROW( model::index_out_of_bounds_exception, "Lane exceeds lane count in RunInfo.xml");
            read_lane_cache.add(*beg, read_number, lane);

            if(surface_count < 2) continue;
            const size_t surface = beg->surface(naming_method);
            INTEROP_ASSERT(surface > 0);
            read_lane_surface_cache.add(*beg, read_number, lane, surface-1);
        }

        ::uint64_t total_useable_calls = 0;
        ::uint64_t useable_calls_gt_q30 = 0;
        float overall_projected_yield = 0;
        float yield_g = 0;

        ::uint64_t total_useable_calls_nonindex = 0;
        ::uint64_t useable_calls_gt_q30_nonindex = 0;
        float projected_yield_nonindex = 0;
        float yield_g_nonindex = 0;
        for(size_t read=0;read<run.size();++read)
        {
            INTEROP_ASSERT(read < run.size());
            const size_t useable_cycles = run[read].read().useable_cycles();
            ::uint64_t total_useable_calls_by_read=0;
            ::uint64_t useable_calls_gt_q30_by_read = 0;
            float read_projected_yield = 0;
            for (size_t lane = 0; lane < run[read].size(); ++lane)
            {
                INTEROP_ASSERT(lane < run[read].size());
                const size_t total_metrics_in_lane_read = read_lane_cache.metric_count(read, lane);
                model::summary::stat_summary& stat_summary=run[read][lane];
                if( total_metrics_in_lane_read > 0)
                {
                    const float prediction_factor = divide(float(useable_cycles*read_lane_cache.tile_count(lane)),
                                                           float(total_metrics_in_lane_read));
                    const qval_total& curr_tot = read_lane_cache.total_for(read, lane);
                    total_useable_calls_by_read += curr_tot.total;
                    useable_calls_gt_q30_by_read += curr_tot.above_qval;
                    stat_summary.percent_gt_q30(curr_tot.percent_above_qval());
                    stat_summary.yield_g(curr_tot.total / 1e9f);
                    const float projected_yield = curr_tot.total * prediction_factor;
                    read_projected_yield += projected_yield;
                    stat_summary.projected_yield_g(
                            ::uint64_t(projected_yield + 0.5f) / 1e9f);
                }
                else
                {
                    float total = 0;
                    float metric_count = 0;
                    for(size_t read_index=0, read_count=read+1;read_index<read_count;++read_index)
                    {
                        if(read_lane_cache.total_for(read_index, lane).total > 0)
                        {
                            total += run[read_index][lane].projected_yield_g();
                            metric_count += static_cast<float>(run[read_index].read().useable_cycles());
                        }
                    }
                    if(metric_count > 0) total /= metric_count;
                    const float projected_yield = total*useable_cycles * 1e9f;
                    stat_summary.projected_yield_g(::uint64_t(projected_yield+0.5f)/ 1e9f);
                    read_projected_yield += projected_yield;
                }
                if(surface_count < 2) continue;
                for(size_t surface=0;surface<surface_count;++surface)
                {
                    const size_t total_in_surface_lane_read = read_lane_surface_cache.metric_count(read, lane, surface);
                    model::summary::stat_summary& stat_summary_surface=run[read][lane][surface];
                    if( total_in_surface_lane_read > 0)
                    {
                        const float prediction_factor = divide(float(useable_cycles*read_lane_surface_cache.tile_count(lane, surface)),
                                                               float(total_metrics_in_lane_read));

                        const qval_total& curr_tot = read_lane_surface_cache.total_for(read, lane, surface);
                        stat_summary_surface.percent_gt_q30(curr_tot.percent_above_qval());
                        stat_summary_surface.yield_g(curr_tot.total / 1e9f);
                        const float projected_yield = curr_tot.total * prediction_factor;
                        stat_summary_surface.projected_yield_g(
                                ::uint64_t(projected_yield + 0.5f) / 1e9f);
                    }
                    else
                    {
                        float total = 0;
                        float metric_count = 0;
                        for(size_t r=0, n=read+1;r<n;++r)
                        {
                            if(read_lane_surface_cache.total_for(r, lane, surface).total > 0)
                            {
                                total += run[r][lane][surface].projected_yield_g();
                                metric_count += static_cast<float>(run[r].read().useable_cycles());
                            }
                        }
                        if(metric_count > 0) total /= metric_count;
                        const float projected_yield = total*useable_cycles * 1e9f;
                        stat_summary_surface.projected_yield_g(::uint64_t(projected_yield+0.5f)/ 1e9f);
                    }
                }
            }
            run[read].summary().projected_yield_g(::uint64_t(read_projected_yield+0.5f)/ 1e9f);
            run[read].summary().yield_g(std::accumulate(run[read].begin(),
                                                        run[read].end(),
                                                        float(0),
                                                        util::op::const_member_function(&lane_summary::yield_g)));
            run[read].summary().percent_gt_q30(100 * divide(float(useable_calls_gt_q30_by_read), float(total_useable_calls_by_read)));
            total_useable_calls += total_useable_calls_by_read;
            useable_calls_gt_q30 += useable_calls_gt_q30_by_read;
            overall_projected_yield += read_projected_yield;
            yield_g += run[read].summary().yield_g();
            // Certain metrics can be biased by the index read, e.g. C1 intensity, total yield
            // So, we include totals that skip the index reads
            if(!run[read].read().is_index())
            {
                total_useable_calls_nonindex += total_useable_calls_by_read;
                useable_calls_gt_q30_nonindex += useable_calls_gt_q30_by_read;
                projected_yield_nonindex += read_projected_yield;
                yield_g_nonindex += run[read].summary().yield_g();
            }
        }
        run.nonindex_summary().projected_yield_g(::uint64_t(projected_yield_nonindex+0.5f)/1e9f);
        run.nonindex_summary().yield_g(yield_g_nonindex);
        run.nonindex_summary().percent_gt_q30(100 * divide(float(useable_calls_gt_q30_nonindex), float(total_useable_calls_nonindex)));

        run.total_summary().projected_yield_g(::uint64_t(overall_projected_yield+0.5f)/1e9f);
        run.total_summary().yield_g(yield_g);
        run.total_summary().percent_gt_q30(100 * divide(float(useable_calls_gt_q30), float(total_useable_calls)));
    }
}}}}

