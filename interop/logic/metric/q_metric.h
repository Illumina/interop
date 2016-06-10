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
#include "interop/model/metrics/q_metric.h"
#include "interop/model/metrics/q_collapsed_metric.h"
#include "interop/model/metrics/q_by_lane_metric.h"
#include "interop/model/model_exceptions.h"
#include "interop/constants/enums.h"
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
     * @param q_metric_set q-metric set
     * @return number of bins
     */
    inline size_t count_q_metric_bins(const model::metric_base::metric_set<model::metrics::q_metric>& metric_set)
    {
        return count_q_metric_bins(metric_set.begin(), metric_set.end());
    }
    /** Count the number of bins in the q metric
     *
     * @param q_metric_set q-metric set
     * @return number of bins
     */
    inline size_t count_q_metric_bins(const model::metric_base::metric_set<model::metrics::q_by_lane_metric>& metric_set)
    {
        return count_q_metric_bins(metric_set.begin(), metric_set.end());
    }
    /** Count the number of bins in the q metric
     *
     * @note Always returns 0
     * @param q_metric_set q-metric set
     * @return 0
     */
    inline size_t count_q_metric_bins(const model::metric_base::metric_set<model::metrics::q_collapsed_metric>&)
    {
        return 0;
    }
    /** Populate cumulative q-metric distribution
     *
     * @param metric_set q-metric set
     */
    template<class QMetric>
    void populate_cumulative_distribution_t(model::metric_base::metric_set<QMetric>& metric_set)
                                                    throw( model::index_out_of_bounds_exception )
    {
        if(metric_set.size()==0) return;
        typedef QMetric q_metric;
        typedef model::metric_base::metric_set<q_metric> q_metric_set;
        typedef typename q_metric_set::id_vector id_vector;
        typedef typename q_metric_set::uint_t uint_t;
        typedef typename id_vector::const_iterator const_id_iterator;
        id_vector lane_ids = metric_set.lanes();
        for(const_id_iterator lane_beg = lane_ids.begin(), lane_end = lane_ids.end();lane_beg != lane_end;++lane_beg)
        {
            const uint_t lane_id = *lane_beg;
            id_vector tile_ids = metric_set.tile_numbers_for_lane(lane_id);
            for(const_id_iterator tile_beg = tile_ids.begin(), tile_end = tile_ids.end();tile_beg != tile_end;++tile_beg)
            {
                const uint_t tile_id = *tile_beg;
                size_t prev_idx = metric_set.find(lane_id, tile_id, 1);
                if(prev_idx >= metric_set.size()) continue;
                QMetric& metric = metric_set.at(prev_idx);
                metric.accumulate(metric);
                const uint_t second_cycle_start = 2; // We have to accumulate the first cycle with itself, and every
                                                     // subsequent with the previous cycle.
                                                     // Also this is not 0-indexed, so we start with 2, the 2nd cycle
                for(uint_t cycle = second_cycle_start;cycle <= metric_set.max_cycle();++cycle)
                {
                    const size_t cur_idx = metric_set.find(lane_id, tile_id, cycle);
                    if(cur_idx>=metric_set.size() || prev_idx>=metric_set.size())
                        continue;// TODO: if this happens zero out following q-scores
                    metric_set.at(cur_idx).accumulate(metric_set.at(prev_idx));
                    prev_idx=cur_idx;
                }
            }
        }
    }
    /** Populate cumulative by lane q-metric distribution
     *
     * @param q_metric_set q-metric set
     */
    inline void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
                                                                                throw( model::index_out_of_bounds_exception )
    {
        if(q_metric_set.size()==0) return;
        typedef model::metrics::q_by_lane_metric q_by_lane_metric;
        typedef model::metric_base::metric_set<q_by_lane_metric> q_by_lane_metric_set;
        typedef q_by_lane_metric_set::id_vector id_vector;
        typedef q_by_lane_metric_set::uint_t uint_t;
        id_vector lane_ids = q_metric_set.lanes();
        const size_t tile_id = 0;
        for(id_vector::const_iterator lane_beg = lane_ids.begin(), lane_end = lane_ids.end();lane_beg != lane_end;++lane_beg)
        {
            size_t prev_idx = q_metric_set.find(*lane_beg, tile_id, 1);
            if(prev_idx >= q_metric_set.size()) continue;
            q_by_lane_metric& metric = q_metric_set.at(prev_idx);
            metric.accumulate(metric);
            const uint_t second_cycle_start = 2; // We have to accumulate the first cycle with itself, and every
            // subsequent with the previous cycle.
            // Also this is not 0-indexed, so we start with 2, the 2nd cycle
            for(uint_t cycle = second_cycle_start;cycle <= q_metric_set.max_cycle();++cycle)
            {
                const size_t cur_idx = q_metric_set.find(*lane_beg, tile_id, cycle);
                if(cur_idx>=q_metric_set.size() || prev_idx>=q_metric_set.size())
                    continue;// TODO: if this happens zero out following q-scores
                q_metric_set.at(cur_idx).accumulate(q_metric_set.at(prev_idx));
                prev_idx=cur_idx;
            }
        }
    }
    /** Populate cumulative q-metric distribution
     *
     * @note This can exist here or in SWIG. This is a swig interface function.
     * @param q_metric_set q-metric set
     */
    inline void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set)
                    throw( model::index_out_of_bounds_exception )
    {
        populate_cumulative_distribution_t(q_metric_set);
    }
    /** Populate cumulative cpllapsed q-metric distribution
     *
     * @note This can exist here or in SWIG. This is a swig interface function.
     * @param q_metric_set q-metric set
     */
    inline void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_collapsed_metric>& q_metric_set)
    throw( model::index_out_of_bounds_exception )
    {
        populate_cumulative_distribution_t(q_metric_set);
    }
    /** Count number of unique counts to determine number
     * of unique bins for legacy binning
     *
     * @Note, if the number of bins is greater than 7, than this function stops counting!
     *
     * @param q_metric_set q-metric set
     * @return number of unique bins
     */
    inline size_t count_legacy_q_score_bins(const model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set)
    {
        // 0 is a sentinel that indicates legacy binning is not required
        if(q_metric_set.version() > 4) return 0;     // Version 5 and later do not require legacy binning
        if(!q_metric_set.bins().empty()) return 0;   // If the metrics already have a header they do not require binning

        const size_t max_bin_count = 7;
        model::metric_base::metric_set<model::metrics::q_metric>::const_iterator beg = q_metric_set.begin(),
                end=q_metric_set.end();
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
     * @param q_metric_set q-metric set
     * @param q_score_bins vector of q-score bins
     * @param instrument type
     */
    inline void populate_legacy_q_score_bins(model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set,
                                             std::vector<model::metrics::q_score_bin>& q_score_bins,
                                             const constants::instrument_type instrument)
    {
        const size_t count = count_legacy_q_score_bins(q_metric_set);
        populate_legacy_q_score_bins(q_score_bins, instrument, count);
    }
    /** Populate the q-score header bins from the data
     *
     * This only for legacy platforms that use older q-metric formats, which do not include bin information
     * in the header.
     *
     * @param q_metric_set q-metric set
     * @param q_score_bins vector of q-score bins
     * @param instrument type
     */
    inline void populate_legacy_q_score_bins(model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set,
                                             std::vector<model::metrics::q_score_bin>& q_score_bins,
                                             const constants::instrument_type instrument)
    {
        const size_t count = count_legacy_q_score_bins(q_metric_set);
        populate_legacy_q_score_bins(q_score_bins, instrument, count);
    }
    /** Populate the q-score header bins from the data
     *
     * This only for legacy platforms that use older q-metric formats, which do not include bin information
     * in the header.
     *
     * @param q_metric_set q-metric set
     * @param q_score_bins vector of q-score bins
     * @param instrument type
     */
    inline void populate_legacy_q_score_bins(model::metric_base::metric_set<model::metrics::q_collapsed_metric>& q_metric_set,
                                             std::vector<model::metrics::q_score_bin>& q_score_bins,
                                             const constants::instrument_type instrument)
    {
        const size_t count = count_legacy_q_score_bins(q_metric_set);
        populate_legacy_q_score_bins(q_score_bins, instrument, count);
    }
    /** Test whether the q-values are compressed
     *
     * @param q_metric_set q-metric set
     * @return number of q-vals
     */
    inline size_t count_qvals(const model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set)
    {
        return q_metric_set.size() > 0 ? q_metric_set.metrics()[0].size() : 0;
    }
    /** Test whether the q-values are compressed
     *
     * @param q_metric_set by lane q-metric set
     * @return number of q-vals
     */
    inline size_t count_qvals( const model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
    {
        return q_metric_set.size() > 0 ? q_metric_set.metrics()[0].size() : 0;
    }
    /** Test whether the q-values are compressed
     *
     * @note This always returns 0
     * @return 0
     */
    inline size_t count_qvals( const model::metric_base::metric_set<model::metrics::q_collapsed_metric>&)
    {
        return 0;
    }
    /** Test whether the q-values are compressed
     *
     * @param q_metric_set q-metric set
     * @return true if the histogram is compressed (no all zero columns)
     */
    inline bool is_compressed(const model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set)
    {
        const size_t q_val_count = count_qvals(q_metric_set);
        return q_val_count > 0 && q_val_count != model::metrics::q_metric::MAX_Q_BINS;
    }
    /** Test whether the q-values are compressed
     *
     * @param q_metric_set collapsed q-metric set
     * @return true if the histogram is compressed (no all zero columns)
     */
    inline bool is_compressed(const model::metric_base::metric_set<model::metrics::q_collapsed_metric>& q_metric_set)
    {
        const size_t q_val_count = count_qvals(q_metric_set);
        return q_val_count > 0 && q_val_count != model::metrics::q_metric::MAX_Q_BINS;
    }
    /** Test whether the q-values are compressed
     *
     * @param q_metric_set by lane q_metric set
     * @return true if the histogram is compressed (no all zero columns)
     */
    inline bool is_compressed(const model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
    {
        const size_t q_val_count = count_qvals(q_metric_set);
        return q_val_count > 0 && q_val_count != model::metrics::q_metric::MAX_Q_BINS;
    }
    /** Determine the maximum Q-value
     *
     * @param q_metric_set q-metric set
     * @return the maximum Q-value
     */
    inline size_t max_qval(
            const model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set)
    {
        return is_compressed(q_metric_set) ? q_metric_set.bins().back().upper() : count_qvals(q_metric_set);
    }
    /** Determine the maximum Q-value
     *
     * @param q_metric_set by lane q-metric set
     * @return the maximum Q-value
     */
    inline size_t max_qval(const model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
    {
        return is_compressed(q_metric_set) ? q_metric_set.bins().back().upper() : count_qvals(q_metric_set);
    }
    /** Determine the maximum Q-value
     *
     * @param q_metric_set collapsed q-metric set
     * @return the maximum Q-value
     */
    inline size_t max_qval(const model::metric_base::metric_set<model::metrics::q_collapsed_metric>& q_metric_set)
    {
        return is_compressed(q_metric_set) ?
               static_cast<size_t>(q_metric_set.bins().back().upper()) : count_qvals(q_metric_set);
    }
    /** Get the index for the given q-value
     *
     * @param q_metric_set q-metric set
     * @param qval threshold
     * @return index of q-val above given threshold
     */
    inline size_t index_for_q_value(
            const model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set,
            const size_t qval)
    {
        if(!is_compressed(q_metric_set)) return qval-1;
        return q_metric_set.index_for_q_value(qval);
    }
    /** Generate collapsed Q-metric data from Q-metrics
     *
     * @param metric_set q-metric set
     * @param collapsed collapsed Q-metrics
     */
    inline void create_collapse_q_metrics(const model::metric_base::metric_set<model::metrics::q_metric>& metric_set,
                                          model::metric_base::metric_set<model::metrics::q_collapsed_metric>& collapsed)
    {
        typedef model::metric_base::metric_set<model::metrics::q_metric>::const_iterator const_iterator;
        typedef model::metric_base::metric_set<model::metrics::q_metric>::uint_t uint_t;

        const uint_t q20_idx = static_cast<uint_t>(index_for_q_value(metric_set, 20));
        const uint_t q30_idx = static_cast<uint_t>(index_for_q_value(metric_set, 30));
        for(const_iterator beg = metric_set.begin(), end = metric_set.end();beg != end;++beg)
        {
            const uint_t q20 = beg->total_over_qscore(q20_idx);
            const uint_t q30 = beg->total_over_qscore(q30_idx);
            const uint_t total = beg->sum_qscore();
            const uint_t median = beg->median(metric_set.bins());
            collapsed.insert(model::metrics::q_collapsed_metric(beg->lane(),
                                                                beg->tile(),
                                                                beg->cycle(),
                                                                q20,
                                                                q30,
                                                                total,
                                                                median));
        }
    }
    /** Generate by lane Q-metric data from Q-metrics
     *
     * @param metrics Q-metrics
     * @param collapsed collapsed Q-metrics
     */
    inline void create_q_metrics_by_lane(const model::metric_base::metric_set<model::metrics::q_metric>& metrics,
                                         model::metric_base::metric_set<model::metrics::q_by_lane_metric>& bylane)
                                        throw(model::index_out_of_bounds_exception)
    {
        typedef model::metric_base::metric_set<model::metrics::q_metric>::const_iterator const_iterator;
        typedef model::metric_base::metric_set<model::metrics::q_metric>::header_type header_type;
        typedef model::metric_base::base_cycle_metric::id_t id_t;

        bylane = static_cast<const header_type&>(metrics);
        for(const_iterator beg = metrics.begin(), end = metrics.end();beg != end;++beg)
        {
            const id_t id = model::metric_base::base_cycle_metric::id(beg->lane(), 0, beg->cycle());
            if(bylane.has_metric(id))
                bylane.get_metric_ref(id).accumulate_by_lane(*beg);
            else
                bylane.insert(model::metrics::q_by_lane_metric(beg->lane(), 0, beg->cycle(), beg->qscore_hist()));
        }
    }
}}}}
