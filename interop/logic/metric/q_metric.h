/** Logic for q-metrics
 *
 *  @file
 *  @date  3/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "interop/model/metrics/q_metric.h"
#include "interop/model/metrics/q_collapsed_metric.h"
#include "interop/model/metrics/q_by_lane_metric.h"
#include "interop/model/model_exceptions.h"
#include "interop/model/metric_base/metric_set.h"


namespace illumina { namespace interop { namespace logic { namespace metric
{

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
     * @param metric_set q-metric set
     * @return number of bins
     */
    inline size_t count_q_metric_bins(const model::metric_base::metric_set<model::metrics::q_metric>& metric_set)
    {
        return count_q_metric_bins(metric_set.begin(), metric_set.end());
    }
    /** Count the number of bins in the q metric
     *
     * @param metric_set q-metric set
     * @return number of bins
     */
    inline size_t count_q_metric_bins(const model::metric_base::metric_set<model::metrics::q_by_lane_metric>& metric_set)
    {
        return count_q_metric_bins(metric_set.begin(), metric_set.end());
    }
    /** Count the number of bins in the q metric
     *
     * @note Always returns 0
     * @return 0
     */
    inline size_t count_q_metric_bins(const model::metric_base::metric_set<model::metrics::q_collapsed_metric>&)
    {
        return 0;
    }
    /** Populate cumulative by lane q-metric distribution
     *
     * @param q_metric_set q-metric set
     */
    void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
                                                                                throw( model::index_out_of_bounds_exception );
    /** Populate cumulative q-metric distribution
     *
     * @note This can exist here or in SWIG. This is a swig interface function.
     * @param q_metric_set q-metric set
     */
    void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set)
                    throw( model::index_out_of_bounds_exception );
    /** Populate cumulative cpllapsed q-metric distribution
     *
     * @note This can exist here or in SWIG. This is a swig interface function.
     * @param q_metric_set q-metric set
     */
    void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_collapsed_metric>& q_metric_set)
                    throw( model::index_out_of_bounds_exception );
    /** Count number of unique counts to determine number
     * of unique bins for legacy binning
     *
     * @note, if the number of bins is greater than 7, than this function stops counting!
     *
     * @param q_metric_set q-metric set
     * @return number of unique bins
     */
    size_t count_legacy_q_score_bins(const model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set);
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
    void populate_legacy_q_score_bins(std::vector<model::metrics::q_score_bin>& q_score_bins,
                                             const constants::instrument_type instrument,
                                             const size_t count);
    /** Compress the q-metric set using the bins in the header
     *
     * @param q_metric_set q-metric set
     */
    void compress_q_metrics(model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set);
    /** Compress the q-metric set using the bins in the header
     *
     * @param q_metric_set q-metric set
     */
    void compress_q_metrics(model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set);
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
        return q_metric_set.size() > 0 ? q_metric_set[0].size() : 0;
    }
    /** Test whether the q-values are compressed
     *
     * @param q_metric_set by lane q-metric set
     * @return number of q-vals
     */
    inline size_t count_qvals( const model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
    {
        return q_metric_set.size() > 0 ? q_metric_set[0].size() : 0;
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
        return is_compressed(q_metric_set) ? q_metric_set.get_bins().back().upper() : count_qvals(q_metric_set);
    }
    /** Determine the maximum Q-value
     *
     * @param q_metric_set by lane q-metric set
     * @return the maximum Q-value
     */
    inline size_t max_qval(const model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
    {
        return is_compressed(q_metric_set) ? q_metric_set.get_bins().back().upper() : count_qvals(q_metric_set);
    }
    /** Determine the maximum Q-value
     *
     * @param q_metric_set collapsed q-metric set
     * @return the maximum Q-value
     */
    inline size_t max_qval(const model::metric_base::metric_set<model::metrics::q_collapsed_metric>& q_metric_set)
    {
        return is_compressed(q_metric_set) ?
               static_cast<size_t>(q_metric_set.get_bins().back().upper()) : count_qvals(q_metric_set);
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
    /** Get the index for the given q-value
     *
     * @param q_metric_set by lane q-metric set
     * @param qval threshold
     * @return index of q-val above given threshold
     */
    inline size_t index_for_q_value(
            const model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set,
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
    void create_collapse_q_metrics(const model::metric_base::metric_set<model::metrics::q_metric>& metric_set,
                                          model::metric_base::metric_set<model::metrics::q_collapsed_metric>& collapsed);
    /** Generate by lane Q-metric data from Q-metrics
     *
     * @param metric_set Q-metrics
     * @param bylane bylane Q-metrics
     * @throws index_out_of_bounds_exception
     */
    void create_q_metrics_by_lane(const model::metric_base::metric_set<model::metrics::q_metric>& metric_set,
                                         model::metric_base::metric_set<model::metrics::q_by_lane_metric>& bylane)
                                        throw(model::index_out_of_bounds_exception);
}}}}

