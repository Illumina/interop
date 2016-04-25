/** Unit tests for the corrected intensity metrics
 *
 *
 *  @file
 *  @date 8/7/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

#include <fstream>
#include "interop/util/math.h"
#include "inc/corrected_intensity_metrics_test.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop;
using namespace illumina::interop::io;
using namespace illumina::interop::unittest;


typedef ::testing::Types<
        hardcoded_fixture<corrected_intensity_v2>,
        hardcoded_fixture<corrected_intensity_v3>,
        write_read_fixture<corrected_intensity_v2>,
        write_read_fixture<corrected_intensity_v3>
> Formats;
TYPED_TEST_CASE(corrected_intensity_metrics_test, Formats);

/** Compare two corrected intensity metric sets
 *
 * @param actual metric set read from stream
 * @param expected metric set setup from known data
 */
template<class T>
void compare_metrics(const T& actual, const T& expected)
{
    EXPECT_EQ(actual.version(), expected.version());
    EXPECT_EQ(actual.size(), expected.size());
    EXPECT_EQ(actual.max_cycle(), expected.max_cycle());

    for(typename T::const_iterator itExpected=expected.begin(), itActual = actual.begin();
        itExpected != expected.end() && itActual != actual.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->cycle(), itActual->cycle());
        if(expected.version() < 3)
        {
            EXPECT_EQ(itExpected->averageCycleIntensity(), itActual->averageCycleIntensity());
        }
        if(expected.version() == 2)
        {
            if (!std::isnan(itExpected->signalToNoise()) || !std::isnan(itActual->signalToNoise()))
                EXPECT_NEAR(itExpected->signalToNoise(), itActual->signalToNoise(), 1e-7f);
        }
        for(ptrdiff_t i=-1;i<constants::NUM_OF_BASES;i++)
            EXPECT_EQ(itExpected->calledCounts(i), itActual->calledCounts(i));
        for(size_t i=0;i<constants::NUM_OF_BASES;i++)
        {
            EXPECT_EQ(itExpected->correctedIntCalled(i), itActual->correctedIntCalled(i));
            if(expected.version() < 3)
            {
                EXPECT_EQ(itExpected->correctedIntAll(i), itActual->correctedIntAll(i));
            }
        }
    }
}

/**
 * @class illumina::interop::model::metrics::corrected_intensity_metric
 * @test Confirm version 2 of the metric matches known binary file
 * @test Confirm version 3 of the metric matches known binary file
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 3 of the metric can be written to and read from a stream
 */
TYPED_TEST(corrected_intensity_metrics_test, test_metric_io_fidelity)
{
    compare_metrics(TypeParam::actual_metric_set, TypeParam::expected_metric_set);
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
#include "inc/stream_tests.hpp"



