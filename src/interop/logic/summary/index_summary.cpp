/** Summary logic for index metrics
 *
 *  @file
 *  @date  5/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/summary/index_summary.h"
#include "interop/logic/metric/index_metric.h"

#include "interop/util/statistics.h"


namespace illumina { namespace interop { namespace logic { namespace summary {

    /** Summarize a index metrics for a specific lane
     *
     * @param index_metrics set of index metrics
     * @param tile_metrics source collection of tile metrics
     * @param lane lane number
     * @param summary destination index flowcell summary
     */
    void summarize_index_metrics(model::metric_base::metric_set<model::metrics::index_metric>& index_metrics,
                                 const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics,
                                 const size_t lane,
                                 model::summary::index_lane_summary &summary)
    throw(model::index_out_of_bounds_exception)
    {
        typedef model::metric_base::metric_set<model::metrics::index_metric>::const_iterator const_iterator;
        typedef model::summary::index_lane_summary::read_count_t read_count_t;
        typedef model::metrics::index_metric::const_iterator const_index_iterator;
        typedef model::summary::index_count_summary index_count_summary;
        typedef INTEROP_UNORDERED_MAP(std::string, index_count_summary) index_count_map_t;
        typedef index_count_map_t::iterator map_iterator;
        const size_t kAllLanes = 0;

        summary.clear();
        if(index_metrics.empty() || tile_metrics.empty()) return;
        logic::metric::populate_indices(tile_metrics, index_metrics);
        index_count_map_t index_count_map;
        ::uint64_t total_mapped_reads = 0;
        read_count_t pf_cluster_count_total = 0;
        read_count_t cluster_count_total = 0;
        for(const_iterator beg = index_metrics.begin();beg != index_metrics.end();++beg)
        {
            if(lane != kAllLanes && beg->lane() != lane) continue;
            if(std::isnan(beg->cluster_count()) || std::isnan(beg->cluster_count_pf()))continue; // TODO: check better
            pf_cluster_count_total += static_cast<read_count_t>(beg->cluster_count_pf());
            cluster_count_total += static_cast<read_count_t>(beg->cluster_count());

            for(const_index_iterator ib = beg->indices().begin(), ie = beg->indices().end();ib != ie;++ib)
            {
                map_iterator found_index = index_count_map.find(ib->unique_id());
                if(found_index == index_count_map.end())
                {
                    index_count_map[ib->unique_id()] = index_count_summary(index_count_map.size()+1,// TODO: get correspondence with plot
                                                                           ib->index1(),
                                                                           ib->index2(),
                                                                           ib->sample_id(),
                                                                           ib->sample_proj(),
                                                                           ib->cluster_count());
                }
                else
                {
                    found_index->second += ib->cluster_count();
                }
                total_mapped_reads += ib->cluster_count();
            }
        }


        float max_fraction_mapped = -std::numeric_limits<float>::max();
        float min_fraction_mapped = std::numeric_limits<float>::max();
        if(!index_count_map.empty())
        {
            summary.reserve(index_count_map.size());
            for(std::vector<std::string>::const_iterator kcurr = index_metrics.index_order().begin(), kbeg=kcurr;
                kcurr != index_metrics.index_order().end();++kcurr)
            {
                if(index_count_map.find(*kcurr) == index_count_map.end()) continue;
                index_count_summary& count_summary = index_count_map[*kcurr];
                count_summary.id(static_cast<size_t>(std::distance(kbeg, kcurr)+1));
                count_summary.update_fraction_mapped(static_cast<double>(pf_cluster_count_total));
                const float fraction_mapped = count_summary.fraction_mapped();
                summary.push_back(count_summary);
                max_fraction_mapped = std::max(max_fraction_mapped, fraction_mapped);
                min_fraction_mapped = std::min(min_fraction_mapped, fraction_mapped);
            }
        }

        const float avg_fraction_mapped =util::mean<float>(summary.begin(),
                                                           summary.end(),
                                                           util::op::const_member_function(&index_count_summary::fraction_mapped));
        const float std_fraction_mapped =
                std::sqrt(util::variance_with_mean<float>(summary.begin(),
                                                          summary.end(),
                                                          avg_fraction_mapped,
                                                          util::op::const_member_function(&index_count_summary::fraction_mapped)));
        summary.set(total_mapped_reads,
                    pf_cluster_count_total,
                    cluster_count_total,
                    min_fraction_mapped,
                    max_fraction_mapped,
                    std_fraction_mapped/avg_fraction_mapped);
    }
    /** Summarize a collection index metrics for a specific lane
     *
     * @param metrics source run metrics
     * @param lane lane number
     * @param summary destination index lane summary
     */
    void summarize_index_metrics(model::metrics::run_metrics &metrics,
                                        const size_t lane,
                                        model::summary::index_lane_summary &summary)
    throw(model::index_out_of_bounds_exception)
    {
        summarize_index_metrics(metrics.get<model::metrics::index_metric>(),
                                metrics.get<model::metrics::tile_metric>(),
                                lane,
                                summary);
    }
    /** Summarize a collection index metrics
     *
     * @ingroup summary_logic
     * @param index_metrics source collection of index metrics
     * @param tile_metrics source collection of tile metrics
     * @param lane_count number of lanes
     * @param summary destination index flowcell summary
     */
    void summarize_index_metrics(model::metric_base::metric_set<model::metrics::index_metric>& index_metrics,
                                        const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics,
                                        const size_t lane_count,
                                        model::summary::index_flowcell_summary &summary)
    throw(model::index_out_of_bounds_exception)
    {
        if(index_metrics.empty() || tile_metrics.empty()) return;
        summary.resize(lane_count);
        for(size_t lane=1;lane <= lane_count;++lane)
        {
            summarize_index_metrics(index_metrics, tile_metrics, lane, summary[lane-1]);
        }
    }

    /** Summarize index metrics from run metrics
     *
     * @ingroup summary_logic
     * @param metrics source collection of all metrics
     * @param summary destination index flowcell summary
     */
    void summarize_index_metrics(model::metrics::run_metrics &metrics,
                                        model::summary::index_flowcell_summary &summary)
    throw(model::index_out_of_bounds_exception)
    {
        const size_t lane_count = metrics.run_info().flowcell().lane_count();
        summarize_index_metrics(metrics.get<model::metrics::index_metric>(),
                                metrics.get<model::metrics::tile_metric>(),
                                lane_count,
                                summary);
    }

}}}}

