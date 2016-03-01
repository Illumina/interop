/** Extraction Metric Set
 *
 *  @file
 *
 *  @date  10/9/15.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/extraction_metric.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {


/** Extraction Metric Set
 *
 */
class extraction_metrics : public metric_base::metric_set<extraction_metric> {
    typedef metric_base::metric_set<extraction_metric> parent_type;
public:
    /** Constructor
     */
    extraction_metrics() : parent_type(header_type()) { }

    /** Constructor
     *
     * @param vec array of metrics
     * @param version of the metric format
     * @param header header information for the metric set
     */
    extraction_metrics(const metric_array_t& vec, const ::int16_t version=-1, const header_type& header=header_type()) :
            parent_type(vec, version, header) {
    }

public:
    /** Example used for the performance test in C#
     *
     * This shows a simple way to speed up analysis of data using the C# binding.
     *
     * @param focus_scores array of focus scores
     * @param channel channel for focus
     * @param n size of array
     */
    void copy_focus(float* focus_scores, size_t channel, size_t n)
    {
        float* focus_scores_end = focus_scores+n;
        for(parent_type::const_iterator b=parent_type::metrics().begin(), e=parent_type::metrics().end();b != e && focus_scores != focus_scores_end;++b, ++focus_scores)
            *focus_scores = b->focusScore(channel);
    }
};
}
}
}
}

