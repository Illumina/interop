/** Logic for index metrics
 *
 *  @file
 *  @date  5/26/17
 *  @version 1.0
 *  @copyright Illumina Use Only
 */
#include "interop/logic/metric/index_metric.h"

#include <vector>
#include <set>

namespace illumina { namespace interop { namespace logic { namespace metric
{


    /** Populate the unique index sequences while maintaining the order
     *
     * @param metrics index metric set
     */
    void populate_indices(model::metric_base::metric_set<model::metrics::index_metric>& metrics)
    {
        typedef model::metric_base::metric_set<model::metrics::index_metric>::const_iterator const_iterator;
        typedef model::metrics::index_metric::const_iterator const_index_iterator;
        if(metrics.empty() || !metrics.index_order().empty())return;
        std::vector<std::string> ordered;
        ordered.reserve(metrics.size()*metrics[0].size());
        std::set<std::string> unique;
        for(const_iterator it = metrics.begin();it != metrics.end();++it)
        {
            for(const_index_iterator indexIt = it->begin();indexIt != it->end();++indexIt)
            {
                if(unique.find(indexIt->unique_id()) != unique.end()) continue;
                unique.insert(indexIt->unique_id());
                ordered.push_back(indexIt->unique_id());
            }
        }
        metrics.index_order(ordered);
    }


}}}}

