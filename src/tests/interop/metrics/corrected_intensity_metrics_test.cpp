/** Unit tests for the corrected intensity metrics
 *
 *  @see https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#how-to-write-value-parameterized-tests
 *  @file
 *  @date 8/7/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#ifdef _MSC_VER
    #pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

#include <gtest/gtest.h>
#include "interop/util/math.h"
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/corrected_intensity_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop;
using namespace illumina::interop::io;
using namespace illumina::interop::unittest;


typedef metric_set< corrected_intensity_metric > corrected_intensity_metric_set;
/** Fixture for tests that compare two corrected intensity metric sets
 *
 * @see https://github.com/google/googletest/blob/master/googletest/docs/AdvancedGuide.md#how-to-write-value-parameterized-tests
 */
struct corrected_intensity_metrics_tests : public generic_test_fixture< corrected_intensity_metric_set > {};

/**
 * This array contains a set of fixtures used to generate data for all TEST_P tests labeled
 * corrected_intensity_metrics_tests.
 *
 * This used the parameter based method in GTest. The genertic_test_fixture will call GetParam to use each fixture
 * to populate the expected and actual metric_set.
 */
corrected_intensity_metrics_tests::generator_type corrected_intensity_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< corrected_intensity_metric_v2 >) ,
        wrap(new write_read_metric_generator< corrected_intensity_metric_v2 >),
        wrap(new hardcoded_metric_generator< corrected_intensity_metric_v3 >),
        wrap(new write_read_metric_generator< corrected_intensity_metric_v3 >)
};

// Specificy the fixtures used for corrected_intensity_metrics_tests TEST_P functions
INSTANTIATE_TEST_CASE_P(corrected_intensity_metric_unit_test,
                        corrected_intensity_metrics_tests,
                        ::testing::ValuesIn(corrected_intensity_unit_test_generators));


/**
 * @class illumina::interop::model::metrics::corrected_intensity_metric
 * @test Confirm version 2 of the metric matches known binary file
 * @test Confirm version 3 of the metric matches known binary file
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 3 of the metric can be written to and read from a stream
 *
 * The test, actual and expected variables are metric sets that are contained in the fixture
 * `corrected_intensity_metrics_tests`.
 *
 * The `test` variable is only set to false when the regression test is being rebaselined.
 */
TEST_P(corrected_intensity_metrics_tests, test_metric_io_fidelity)
{
    typedef corrected_intensity_metric_set::const_iterator const_iterator;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    const float tol = 1e-3f;
    EXPECT_EQ(actual.version(), expected.version());
    ASSERT_EQ(actual.size(), expected.size());
    EXPECT_EQ(actual.max_cycle(), expected.max_cycle());

    for(const_iterator it_expected=expected.begin(),
                it_actual = actual.begin();
        it_expected != expected.end() && it_actual != actual.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());
        EXPECT_EQ(it_expected->cycle(), it_actual->cycle());
        if(expected.version() < 3)
        {
            EXPECT_EQ(it_expected->average_cycle_intensity(), it_actual->average_cycle_intensity());
        }
        if(expected.version() == 2)
        {
            INTEROP_ASSERT_NEAR(it_expected->signal_to_noise(), it_actual->signal_to_noise(), tol);
        }
        for(ptrdiff_t i=-1;i<constants::NUM_OF_BASES;i++)
            EXPECT_EQ(it_expected->called_counts(static_cast<constants::dna_bases>(i)),
                      it_actual->called_counts(static_cast<constants::dna_bases>(i)));
        for(size_t i=0;i<constants::NUM_OF_BASES;i++)
        {
            EXPECT_EQ(it_expected->corrected_int_called(static_cast<constants::dna_bases>(i)),
                      it_actual->corrected_int_called(static_cast<constants::dna_bases>(i)));
            if(expected.version() < 3)
            {
                EXPECT_EQ(it_expected->corrected_int_all(static_cast<constants::dna_bases>(i)),
                          it_actual->corrected_int_all(static_cast<constants::dna_bases>(i)));
            }
        }
    }
}

TEST(corrected_intensity_metrics_test, test_percent_base_nan)
{
    typedef corrected_intensity_metric::ushort_array_t ushort_array_t;
    typedef corrected_intensity_metric::uint_array_t uint_array_t;
    corrected_intensity_metric metric(1, 1112, 1, ushort_array_t(4,0), uint_array_t(5,0));
    EXPECT_TRUE(std::isnan(metric.percent_bases()[0]));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Use the sub folder `metrics` in the baseline folder to contain all the baseline data
regression_test_metric_generator<corrected_intensity_metric_set> corrected_intensity_regression_gen;

/**
 * This method populates the regression test with run folders specified on the command line. The run folders
 * are contained in the method `files` within a singleton class called `regression_test_data`. This is populated
 * in the `unit_tests.cpp` file.
 *
 * The `ProxyValuesIn` function is a hack in GTest that allows the population of the files to be deferred.
 */
INSTANTIATE_TEST_CASE_P(corrected_intensity_metric_regression_test,
                        corrected_intensity_metrics_tests,
                        ProxyValuesIn(corrected_intensity_regression_gen, regression_test_data::instance().files()));





