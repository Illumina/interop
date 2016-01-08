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
    /** Estimate the median cycle time for extraction
     *
     * @return median cycle time for extraction
     */
    double medianCycleTime() const
    {
        std::vector<double> times;
        times.reserve(parent_type::size());
        typedef parent_type::metric_type::ulong_t ulong_t;
        parent_type::metric_array_t::const_iterator beg = parent_type::metrics().begin();
        parent_type::metric_array_t::const_iterator end = parent_type::metrics().end();
        if (beg != end) end--;
        for (; beg != end;) {
            ulong_t startTime = beg->dateTime();
            beg++;
            if (startTime == std::numeric_limits<ulong_t>::min()) continue;
            ulong_t endTime = beg->dateTime();
            if (endTime == std::numeric_limits<ulong_t>::min()) continue;
            double minutes = std::difftime(static_cast<std::time_t>(startTime),
                                           static_cast<std::time_t>(endTime)) / 60.0;
            times.push_back(minutes);
        }
        if (times.size() == 0) return -1;
        size_t medianIndex = times.size() / 2;
        std::nth_element(times.begin(), times.begin() + medianIndex, times.end());
        return times[medianIndex];
    }
};
}
}
}
}