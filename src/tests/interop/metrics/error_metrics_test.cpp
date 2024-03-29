/** Unit tests for the error metrics
 *
 *
 *  @file
 *  @date 8/23/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#ifdef _MSC_VER
#pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;

typedef metric_set< error_metric > error_metric_set;
/** Setup for tests that compare two error metric sets */
struct error_metrics_tests : public generic_test_fixture< error_metric_set > {};

error_metrics_tests::generator_type error_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< error_metric_v3 >) ,
        wrap(new hardcoded_metric_generator< error_metric_v4 >),
        wrap(new hardcoded_metric_generator< error_metric_v5 >),
        wrap(new hardcoded_metric_generator< error_metric_v6 >),
        wrap(new write_read_metric_generator< error_metric_v3 >),
        wrap(new write_read_metric_generator< error_metric_v4 >),
        wrap(new write_read_metric_generator< error_metric_v5 >),
        wrap(new write_read_metric_generator< error_metric_v6 >),
        wrap(new by_cycle_metric_generator< error_metric_v3 >),
        wrap(new by_cycle_metric_generator< error_metric_v4 >),
        wrap(new by_cycle_metric_generator< error_metric_v5 >),
        wrap(new by_cycle_metric_generator< error_metric_v6 >),
        wrap(new clear_metric_generator< error_metric_v3 >),
        wrap(new clear_metric_generator< error_metric_v4 >),
        wrap(new clear_metric_generator< error_metric_v5 >),
        wrap(new clear_metric_generator< error_metric_v6 >)
};

// Setup unit tests for error_metrics_tests
INSTANTIATE_TEST_CASE_P(error_metric_unit_test,
                        error_metrics_tests,
                        ::testing::ValuesIn(error_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::error_metric
 * @test Confirm version 3 of the metric can be written to and read from a stream
 * @test Confirm version 3 of the metric matches known binary file
 */
TEST_P(error_metrics_tests, compare_expected_actual)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    ASSERT_EQ(actual.version(), expected.version());
    ASSERT_EQ(actual.size(), expected.size());
    EXPECT_EQ(actual.max_cycle(), expected.max_cycle()) << actual.offset_map().size()
                                                        << " == "
                                                        << expected.offset_map().size();

    for(value_type::const_iterator it_expected=expected.begin(), it_actual = actual.begin();
        it_expected != expected.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
        EXPECT_EQ(it_expected->mismatch_count(), it_actual->mismatch_count());
        EXPECT_NEAR(it_expected->error_rate(), it_actual->error_rate(), 1e-5f);
        INTEROP_EXPECT_NEAR(it_expected->phix_adapter_rate(), it_actual->phix_adapter_rate(), 1e-5f);
        ASSERT_EQ(it_expected->phix_adapter_count(), it_actual->phix_adapter_count());
        for(size_t i=0;i<it_expected->phix_adapter_count();++i)
            INTEROP_EXPECT_NEAR(it_expected->phix_adapter_rate_at(i), it_actual->phix_adapter_rate_at(i), 1e-5f);

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
 * @test Ensure the adapter sequences are correct in header
 */
TEST(error_metrics_single_test, test_error_header_v6)
{
    error_metric_set metrics;
    error_metric_v6::create_expected(metrics);
    EXPECT_EQ(metrics.number_adapters(), 2u);
    EXPECT_EQ(metrics.adapter_length(), 8u);
    EXPECT_EQ(metrics.adapter_sequences().size(), 2u);
    EXPECT_EQ(metrics.adapter_sequences()[0], "CCCCCCCC");
    EXPECT_EQ(metrics.adapter_sequences()[1], "AAAAAAAA");
}

/**
 * @test Ensure the rates per sequences are correct
 */
TEST(error_metrics_single_test, test_error_rates_v6)
{
    error_metric_set metrics;
    error_metric_v6::create_expected(metrics);

    float rates[3][2] = {{0.298748f, 0.1234f},
                         {0.287257f, 0.3934f},
                         {0.608985f, 0.5034f}};

    EXPECT_EQ(metrics.size(), 3u);
    size_t i = 0;
    for(error_metric_v6::const_iterator it=metrics.begin(); it != metrics.end();it++, i++) {
        EXPECT_EQ(it->phix_adapter_rate(), (rates[i][0]+rates[i][1])/2);
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<error_metric_set> error_regression_gen;
INSTANTIATE_TEST_CASE_P(error_metric_regression_test,
                        error_metrics_tests,
                        ProxyValuesIn(error_regression_gen, regression_test_data::instance().files()));




