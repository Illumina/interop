/** Unit tests for the q-score metrics
 *
 *
 *  @file
 *  @date 8/25/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <limits>
#include <fstream>
#include <gtest/gtest.h>
#include "interop/logic/metric/q_metric.h"
#include "inc/q_metrics_test.h"
#include "interop/model/run_metrics.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef ::testing::Types<
        hardcoded_fixture<q_v4>,
        write_read_fixture<q_v4>,
        hardcoded_fixture<q_v5>,
        write_read_fixture<q_v5>,
        hardcoded_fixture<q_v6>,
        write_read_fixture<q_v6>
> Formats;
TYPED_TEST_CASE(q_metrics_test, Formats);

/**
 * @class illumina::interop::model::metrics::q_metric
 * @test Confirm version 4 of the metric can be written to and read from a stream
 * @test Confirm version 4 of the metric matches known binary file
 * @test Confirm version 5 of the metric can be written to and read from a stream
 * @test Confirm version 5 of the metric matches known binary file
 * @test Confirm version 6 of the metric can be written to and read from a stream
 * @test Confirm version 6 of the metric matches known binary file
 */
TYPED_TEST(q_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metric_set.size(), TypeParam::expected_metric_set.size());
    EXPECT_EQ(TypeParam::actual_metric_set.max_cycle(), TypeParam::expected_metric_set.max_cycle());

    for(typename TypeParam::const_iterator it_expected=TypeParam::expected_metric_set.begin(), it_actual = TypeParam::actual_metric_set.begin();
        it_expected != TypeParam::expected_metric_set.end() && it_actual != TypeParam::actual_metric_set.end();
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
    EXPECT_EQ(logic::metric::count_q_metric_bins(TypeParam::actual_metric_set),logic::metric::count_q_metric_bins(TypeParam::expected_metric_set));
    EXPECT_EQ(TypeParam::actual_metric_set.bin_count(), TypeParam::expected_metric_set.bin_count());
    for(size_t i=0;i<std::min(TypeParam::actual_metric_set.bin_count(), TypeParam::expected_metric_set.bin_count());i++)
    {
        EXPECT_EQ(TypeParam::actual_metric_set.bin_at(i).lower(), TypeParam::expected_metric_set.bin_at(i).lower());
        EXPECT_EQ(TypeParam::actual_metric_set.bin_at(i).upper(), TypeParam::expected_metric_set.bin_at(i).upper());
        EXPECT_EQ(TypeParam::actual_metric_set.bin_at(i).value(), TypeParam::expected_metric_set.bin_at(i).value());
    }
}

/**
 * @class illumina::interop::model::metrics::q_metrics
 * @test Confirm populate does not crash on empty q-metrics
 */
TYPED_TEST(q_metrics_test, test_populate_cumulative_on_empty)
{
    logic::metric::populate_cumulative_distribution(TypeParam::actual_metric_set);
    for(typename TypeParam::const_iterator cur=TypeParam::actual_metric_set.metrics().begin();cur != TypeParam::actual_metric_set.metrics().end();++cur)
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

    for(uint_t i=0;i<helper_t::size_of(hist_all1);i++)
    {
        qsum += hist_all1[i];
        qsum += hist_all2[i];
        qsum += hist_all3[i];
    }

    EXPECT_EQ(q_metric_set.get_metric(7, 1114, 3).sum_qscore_cumulative(), qsum);
}


TEST(run_metrics_q_test, test_is_group_empty)
{
    run_metrics metrics;
    EXPECT_TRUE(metrics.is_group_empty(constants::Q));
    std::istringstream fin(q_v4::binary_data());
    io::read_metrics(fin, metrics.get_set<q_metric>());
    EXPECT_FALSE(metrics.is_group_empty(constants::Q));
}

#define FIXTURE q_metrics_test
/**
 * @class illumina::interop::model::metrics::q_metric
 * @test Confirm binary write matches expected binary data
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "inc/stream_tests.hpp"


