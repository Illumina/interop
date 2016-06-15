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
#include "interop/logic/metric/q_metric.h"
using namespace illumina::interop;
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

// Test if we can parse by lane q-metrics
TEST(q_by_lane_metrics_test, test_convert_write_read)
{
    metric_set<q_metric> metrics;
    std::istringstream fin(q_v4::binary_data());
    illumina::interop::io::read_metrics(fin, metrics);


    metric_set<q_by_lane_metric> expected_metric_set;
    logic::metric::create_q_metrics_by_lane(metrics, expected_metric_set);

    std::ostringstream fout;
    illumina::interop::io::write_metrics(fout, expected_metric_set);

    metric_set<q_by_lane_metric> actual_metric_set;
    std::istringstream iss(fout.str());
    illumina::interop::io::read_metrics(iss, actual_metric_set);


    EXPECT_EQ(actual_metric_set.version(), expected_metric_set.version());
    EXPECT_EQ(actual_metric_set.max_cycle(), expected_metric_set.max_cycle());
    ASSERT_EQ(actual_metric_set.size(), expected_metric_set.size());
    for(typename metric_set<q_by_lane_metric>::const_iterator itExpected=expected_metric_set.begin(),
                itActual = actual_metric_set.begin();
        itExpected != expected_metric_set.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->cycle(), itActual->cycle());
        EXPECT_EQ(itExpected->size(), itActual->size());
        for(size_t i=0;i<std::min(itExpected->size(), itActual->size());i++)
        {
            EXPECT_EQ(itExpected->qscoreHist(i), itActual->qscoreHist(i));
        }
    }
}
