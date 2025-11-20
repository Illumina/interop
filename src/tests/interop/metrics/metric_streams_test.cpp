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
#include "src/tests/interop/run/info_test.h"

using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Fixture for expected vs actual binary data */
template<typename TestSetup>
struct metric_stream_test : public ::testing::Test, public TestSetup
{
    /** Type of metric set */
    typedef typename TestSetup::metric_set_t metric_set_t;
    /** Type of metric */
    typedef typename metric_set_t::metric_type metric_t;
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



void print_actual(std::ostream& out, const std::string& data)
{
    for(size_t i=0;i<data.size();++i)
    {
        out << "," << int(char(data[i]));
        if( i%20 == 0) out << std::endl;
    }
    out << std::endl;
    out << std::endl;
    out << std::endl;
}

/**
 * Confirm binary write matches expected binary data
 */
TYPED_TEST_P(metric_stream_test, test_write_read_binary_data)
{
    typedef typename TestFixture::metric_t metric_t;
    if (TypeParam::disable_binary_data_size) return;
    if (TestFixture::expected.size() != TestFixture::actual.size())
    {
        print_actual(std::cout, TestFixture::actual);
    }
    ASSERT_EQ(TestFixture::expected.size(), TestFixture::actual.size());
    if (TypeParam::disable_binary_data) return;
    for (::uint32_t i = 0;i<TestFixture::expected.size(); ++i)
    {
        // uncomment
        //---------------------------------------------------------------
        //if(int(TestFixture::expected[i]) != int(TestFixture::actual[i]))
        //    print_actual(std::cout, TestFixture::actual);
        //---------------------------------------------------------------
        ASSERT_EQ(int(TestFixture::expected[i]), int(TestFixture::actual[i]))
                                    << " i= " << i
                                    << " -> " << metric_t::prefix();
    }
}

/** Confirm the header size matches what is read
 */
TYPED_TEST_P(metric_stream_test, test_header_size)
{
    std::string tmp;
    typename TypeParam::metric_set_t metrics;
    TypeParam::create_expected(metrics);
    io::write_header_to_string(tmp, metrics);
    const size_t actual_size = tmp.length();

    const size_t expected_size = io::header_size(metrics);
    EXPECT_EQ(expected_size, actual_size);
}

/** Confirm the header size matches what is read
 */
TYPED_TEST_P(metric_stream_test, test_read_data_size)
{
    std::string tmp;
    typename TypeParam::metric_set_t metrics;
    TypeParam::create_expected(metrics);
    io::write_interop_to_string(tmp, metrics);
    const size_t computed_size = io::compute_buffer_size(metrics);
    EXPECT_EQ(tmp.size(), computed_size);
}

/** Confirm the header size matches what is read
 */
TYPED_TEST_P(metric_stream_test, test_write_data_size)
{
    typename TypeParam::metric_set_t metrics;
    TypeParam::create_expected(metrics);
    const size_t expected_size = io::compute_buffer_size(metrics);
    std::vector< ::uint8_t > buffer(expected_size);
    EXPECT_NO_THROW(io::write_interop_to_buffer(metrics, &buffer.front(), buffer.size()));
}

TEST(metric_stream_test, list_filenames)
{
    std::vector<std::string> error_metric_files;
    io::list_interop_filenames< model::metrics::error_metric >(error_metric_files, "", 3);
    ASSERT_EQ(error_metric_files.size(), 4u);
    EXPECT_EQ(error_metric_files[0], io::combine("InterOp", "ErrorMetricsOut.bin"));
    EXPECT_EQ(error_metric_files[1], io::combine("InterOp", "C1.1", "ErrorMetricsOut.bin"));
    EXPECT_EQ(error_metric_files[2], io::combine("InterOp", "C2.1", "ErrorMetricsOut.bin"));
    EXPECT_EQ(error_metric_files[3], io::combine("InterOp", "C3.1", "ErrorMetricsOut.bin"));
}

TEST(metric_stream_test, list_filenames_aggregated)
{
    model::metrics::run_metrics metrics;
    std::vector<std::string> files;
    metrics.list_filenames(files, "");
    const size_t metric_count = static_cast<size_t>(constants::MetricCount);
    EXPECT_EQ(metric_count, files.size());
}

TEST(metric_stream_test, list_filenames_by_cycle)
{
    model::run::read_info reads[] = {model::run::read_info(1, 1, 3, false)};
    model::metrics::run_metrics metrics(
            model::run::info(
                    model::run::flowcell_layout(),
                    util::to_vector(reads)
            ));
    std::vector<std::string> files;
    ASSERT_NE(0u, metrics.run_info().reads().size());
    ASSERT_NE(0u, metrics.run_info().reads()[0].total_cycles());
    ASSERT_NE(0u, metrics.run_info().total_cycles());
    metrics.list_filenames(files, "", true);
    const size_t metric_count = static_cast<size_t>(constants::MetricCount);
    EXPECT_EQ(metric_count*metrics.run_info().total_cycles()+metric_count, files.size());
}




REGISTER_TYPED_TEST_CASE_P(metric_stream_test,
                           test_read_data_size,
                           test_header_size,
                           test_write_read_binary_data,
                           test_write_data_size
);


INSTANTIATE_TYPED_TEST_CASE_P(Public, metric_stream_test, PublicFormats);

