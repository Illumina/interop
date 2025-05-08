/** Unit tests for the phasing metrics
 *
 *
 *  @file
 *  @date 10/22/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/phasing_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef metric_set< phasing_metric > phasing_metric_set;
/** Setup for tests that compare two metric sets */
struct phasing_metrics_tests : public generic_test_fixture< phasing_metric_set > {};

phasing_metrics_tests::generator_type phasing_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< phasing_metric_v1 >) ,
        wrap(new write_read_metric_generator< phasing_metric_v1 >),
        wrap(new by_cycle_metric_generator< phasing_metric_v1 >),
        wrap(new clear_metric_generator< phasing_metric_v1 >),
        wrap(new hardcoded_metric_generator< phasing_metric_v2 >) ,
        wrap(new write_read_metric_generator< phasing_metric_v2 >),
        wrap(new by_cycle_metric_generator< phasing_metric_v2 >),
        wrap(new clear_metric_generator< phasing_metric_v2 >)
};

// Setup unit tests for phasing_metrics_tests
INSTANTIATE_TEST_SUITE_P(phasing_metric_unit_test,
                        phasing_metrics_tests,
                        ::testing::ValuesIn(phasing_unit_test_generators));
/**
 * @class illumina::interop::model::metrics::phasing_metric
 * @test Confirm version 1 of the metric can be written to and read from a stream
 * @test Confirm version 1 of the metric matches known binary file
 */
TEST_P(phasing_metrics_tests, test_read_write)
{
    typedef phasing_metric_set::const_iterator const_iterator;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    EXPECT_EQ(actual.version(), expected.version());
    EXPECT_EQ(actual.size(), expected.size());

    for(const_iterator it_expected=expected.begin(), it_actual = actual.begin();
        it_expected != expected.end() && it_actual != actual.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
        EXPECT_NEAR(it_expected->prephasing_weight(), it_actual->prephasing_weight(), 1e-7);
        EXPECT_NEAR(it_expected->phasing_weight(), it_actual->phasing_weight(), 1e-7);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<phasing_metric_set> phasing_regression_gen;
INSTANTIATE_TEST_SUITE_P(phasing_metric_regression_test,
                        phasing_metrics_tests,
                        ProxyValuesIn(phasing_regression_gen, regression_test_data::instance().files()));

