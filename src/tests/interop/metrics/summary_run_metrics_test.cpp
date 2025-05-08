/** Unit tests for the summary run metrics
 *
 *
 *  @file
 *  @date 6/23/2016
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <limits>
#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/summary_run_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop::unittest;
using namespace illumina::interop;


typedef metric_set< summary_run_metric > summary_run_metric_set;
/** Setup for tests that compare two metric sets */
struct summary_run_metrics_tests : public generic_test_fixture< summary_run_metric_set > {};

/** This does not change per cycle */
by_cycle_metric_generator< summary_run_v1 > summary_run_dummy_v1;


summary_run_metrics_tests::generator_type summary_run_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< summary_run_v1 >),
        wrap(new write_read_metric_generator< summary_run_v1 >),
        wrap(new clear_metric_generator< summary_run_v1 >)
};

// Setup unit tests for summary_run_metrics_tests
INSTANTIATE_TEST_CASE_P(summary_run_metric_unit_test,
                        summary_run_metrics_tests,
                        ::testing::ValuesIn(summary_run_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::summary_run_metric
 * @test Confirm version 1 of the metric can be written to and read from a stream
 * @test Confirm version 1 of the metric matches known binary file
 */
TEST_P(summary_run_metrics_tests, test_read_write)
{
    typedef summary_run_metric_set::const_iterator const_iterator;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    EXPECT_EQ(actual.version(), expected.version());
    EXPECT_EQ(actual.size(), expected.size());
    const float eps = 1e-7f;
    for (const_iterator it_expected = expected.begin(), it_actual = actual.begin();
         it_expected != expected.end() && it_actual != actual.end();
         it_expected++, it_actual++)
    {
        EXPECT_NEAR(it_expected->raw_cluster_count(), it_actual->raw_cluster_count(), eps);
        EXPECT_NEAR(it_expected->occupied_cluster_count(), it_actual->occupied_cluster_count(), eps);
        EXPECT_NEAR(it_expected->pf_cluster_count(), it_actual->pf_cluster_count(), eps);
        EXPECT_NEAR(it_expected->occupancy_proxy_cluster_count(), it_actual->occupancy_proxy_cluster_count(), eps);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<summary_run_metric_set> summary_run_regression_gen;
INSTANTIATE_TEST_CASE_P(summary_run_metric_regression_test,
                        summary_run_metrics_tests,
                        ProxyValuesIn(summary_run_regression_gen, regression_test_data::instance().files()));

