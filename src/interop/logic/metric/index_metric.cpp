/** Logic for index metrics
 *
 *  @file
 *  @date  5/26/17
 *  @version 1.0
 *  @copyright Illumina Use Only
 */
#include "interop/logic/metric/index_metric.h"
#include "interop/util/map.h"

#include <vector>
#include <set>

namespace illumina { namespace interop { namespace logic { namespace metric
{
    /** Look up map type for id to offset mapping */
    typedef INTEROP_UNORDERED_MAP(model::metric_base::base_metric::id_t, size_t) lookup_map_t;
    /** Build a mapping from the metric ID to its position in the metric set
     *
     * @tparam I iterator type
     * @param b iterator to start of collection
     * @param e iterator to end of collection
     * @param id_lookup_map map of ID to offset in collection
     */
    template<typename I>
    void build_index_map(I b, I e, lookup_map_t& id_lookup_map)
    {
        size_t offset = 0;
        for (;b != e; ++b)
        {
            id_lookup_map[b->tile_hash()] = offset;
            ++offset;
        }
    }
    /** Populate the unique index sequences while maintaining the order
     *
     * @param tile_metrics tile metric set
     * @param metrics index metric set
     */
    void populate_indices(const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics,
                          model::metric_base::metric_set<model::metrics::index_metric>& metrics)
    {
        typedef model::metric_base::metric_set<model::metrics::index_metric>::iterator iterator;
        typedef model::metrics::index_metric::const_iterator const_index_iterator;
        if(metrics.empty() || !metrics.index_order().empty()) return;
        std::vector<std::string> ordered;
        ordered.reserve(metrics.size()*metrics[0].size());
        std::set<std::string> unique;

        lookup_map_t id_lookup_map;
        build_index_map(tile_metrics.begin(), tile_metrics.end(), id_lookup_map);
        for(iterator it = metrics.begin();it != metrics.end();++it)
        {
            for(const_index_iterator indexIt = it->begin();indexIt != it->end();++indexIt)
            {
                if(unique.find(indexIt->unique_id()) != unique.end()) continue;
                unique.insert(indexIt->unique_id());
                ordered.push_back(indexIt->unique_id());
            }
            const model::metric_base::base_metric::id_t tile_hash = it->tile_hash();
            lookup_map_t::const_iterator lookup_it = id_lookup_map.find(tile_hash);
            if(lookup_it != id_lookup_map.end())
            {
                it->set_cluster_counts(tile_metrics[lookup_it->second].cluster_count(),
                                          tile_metrics[lookup_it->second].cluster_count_pf());
            }
        }
        metrics.index_order(ordered);
    }


}}}}

