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
#include "interop/model/metric_sets/error_metric_set.h"
#include "src/tests/interop/metrics/metric_test_utils.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;


namespace illumina{ namespace interop { namespace unittest {
/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * @note Version 3
 */
struct error_metrics_hardcoded_fixture_v3 : util::fixture_helper<error_metrics, 3>
{
    /** Setup fixture */
    error_metrics_hardcoded_fixture_v3()
    {
        expected_metrics.push_back(metric_type(7, 1114, 1, 0.450100899f));
        expected_metrics.push_back(metric_type(7, 1114, 2, 0.900201797f));
        expected_metrics.push_back(metric_type(7, 1114, 3, 0.465621591f));
        int tmp[] = {3,30,7,0,90,4,1,0,-96,115,-26,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,2,0,-96,115,102,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,3,0,-12,101,-18,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };
        setup_hardcoded_binary(tmp, header_type());
    }
};


/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 3
 */
struct error_metrics_write_read_fixture_v3 : util::fixture_helper<error_metrics, 3>
{
    /** Setup fixture */
    error_metrics_write_read_fixture_v3()
    {
        expected_metrics.push_back(metric_type(7, 1114, 1, 0.450100899f));
        expected_metrics.push_back(metric_type(7, 1114, 2, 0.900201797f));
        expected_metrics.push_back(metric_type(7, 1114, 3, 0.465621591f));
        setup_write_read();
    }
};

/** Interface between fixtures and Google Test */
template<typename TestSetup>
struct error_metrics_test : public ::testing::Test, public TestSetup { };
}}}
using namespace illumina::interop::unittest;


typedef ::testing::Types<
error_metrics_hardcoded_fixture_v3,
error_metrics_write_read_fixture_v3
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
    EXPECT_EQ(TypeParam::actual_metrics.size(), TypeParam::expected_metrics.size());
    EXPECT_EQ(TypeParam::actual_metric_set.max_cycle(), TypeParam::expected_metric_set.max_cycle());

    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metrics.begin(), itActual = TypeParam::actual_metrics.begin();
        itExpected != TypeParam::expected_metrics.end() && itActual != TypeParam::actual_metrics.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->cycle(), itActual->cycle());
        EXPECT_EQ(itExpected->mismatch_count(), itActual->mismatch_count());
        EXPECT_NEAR(itExpected->errorRate(), itActual->errorRate(), 1e-7f);
        for(ptrdiff_t i=0;i<itExpected->mismatch_count();i++)
            EXPECT_EQ(itExpected->mismatch_cluster_count(i), itActual->mismatch_cluster_count(i));
    }
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
#include "src/tests/interop/metrics/stream_exception_tests.hpp"

