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
     * @param type specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output flowcell map
     * @param buffer preallocated memory for data
     * @param buffer_size size of the buffer
     * @param id_buffer preallocated memory for tile ids
     * @param id_buffer_size size of the buffer
     */
    inline void plot_flowcell_map2(model::metrics::run_metrics& metrics,
                           const constants::metric_type type,
                           const model::plot::filter_options& options,
                           model::plot::flowcell_data& data,
                           float* buffer,
                            const size_t buffer_size,
                           ::uint32_t* id_buffer,
                           const size_t id_buffer_size)
            throw(model::invalid_filter_option,
            model::invalid_metric_type,
            model::index_out_of_bounds_exception)
    {
        (void)buffer_size;
        (void)id_buffer_size;
        plot_flowcell_map(metrics, type, options, data, buffer, id_buffer);
    }

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
    /** Plot a flowcell map
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param metric_name specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output flowcell map
     * @param buffer preallocated memory for data
     * @param buffer_size size of the buffer
     * @param id_buffer preallocated memory for tile ids
     * @param id_buffer_size size of the buffer
     */
    inline void plot_flowcell_map2(model::metrics::run_metrics& metrics,
                           const std::string& metric_name,
                           const model::plot::filter_options& options,
                           model::plot::flowcell_data& data,
                           float* buffer,
                            const size_t buffer_size,
                           ::uint32_t* id_buffer,
                            const size_t id_buffer_size)
            throw(model::invalid_filter_option,
            model::invalid_metric_type,
            model::index_out_of_bounds_exception)
    {
        (void)buffer_size;
        (void)id_buffer_size;
        plot_flowcell_map(metrics, metric_name, options, data, buffer, id_buffer);
    }

    /** List metric type names available for flowcell
     *
     * @param types destination vector to fill with metric type names
     * @param ignore_accumulated exclude accumulated q-metrics
     */
    void list_flowcell_metrics(std::vector< logic::utils::metric_type_description_t > &types,
                               const bool ignore_accumulated=false);

    /** Calculate the required buffer size
     *
     * @param metrics run metrics
     * @param options options to filter the data
     */
    size_t calculate_flowcell_buffer_size(const model::metrics::run_metrics& metrics,
                                          const model::plot::filter_options& options);


}}}}

