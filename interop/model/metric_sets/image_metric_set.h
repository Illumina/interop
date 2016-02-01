/** Image Metric Set
 *
 *  @file
 *
 *  @date 10/9/15.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/image_metric.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {



/** Image Metric Set
 */
class image_metrics : public metric_base::metric_set<image_metric>
{
    typedef metric_base::metric_set<image_metric> parent_type;
public:
    /** Constructor
     */
    image_metrics() : parent_type(header_type(image_metric::MAX_CHANNELS))
    { }

    /** Constructor
     *
     * @param vec array of metrics
     * @param version of the metric format
     * @param header header information for the metric set
     */
    image_metrics(const metric_array_t &vec, const ::int16_t version = -1, const header_type &header = header_type(image_metric::MAX_CHANNELS)) :
            parent_type(vec, version, header)
    {
    }
};
}
}
}
}

