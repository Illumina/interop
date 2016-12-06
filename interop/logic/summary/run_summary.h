/** Summary logic for run metrics
 *
 *  @file
 *  @date  3/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/model_exceptions.h"
#include "interop/model/summary/run_summary.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/utils/metrics_to_load.h"


namespace illumina { namespace interop { namespace logic { namespace summary
{

    /** Summarize a collection run metrics
     *
     * TODO speed up calculation by adding no_median flag
     *
     * @ingroup summary_logic
     * @param metrics source collection of all metrics
     * @param summary destination run summary
     * @param skip_median skip the median calculation
     * @param trim flag indicating whether to trim the summary model (default: true)
     */
    void summarize_run_metrics(model::metrics::run_metrics& metrics,
                               model::summary::run_summary& summary,
                               const bool skip_median=false,
                               const bool trim=true)
    throw( model::index_out_of_bounds_exception, model::invalid_channel_exception );


}}}}

