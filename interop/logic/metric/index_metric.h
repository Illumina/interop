/** Logic for index metrics
 *
 *  @file
 *  @date  5/26/17
 *  @version 1.0
 *  @copyright Illumina Use Only
 */
#pragma once
#include "interop/model/metrics/index_metric.h"
#include "interop/model/metric_base/metric_set.h"


namespace illumina { namespace interop { namespace logic { namespace metric
{


    /** Populate the unique index sequences while maintaining the order
     *
     * @param metrics index metric set
     */
    void populate_indices(model::metric_base::metric_set<model::metrics::index_metric>& metrics);


}}}}
