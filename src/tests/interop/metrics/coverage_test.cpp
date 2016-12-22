/** Unit tests for test coverage over metrics and their formats
 *
 *
 *  @file
 *  @date 11/20/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "interop/io/metric_stream.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
#include "src/tests/interop/metrics/inc/metric_format_fixtures.h"

#ifdef _MSC_VER
#pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif


using namespace illumina::interop;
using namespace illumina::interop::unittest;



format_registry actual_format_registry;

/** Fixture to track formats available and being tested */
template<typename TestSetup>
struct metric_coverage_test : public ::testing::Test, public TestSetup
{
    /** Type of metric */
    typedef typename TestSetup::metric_t metric_t;
    /** Constructor */
    metric_coverage_test()
    {
        actual_format_registry(metric_t(), TestSetup::VERSION);
    }
};

TYPED_TEST_CASE_P(metric_coverage_test);


TYPED_TEST_P(metric_coverage_test, sentinel){}

REGISTER_TYPED_TEST_CASE_P(metric_coverage_test,
                           sentinel
);


INSTANTIATE_TYPED_TEST_CASE_P(Public, metric_coverage_test, PublicFormats);

/** Confirm that every metric and every version of their format is being tested here */
TEST(metric_coverage_test, stream_tests)
{
    model::metrics::run_metrics run;
    format_registry expected_format_registry;
    run.metrics_callback(expected_format_registry);
    std::sort(actual_format_registry.m_keys.begin(), actual_format_registry.m_keys.end());
    std::sort(expected_format_registry.m_keys.begin(), expected_format_registry.m_keys.end());
    ASSERT_THAT(actual_format_registry.m_keys, ::testing::ContainerEq(expected_format_registry.m_keys));
    for(format_registry::const_key_iterator it = expected_format_registry.m_keys.begin();
        it != expected_format_registry.m_keys.end();++it)
    {
        EXPECT_THAT(actual_format_registry.m_format_map[*it],
            ::testing::ContainerEq(expected_format_registry.m_format_map[*it])) << *it;
    }
}

/** Fixture to track formats available and being tested */
template<typename TestSetup>
struct format_coverage_test : public ::testing::Test
{
    /** Constructor */
    format_coverage_test() : actual(TestSetup::instance())
    {
        model::metrics::run_metrics run;
        run.metrics_callback(expected);
    }
    /** Expected format registry (from run metrics) */
    format_registry expected;
    /** Actual format registry (from tests) */
    format_registry& actual;
};
typedef ::testing::Types<
        hardcoded_metric_registry_t,
        write_read_metric_registry_t
> generators_t;
TYPED_TEST_CASE(format_coverage_test, generators_t);

/** Confirm that every metric and every version of their format is being tested here */
TYPED_TEST(format_coverage_test, format)
{
    std::sort(TestFixture::actual.m_keys.begin(), TestFixture::actual.m_keys.end());
    std::sort(TestFixture::expected.m_keys.begin(), TestFixture::expected.m_keys.end());
    ASSERT_THAT(TestFixture::actual.m_keys, ::testing::ContainerEq(TestFixture::expected.m_keys));
    for(format_registry::const_key_iterator it = TestFixture::expected.m_keys.begin();
        it != TestFixture::expected.m_keys.end();++it)
    {
        EXPECT_THAT(TestFixture::actual.m_format_map[*it],
                    ::testing::ContainerEq(TestFixture::expected.m_format_map[*it])) << *it;
    }
}


