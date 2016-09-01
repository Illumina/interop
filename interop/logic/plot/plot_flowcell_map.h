/** Plot the flowcell map
 *
 *  @file
 *  @date 5/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/constants/enums.h"
#include "interop/model/run_metrics.h"
#include "interop/model/plot/filter_options.h"
#include "interop/model/plot/flowcell_data.h"
#include "interop/logic/utils/metrics_to_load.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{


    /** Plot a flowcell map
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param type specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output flowcell map
     * @param buffer preallocated memory for data
     * @param tile_buffer preallocated memory for tile ids
     */
    void plot_flowcell_map(model::metrics::run_metrics& metrics,
                                  const constants::metric_type type,
                                  const model::plot::filter_options& options,
                                  model::plot::flowcell_data& data,
                                  float* buffer=0,
                                  ::uint32_t* tile_buffer=0)
                                  throw(model::invalid_filter_option,
                                  model::invalid_metric_type,
                                  model::index_out_of_bounds_exception);

    /** Plot a flowcell map
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param metric_name specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output flowcell map
     * @param buffer preallocated memory for data
     * @param tile_buffer preallocated memory for tile ids
     */
    void plot_flowcell_map(model::metrics::run_metrics& metrics,
                                  const std::string& metric_name,
                                  const model::plot::filter_options& options,
                                  model::plot::flowcell_data& data,
                                  float* buffer=0,
                                  ::uint32_t* tile_buffer=0)
                                  throw(model::invalid_filter_option,
                                  model::invalid_metric_type,
                                  model::index_out_of_bounds_exception);
    /** List metric types available for flowcell
     *
     * @param types destination vector to fill with metric types
     */
    void list_flowcell_metrics(std::vector<constants::metric_type>& types);
    /** List metric type names available for flowcell
     *
     * @param names destination vector to fill with metric type names
     */
    void list_flowcell_metrics(std::vector<std::string>& names);
    /** Calculate the required buffer size
     *
     * @param metrics run metrics
     * @param options options to filter the data
     */
    size_t calculate_flowcell_buffer_size(const model::metrics::run_metrics& metrics,
                                          const model::plot::filter_options& options);


}}}}
