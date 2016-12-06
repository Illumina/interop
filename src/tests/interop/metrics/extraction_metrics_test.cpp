/** Unit tests for the extraction metrics
 *
 *
 *  @file
 *  @date 8/24/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <limits>
#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/extraction_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef metric_set< extraction_metric > extraction_metric_set;
/** Setup for tests that compare two extraction metric sets */
struct extraction_metrics_tests : public generic_test_fixture< extraction_metric_set > {};


extraction_metrics_tests::generator_type extraction_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< extraction_metric_v2 >) ,
        wrap(new write_read_metric_generator< extraction_metric_v2 >)
};

// Setup unit tests for extraction_metrics_tests
INSTANTIATE_TEST_CASE_P(extraction_metric_unit_test,
                        extraction_metrics_tests,
                        ::testing::ValuesIn(extraction_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::extraction_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 */
TEST_P(extraction_metrics_tests, test_read_write)
{
    typedef extraction_metric_set::const_iterator const_iterator;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    const float tol = 1e-7f;
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
        EXPECT_EQ(it_expected->date_time(), it_actual->date_time());


        std::time_t t = static_cast<std::time_t>(it_actual->date_time());
        std::tm* tm = std::localtime(&t);
        EXPECT_NE(tm, static_cast<std::tm*>(0));
        ASSERT_EQ(it_expected->channel_count(), it_actual->channel_count());
        for(size_t i=0;i<it_expected->channel_count();i++)
        {
            EXPECT_EQ(it_expected->max_intensity(i), it_actual->max_intensity(i));
            EXPECT_NEAR(it_expected->focus_score(i), it_actual->focus_score(i), tol);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<extraction_metric_set> extraction_regression_gen;
INSTANTIATE_TEST_CASE_P(extraction_metric_regression_test,
                        extraction_metrics_tests,
                        ProxyValuesIn(extraction_regression_gen, regression_test_data::instance().files()));




