/** Unit tests for the image metrics
 *
 *
 *  @file
 *
 *  @date 8/24/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <limits>
#include <fstream>
#include <gtest/gtest.h>
#include "interop/model/metric_sets/image_metric_set.h"
#include "src/tests/interop/metrics/metric_test_utils.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;



namespace illumina{ namespace interop { namespace unittest {
/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * Regression Test: 1947950_117213Bin2R0I
 *
 * @note Version 1
 */
struct image_metrics_hardcoded_fixture_v1 : util::fixture_helper<image_metrics, 1>
{
    enum{
        /** Do not check the expected binary data */
                disable_binary_data=true
    };
    /** Setup fixture */
    image_metrics_hardcoded_fixture_v1()
    {
        typedef image_metric::ushort_t ushort_t;
        const ushort_t channel_count = image_metric::MAX_CHANNELS;
        ushort_t min_contrast1[]  = {896, 1725,738,812};
        ushort_t min_contrast2[]  = {908, 1770,739,806};
        ushort_t min_contrast3[]  = {923, 1829,734,802};

        ushort_t max_contrast1[]  = {4840, 8144,3308,4974};
        ushort_t max_contrast2[]  = {4829, 8159,3302,4985};
        ushort_t max_contrast3[]  = {4829, 8236,3304,4947};
        expected_metrics.push_back(metric_type(1, 1101, 1, channel_count, to_vector(min_contrast1), to_vector(max_contrast1)));
        expected_metrics.push_back(metric_type(1, 1102, 1, channel_count, to_vector(min_contrast2), to_vector(max_contrast2)));
        expected_metrics.push_back(metric_type(1, 1103, 1, channel_count, to_vector(min_contrast3), to_vector(max_contrast3)));
        int tmp[] = {1,12,
                     1,0,77,4,1,0,0,0,128,3,232,18,
                     1,0,77,4,1,0,1,0,189,6,208,31,
                     1,0,77,4,1,0,2,0,226,2,236,12,
                     1,0,77,4,1,0,3,0,44,3,110,19,
                     1,0,78,4,1,0,0,0,140,3,221,18,
                     1,0,78,4,1,0,1,0,234,6,223,31,
                     1,0,78,4,1,0,2,0,227,2,230,12,
                     1,0,78,4,1,0,3,0,38,3,121,19,
                     1,0,79,4,1,0,0,0,155,3,221,18,
                     1,0,79,4,1,0,1,0,37,7,44,32,
                     1,0,79,4,1,0,2,0,222,2,232,12,
                     1,0,79,4,1,0,3,0,34,3,83,19
        };
        setup_hardcoded_binary(tmp, header_type(metric_type::MAX_CHANNELS));
    }
};

/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * @note Version 2
 */
struct image_metrics_hardcoded_fixture_v2 : util::fixture_helper<image_metrics, 2>
{
    /** Setup fixture */
    image_metrics_hardcoded_fixture_v2()
    {
        typedef image_metric::ushort_t ushort_t;
        const ushort_t channel_count = 2;
        ushort_t min_contrast1[]  = {231, 207};
        ushort_t min_contrast2[]  = {229, 205};
        ushort_t min_contrast3[]  = {231, 222};

        ushort_t max_contrast1[]  = {462, 387};
        ushort_t max_contrast2[]  = {457, 387};
        ushort_t max_contrast3[]  = {473, 416};
        expected_metrics.push_back(metric_type(7, 1114, 1, channel_count, to_vector(min_contrast1), to_vector(max_contrast1)));
        expected_metrics.push_back(metric_type(7, 1214, 1, channel_count, to_vector(min_contrast2), to_vector(max_contrast2)));
        expected_metrics.push_back(metric_type(7, 2114, 1, channel_count, to_vector(min_contrast3), to_vector(max_contrast3)));
        int tmp[] = {2,14,2
                ,7,0,90,4,1,0,-25,0,-49,0,-50,1,-125,1
                ,7,0,-66,4,1,0,-27,0,-51,0,-55,1,-125,1
                ,7,0,66,8,1,0,-25,0,-34,0,-39,1,-96,1
        };
        setup_hardcoded_binary(tmp, header_type(channel_count));
    }
};
/** Defines buffers shared by many different tests
*/
struct image_metrics_write_read_fixture_v1 : util::fixture_helper<image_metrics, 1>
{
    enum{
        /** Do not check the expected binary data */
                disable_binary_data=true
    };
    /** Setup fixture */
    image_metrics_write_read_fixture_v1()
    {
        typedef image_metric::ushort_t ushort_t;
        const ushort_t channel_count = image_metric::MAX_CHANNELS;
        ushort_t min_contrast1[]  = {231, 207,0,0};
        ushort_t min_contrast2[]  = {229, 205,0,0};
        ushort_t min_contrast3[]  = {231, 222,0,0};

        ushort_t max_contrast1[]  = {462, 387,0,0};
        ushort_t max_contrast2[]  = {457, 387,0,0};
        ushort_t max_contrast3[]  = {473, 416,0,0};
        expected_metrics.push_back(metric_type(7, 1114, 1, channel_count, to_vector(min_contrast1), to_vector(max_contrast1)));
        expected_metrics.push_back(metric_type(7, 1214, 1, channel_count, to_vector(min_contrast2), to_vector(max_contrast2)));
        expected_metrics.push_back(metric_type(7, 2114, 1, channel_count, to_vector(min_contrast3), to_vector(max_contrast3)));
        setup_write_read(header_type(channel_count));
    }
};

/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 2
 */
struct image_metrics_write_read_fixture_v2 : util::fixture_helper<image_metrics, 2>
{
    /** Setup fixture */
    image_metrics_write_read_fixture_v2()
    {
        typedef image_metric::ushort_t ushort_t;
        const ushort_t channel_count = 2;
        ushort_t min_contrast1[]  = {231, 207};
        ushort_t min_contrast2[]  = {229, 205};
        ushort_t min_contrast3[]  = {231, 222};

        ushort_t max_contrast1[]  = {462, 387};
        ushort_t max_contrast2[]  = {457, 387};
        ushort_t max_contrast3[]  = {473, 416};
        expected_metrics.push_back(metric_type(7, 1114, 1, channel_count, to_vector(min_contrast1), to_vector(max_contrast1)));
        expected_metrics.push_back(metric_type(7, 1214, 1, channel_count, to_vector(min_contrast2), to_vector(max_contrast2)));
        expected_metrics.push_back(metric_type(7, 2114, 1, channel_count, to_vector(min_contrast3), to_vector(max_contrast3)));
        setup_write_read(header_type(channel_count));
    }
};

/** Interface between fixtures and Google Test */
template<typename TestSetup>
struct image_metrics_test : public ::testing::Test, public TestSetup { };
}}}
using namespace illumina::interop::unittest;


typedef ::testing::Types<
image_metrics_hardcoded_fixture_v1,
image_metrics_hardcoded_fixture_v2,
image_metrics_write_read_fixture_v1
,image_metrics_write_read_fixture_v2
> Formats;
TYPED_TEST_CASE(image_metrics_test, Formats);


/**
 * @class illumina::interop::model::metrics::image_metric
 * @test Confirm version 1 of the metric can be written to and read from a stream
 * @test Confirm version 1 of the metric matches known binary file
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 */
TYPED_TEST(image_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metrics.size(), TypeParam::expected_metrics.size());
    EXPECT_EQ(TypeParam::actual_metric_set.channelCount(), TypeParam::expected_metric_set.channelCount());
    EXPECT_EQ(TypeParam::actual_metric_set.max_cycle(), TypeParam::expected_metric_set.max_cycle());

    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metrics.begin(), itActual = TypeParam::actual_metrics.begin();
        itExpected != TypeParam::expected_metrics.end() && itActual != TypeParam::actual_metrics.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->cycle(), itActual->cycle());
        EXPECT_EQ(itExpected->channelCount(), itActual->channelCount());
        for(size_t i=0;i<std::min(itExpected->channelCount(), itActual->channelCount());i++)
        {
            EXPECT_EQ(itExpected->minContrast(i), itActual->minContrast(i));
            EXPECT_EQ(itExpected->maxContrast(i), itActual->maxContrast(i));
        }
    }
}
#define FIXTURE image_metrics_test
/**
 * @class illumina::interop::model::metrics::image_metric
 * @test Confirm binary write matches expected binary data
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "src/tests/interop/metrics/stream_exception_tests.hpp"


