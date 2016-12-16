/** Unit tests for heatmap type plot logic
 *
 *
 *  @file
 *  @date 11/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "interop/model/plot/heatmap_data.h"
#include "interop/model/plot/filter_options.h"
#include "src/tests/interop/logic/inc/empty_plot_test_generator.h"
#include "src/tests/interop/logic/inc/plot_regression_test_generator.h"


using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::plot;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Setup for tests that compare two heat map plots */
struct heatmap_plot_tests : public generic_test_fixture<heatmap_data> {};


/** Test if Heatmap matchs expected data
 */
TEST_P(heatmap_plot_tests, plot_data)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    const float tol = 1e-4f;
    EXPECT_EQ(actual.title(), expected.title());
    EXPECT_EQ(actual.x_axis().min(), expected.x_axis().min());
    EXPECT_EQ(actual.x_axis().max(), expected.x_axis().max());
    EXPECT_EQ(actual.x_axis().label(), expected.x_axis().label());
    EXPECT_EQ(actual.y_axis().min(), expected.y_axis().min());
    EXPECT_EQ(actual.y_axis().max(), expected.y_axis().max());
    EXPECT_EQ(actual.y_axis().label(), expected.y_axis().label());
    ASSERT_EQ(actual.row_count(), expected.row_count());
    ASSERT_EQ(actual.column_count(), expected.column_count());
    for(size_t i=0, n=actual.length();i<n;++i)
    {
        INTEROP_ASSERT_NEAR(actual[i], expected[i], tol);
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Unit test parameter section
//---------------------------------------------------------------------------------------------------------------------

empty_plot_test_generator<heatmap_data> plot_heatmap_gen;
const constants::plot_types heatmap_types[] = {constants::QHeatmapPlot};
std::vector<constants::plot_types> heatmap_gen_data(heatmap_types,
                                                     heatmap_types+util::length_of(heatmap_types));
// Setup unit tests for run summary tests
INSTANTIATE_TEST_CASE_P(plot_heatmap_unit_test,
                        heatmap_plot_tests,
                        ProxyValuesIn(plot_heatmap_gen, heatmap_gen_data));

class heatmap_write_read_generator
{
public:
    /** Define parameter type */
    typedef int parameter_type;
public:
    /** Constructor */
    heatmap_write_read_generator(const int){}
    /** Generate the expected and actual metric sets
     *
     * @param expected expected plot data
     * @param actual actual plot data
     */
    ::testing::AssertionResult generate(heatmap_data& expected, heatmap_data &actual) const
    {
        expected.set_title("Test Plot");
        expected.set_xlabel("X-axis");
        expected.set_ylabel("Y-axis");
        expected.set_range(0, 1, 0, 2);
        expected.resize(2,3);
        size_t i=0;
        expected(0,0) = std::numeric_limits<float>::quiet_NaN();
        expected(0,1) = static_cast<float>(i++);
        expected(0,2) = static_cast<float>(i++);
        expected(1,0) = static_cast<float>(i++);
        expected(1,1) = static_cast<float>(i++);
        expected(1,2) = static_cast<float>(i++);
        std::ostringstream oss;
        oss << expected;

        std::istringstream iss(oss.str());
        iss >> actual;
        return ::testing::AssertionSuccess();
    }
    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream &out) const
    {
        out << "heatmap_read_generator";
    }
};

heatmap_plot_tests::generator_type plot_heatmap_generators[] = {
        wrap(new standard_parameter_generator<heatmap_data, heatmap_write_read_generator>(0)),
};

// Setup unit tests for extended_tile_metrics_tests
INSTANTIATE_TEST_CASE_P(heatmap_unit_tests,
                        heatmap_plot_tests,
                        ::testing::ValuesIn(plot_heatmap_generators));

//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------


plot_regression_test_generator<heatmap_data> qscore_heatmap_regression_test_gen("plot", constants::QHeatmapPlot);

INSTANTIATE_TEST_CASE_P(qscore_heatmap_regression_test,
                        heatmap_plot_tests,
                        ProxyValuesIn(qscore_heatmap_regression_test_gen, regression_test_data::instance().files()));
