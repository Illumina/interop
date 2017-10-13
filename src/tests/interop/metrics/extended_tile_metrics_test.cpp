/** Unit tests for the tile metrics
 *
 *
 *  @file
 *  @date 10/22/15
 *  @version 1.0
 *  @copyright GNU Public License
 */
#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/extended_tile_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef metric_set< extended_tile_metric > extended_tile_metric_set;
/** Setup for tests that compare two metric sets */
struct extended_tile_metrics_tests : public generic_test_fixture< extended_tile_metric_set > {};

extended_tile_metrics_tests::generator_type extended_tile_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< extended_tile_metric_v2 >),
        wrap(new by_cycle_metric_generator< extended_tile_metric_v2 >),
        wrap(new write_read_metric_generator< extended_tile_metric_v2 >),
        wrap(new clear_metric_generator< extended_tile_metric_v2 >),
        wrap(new hardcoded_metric_generator< extended_tile_metric_v3 >),
        wrap(new by_cycle_metric_generator< extended_tile_metric_v3 >),
        wrap(new write_read_metric_generator< extended_tile_metric_v3 >),
        wrap(new clear_metric_generator< extended_tile_metric_v3 >)
};

// Setup unit tests for extended_tile_metrics_tests
INSTANTIATE_TEST_CASE_P(extended_tile_metric_unit_test,
                        extended_tile_metrics_tests,
                        ::testing::ValuesIn(extended_tile_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::extended_tile_metric
 * @test Confirm version 1 of the metric can be written to and read from a stream
 * @test Confirm version 1 of the metric matches known binary file
 */
TEST_P(extended_tile_metrics_tests, test_read_write)
{
    typedef extended_tile_metric_set::const_iterator const_iterator;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    EXPECT_EQ(actual.version(), expected.version());
    EXPECT_EQ(actual.size(), expected.size());

    const float tol = 1e-7f / 0.01f;
    for(const_iterator it_expected=expected.begin(), it_actual = actual.begin();
    it_expected != expected.end() && it_actual != actual.end();
    it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());

        EXPECT_NEAR(it_expected->cluster_count_occupied(), it_actual->cluster_count_occupied(), tol);
        INTEROP_EXPECT_NEAR(it_expected->upper_left().x(), it_actual->upper_left().x(), tol);
        INTEROP_EXPECT_NEAR(it_expected->upper_left().y(), it_actual->upper_left().y(), tol);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<extended_tile_metric_set> extended_tile_regression_gen;
INSTANTIATE_TEST_CASE_P(extended_tile_metric_regression_test,
                        extended_tile_metrics_tests,
                        ProxyValuesIn(extended_tile_regression_gen, regression_test_data::instance().files()));


