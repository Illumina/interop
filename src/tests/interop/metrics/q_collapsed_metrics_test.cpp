/** Unit tests for the collapsed q-score metrics
 *
 *
 *  @file
 *  @date 8/25/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/q_collapsed_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
#include "interop/logic/metric/q_metric.h"
#include "src/tests/interop/metrics/inc/q_metrics_test.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef metric_set< q_collapsed_metric > q_collapsed_metric_set;
/** Setup for tests that compare two Q-collapsed metric sets */
struct q_collapsed_metrics_tests : public generic_test_fixture< q_collapsed_metric_set > {};


q_collapsed_metrics_tests::generator_type q_collapsed_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< q_collapsed_metric_v2 >),
        wrap(new write_read_metric_generator< q_collapsed_metric_v2 >),
        wrap(new hardcoded_metric_generator< q_collapsed_metric_v3 >),
        wrap(new write_read_metric_generator< q_collapsed_metric_v3 >),
        wrap(new hardcoded_metric_generator< q_collapsed_metric_v4 >),
        wrap(new write_read_metric_generator< q_collapsed_metric_v4 >),
        wrap(new hardcoded_metric_generator< q_collapsed_metric_v5 >),
        wrap(new write_read_metric_generator< q_collapsed_metric_v5 >),
        wrap(new hardcoded_metric_generator< q_collapsed_metric_v6 >),
        wrap(new write_read_metric_generator< q_collapsed_metric_v6 >)
};

// Setup unit tests for q_collapsed_metrics_tests
INSTANTIATE_TEST_CASE_P(q_collapsed_metric_unit_test,
                        q_collapsed_metrics_tests,
                        ::testing::ValuesIn(q_collapsed_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::q_collapsed_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 * @test Confirm version 6 of the metric can be written to and read from a stream
 * @test Confirm version 6 of the metric matches known binary file
 */
TEST_P(q_collapsed_metrics_tests, test_read_write)
{
    typedef q_collapsed_metric_set::const_iterator const_iterator;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    EXPECT_EQ(actual.version(), expected.version());
    ASSERT_EQ(actual.size(), expected.size());
    EXPECT_EQ(actual.max_cycle(), expected.max_cycle());

    for(const_iterator it_expected=expected.begin(), it_actual = actual.begin();
        it_expected != expected.end() && it_actual != actual.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
    }
}


// Test if we can parse collapsed q-metrics
TEST(q_collapsed_metrics_test, test_convert_write_read)
{
    metric_set<q_metric> metrics;
    std::string data;
    q_metric_v4::create_binary_data(data);
    io::read_interop_from_string(data,
                                 metrics);


    metric_set<q_collapsed_metric> expected_metric_set;
    logic::metric::create_collapse_q_metrics(metrics, expected_metric_set);

    std::ostringstream fout;
    illumina::interop::io::write_metrics(fout, expected_metric_set);

    metric_set<q_collapsed_metric> actual_metric_set;
    io::read_interop_from_string(fout.str(),
                                 actual_metric_set);


    EXPECT_EQ(actual_metric_set.version(), expected_metric_set.version());
    EXPECT_EQ(actual_metric_set.max_cycle(), expected_metric_set.max_cycle());
    ASSERT_EQ(actual_metric_set.size(), expected_metric_set.size());
    for(metric_set<q_collapsed_metric>::const_iterator it_expected=expected_metric_set.begin(),
                it_actual = actual_metric_set.begin();
        it_expected != expected_metric_set.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
        EXPECT_EQ(it_expected->q20(), it_actual->q20());
        EXPECT_EQ(it_expected->q30(), it_actual->q30());
        EXPECT_EQ(it_expected->median_qscore(), it_actual->median_qscore());
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<q_collapsed_metric_set> q_collapsed_regression_gen;
INSTANTIATE_TEST_CASE_P(q_collapsed_metric_regression_test,
                        q_collapsed_metrics_tests,
                        ProxyValuesIn(q_collapsed_regression_gen, regression_test_data::instance().files()));



