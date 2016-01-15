/** Unit tests for the extraction metrics
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
#include "interop/model/metric_sets/extraction_metric_set.h"
#include "src/tests/interop/metrics/metric_test_utils.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;


namespace illumina{ namespace interop { namespace unittest {
/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * @note Version 2
 */
struct extraction_metrics_hardcoded_fixture_v2 : util::fixture_helper<extraction_metrics, 2>
{
    enum{
        /** Do not check the expected binary data */
        disable_binary_data=true
    };
    /** Setup fixture */
    extraction_metrics_hardcoded_fixture_v2()
    {
        typedef extraction_metric::ushort_t ushort_t;
        float focus1[] = {2.24664021f, 2.1896739f, 0, 0};
        ushort_t p90_1[]  = {302, 273, 0, 0};
        expected_metrics.push_back(metric_type(7, 1114, 1, interop::util::csharp_date_time(9859129975844165472ul), to_vector(p90_1), to_vector(focus1)));
        float focus2[] = {2.23177338f, 2.20616174f, 0, 0};
        ushort_t p90_2[]  = {312, 273, 0, 0};
        expected_metrics.push_back(metric_type(7, 1214, 1, interop::util::csharp_date_time(9859129975872781680ul), to_vector(p90_2), to_vector(focus2)));
        float focus3[] = {2.10524225f, 2.14023066f, 0, 0};
        ushort_t p90_3[]  = {349, 302, 0, 0};
        expected_metrics.push_back(metric_type(7, 2114, 1, interop::util::csharp_date_time(9859129975901427921ul), to_vector(p90_3), to_vector(focus3)));
        int tmp[] = {2,38,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
                ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
                ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120
        };
        setup_hardcoded_binary(tmp, header_type());

    }
};


/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 2
 */
struct extraction_metrics_write_read_fixture_v2 : util::fixture_helper<extraction_metrics, 2>
{
    enum{
        /** Do not check the expected binary data */
        disable_binary_data=true
    };
    /** Setup fixture */
    extraction_metrics_write_read_fixture_v2()
    {
        typedef extraction_metric::ushort_t ushort_t;
        float focus1[] = {2.24664021f, 2.1896739f, 0, 0};
        ushort_t p90_1[]  = {302, 273, 0, 0};
        expected_metrics.push_back(metric_type(7, 1114, 1, interop::util::csharp_date_time(9859129975844165472ul), to_vector(p90_1), to_vector(focus1)));
        float focus2[] = {2.23177338f, 2.20616174f, 0, 0};
        ushort_t p90_2[]  = {312, 273, 0, 0};
        expected_metrics.push_back(metric_type(7, 1214, 1, interop::util::csharp_date_time(9859129975872781680ul), to_vector(p90_2), to_vector(focus2)));
        float focus3[] = {2.10524225f, 2.14023066f, 0, 0};
        ushort_t p90_3[]  = {349, 302, 0, 0};
        expected_metrics.push_back(metric_type(7, 2114, 1, interop::util::csharp_date_time(9859129975901427921ul), to_vector(p90_3), to_vector(focus3)));
        setup_write_read();

    }
};

/** Interface between fixtures and Google Test */
template<typename TestSetup>
struct extraction_metrics_test : public ::testing::Test, public TestSetup { };
}}}
using namespace illumina::interop::unittest;


typedef ::testing::Types<
extraction_metrics_hardcoded_fixture_v2,
extraction_metrics_write_read_fixture_v2
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
    EXPECT_EQ(TypeParam::actual_metrics.size(), TypeParam::expected_metrics.size());

    double medianCycleTime = TypeParam::actual_metric_set.medianCycleTime();
    EXPECT_GT(medianCycleTime, 0.0);


    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metrics.begin(), itActual = TypeParam::actual_metrics.begin();
        itExpected != TypeParam::expected_metrics.end() && itActual != TypeParam::actual_metrics.end();
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
#include "src/tests/interop/metrics/stream_exception_tests.hpp"
