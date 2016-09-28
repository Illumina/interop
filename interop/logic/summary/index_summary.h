/** Summary logic for index metrics
 *
 *  @file
 *  @date  5/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/model_exceptions.h"
#include "interop/model/summary/index_flowcell_summary.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/utils/metrics_to_load.h"


namespace illumina { namespace interop { namespace logic { namespace summary
{

    /** Summarize a collection index metrics for a specific lane
     *
     * @param metrics source run metrics
     * @param lane lane number
     * @param summary destination index lane summary
     */
    void summarize_index_metrics(const model::metrics::run_metrics &metrics,
                                        const size_t lane,
                                        model::summary::index_lane_summary &summary)
                                        throw(model::index_out_of_bounds_exception);
    /** Summarize a collection index metrics
     *
     * @ingroup summary_logic
     * @param index_metrics source collection of index metrics
     * @param tile_metrics source collection of tile metrics
     * @param lane_count number of lanes
     * @param summary destination index flowcell summary
     */
    void summarize_index_metrics(const model::metric_base::metric_set<model::metrics::index_metric>& index_metrics,
                                        const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics,
                                        const size_t lane_count,
                                        model::summary::index_flowcell_summary &summary)
                                        throw(model::index_out_of_bounds_exception);

    /** Summarize index metrics from run metrics
     *
     * @ingroup summary_logic
     * @param metrics source collection of all metrics
     * @param summary destination index flowcell summary
     */
    void summarize_index_metrics(const model::metrics::run_metrics &metrics,
                                        model::summary::index_flowcell_summary &summary)
                                            throw(model::index_out_of_bounds_exception);
}}}}

