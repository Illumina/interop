/** Unit tests for the by lane q-score metrics
 *
 *
 *  @file
 *  @date 5/18/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include "interop/logic/metric/q_metric.h"
#include "interop/model/run_metrics.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
#include "src/tests/interop/metrics/inc/q_metrics_test.h"
using namespace illumina::interop;
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::unittest;

// Test if we can parse by lane q-metrics
TEST(q_by_lane_metrics_test, test_convert_write_read)
{
    metric_set<q_metric> metrics;
    std::string data;
    q_metric_v4::create_binary_data(data);
    io::read_interop_from_string(data,
                                 metrics);


    metric_set<q_by_lane_metric> expected_metric_set;
    logic::metric::create_q_metrics_by_lane(metrics, expected_metric_set);

    std::ostringstream fout;
    illumina::interop::io::write_metrics(fout, expected_metric_set);

    metric_set<q_by_lane_metric> actual_metric_set;
    io::read_interop_from_string(fout.str(),
                                 actual_metric_set);


    EXPECT_EQ(actual_metric_set.version(), expected_metric_set.version());
    EXPECT_EQ(actual_metric_set.max_cycle(), expected_metric_set.max_cycle());
    ASSERT_EQ(actual_metric_set.size(), expected_metric_set.size());
    for(metric_set<q_by_lane_metric>::const_iterator it_expected=expected_metric_set.begin(),
                it_actual = actual_metric_set.begin();
        it_expected != expected_metric_set.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
        EXPECT_EQ(it_expected->size(), it_actual->size());
        for(size_t i=0;i<std::min(it_expected->size(), it_actual->size());i++)
        {
            EXPECT_EQ(it_expected->qscore_hist(i), it_actual->qscore_hist(i));
        }
    }
}

// Test that q_by_lane_metric can use q_metric parsers/writers
TEST(q_by_lane_metrics_test, test_read_v4)
{
    metric_set<q_by_lane_metric> metrics;
    std::string data;
    q_metric_v4::create_binary_data(data);
    io::read_interop_from_string(data,
                                 metrics);
}

TEST(run_metrics_q_by_lane_test, test_is_group_empty)
{
    run_metrics metrics;
    EXPECT_TRUE(metrics.is_group_empty(constants::QByLane));
    std::string data;
    q_metric_v4::create_binary_data(data);
    io::read_interop_from_string(data,
                                 metrics.get<q_metric>());
    logic::metric::create_q_metrics_by_lane(metrics.get<q_metric>(), metrics.get<q_by_lane_metric>());
    EXPECT_FALSE(metrics.is_group_empty(constants::QByLane));
}
