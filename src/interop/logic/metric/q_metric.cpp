/** Logic for q-metrics
 *
 *  @file
 *  @date  3/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <vector>
#include "interop/util/map.h"
#include "interop/logic/metric/q_metric.h"


namespace illumina { namespace interop { namespace logic { namespace metric
{
    /** Populate cumulative q-metric distribution
     *
     * @param metric_set q-metric set
     */
    template<class QMetric>
    bool populate_cumulative_distribution_sorted(model::metric_base::metric_set<QMetric>& metric_set)
    INTEROP_THROW_SPEC(( model::index_out_of_bounds_exception ))
    {
        if(metric_set.size()==0) return true;
        typedef model::metric_base::base_metric::id_t id_t;
        typedef typename model::metric_base::metric_set<QMetric>::iterator iterator;
        typedef INTEROP_UNORDERED_MAP(id_t, iterator) lookup_map_t;

        lookup_map_t tile_id_map;
        for(iterator beg = metric_set.begin();beg != metric_set.end();++beg)
        {
            const id_t tile_id = beg->tile_hash();
            if(tile_id_map.find(tile_id) == tile_id_map.end())
            {
                tile_id_map[tile_id] = beg;
                beg->accumulate(*beg);
            }
            else if(tile_id_map[tile_id]->cycle() >= beg->cycle())
            {
                return false;
            }
            else
            {
                beg->accumulate(*tile_id_map[tile_id]);
                tile_id_map[tile_id] = beg;
            }

        }
        return true;
    }
    namespace detail
    {
        template<class QMetric>
        struct by_cycle
        {
            bool operator()(const QMetric &lhs, const QMetric &rhs) const
            {
                return lhs.cycle() < rhs.cycle();
            }
        };
    }

    /** Populate cumulative q-metric distribution
     *
     * @param metric_set q-metric set
     */
    template<class QMetric>
    void populate_cumulative_distribution_t(model::metric_base::metric_set<QMetric>& metric_set)
    INTEROP_THROW_SPEC(( model::index_out_of_bounds_exception ))
    {
        if(metric_set.size()==0) return;
        if(!populate_cumulative_distribution_sorted(metric_set))
        {
            std::sort(metric_set.begin(), metric_set.end(), detail::by_cycle<QMetric>());
            metric_set.clear_lookup();
            populate_cumulative_distribution_sorted(metric_set);
        }
    }

    /** Populate cumulative by lane q-metric distribution
     *
     * @param q_metric_set q-metric set
     */
    void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
    INTEROP_THROW_SPEC(( model::index_out_of_bounds_exception ))
    {
        populate_cumulative_distribution_t(q_metric_set);
    }
    /** Populate cumulative q-metric distribution
     *
     * @note This can exist here or in SWIG. This is a swig interface function.
     * @param q_metric_set q-metric set
     */
    void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set)
    INTEROP_THROW_SPEC(( model::index_out_of_bounds_exception ))
    {
        populate_cumulative_distribution_t(q_metric_set);
    }
    /** Populate cumulative cpllapsed q-metric distribution
     *
     * @note This can exist here or in SWIG. This is a swig interface function.
     * @param q_metric_set q-metric set
     */
    void populate_cumulative_distribution(model::metric_base::metric_set<model::metrics::q_collapsed_metric>& q_metric_set)
    INTEROP_THROW_SPEC(( model::index_out_of_bounds_exception ))
    {
        populate_cumulative_distribution_t(q_metric_set);
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
                                      const size_t count)
    {
        typedef model::metrics::q_score_bin q_score_bin;
        if(!q_score_bins.empty())return;
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
    /** Generate collapsed Q-metric data from Q-metrics
     *
     * @param metric_set q-metric set
     * @param collapsed collapsed Q-metrics
     */
    void create_collapse_q_metrics(const model::metric_base::metric_set<model::metrics::q_metric>& metric_set,
                                   model::metric_base::metric_set<model::metrics::q_collapsed_metric>& collapsed)
    {
        typedef model::metric_base::metric_set<model::metrics::q_metric>::const_iterator const_iterator;
        typedef model::metric_base::metric_set<model::metrics::q_metric>::uint_t uint_t;

        const uint_t q20_idx = static_cast<uint_t>(index_for_q_value(metric_set, 20));
        const uint_t q30_idx = static_cast<uint_t>(index_for_q_value(metric_set, 30));

        collapsed.set_version(model::metrics::q_collapsed_metric::LATEST_VERSION);
        for(const_iterator beg = metric_set.begin(), end = metric_set.end();beg != end;++beg)
        {
            const uint_t q20 = beg->total_over_qscore(q20_idx);
            const uint_t q30 = beg->total_over_qscore(q30_idx);
            const uint_t total = beg->sum_qscore();
            const uint_t median = beg->median(metric_set.get_bins());
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
     * @note This function is only used for unit testing
     * @param metric_set Q-metrics
     * @param bylane bylane Q-metrics
     * @throws index_out_of_bounds_exception
     */
    void create_q_metrics_by_lane_base(const model::metric_base::metric_set<model::metrics::q_metric>& metric_set,
                                  model::metric_base::metric_set<model::metrics::q_by_lane_metric>& bylane)
    INTEROP_THROW_SPEC((model::index_out_of_bounds_exception))
    {
        typedef model::metric_base::metric_set<model::metrics::q_metric>::const_iterator const_iterator;
        typedef model::metric_base::metric_set<model::metrics::q_metric>::header_type header_type;
        typedef model::metric_base::base_cycle_metric::id_t id_t;
        typedef INTEROP_UNORDERED_MAP(id_t, size_t) lookup_map_t;
        typedef lookup_map_t::iterator lookup_iterator;

        lookup_map_t tile_id_map;
        bylane = static_cast<const header_type&>(metric_set);
        for(const_iterator beg = metric_set.begin(), end = metric_set.end();beg != end;++beg)
        {
            const id_t id = model::metric_base::base_cycle_metric::create_id(beg->lane(), 0, beg->cycle());
            lookup_iterator it = tile_id_map.find(id);
            if(it == tile_id_map.end())
            {
                tile_id_map[id] = bylane.size();
                bylane.insert(model::metrics::q_by_lane_metric(beg->lane(), 0, beg->cycle(), beg->qscore_hist()));
            }
            else
            {
                bylane[it->second].accumulate_by_lane(*beg);
            }
        }
    }

    /** Generate by lane Q-metric data from Q-metrics
     *
     * @param metric_set Q-metrics
     * @param bylane bylane Q-metrics
     * @throws index_out_of_bounds_exception
     */
    void create_q_metrics_by_lane(const model::metric_base::metric_set<model::metrics::q_metric>& metric_set,
                                  model::metric_base::metric_set<model::metrics::q_by_lane_metric>& bylane,
                                  const constants::instrument_type instrument)
    INTEROP_THROW_SPEC((model::index_out_of_bounds_exception))
    {
        create_q_metrics_by_lane_base(metric_set, bylane);
        const size_t bin_count = logic::metric::count_legacy_q_score_bins(bylane);
        if(requires_legacy_bins(bin_count))
        {
            populate_legacy_q_score_bins(bylane.bins(), instrument, bin_count);
            compress_q_metrics(bylane);
        }
        bylane.set_version(model::metrics::q_by_lane_metric::LATEST_VERSION);
    }

    /** Compress the q-metric set using the bins in the header
     *
     * @param q_metric_set q-metric set
     */
    template<class QMetric>
    void compress_q_metrics_t(model::metric_base::metric_set<QMetric>& q_metric_set)
    {
        typedef model::metric_base::metric_set<QMetric> metric_set_t;
        if(q_metric_set.empty()) return;
        if(logic::metric::is_compressed(q_metric_set) || q_metric_set.bin_count() == 0 ) return;
        for(typename metric_set_t::iterator it = q_metric_set.begin();it != q_metric_set.end();++it)
        {
            it->compress(q_metric_set);
        }
    }

    /** Compress the q-metric set using the bins in the header
     *
     * @param q_metric_set q-metric set
     */
    void compress_q_metrics(model::metric_base::metric_set<model::metrics::q_metric>& q_metric_set)
    {
        compress_q_metrics_t(q_metric_set);
    }
    /** Compress the q-metric set using the bins in the header
     *
     * @param q_metric_set q-metric set
     */
    void compress_q_metrics(model::metric_base::metric_set<model::metrics::q_by_lane_metric>& q_metric_set)
    {
        compress_q_metrics_t(q_metric_set);
    }

}}}}

