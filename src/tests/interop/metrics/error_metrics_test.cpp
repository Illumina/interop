/** Unit tests for the error metrics
 *
 *
 *  @file
 *
 *  @date 8/23/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <fstream>
#include <gtest/gtest.h>
#include "inc/error_metrics_test.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef ::testing::Types<
        hardcoded_fixture<error_v3>,
        write_read_fixture<error_v3>
> Formats;
TYPED_TEST_CASE(error_metrics_test, Formats);

/**
 * @class illumina::interop::model::metrics::error_metric
 * @test Confirm version 3 of the metric can be written to and read from a stream
 * @test Confirm version 3 of the metric matches known binary file
 */
TYPED_TEST(error_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metric_set.size(), TypeParam::expected_metric_set.size());
    EXPECT_EQ(TypeParam::actual_metric_set.max_cycle(), TypeParam::expected_metric_set.max_cycle());

    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metric_set.begin(), itActual = TypeParam::actual_metric_set.begin();
        itExpected != TypeParam::expected_metric_set.end() && itActual != TypeParam::actual_metric_set.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->cycle(), itActual->cycle());
        EXPECT_EQ(itExpected->mismatch_count(), itActual->mismatch_count());
        EXPECT_NEAR(itExpected->errorRate(), itActual->errorRate(), 1e-7f);
        for(ptrdiff_t i=0;i<static_cast<ptrdiff_t>(itExpected->mismatch_count());i++)
            EXPECT_EQ(itExpected->mismatch_cluster_count(i), itActual->mismatch_cluster_count(i));
    }
}


TYPED_TEST(error_metrics_test, test_tile_metric_count_for_lane)
{
    EXPECT_EQ(TypeParam::expected_metric_set.tile_numbers_for_lane(7).size(), 1u);
}

#define FIXTURE error_metrics_test
/**
 * @class illumina::interop::model::metrics::error_metric
 * @test Confirm binary write matches expected binary data
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "inc/stream_tests.hpp"

