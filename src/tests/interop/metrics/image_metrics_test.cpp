/** Unit tests for the image metrics
 *
 *
 *  @file
 *  @date 8/24/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include "src/tests/interop/metrics/inc/image_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
#include "interop/model/run_metrics.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef metric_set< image_metric > image_metric_set;
/** Setup for tests that compare two image metric sets */
struct image_metrics_tests : public generic_test_fixture< image_metric_set > {};


image_metrics_tests::generator_type image_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< image_metric_v1 >),
        wrap(new write_read_metric_generator< image_metric_v1 >),
        wrap(new hardcoded_metric_generator< image_metric_v2 >) ,
        wrap(new write_read_metric_generator< image_metric_v2 >)
};

// Setup unit tests for image_metrics_tests
INSTANTIATE_TEST_CASE_P(image_metric_unit_test,
                        image_metrics_tests,
                        ::testing::ValuesIn(image_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::image_metric
 * @test Confirm version 1 of the metric can be written to and read from a stream
 * @test Confirm version 1 of the metric matches known binary file
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 */
TEST_P(image_metrics_tests, test_read_write)
{
    typedef image_metric_set::const_iterator const_iterator;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    EXPECT_EQ(actual.version(), expected.version());
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.channel_count(), expected.channel_count());
    EXPECT_EQ(actual.max_cycle(), expected.max_cycle());

    for(const_iterator it_expected=expected.begin(), it_actual = actual.begin();
        it_expected != expected.end() && it_actual != actual.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
        ASSERT_EQ(it_expected->channel_count(), it_actual->channel_count());
        for(size_t i=0;i<it_actual->channel_count();i++)
        {
            EXPECT_EQ(it_expected->min_contrast(i), it_actual->min_contrast(i));
            EXPECT_EQ(it_expected->max_contrast(i), it_actual->max_contrast(i));
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<image_metric_set> image_regression_gen;
INSTANTIATE_TEST_CASE_P(image_metric_regression_test,
                        image_metrics_tests,
                        ProxyValuesIn(image_regression_gen, regression_test_data::instance().files()));



