/** Unit tests for the q-score metrics
 *
 *
 *  @file
 *  @date 8/25/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <limits>
#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/q_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
#include "interop/logic/metric/q_metric.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef metric_set< q_metric > q_metric_set;
/** Setup for tests that compare two Q metric sets */
struct q_metrics_tests : public generic_test_fixture< q_metric_set > {};

q_metrics_tests::generator_type q_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< q_metric_v4 >),
        wrap(new write_read_metric_generator< q_metric_v4 >),
        wrap(new hardcoded_metric_generator< q_metric_v5 >),
        wrap(new write_read_metric_generator< q_metric_v5 >),
        wrap(new hardcoded_metric_generator< q_metric_v6 >),
        wrap(new write_read_metric_generator< q_metric_v6 >),
        wrap(new hardcoded_metric_generator< q_metric_v6_unbinned>),
        wrap(new write_read_metric_generator< q_metric_v6_unbinned>)
};

// Setup unit tests for q_metrics_tests
INSTANTIATE_TEST_CASE_P(q_metric_unit_test,
                        q_metrics_tests,
                        ::testing::ValuesIn(q_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::q_metric
 * @test Confirm version 4 of the metric can be written to and read from a stream
 * @test Confirm version 4 of the metric matches known binary file
 * @test Confirm version 5 of the metric can be written to and read from a stream
 * @test Confirm version 5 of the metric matches known binary file
 * @test Confirm version 6 of the metric can be written to and read from a stream
 * @test Confirm version 6 of the metric matches known binary file
 */
TEST_P(q_metrics_tests, test_read_write)
{
    typedef q_metric_set::const_iterator const_iterator;
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
        EXPECT_EQ(it_expected->size(), it_actual->size());
        for(size_t i=0;i<std::min(it_expected->size(), it_actual->size());i++)
        {
            EXPECT_EQ(it_expected->qscore_hist(i), it_actual->qscore_hist(i));
        }
    }
    EXPECT_EQ(logic::metric::count_q_metric_bins(actual),logic::metric::count_q_metric_bins(expected));
    EXPECT_EQ(actual.bin_count(), expected.bin_count());
    for(size_t i=0;i<std::min(actual.bin_count(), expected.bin_count());i++)
    {
        EXPECT_EQ(actual.bin_at(i).lower(), expected.bin_at(i).lower());
        EXPECT_EQ(actual.bin_at(i).upper(), expected.bin_at(i).upper());
        EXPECT_EQ(actual.bin_at(i).value(), expected.bin_at(i).value());
    }
}

/**
 * @class illumina::interop::model::metrics::q_metrics
 * @test Confirm populate does not crash on empty q-metrics
 */
TEST(q_metrics_test, test_populate_cumulative_on_empty)
{
    q_metric_set actual;
    q_metric_v6::create_expected(actual);
    logic::metric::populate_cumulative_distribution(actual);

    for(q_metric_set::const_iterator cur=actual.begin();cur != actual.end();++cur)
        EXPECT_TRUE(!cur->is_cumulative_empty());
    metric_set<q_metric> empty_metrics;
    logic::metric::populate_cumulative_distribution(empty_metrics);
}

TEST(q_metrics_test, test_cumulative)
{
    typedef q_metric::uint_t uint_t;
    typedef metric_test<q_metric, 0> helper_t;

    uint_t qsum = 0;
    uint_t hist_all0[] = {0, 267963, 118702, 4281, 2796111, 0, 0};
    uint_t hist_all1[] = {0, 267962, 118703, 4284, 2796110, 0, 0};
    uint_t hist_all2[] = {0, 241483, 44960, 1100, 2899568, 0 ,0};
    uint_t hist_all3[] = {0, 212144, 53942, 427, 2920598, 0, 0};

    std::vector<q_metric> q_metric_vec;
    q_metric_vec.push_back(q_metric(7, 1114, 1, helper_t::to_vector(hist_all1)));
    q_metric_vec.push_back(q_metric(7, 1114, 2, helper_t::to_vector(hist_all2)));
    q_metric_vec.push_back(q_metric(6, 1114, 1, helper_t::to_vector(hist_all0)));
    q_metric_vec.push_back(q_metric(7, 1114, 3, helper_t::to_vector(hist_all3)));

    metric_set<q_metric> q_metric_set(q_metric_vec, 6, q_metric::header_type());
    logic::metric::populate_cumulative_distribution(q_metric_set);

    for(uint_t i=0;i<util::length_of(hist_all1);i++)
    {
        qsum += hist_all1[i];
        qsum += hist_all2[i];
        qsum += hist_all3[i];
    }

    EXPECT_EQ(q_metric_set.get_metric(7, 1114, 3).sum_qscore_cumulative(), qsum);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<q_metric_set> q_regression_gen;
INSTANTIATE_TEST_CASE_P(q_metric_regression_test,
                        q_metrics_tests,
                        ProxyValuesIn(q_regression_gen, regression_test_data::instance().files()));







