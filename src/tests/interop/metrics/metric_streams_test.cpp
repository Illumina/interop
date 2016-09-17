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
#include "interop/model/run_metrics.h"
#include "inc/corrected_intensity_metrics_test.h"
#include "inc/error_metrics_test.h"
#include "inc/extraction_metrics_test.h"
#include "inc/image_metrics_test.h"
#include "inc/index_metrics_test.h"
#include "inc/q_collapsed_metrics_test.h"
#include "inc/q_metrics_test.h"
#include "inc/tile_metrics_test.h"

using namespace illumina::interop;
using namespace illumina::interop::unittest;

typedef ::testing::Types
        <
                corrected_intensity_metric_v2,
                corrected_intensity_metric_v3,
                error_metric_v3,
                extraction_metric_v2,
                image_metric_v1,
                image_metric_v2,
                index_metric_v1,
                q_collapsed_metric_v2,
                q_collapsed_metric_v6,
                q_metric_v4,
                q_metric_v5,
                q_metric_v6,
                q_metric_v6_unbinned,
                tile_metric_v2
        > Formats;

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

TYPED_TEST_CASE(metric_stream_test, Formats);

/** Confirm that is_group_empty reports True */
TYPED_TEST(metric_stream_test, is_group_empty_true)
{
    typedef typename TypeParam::metric_t metric_t;
    model::metrics::run_metrics metrics;
    EXPECT_TRUE(metrics.is_group_empty(static_cast<constants::metric_group>(metric_t::TYPE)));
}
/** Confirm that is_group_empty reports True */
TYPED_TEST(metric_stream_test, is_group_empty_false)
{
    typedef typename TypeParam::metric_t metric_t;
    model::metrics::run_metrics metrics;
    io::read_interop_from_string(TestFixture::expected, metrics.get_set<metric_t>());
    EXPECT_FALSE(metrics.is_group_empty(static_cast<constants::metric_group>(metric_t::TYPE)));
}

/**
 * Confirm binary write matches expected binary data
 */
TYPED_TEST(metric_stream_test, test_expected_get_metric)
{
    typedef typename TypeParam::metric_set_t metric_set_t;
    metric_set_t metrics;
    TypeParam::create_expected(metrics);
    typename metric_set_t::key_vector keys = metrics.keys();
    for (size_t i = 0; i < keys.size(); i++)
    {
        ASSERT_EQ(keys[i], metrics.get_metric(keys[i]).id());
    }
}

/**
 * Confirm binary write matches expected binary data
 */
TYPED_TEST(metric_stream_test, test_write_read_binary_data)
{
    if (TypeParam::disable_binary_data_size) return;
    ASSERT_EQ(TestFixture::expected.size(), TestFixture::actual.size());
    if (TypeParam::disable_binary_data) return;
    for (::uint32_t i = 0;i<TestFixture::expected.size(); ++i)
    {
        ASSERT_EQ(int(TestFixture::expected[i]), int(TestFixture::actual[i])) << " i= " << i;
    }
}

/** Confirm bad_format_exception is thrown when version is unsupported
 */
TYPED_TEST(metric_stream_test, test_hardcoded_bad_format_exception)
{
    std::string tmp = std::string(TestFixture::expected);
    tmp[0] = 34;
    typename TypeParam::metric_set_t metrics;
    EXPECT_THROW(io::read_interop_from_string(tmp, metrics), io::bad_format_exception);
}

/** Confirm incomplete_file_exception is thrown for a small partial record
 */
TYPED_TEST(metric_stream_test, test_hardcoded_incomplete_file_exception)
{
    ::uint32_t incomplete = 0;
    for (::uint32_t i = 2; i < 25; i++)
    {
        typename TypeParam::metric_set_t metrics;
        try
        {
            io::read_interop_from_string(TestFixture::expected.substr(0, i), metrics);
        }
        catch (const io::incomplete_file_exception &)
        { incomplete++; }
        catch (const std::exception &ex)
        {
            std::cerr << i << " " << ex.what() << std::endl;
            throw;
        }
    }
    EXPECT_TRUE(incomplete > 10) << "incomplete: " << incomplete;
}
/** Confirm incomplete_file_exception is thrown for a mostly complete file
 */
TYPED_TEST(metric_stream_test, test_hardcoded_incomplete_file_exception_last_metric)
{
    typename TypeParam::metric_set_t metrics;
    EXPECT_THROW(io::read_interop_from_string(
            TestFixture::expected.substr(0, TestFixture::expected.length() - 4), metrics),
                 io::incomplete_file_exception);
}
/** Confirm bad_format_exception is thrown when record size is incorrect
 */
TYPED_TEST(metric_stream_test, test_hardcoded_incorrect_record_size)
{
    if (TypeParam::disable_check_record_size) return;
    std::string tmp = std::string(TestFixture::expected);
    tmp[1] = 0;
    typename TypeParam::metric_set_t metrics;
    EXPECT_THROW(io::read_interop_from_string(tmp, metrics), io::bad_format_exception);
}
/** Confirm file_not_found_exception is thrown when a file is not found
 */
TYPED_TEST(metric_stream_test, test_hardcoded_file_not_found)
{
    typename TypeParam::metric_set_t metrics;
    EXPECT_THROW(io::read_interop("/NO/FILE/EXISTS", metrics), io::file_not_found_exception);
}
/** Confirm reading from good data does not throw an exception
 */
TYPED_TEST(metric_stream_test, test_hardcoded_read)
{
    std::string tmp = std::string(TestFixture::expected);
    typename TypeParam::metric_set_t metrics;
    EXPECT_NO_THROW(io::read_interop_from_string(tmp, metrics));
}
/** Confirm the clear function works
 */
TYPED_TEST(metric_stream_test, test_clear)
{
    std::string tmp = std::string(TestFixture::expected);
    typename TypeParam::metric_set_t metrics;
    EXPECT_NO_THROW(io::read_interop_from_string(tmp, metrics));
    size_t cnt = metrics.size();
    metrics.clear();
    EXPECT_NO_THROW(io::read_interop_from_string(tmp, metrics));
    EXPECT_EQ(cnt, metrics.size());
}