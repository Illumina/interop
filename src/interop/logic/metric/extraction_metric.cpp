/** Logic for extraction metrics
 *
 *  @file
 *  @date  7/7/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/metric/extraction_metric.h"

namespace illumina { namespace interop { namespace logic { namespace metric
{

    void copy_focus(const model::metric_base::metric_set<model::metrics::extraction_metric>& metrics,
                    float *focus_scores,
                    const size_t channel,
                    const size_t n) throw(model::invalid_parameter, model::index_out_of_bounds_exception)
    {
        typedef model::metric_base::metric_set<model::metrics::extraction_metric>::const_iterator const_iterator;
        if(metrics.size()==0)return;
        if(n < metrics.size()) INTEROP_THROW(model::invalid_parameter, "Buffer size too small for metric set");
        if(channel >= metrics[0].channel_count())
            INTEROP_THROW(model::invalid_parameter, "Channel exceeds channel count");
        for (const_iterator it = metrics.begin(); it != metrics.end(); ++it, ++focus_scores)
            *focus_scores = it->focus_score(channel);
    }


}}}}

