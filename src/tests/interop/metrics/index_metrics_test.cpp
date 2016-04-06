/** Unit tests for the index metrics
 *
 *
 *  @file
 *  @date 11/4/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <fstream>
#include <gtest/gtest.h>
#include "inc/index_metrics_test.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;
using namespace illumina::interop::unittest;

typedef ::testing::Types<
        hardcoded_fixture<index_v1>,
        write_read_fixture<index_v1>
> Formats;
TYPED_TEST_CASE(index_metrics_test, Formats);

/**
 * @class illumina::interop::model::metrics::index_metric
 * @test Confirm version 1 of the metric can be written to and read from a stream
 * @test Confirm version 1 of the metric matches known binary file
 */
TYPED_TEST(index_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metric_set.size(), TypeParam::expected_metric_set.size());

    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metric_set.begin(), itActual = TypeParam::actual_metric_set.begin();
        itExpected != TypeParam::expected_metric_set.end() && itActual != TypeParam::actual_metric_set.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->read(), itActual->read());
        EXPECT_EQ(itExpected->size(), itActual->size());
        for(size_t i=0;i<std::min(itExpected->size(), itActual->size());i++)
        {
            EXPECT_EQ(itExpected->indices(i).index_seq(), itActual->indices(i).index_seq());
            EXPECT_EQ(itExpected->indices(i).sample_id(), itActual->indices(i).sample_id());
            EXPECT_EQ(itExpected->indices(i).sample_proj(), itActual->indices(i).sample_proj());
            EXPECT_EQ(itExpected->indices(i).count(), itActual->indices(i).count());
        }
    }
}

#define FIXTURE index_metrics_test
/**
 * @class illumina::interop::model::metrics::index_metric
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "inc/stream_tests.hpp"


