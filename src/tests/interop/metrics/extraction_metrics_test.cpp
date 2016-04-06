/** Unit tests for the extraction metrics
 *
 *
 *  @file
 *  @date 8/24/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <limits>
#include <fstream>
#include <gtest/gtest.h>
#include "inc/extraction_metrics_test.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


typedef ::testing::Types<
        hardcoded_fixture<extraction_v2>,
        write_read_fixture<extraction_v2>
> Formats;
TYPED_TEST_CASE(extraction_metrics_test, Formats);


/**
 * @class illumina::interop::model::metrics::extraction_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 */
TYPED_TEST(extraction_metrics_test, test_read_write)
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
        EXPECT_EQ(itExpected->dateTime(), itActual->dateTime());


        std::time_t t = static_cast<std::time_t>(itActual->dateTime());
        std::tm* tm = std::localtime(&t);
        EXPECT_NE(tm, static_cast<std::tm*>(0));

        for(size_t i=0;i<extraction_metric::MAX_CHANNELS;i++)
        {
            EXPECT_EQ(itExpected->max_intensity(i), itActual->max_intensity(i));
            EXPECT_NEAR(itExpected->focusScore(i), itActual->focusScore(i), 1e-7);
        }
    }
}

#define FIXTURE extraction_metrics_test
/**
 * @class illumina::interop::model::metrics::extraction_metric
 * @test Confirm binary write matches expected binary data
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "inc/stream_tests.hpp"
