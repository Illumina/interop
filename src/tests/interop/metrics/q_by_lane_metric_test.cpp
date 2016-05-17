/** Unit tests for the by lane q-score metrics
 *
 *
 *  @file
 *  @date 5/18/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include "interop/model/metrics/q_by_lane_metric.h"
#include "inc/q_metrics_test.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::unittest;

// Test that q_by_lane_metric can use q_metric parsers/writers
TEST(q_by_lane_metrics_test, test_read_v4)
{
    metric_set<q_by_lane_metric> metrics;
    std::istringstream fin(q_v4::binary_data());
    illumina::interop::io::read_metrics(fin, metrics);
}

