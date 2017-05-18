/** List all metrics for plotting
 *
 *  @file
 *  @date  5/8/17
 *  @version 1.0
 *  @copyright Illumina Use Only
 */
#pragma once
#include "interop/model/run_metrics.h"
#include "interop/constants/enums.h"
#include "interop/logic/utils/enums.h"
#include "interop/logic/utils/metric_type_ext.h"
#include "interop/logic/utils/metrics_to_load.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{
    /** List metric types available for all plots
     *
     * @param types destination vector to fill with metric types
     */
    void list_plot_metrics(std::vector< logic::utils::metric_type_description_t > &types);

    /** List all available metric types for all plots
     *
     * That is, the metric must be populated
     *
     * @param metrics run metrics object
     * @param types destination vector to fill with metric types
     */
    void list_available_plot_metrics(const model::metrics::run_metrics& metrics,
                                     std::vector< logic::utils::metric_type_description_t > &types);
}}}}

