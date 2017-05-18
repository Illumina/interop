/** List all metrics for plotting
 *
 *  @file
 *  @date  5/8/17
 *  @version 1.0
 *  @copyright Illumina Use Only
 */
#include "interop/logic/plot/plot_metric_list.h"
//
#include "interop/logic/plot/plot_metric_proxy.h"


namespace illumina { namespace interop { namespace logic { namespace plot
{
    /** List metric types available for all plots
     *
     * @param types destination vector to fill with metric types
     */
    void list_plot_metrics(std::vector<logic::utils::metric_type_description_t> &types)
    {
        utils::list_descriptions(types);
    }

    /** List all available metric types for all plots
     *
     * That is, the metric must be populated
     *
     * @param types destination vector to fill with metric types
     */
    void list_available_plot_metrics(const model::metrics::run_metrics &metrics,
                                     std::vector<logic::utils::metric_type_description_t> &types)
    {
        if(types.empty()) utils::list_descriptions(types);
        std::vector< logic::utils::metric_type_description_t >::iterator dst = types.begin();
        for(std::vector< logic::utils::metric_type_description_t >::iterator src = types.begin();src != types.end();++src)
        {
            const constants::metric_type type = *src;
            if(!plot_metric_proxy::is_present(metrics, type))continue;
            if(src != dst) std::swap(*src, *dst);
            ++dst;
        }
        types.resize(std::distance(types.begin(), dst));
    }
}}}}
