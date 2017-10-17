/** Logic for index metrics
 *
 *  @file
 *  @date  5/26/17
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/index_metric.h"
#include "interop/logic/metric/tile_metric.h"
#include "interop/model/metric_base/metric_set.h"


namespace illumina { namespace interop { namespace logic { namespace metric
{


    /** Populate the unique index sequences while maintaining the order
     *
     * @param tile_metrics tile metric set
     * @param metrics index metric set
     */
    void populate_indices(const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics,
                          model::metric_base::metric_set<model::metrics::index_metric>& metrics);


}}}}
