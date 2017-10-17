/** Logic for extended tile metrics
 *
 *  @file
 *  @date  7/31/2017
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/extended_tile_metric.h"
#include "interop/model/metrics/tile_metric.h"

namespace illumina { namespace interop { namespace logic { namespace metric
{
    /** Populate percent occupied in extended tile metrics
     *
     * @param tile_metrics tile metric set
     * @param extended_tile_metrics destination extended tile metric set
     */
    void populate_percent_occupied(const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics,
                                     model::metric_base::metric_set<model::metrics::extended_tile_metric>& extended_tile_metrics);
}}}}

