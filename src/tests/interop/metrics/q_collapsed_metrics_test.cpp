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

    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metric_set.begin(), itActual = TypeParam::actual_metric_set.begin();
        itExpected != TypeParam::expected_metric_set.end() && itActual != TypeParam::actual_metric_set.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->cycle(), itActual->cycle());
    }
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


