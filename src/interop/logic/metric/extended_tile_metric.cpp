/** Logic for extended tile metrics
 *
 *  @file
 *  @date  7/31/2017
 *  @version 1.0
 *  @copyright GNU Public License
 */
#include "interop/logic/metric/extended_tile_metric.h"
#include "interop/util/map.h"

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
            id_lookup_map[b->id()] = offset;
            ++offset;
        }
    }
    /** Populate percent occupied in extended tile metrics
     *
     * @param tile_metrics tile metric set
     * @param extended_tile_metrics destination extended tile metric set
     */
    void populate_percent_occupied(const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics,
                                   model::metric_base::metric_set<model::metrics::extended_tile_metric>& extended_tile_metrics)
    {
        typedef model::metric_base::metric_set<model::metrics::extended_tile_metric>::iterator extended_tile_iterator;
        lookup_map_t id_lookup_map;
        build_index_map(tile_metrics.begin(), tile_metrics.end(), id_lookup_map);
        for(extended_tile_iterator beg = extended_tile_metrics.begin();beg != extended_tile_metrics.end();++beg)
        {
            beg->set_cluster_count_k(tile_metrics[id_lookup_map[beg->id()]].cluster_count_k());
        }
    }

}}}}

