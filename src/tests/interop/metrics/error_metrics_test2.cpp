/** Unit tests for the error metrics
 *
 *
 *  @file
 *  @date 8/23/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <fstream>
#include <gtest/gtest.h>
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"
#include "interop/model/run_metrics.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;

typedef metric_set< error_metric > error_metric_set;
/** Setup for tests that compare two error metric sets */
struct error_metrics_tests : public generic_test_fixture< error_metric_set > {};

error_metrics_tests::generator_type unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< error_metric_v3 >) ,
        wrap(new write_read_metric_generator< error_metric_v3 >)
};

// Setup unit tests for error_metrics_tests
INSTANTIATE_TEST_CASE_P(error_metric_unit_test,
                        error_metrics_tests,
                        ::testing::ValuesIn(unit_test_generators));

/**
 * @class illumina::interop::model::metrics::error_metric
 * @test Confirm version 3 of the metric can be written to and read from a stream
 * @test Confirm version 3 of the metric matches known binary file
 */
TEST_P(error_metrics_tests, compare_expected_actual)
{
    if(!test) return;// Disable test for rebaseline
    EXPECT_EQ(actual.version(), expected.version());
    EXPECT_EQ(actual.size(), expected.size());
    EXPECT_EQ(actual.max_cycle(), expected.max_cycle());

    for(value_type::const_iterator it_expected=expected.begin(), it_actual = actual.begin();
        it_expected != expected.end() && it_actual != actual.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
        EXPECT_EQ(it_expected->mismatch_count(), it_actual->mismatch_count());
        EXPECT_NEAR(it_expected->error_rate(), it_actual->error_rate(), 1e-7f);
        for(ptrdiff_t i=0;i<static_cast<ptrdiff_t>(it_expected->mismatch_count());i++)
            EXPECT_EQ(it_expected->mismatch_cluster_count(i), it_actual->mismatch_cluster_count(i));
    }
}

/**
 * @test Ensure tile_numbers_for_lane returns the proper number
 */
TEST(error_metrics_single_test, test_tile_metric_count_for_lane)
{
    error_metric_set metrics;
    error_metric_v3::create_expected(metrics);
    EXPECT_EQ(metrics.tile_numbers_for_lane(7).size(), 1u);
}

/**
 * @test Ensure the keys function returns the proper metric
 */
TEST(error_metrics_single_test, test_expected_get_metric)
{
    error_metric_set metrics;
    error_metric_v3::create_expected(metrics);
    error_metric_set::key_vector keys = metrics.keys();
    for(size_t i=0;i<keys.size();i++)
    {
        EXPECT_EQ(keys[i], metrics.get_metric(keys[i]).id());
    }
}


TEST(run_metrics_error_metric_test, test_is_group_empty)
{
    run_metrics metrics;
    EXPECT_TRUE(metrics.is_group_empty(constants::Error));
    io::read_interop_from_string(error_v3::binary_data(),
                                 metrics.get_set<error_metric>());
    EXPECT_FALSE(metrics.is_group_empty(constants::Error));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<error_metric_set> regression_gen("metrics");
INSTANTIATE_TEST_CASE_P(error_metric_regression_test,
                        error_metrics_tests,
                        ProxyValuesIn(regression_gen, regression_test_data::instance().files()));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shared stream tests
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Setup for tests that compare two binary buffers created by writing error metrics */
struct error_metric_write_tests : public generic_test_fixture< std::string >
{
    /** Define a metric set type */
    typedef error_metric_set metric_set_t;
};

error_metric_write_tests::generator_type write_generators[] = {
         new write_metric_generator< error_metric_v3 >()
};
INSTANTIATE_TEST_CASE_P(error_metric_write_test_params,
                        error_metric_write_tests,
                        ::testing::ValuesIn(write_generators));


#define FIXTURE error_metric_write_tests
#include "src/tests/interop/metrics/inc/metric_stream_tests.hpp"

