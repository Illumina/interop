/** Plot an arbitrary metric by lane
 *
 *  @file
 *  @date 5/1/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/constants/enums.h"
#include "interop/model/run_metrics.h"
#include "interop/model/plot/filter_options.h"
#include "interop/model/plot/plot_data.h"
#include "interop/model/plot/candle_stick_point.h"
#include "interop/logic/utils/metrics_to_load.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{

    /** Plot a specified metric value by lane
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param type specific metric value to plot by lane
     * @param options options to filter the data
     * @param data output plot data
     */
    void plot_by_lane(const model::metrics::run_metrics& metrics,
                      const constants::metric_type type,
                      const model::plot::filter_options& options,
                      model::plot::plot_data<model::plot::candle_stick_point>& data)
                    throw(model::index_out_of_bounds_exception,
                    model::invalid_metric_type,
                    model::invalid_filter_option);

    /** Plot a specified metric value by cycle
     *
     * @ingroup plot_logic
     * @todo Is this temporary?
     * @param metrics run metrics
     * @param metric_name name of metric value to plot by cycle
     * @param options options to filter the data
     * @param data output plot data
     */
    void plot_by_lane(const model::metrics::run_metrics& metrics,
                      const std::string& metric_name,
                      const model::plot::filter_options& options,
                      model::plot::plot_data<model::plot::candle_stick_point>& data)
            throw(model::index_out_of_bounds_exception,
            model::invalid_metric_type,
            model::invalid_filter_option);

    /** List metric types available for by lane plots
     *
     * @param types destination vector to fill with metric types
     * @param ignore_pf if true, ignore density PF and cluster PF
     */
    void list_by_lane_metrics(std::vector< logic::utils::metric_type_description_t > &types, const bool ignore_pf=false);


}}}}

