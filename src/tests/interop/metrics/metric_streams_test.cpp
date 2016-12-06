/** Unit tests for the metric streams
 *
 *
 *  @file
 *  @date 9/17/2016
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#ifdef _MSC_VER
#pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

#include <gtest/gtest.h>
#include "interop/io/metric_stream.h"
#include "interop/io/metric_file_stream.h"
#include "src/tests/interop/metrics/inc/metric_format_fixtures.h"

using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Fixture for expected vs actual binary data */
template<typename TestSetup>
struct metric_stream_test : public ::testing::Test, public TestSetup
{
    /** Type of metric set */
    typedef typename TestSetup::metric_set_t metric_set_t;
    /** Constructor */
    metric_stream_test()
    {
        TestSetup::create_binary_data(expected);
        metric_set_t metrics;
        TestSetup::create_expected(metrics);
        std::ostringstream fout;
        io::write_metrics(fout, metrics);
        actual = fout.str();
    }
    /** Expected binary data */
    std::string expected;
    /** Actual binary data */
    std::string actual;
};
TYPED_TEST_CASE_P(metric_stream_test);

/**
 * Confirm binary write matches expected binary data
 */
TYPED_TEST_P(metric_stream_test, test_write_read_binary_data)
{
    if (TypeParam::disable_binary_data_size) return;
    ASSERT_EQ(TestFixture::expected.size(), TestFixture::actual.size());
    if (TypeParam::disable_binary_data) return;
    for (::uint32_t i = 0;i<TestFixture::expected.size(); ++i)
    {
        ASSERT_EQ(int(TestFixture::expected[i]), int(TestFixture::actual[i])) << " i= " << i;
    }
}

/** Confirm the header size matches what is read
 */
TYPED_TEST_P(metric_stream_test, test_header_size)
{
    std::string tmp = std::string(TestFixture::expected);
    typename TypeParam::metric_set_t metrics;
    const size_t actual_size = io::read_header_from_string(tmp, metrics);
    const size_t expected_size = io::header_size(metrics);
    EXPECT_EQ(expected_size, actual_size);
}

/** Confirm the header size matches what is read
 */
TYPED_TEST_P(metric_stream_test, test_read_data_size)
{
    if (TypeParam::disable_binary_data_size || TypeParam::disable_binary_data) return;
    std::string tmp = std::string(TestFixture::expected);
    typename TypeParam::metric_set_t metrics;
    io::read_interop_from_string(tmp, metrics);
    if(io::is_multi_record(metrics)) return;
    const size_t expected_size = io::compute_buffer_size(metrics);
    EXPECT_EQ(tmp.size(), expected_size);
}

TEST(metric_stream_test, list_filenames)
{
    std::vector<std::string> error_metric_files;
    io::list_interop_filenames< model::metrics::error_metric >(error_metric_files, "");
    ASSERT_EQ(error_metric_files.size(), 1u);
    EXPECT_EQ(error_metric_files[0], io::combine("InterOp", "ErrorMetricsOut.bin"));
}



REGISTER_TYPED_TEST_CASE_P(metric_stream_test,
                           test_read_data_size,
                           test_header_size,
                           test_write_read_binary_data
);


INSTANTIATE_TYPED_TEST_CASE_P(Public, metric_stream_test, PublicFormats);

