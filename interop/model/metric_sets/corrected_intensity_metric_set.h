/** Corrected Intensity Metric Set
 *
 *  @file
 *
 *  @date 10/8/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/corrected_intensity_metric.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {

/** Corrected Intensity Metric Set
 *
 * @deprecated Will be removed in 1.1.x (use metric_set<corrected_intensity_metric> instead)
 * @see metric_base::metric_set
 * @see corrected_intensity_metric
 */
class corrected_intensity_metrics : public metric_base::metric_set<corrected_intensity_metric>
{
    typedef metric_base::metric_set<corrected_intensity_metric> parent_type;
public:
    /** Constructor
     */
    corrected_intensity_metrics() : parent_type(header_type())
    {
    }

    /** Constructor
     *
     * @param vec array of metrics
     * @param version of the metric format
     * @param header header information for the metric set
     */
    corrected_intensity_metrics(const metric_array_t &vec, const ::int16_t version = -1, const header_type &header = header_type()) :
            parent_type(vec, version, header)
    {
    }
};
}
}
}
}
