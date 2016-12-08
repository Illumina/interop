/** Plot the Q-score histogram
 *
 *  @file
 *  @date 5/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/model/run_metrics.h"
#include "interop/model/plot/filter_options.h"
#include "interop/model/plot/bar_point.h"
#include "interop/logic/plot/plot_data.h"
#include "interop/logic/utils/metrics_to_load.h"

namespace illumina { namespace interop { namespace logic { namespace plot {


    /** Plot a histogram of q-scores
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param options options to filter the data
     * @param data output plot data
     * @param boundary index of bin to create the boundary sub plots (0 means do nothing)
     */
    void plot_qscore_histogram(model::metrics::run_metrics& metrics,
                               const model::plot::filter_options& options,
                               model::plot::plot_data<model::plot::bar_point>& data,
                               const size_t boundary=0)
                                throw( model::invalid_read_exception,
                                model::index_out_of_bounds_exception,
                                model::invalid_filter_option);

}}}}

