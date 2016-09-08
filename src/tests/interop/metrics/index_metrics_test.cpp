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
#include "interop/model/run_metrics.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;
using namespace illumina::interop;
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

    for(typename TypeParam::const_iterator it_expected=TypeParam::expected_metric_set.begin(), it_actual = TypeParam::actual_metric_set.begin();
        it_expected != TypeParam::expected_metric_set.end() && it_actual != TypeParam::actual_metric_set.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->read(), it_actual->read());
        EXPECT_EQ(it_expected->size(), it_actual->size());
        for(size_t i=0;i<std::min(it_expected->size(), it_actual->size());i++)
        {
            EXPECT_EQ(it_expected->indices(i).index_seq(), it_actual->indices(i).index_seq());
            EXPECT_EQ(it_expected->indices(i).sample_id(), it_actual->indices(i).sample_id());
            EXPECT_EQ(it_expected->indices(i).sample_proj(), it_actual->indices(i).sample_proj());
            EXPECT_EQ(it_expected->indices(i).count(), it_actual->indices(i).count());
        }
    }
}

TEST(run_metrics_index_test, test_is_group_empty)
{
    run_metrics metrics;
    EXPECT_TRUE(metrics.is_group_empty(constants::Index));
    io::read_interop_from_string(index_v1::binary_data(),
                                 metrics.get_set<index_metric>());
    EXPECT_FALSE(metrics.is_group_empty(constants::Index));
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


