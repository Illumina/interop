/** Logic for q-metrics
 *
 *  @file
 *  @date  3/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include <set>
#include "interop/model/model_exceptions.h"
#include "interop/constants/enums.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/model/metric_base/metric_set.h"


namespace illumina { namespace interop { namespace logic { namespace metric {

    /** Count the number of bins in the q metric
     *
     * @param beg iterator to start of q_metric collection
     * @param end iterator to end of q_metric collection
     * @return number of bins
     */
    template<typename I>
    size_t count_q_metric_bins(I beg, I end)
    {
        return (beg==end) ? 0 : beg->size();
    }
    /** Count the number of bins in the q metric
     *
     * @param set q_metric set
     * @return number of bins
     */
    inline size_t count_q_metric_bins(const model::metric_base::metric_set<model::metrics::q_metric>& set)
    {
        return count_q_metric_bins(set.begin(), set.end());
    }
    /** Populate cumulative q-metric distribution
     *
     * @param metrics q-metric set
     */
    inline void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_metric>& metrics)
                                                    throw( model::index_out_of_bounds_exception )
    {
        if(metrics.size()==0) return;
        typedef model::metrics::q_metric q_metric;
        typedef model::metric_base::metric_set<q_metric> q_metric_set;
        typedef q_metric_set::id_vector id_vector;
        typedef q_metric_set::uint_t uint_t;
        id_vector lane_ids = metrics.lanes();
        for(id_vector::const_iterator lane_beg = lane_ids.begin(), lane_end = lane_ids.end();lane_beg != lane_end;++lane_beg)
        {
            id_vector tile_ids = metrics.tile_numbers_for_lane(*lane_beg);
            for(id_vector::const_iterator tile_beg = tile_ids.begin(), tile_end = tile_ids.end();tile_beg != tile_end;++tile_beg)
            {
                if(! metrics.has_metric(*lane_beg, *tile_beg, 1)) continue;
                q_metric& metric = metrics.get_metric_ref(*lane_beg, *tile_beg, 1);
                metric.accumulate(metric);
                const uint_t second_cycle_start = 2; // We have to accumulate the first cycle with itself, and every
                                                     // subsequent with the previous cycle.
                                                     // Also this is not 0-indexed, so we start with 2, the 2nd cycle
                for(uint_t cycle = second_cycle_start;cycle <= metrics.max_cycle();++cycle)
                {   // TODO: if this happens zero out following q-scores
                    if(!metrics.has_metric(*lane_beg, *tile_beg, cycle) ||
                            !metrics.has_metric(*lane_beg, *tile_beg, cycle-1)) continue;
                    metrics.get_metric_ref(*lane_beg, *tile_beg, cycle).accumulate(
                            metrics.get_metric_ref(*lane_beg, *tile_beg, cycle-1)
                    );
                }
            }
        }
    }
    /** Count number of unique counts to determine number
     * of unique bins for legacy binning
     *
     * @Note, if the number of bins is greater than 7, than this function stops counting!
     *
     * @param metrics q-metric set
     * @return number of unique bins
     */
    inline size_t count_legacy_q_score_bins(const model::metric_base::metric_set<model::metrics::q_metric>& metrics)
    {
        // 0 is a sentinel that indicates legacy binning is not required
        if(metrics.version() > 4) return 0;     // Version 5 and later do not require legacy binning
        if(!metrics.bins().empty()) return 0;   // If the metrics already have a header they do not require binning

        const size_t max_bin_count = 7;
        model::metric_base::metric_set<model::metrics::q_metric>::const_iterator beg = metrics.begin(),end=metrics.end();
        if(beg == end) return 0 ;
        typedef model::metrics::q_metric::uint_t uint_t;
        std::set<uint_t> bins_found;
        for(;beg != end;++beg)
        {
            for(uint_t i=0;i<static_cast<uint_t>(beg->qscoreHist().size());++i)
                if(beg->qscoreHist()[i] > 0) bins_found.insert(i);
            if(bins_found.size() > max_bin_count) break; // Number of bins greater than 7 indicates this is unbinned
        }
        return bins_found.size();
    }
    /** Test if legacy binning should be performed
     *
     * @param count number of bins
     * @return true if determining the bins is required
     */
    inline bool requires_legacy_bins(const size_t count)
    {
        return count <= 7 && count > 0;
    }
    /** Populate the q-score header bins from the data
     *
     * This only for legacy platforms that use older q-metric formats, which do not include bin information
     * in the header.
     *
     * @param q_score_bins vector of q-score bins
     * @param instrument instrument type
     * @param count number of bins
     */
    inline void populate_legacy_q_score_bins(std::vector<model::metrics::q_score_bin>& q_score_bins,
                                             const constants::instrument_type instrument,
                                             const size_t count)
    {
        typedef model::metrics::q_score_bin q_score_bin;
        if(!requires_legacy_bins(count)) return;
        q_score_bins.reserve(count);
        if(instrument == constants::NextSeq)
        {
            q_score_bins.push_back(q_score_bin(0, 9, 8));
            q_score_bins.push_back(q_score_bin(10, 19, 13));
            q_score_bins.push_back(q_score_bin(20, 24, 22));
            q_score_bins.push_back(q_score_bin(25, 29, 27));
            q_score_bins.push_back(q_score_bin(30, 34, 32));
            q_score_bins.push_back(q_score_bin(35, 39, 37));
        }
        else if(count == 7)
        {
            q_score_bins.push_back(q_score_bin(0,   9,  6));
            q_score_bins.push_back(q_score_bin(10, 19, 15));
            q_score_bins.push_back(q_score_bin(20, 24, 22));
            q_score_bins.push_back(q_score_bin(25, 29, 27));
            q_score_bins.push_back(q_score_bin(30, 34, 33));
            q_score_bins.push_back(q_score_bin(35, 39, 37));
            q_score_bins.push_back(q_score_bin(40, 49, 40));
        }
        else if(count == 6)
        {
            q_score_bins.push_back(q_score_bin(0, 9, 7));
            q_score_bins.push_back(q_score_bin(10, 19, 16));
            q_score_bins.push_back(q_score_bin(20, 26, 24));
            q_score_bins.push_back(q_score_bin(27, 29, 29));
            q_score_bins.push_back(q_score_bin(30, 34, 33));
            q_score_bins.push_back(q_score_bin(35, 49, 38));
        }
        else if(count == 5)
        {
            q_score_bins.push_back(q_score_bin(0,   9, 7));
            q_score_bins.push_back(q_score_bin(10, 19, 16));
            q_score_bins.push_back(q_score_bin(20, 29, 25));
            q_score_bins.push_back(q_score_bin(30, 34, 33));
            q_score_bins.push_back(q_score_bin(35, 49, 38));
        }
        else if(count == 4)
        {
            q_score_bins.push_back(q_score_bin(0,   9, 7));
            q_score_bins.push_back(q_score_bin(10, 29, 20));
            q_score_bins.push_back(q_score_bin(30, 34, 33));
            q_score_bins.push_back(q_score_bin(35, 49, 38));
        }
        else if(count == 3)
        {
            q_score_bins.push_back(q_score_bin(0,   9, 7));
            q_score_bins.push_back(q_score_bin(10, 29, 20));
            q_score_bins.push_back(q_score_bin(30, 49, 36));
        }
        else if(count == 2)
        {
            q_score_bins.push_back(q_score_bin(0, 27, 13));
            q_score_bins.push_back(q_score_bin(28, 49, 35));
        }
        else
        {
            q_score_bins.push_back(q_score_bin(0, 50, 20));
        }
    }
    /** Populate the q-score header bins from the data
     *
     * This only for legacy platforms that use older q-metric formats, which do not include bin information
     * in the header.
     *
     * @param metrics q_metric set
     * @param q_score_bins vector of q-score bins
     * @param instrument type
     */
    inline void populate_legacy_q_score_bins(model::metric_base::metric_set<model::metrics::q_metric>& metrics,
                                             std::vector<model::metrics::q_score_bin>& q_score_bins,
                                             const constants::instrument_type instrument)
    {
        const size_t count = count_legacy_q_score_bins(metrics);
        populate_legacy_q_score_bins(q_score_bins, instrument, count);
    }
    /** Test whether the q-values are compressed
     *
     * @param set q_metric set
     * @return number of q-vals
     */
    inline size_t count_qvals(const model::metric_base::metric_set<model::metrics::q_metric>& metrics)
    {
        return metrics.size() > 0 ? metrics.metrics()[0].size() : 0;
    }
    /** Test whether the q-values are compressed
     *
     * @param set q_metric set
     * @return true if the histogram is compressed (no all zero columns)
     */
    inline bool is_compressed(const model::metric_base::metric_set<model::metrics::q_metric>& metrics)
    {
        const size_t q_val_count = count_qvals(metrics);
        return q_val_count > 0 && q_val_count != 50;
    }
    /** Determine the maximum Q-value
     *
     * @param set q_metric set
     * @return the maximum Q-value
     */
    inline size_t max_qval(const model::metric_base::metric_set<model::metrics::q_metric>& metrics)
    {
        return is_compressed(metrics) ? static_cast<size_t>(metrics.bins().back().upper()) : count_qvals(metrics);
    }
    /** Get the index for the given q-value
     *
     * @param metrics q_metric set
     * @param qval threshold
     * @return index of q-val above given threshold
     */
    inline size_t index_for_q_value(const model::metric_base::metric_set<model::metrics::q_metric>& metrics,
                                    const size_t qval)
    {
        if(!is_compressed(metrics)) return qval;
        return metrics.index_for_q_value(qval);
    }
}}}}
