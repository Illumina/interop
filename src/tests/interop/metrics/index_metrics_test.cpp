/** Unit tests for the index metrics
 *
 *
 *  @file
 *  @date 11/4/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/index_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef metric_set< index_metric > index_metric_set;
/** Setup for tests that compare two index metric sets */
struct index_metrics_tests : public generic_test_fixture< index_metric_set > {};

index_metrics_tests::generator_type index_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< index_metric_v1 >),
        wrap(new write_read_metric_generator< index_metric_v1 >)
};

// Setup unit tests for index_metrics_tests
INSTANTIATE_TEST_CASE_P(index_metric_unit_test,
                        index_metrics_tests,
                        ::testing::ValuesIn(index_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::index_metric
 * @test Confirm version 1 of the metric can be written to and read from a stream
 * @test Confirm version 1 of the metric matches known binary file
 */
TEST_P(index_metrics_tests, test_read_write)
{
    typedef index_metric_set::const_iterator const_iterator;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    EXPECT_EQ(actual.version(), expected.version());
    ASSERT_EQ(actual.size(), expected.size());

    for(const_iterator it_expected=expected.begin(), it_actual = actual.begin();
        it_expected != expected.end() && it_actual != actual.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->read(), it_actual->read());
        EXPECT_EQ(it_expected->size(), it_actual->size());
        for(size_t i=0;i<std::min(it_expected->size(), it_actual->size());i++)
        {
            EXPECT_EQ(it_expected->indices(i).index_seq(), it_actual->indices(i).index_seq());
            EXPECT_EQ(it_expected->indices(i).sample_id(), it_actual->indices(i).sample_id());
            EXPECT_EQ(it_expected->indices(i).sample_proj(), it_actual->indices(i).sample_proj());
            EXPECT_EQ(it_expected->indices(i).cluster_count(), it_actual->indices(i).cluster_count());
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<index_metric_set> index_regression_gen;
INSTANTIATE_TEST_CASE_P(index_metric_regression_test,
                        index_metrics_tests,
                        ProxyValuesIn(index_regression_gen, regression_test_data::instance().files()));





