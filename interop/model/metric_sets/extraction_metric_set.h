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
};
}
}
}
}

