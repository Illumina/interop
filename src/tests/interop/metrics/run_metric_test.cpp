/** Unit tests for the run metrics class
 *
 *
 *  @file
 *  @date 9/21/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */


#include <gtest/gtest.h>
#include "src/tests/interop/metrics/inc/metric_format_fixtures.h"


using namespace illumina::interop;
using namespace illumina::interop::unittest;

/** Fixture for a metric set */
template<typename TestSetup>
struct run_metric_test : public ::testing::Test, public TestSetup
{
    /** Type of metric set */
    typedef typename TestSetup::metric_set_t metric_set_t;
    /** Constructor */
    run_metric_test()
    {
        TestSetup::create_expected(expected.template get<metric_set_t>());
    }
    /** Expected metric set */
    model::metrics::run_metrics expected;
};
TYPED_TEST_CASE_P(run_metric_test);

/** Confirm the clear function works
 */
TYPED_TEST_P(run_metric_test, test_clear)
{
    typedef typename TestFixture::metric_set_t metric_set_t;
    metric_set_t& metric_set = TestFixture::expected.template get<metric_set_t>();
    size_t cnt = metric_set.size();
    TestFixture::expected.clear();
    TypeParam::create_expected(metric_set);
    EXPECT_EQ(cnt, metric_set.size());
}

/** Confirm that is_group_empty reports True */
TYPED_TEST_P(run_metric_test, is_group_empty_true)
{
    typedef typename TestFixture::metric_set_t metric_set_t;
    typedef typename metric_set_t::metric_type metric_t;
    TestFixture::expected.clear();
    EXPECT_TRUE(TestFixture::expected.is_group_empty(static_cast<constants::metric_group>(metric_t::TYPE)));
}

/** Confirm that is_group_empty reports True */
TYPED_TEST_P(run_metric_test, is_group_empty_false)
{
    typedef typename TestFixture::metric_set_t metric_set_t;
    typedef typename metric_set_t::metric_type metric_t;
    EXPECT_FALSE(TestFixture::expected.is_group_empty(static_cast<constants::metric_group>(metric_t::TYPE)));
}

/**
 * Confirm binary write matches expected binary data
 */
TYPED_TEST_P(run_metric_test, test_expected_get_metric)
{
    typedef typename TestFixture::metric_set_t metric_set_t;
    metric_set_t& metric_set = TestFixture::expected. template  get<metric_set_t>();
    const typename metric_set_t::key_vector keys = metric_set.keys();
    for (size_t i = 0; i < keys.size(); i++)
    {
        ASSERT_EQ(keys[i], metric_set.get_metric(keys[i]).id());
    }
}


REGISTER_TYPED_TEST_CASE_P(run_metric_test,
                           test_clear,
                           is_group_empty_true,
                           is_group_empty_false,
                           test_expected_get_metric
);


INSTANTIATE_TYPED_TEST_CASE_P(Public, run_metric_test, PublicFormats);

