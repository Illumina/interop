/** Plot reads identified versus index
 *
 *  @file
 *  @date 5/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/constants/enums.h"
#include "interop/model/run_metrics.h"
#include "interop/model/plot/bar_point.h"
#include "interop/logic/plot/plot_data.h"
#include "interop/logic/utils/metrics_to_load.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{


    /** Plot reads identified versus index
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param lane lane number
     * @param data output plot data
     */
    void plot_sample_qc(const model::metrics::run_metrics& metrics,
                               const size_t lane,
                               model::plot::plot_data<model::plot::bar_point>& data)
                                throw(model::index_out_of_bounds_exception);


}}}}

