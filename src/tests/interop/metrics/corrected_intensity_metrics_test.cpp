/** Unit tests for the corrected intensity metrics
 *
 *
 *  @file
 *
 *  @date 8/7/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */


#include <fstream>
#include <gtest/gtest.h>
#include "interop/model/metric_sets/corrected_intensity_metric_set.h"
#include "src/tests/interop/metrics/metric_test_utils.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop;
using namespace illumina::interop::io;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif


namespace illumina{ namespace interop { namespace unittest {
/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * Regression Test: 1947950_117213Bin2R0I
 *
 * @note Version 2
 */
struct corrected_intensity_hardcoded_fixture_v2 : util::fixture_helper<corrected_intensity_metrics, 2>
{
    /** Setup fixture */
    corrected_intensity_hardcoded_fixture_v2()
    {
        typedef metric_type::uint_t uint_t;
        typedef metric_type::ushort_t ushort_t;
        ushort_t correctedIntAll1[] = {1213, 966, 960, 1095};
        ushort_t correctedIntCalled1[] = {4070, 4074, 4029, 3972};
        uint_t calledCounts1[] = {0, 698433, 548189, 548712, 646638};
        expected_metrics.push_back(
                metric_type(1, 1104, 25, 1063, 11.9458876f, to_vector(correctedIntCalled1), to_vector(correctedIntAll1),
                            to_vector(calledCounts1)));
        ushort_t correctedIntAll2[] = {1558, 1151, 1158, 1293};
        uint_t calledCounts2[] = {10938, 733661, 537957, 543912, 615504};
        ushort_t correctedIntCalled2[] = {5013, 4983, 4915, 4932};
        expected_metrics.push_back(
                metric_type(1, 1104, 1, 1295, 13.3051805f, to_vector(correctedIntCalled2), to_vector(correctedIntAll2),
                            to_vector(calledCounts2)));
        ushort_t correctedIntAll3[] = {1171, 932, 912, 1069};
        uint_t calledCounts3[] = {0, 706987, 556441, 556067, 653959};
        ushort_t correctedIntCalled3[] = {3931, 3931, 3923, 3878};
        expected_metrics.push_back(
                metric_type(1, 1105, 25, 1025, 11.7396259f, to_vector(correctedIntCalled3), to_vector(correctedIntAll3),
                            to_vector(calledCounts3)));

        int tmp[] = {2, 48, 1, 0, 80, 4, 25, 0, 39, 4, 189, 4, 198, 3, 192, 3, 71, 4, 230, 15, 234, 15, 189, 15, 132,
                     15, 0, 0, 0, 0, 65, 168, 10, 0, 93, 93, 8, 0, 104, 95, 8, 0, 238, 221, 9, 0, 91, 34, 63, 65, 1, 0,
                     80, 4, 1, 0, 15, 5, 22, 6, 127, 4, 134, 4, 13, 5, 149, 19, 119, 19, 51, 19, 68, 19, 186, 42, 0, 0,
                     221, 49, 11, 0, 101, 53, 8, 0, 168, 76, 8, 0, 80, 100, 9, 0, 5, 226, 84, 65, 1, 0, 81, 4, 25, 0, 1,
                     4, 147, 4, 164, 3, 144, 3, 45, 4, 91, 15, 91, 15, 83, 15, 38, 15, 0, 0, 0, 0, 171, 201, 10, 0, 153,
                     125, 8, 0, 35, 124, 8, 0, 135, 250, 9, 0, 130, 213, 59, 65
        };
        setup_hardcoded_binary(tmp, header_type());
    }
};

/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * @note Version 3
 */
struct corrected_intensity_hardcoded_fixture_v3 : util::fixture_helper<corrected_intensity_metrics, 3>
{
    /** Setup fixture */
    corrected_intensity_hardcoded_fixture_v3()
    {
        typedef metric_type::uint_t uint_t;
        typedef metric_type::ushort_t ushort_t;
        uint_t calledCounts1[] = {52, 1049523, 654071, 500476, 982989};
        ushort_t correctedIntCalled1[] = {245, 252, 61, 235};
        //expected_metrics.push_back(metric_type(7, 1114, 1, to_vector(correctedIntCalled1), to_vector(calledCounts1)));
        expected_metrics.push_back(metric_type(7, 1114, 1, (correctedIntCalled1), (calledCounts1)));
        uint_t calledCounts2[] = {0, 1063708, 582243, 588028, 953132};
        ushort_t correctedIntCalled2[] = {232, 257, 68, 228};
        //expected_metrics.push_back(metric_type(7, 1114, 2, to_vector(correctedIntCalled2), to_vector(calledCounts2)));
        expected_metrics.push_back(metric_type(7, 1114, 2, (correctedIntCalled2), (calledCounts2)));
        uint_t calledCounts3[] = {0, 1022928, 617523, 594836, 951825};
        ushort_t correctedIntCalled3[] = {227, 268, 68, 229};
        expected_metrics.push_back(metric_type(7, 1114, 3, (correctedIntCalled3), (calledCounts3)));
        //expected_metrics.push_back(metric_type(7, 1114, 3, to_vector(correctedIntCalled3), to_vector(calledCounts3)));

        int tmp[] = {3, 34, 7, 0, 90, 4, 1, 0, -11, 0, -4, 0, 61, 0, -21, 0, 52, 0, 0, 0, -77, 3, 16, 0, -9, -6, 9, 0,
                     -4, -94, 7, 0, -51, -1, 14, 0, 7, 0, 90, 4, 2, 0, -24, 0, 1, 1, 68, 0, -28, 0, 0, 0, 0, 0, 28, 59,
                     16, 0, 99, -30, 8, 0, -4, -8, 8, 0, 44, -117, 14, 0, 7, 0, 90, 4, 3, 0, -29, 0, 12, 1, 68, 0, -27,
                     0, 0, 0, 0, 0, -48, -101, 15, 0, 51, 108, 9, 0, -108, 19, 9, 0, 17, -122, 14, 0
        };
        setup_hardcoded_binary(tmp, header_type());
    }
};

/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 2
 */
struct corrected_intensity_write_read_fixture_v2 : util::fixture_helper<corrected_intensity_metrics, 2>
{
    /** Setup fixture */
    corrected_intensity_write_read_fixture_v2()
    {
        typedef metric_type::uint_t uint_t;
        typedef metric_type::ushort_t ushort_t;
        ushort_t correctedIntAll1[] = {1213, 966, 960, 1095};
        ushort_t correctedIntCalled1[] = {4070, 4074, 4029, 3972};
        uint_t calledCounts1[] = {0, 698433, 548189, 548712, 646638};
        expected_metrics.push_back(
                metric_type(1, 1104, 25, 1063, 11.9458876f, to_vector(correctedIntCalled1), to_vector(correctedIntAll1),
                            to_vector(calledCounts1)));
        ushort_t correctedIntAll2[] = {1558, 1151, 1158, 1293};
        uint_t calledCounts2[] = {10938, 733661, 537957, 543912, 615504};
        ushort_t correctedIntCalled2[] = {5013, 4983, 4915, 4932};
        expected_metrics.push_back(
                metric_type(1, 1104, 1, 1295, 13.3051805f, to_vector(correctedIntCalled2), to_vector(correctedIntAll2),
                            to_vector(calledCounts2)));
        ushort_t correctedIntAll3[] = {1171, 932, 912, 1069};
        uint_t calledCounts3[] = {0, 706987, 556441, 556067, 653959};
        ushort_t correctedIntCalled3[] = {3931, 3931, 3923, 3878};
        expected_metrics.push_back(
                metric_type(1, 1105, 25, 1025, 11.7396259f, to_vector(correctedIntCalled3), to_vector(correctedIntAll3),
                            to_vector(calledCounts3)));
        setup_write_read();
    }
};


/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 3
 */
struct corrected_intensity_write_read_fixture_v3 : util::fixture_helper<corrected_intensity_metrics, 3>
{
    /** Setup fixture */
    corrected_intensity_write_read_fixture_v3()
    {
        typedef metric_type::uint_t uint_t;
        typedef metric_type::ushort_t ushort_t;
        uint_t calledCounts1[] = {52, 1049523, 654071, 500476, 982989};
        ushort_t correctedIntCalled1[] = {245, 252, 61, 235};
        expected_metrics.push_back(metric_type(7, 1114, 1, to_vector(correctedIntCalled1), to_vector(calledCounts1)));
        uint_t calledCounts2[] = {0, 1063708, 582243, 588028, 953132};
        ushort_t correctedIntCalled2[] = {232, 257, 68, 228};
        expected_metrics.push_back(metric_type(7, 1114, 2, to_vector(correctedIntCalled2), to_vector(calledCounts2)));
        uint_t calledCounts3[] = {0, 1022928, 617523, 594836, 951825};
        ushort_t correctedIntCalled3[] = {227, 268, 68, 229};
        expected_metrics.push_back(metric_type(7, 1114, 3, to_vector(correctedIntCalled3), to_vector(calledCounts3)));
        setup_write_read();
    }
};

/** Interface between fixtures and Google Test */
template<typename TestSetup>
struct corrected_intensity_metrics_test : public ::testing::Test, public TestSetup { };

}}}
using namespace illumina::interop::unittest;


typedef ::testing::Types<
corrected_intensity_hardcoded_fixture_v2,
corrected_intensity_hardcoded_fixture_v3
,corrected_intensity_write_read_fixture_v2,
corrected_intensity_write_read_fixture_v3
> Formats;
TYPED_TEST_CASE(corrected_intensity_metrics_test, Formats);


/**
 * @class illumina::interop::model::metrics::corrected_intensity_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 3 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 * @test Confirm version 3 of the metric matches known binary file
 */
TYPED_TEST(corrected_intensity_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metrics.size(), TypeParam::expected_metrics.size());

    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metrics.begin(), itActual = TypeParam::actual_metrics.begin();
        itExpected != TypeParam::expected_metrics.end() && itActual != TypeParam::actual_metrics.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->cycle(), itActual->cycle());
        if(TypeParam::VERSION < 3)
        {
            EXPECT_EQ(itExpected->averageCycleIntensity(), itActual->averageCycleIntensity());
        }
        if(TypeParam::VERSION == 2)
        {
            if (!std::isnan(itExpected->signalToNoise()) || !std::isnan(itActual->signalToNoise()))
                EXPECT_NEAR(itExpected->signalToNoise(), itActual->signalToNoise(), 1e-7f);
        }
        for(ptrdiff_t i=-1;i<constants::NUM_OF_BASES;i++)
            EXPECT_EQ(itExpected->calledCounts(i), itActual->calledCounts(i));
        for(size_t i=0;i<constants::NUM_OF_BASES;i++)
        {
            EXPECT_EQ(itExpected->correctedIntCalled(i), itActual->correctedIntCalled(i));
            if(TypeParam::VERSION < 3) {
                EXPECT_EQ(itExpected->correctedIntAll(i), itActual->correctedIntAll(i));
            }
        }
    }
}

#define FIXTURE corrected_intensity_metrics_test
/**
 * @class illumina::interop::model::metrics::corrected_intensity_metric
 * @test Confirm binary write matches expected binary data
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "src/tests/interop/metrics/stream_exception_tests.hpp"



