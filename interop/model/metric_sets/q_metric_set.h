/** Q-score Metric Set
 *
 *  @file
 *
 *  @date 10/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metric_base/metric_set.h"
#include "interop/constants/enums.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/logic/metric/q_metric.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {


/** Q-score Metric Set
 */
class q_metrics : public metric_base::metric_set<q_metric>
{
    typedef metric_base::metric_set<q_metric> parent_type;

    public:
        /** Unsigned integer type */
        typedef q_metric::uint_t uint_t;
    public:
        /** Constructor
         */
        q_metrics() : parent_type(header_type()) {
        }

        /** Constructor
         *
         * @param vec array of metrics
         * @param version of the metric format
         * @param header header information for the metric set
         */
        q_metrics(const metric_array_t &vec, const ::int16_t version = -1, const header_type &header = header_type()) :
                parent_type(vec, version, header) {
        }

    public:
        /** Get the number of bins for each metric
         *
         * @return number of bins for each metric
         */
        size_t histBinCount() const {
            return logic::metric::count_q_metric_bins(*this);
        }
    };
}
}
}
}

