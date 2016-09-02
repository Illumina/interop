/** Unit tests for the collapsed q-score metrics
 *
 *
 *  @file
 *  @date 8/25/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include "interop/logic/metric/q_metric.h"
#include "inc/q_collapsed_metrics_test.h"
#include "inc/q_metrics_test.h"
#include "interop/model/run_metrics.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef ::testing::Types<
        hardcoded_fixture<q_collapsed_v2>,
        write_read_fixture<q_collapsed_v2>,
        hardcoded_fixture<q_collapsed_v6>,
        write_read_fixture<q_collapsed_v6>
> Formats;
TYPED_TEST_CASE(q_collapsed_metrics_test, Formats);

/**
 * @class illumina::interop::model::metrics::q_collapsed_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 * @test Confirm version 6 of the metric can be written to and read from a stream
 * @test Confirm version 6 of the metric matches known binary file
 */
TYPED_TEST(q_collapsed_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::expected_metric_set.version());
    EXPECT_EQ(TypeParam::actual_metric_set.size(), TypeParam::expected_metric_set.size());
    EXPECT_EQ(TypeParam::actual_metric_set.max_cycle(), TypeParam::expected_metric_set.max_cycle());

    for(typename TypeParam::const_iterator it_expected=TypeParam::expected_metric_set.begin(), it_actual = TypeParam::actual_metric_set.begin();
        it_expected != TypeParam::expected_metric_set.end() && it_actual != TypeParam::actual_metric_set.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
    }
}


// Test if we can parse collapsed q-metrics
TEST(q_collapsed_metrics_test, test_convert_write_read)
{
    metric_set<q_metric> metrics;
    std::istringstream fin(q_v4::binary_data());
    io::read_interop_from_string(q_v4::binary_data(),
                                 metrics);


    metric_set<q_collapsed_metric> expected_metric_set;
    logic::metric::create_collapse_q_metrics(metrics, expected_metric_set);

    std::ostringstream fout;
    illumina::interop::io::write_metrics(fout, expected_metric_set);

    metric_set<q_collapsed_metric> actual_metric_set;
    io::read_interop_from_string(fout.str(),
                                 actual_metric_set);


    EXPECT_EQ(actual_metric_set.version(), expected_metric_set.version());
    EXPECT_EQ(actual_metric_set.max_cycle(), expected_metric_set.max_cycle());
    ASSERT_EQ(actual_metric_set.size(), expected_metric_set.size());
    for(metric_set<q_collapsed_metric>::const_iterator it_expected=expected_metric_set.begin(),
                it_actual = actual_metric_set.begin();
        it_expected != expected_metric_set.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
        EXPECT_EQ(it_expected->q20(), it_actual->q20());
        EXPECT_EQ(it_expected->q30(), it_actual->q30());
        EXPECT_EQ(it_expected->median_qscore(), it_actual->median_qscore());
    }
}

TEST(run_metrics_q_collapsed_test, test_is_group_empty)
{
    run_metrics metrics;
    EXPECT_TRUE(metrics.is_group_empty(constants::QCollapsed));
    io::read_interop_from_string(q_v4::binary_data(),
                                 metrics.get_set<q_metric>());
    logic::metric::create_collapse_q_metrics(metrics.get_set<q_metric>(), metrics.get_set<q_collapsed_metric>());
    EXPECT_FALSE(metrics.is_group_empty(constants::QCollapsed));
}

#define FIXTURE q_collapsed_metrics_test
/**
 * @class illumina::interop::model::metrics::q_collapsed_metric
 * @test Confirm binary write matches expected binary data
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "inc/stream_tests.hpp"


