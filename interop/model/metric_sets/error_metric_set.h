/** Error Metric Set
 *
 *  @file
 *
 *  @date 10/8/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/error_metric.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {

/** Error Metric Set
 *
 * The error metric is the calculated error rate, as determined by a spiked in PhiX control sample.
 * This metric is available for each lane and tile for every cycle.
 *
 * @deprecated Will be removed in 1.1.x (use metric_set<error_metric> instead)
 * @see metric_base::metric_set
 * @see error_metric
 */
class error_metrics : public metric_base::metric_set<error_metric>
{
    typedef metric_base::metric_set<error_metric> parent_type;
public:
    /** Constructor
     */
    error_metrics() : parent_type(header_type())
    { }

    /** Constructor
     *
     * @param vec array of metrics
     * @param version of the metric format
     * @param header header information for the metric set
     */
    error_metrics(const metric_array_t &vec, const ::int16_t version = -1, const header_type &header = header_type()) :
            parent_type(vec, version, header)
    {
    }
};
}
}
}
}

