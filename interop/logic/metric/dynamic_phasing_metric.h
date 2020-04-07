/** Logic for dynamic phasing metrics
 *
 *  @file
 *  @date  7/7/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include <set>
#include "interop/model/run/info.h"
#include "interop/model/metrics/dynamic_phasing_metric.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/metrics/phasing_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/logic/summary/map_cycle_to_read.h"


namespace illumina { namespace interop { namespace logic { namespace metric
{

    /** Populate the dynamic phasing metrics (slope & offset for phasing & prephasing) data structure given the phasing metrics data
     *
     * @param phasing_metrics phasing metric set
     * @param cycle_to_read map of cycle to read information
     * @param dynamic_phasing_metrics dynamic phasing metric set (to be populated)
     * @param tile_metrics tile metric set (to be populated)
     */
    void populate_dynamic_phasing_metrics(model::metric_base::metric_set<model::metrics::phasing_metric>& phasing_metrics,
                                          const logic::summary::read_cycle_vector_t& cycle_to_read,
                                          model::metric_base::metric_set<model::metrics::dynamic_phasing_metric>& dynamic_phasing_metrics,
                                          model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics);

}}}}
