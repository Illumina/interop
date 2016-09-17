/** Unit tests for the corrected intensity metrics
 *
 *
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
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
#include "src/tests/interop/metrics/inc/corrected_intensity_metrics_test.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop;
using namespace illumina::interop::io;
using namespace illumina::interop::unittest;


typedef metric_set< corrected_intensity_metric > corrected_intensity_metric_set;
/** Setup for tests that compare two corrected intensity metric sets */
struct corrected_intensity_metrics_tests : public generic_test_fixture< corrected_intensity_metric_set > {};

corrected_intensity_metrics_tests::generator_type corrected_intensity_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< corrected_intensity_metric_v2 >) ,
        wrap(new write_read_metric_generator< corrected_intensity_metric_v2 >),
        wrap(new hardcoded_metric_generator< corrected_intensity_metric_v3 >),
        wrap(new write_read_metric_generator< corrected_intensity_metric_v3 >)
};

// Setup unit tests for corrected_intensity_metrics_tests
INSTANTIATE_TEST_CASE_P(corrected_intensity_metric_unit_test,
                        corrected_intensity_metrics_tests,
                        ::testing::ValuesIn(corrected_intensity_unit_test_generators));


/**
 * @class illumina::interop::model::metrics::corrected_intensity_metric
 * @test Confirm version 2 of the metric matches known binary file
 * @test Confirm version 3 of the metric matches known binary file
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 3 of the metric can be written to and read from a stream
 */
TEST_P(corrected_intensity_metrics_tests, test_metric_io_fidelity)
{
    typedef corrected_intensity_metric_set::const_iterator const_iterator;
    if(!test) return;// Disable test for rebaseline
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
            if (!std::isnan(it_expected->signal_to_noise()) || !std::isnan(it_actual->signal_to_noise()))
                EXPECT_NEAR(it_expected->signal_to_noise(), it_actual->signal_to_noise(), tol);
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
regression_test_metric_generator<corrected_intensity_metric_set> corrected_intensity_regression_gen("metrics");
INSTANTIATE_TEST_CASE_P(corrected_intensity_metric_regression_test,
                        corrected_intensity_metrics_tests,
                        ProxyValuesIn(corrected_intensity_regression_gen, regression_test_data::instance().files()));




