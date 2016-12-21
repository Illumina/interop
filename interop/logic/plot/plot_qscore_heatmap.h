/** Plot the Q-score heat map
 *
 *  @file
 *  @date 5/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/model/run_metrics.h"
#include "interop/model/plot/filter_options.h"
#include "interop/model/plot/heatmap_data.h"
#include "interop/logic/utils/metrics_to_load.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{
    /** Plot a heat map of q-scores
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param options options to filter the data
     * @param data output heat map data
     * @param buffer optional buffer of preallocated memory (for SWIG)
     * @param buffer_size number of elements in buffer
     */
    void plot_qscore_heatmap(model::metrics::run_metrics& metrics,
                                    const model::plot::filter_options& options,
                                    model::plot::heatmap_data& data,
                                    float* buffer=0,
                                    const size_t buffer_size=0)
                                    throw(model::index_out_of_bounds_exception,
                                    model::invalid_filter_option);
    /** Count number of rows for the heat map
     *
     * @param metrics run metrics
     * @return number of rows
     */
    size_t count_rows_for_heatmap(const model::metrics::run_metrics& metrics);
    /** Count number of columns for the heat map
     *
     * @param metrics run metrics
     * @return number of columns
     */
    size_t count_columns_for_heatmap(const model::metrics::run_metrics& metrics);


}}}}

