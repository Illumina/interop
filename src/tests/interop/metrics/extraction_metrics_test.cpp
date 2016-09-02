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
#include "interop/model/run_metrics.h"
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
    const float tol = 1e-7f;
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
        EXPECT_EQ(it_expected->date_time(), it_actual->date_time());


        std::time_t t = static_cast<std::time_t>(it_actual->date_time());
        std::tm* tm = std::localtime(&t);
        EXPECT_NE(tm, static_cast<std::tm*>(0));
        ASSERT_EQ(it_expected->channel_count(), it_actual->channel_count());
        for(size_t i=0;i<it_expected->channel_count();i++)
        {
            EXPECT_EQ(it_expected->max_intensity(i), it_actual->max_intensity(i));
            EXPECT_NEAR(it_expected->focus_score(i), it_actual->focus_score(i), tol);
        }
    }
}
TEST(run_metrics_extraction_test, test_is_group_empty)
{
    run_metrics metrics;
    EXPECT_TRUE(metrics.is_group_empty(constants::Extraction));
    io::read_interop_from_string(extraction_v2::binary_data(),
                                 metrics.get_set<extraction_metric>());
    EXPECT_FALSE(metrics.is_group_empty(constants::Extraction));
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
