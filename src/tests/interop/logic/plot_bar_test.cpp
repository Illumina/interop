/** Unit tests for bar type plot logic
 *
 *
 *  @file
 *  @date 11/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "interop/model/plot/bar_point.h"
#include "interop/model/plot/plot_data.h"
#include "interop/model/plot/filter_options.h"
#include "src/tests/interop/logic/inc/empty_plot_test_generator.h"
#include "src/tests/interop/logic/inc/plot_regression_test_generator.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::plot;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Define the plot data */
typedef plot_data<bar_point> bar_plot_data;
/** Setup for tests that compare two bar plots */
struct bar_plot_tests : public generic_test_fixture<bar_plot_data> {};


/** Test if bar plot matchs expected data
 */
TEST_P(bar_plot_tests, plot_data)
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
    ASSERT_EQ(actual.size(), expected.size());
    for(size_t i=0;i<actual.size();++i)
    {
        EXPECT_EQ(actual[i].title(), expected[i].title());
        EXPECT_EQ(actual[i].color(), expected[i].color());
        EXPECT_EQ(actual[i].series_type(), expected[i].series_type());
        EXPECT_THAT(actual[i].options(), ::testing::ElementsAreArray(expected[i].options()));
        ASSERT_EQ(actual[i].size(), expected[i].size());
        for(size_t j=0;j<actual[i].size();++j)
        {
            INTEROP_ASSERT_NEAR(actual[i][j].x(), expected[i][j].x(), tol);
            INTEROP_ASSERT_NEAR(actual[i][j].y(), expected[i][j].y(), tol);
            INTEROP_ASSERT_NEAR(actual[i][j].max_value(), expected[i][j].max_value(), tol);
            INTEROP_ASSERT_NEAR(actual[i][j].min_value(), expected[i][j].min_value(), tol);
            INTEROP_ASSERT_NEAR(actual[i][j].width(), expected[i][j].width(), tol);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Unit test parameter section
//---------------------------------------------------------------------------------------------------------------------

empty_plot_test_generator<bar_plot_data> plot_bar_gen;
const constants::plot_types bar_types[] = {constants::QHistogramPlot, constants::SampleQCPlot};
std::vector<constants::plot_types> plot_bar_gen_data(bar_types,
                                                          bar_types+util::length_of(bar_types));
// Setup unit tests for run summary tests
INSTANTIATE_TEST_CASE_P(bar_plot_unit_test,
                        bar_plot_tests,
                        ProxyValuesIn(plot_bar_gen, plot_bar_gen_data));


class bar_write_read_generator
{
public:
    /** Define parameter type */
    typedef int parameter_type;
public:
    /** Constructor */
    bar_write_read_generator(const int){}
    /** Generate the expected and actual metric sets
     *
     * @param expected expected plot data
     * @param actual actual plot data
     */
    ::testing::AssertionResult generate(bar_plot_data& expected, bar_plot_data &actual) const
    {
        expected.set_title("Test Plot");
        expected.set_xlabel("X-axis");
        expected.set_ylabel("Y-axis");
        expected.set_range(0, 1, 0, 2);
        expected.resize(2);
        expected.push_back(series<bar_point>("Q Score", "DarkGreen", model::plot::series<bar_point>::Bar));
        expected.push_back(model::plot::series<bar_point>("Threshold(>=Q30)", "Green", model::plot::series<bar_point>::Line));
        expected[0].resize(3);
        expected[1].resize(3);
        expected[0][0].set(std::numeric_limits<float>::quiet_NaN(), 1, 2);
        expected[0][1].set(3, 4, 5);
        expected[0][2].set(6, 7, 8);
        expected[1][0].set(9, 10, 11);
        expected[1][1].set(12, 13, 14);
        expected[1][2].set(15, 16, 17);
        expected[0].add_option("Dummy1");


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
        out << "bar_write_read_generator";
    }
};

bar_plot_tests::generator_type plot_bar_generators[] = {
        wrap(new standard_parameter_generator<bar_plot_data, bar_write_read_generator>(0)),
};

// Setup unit tests for extended_tile_metrics_tests
INSTANTIATE_TEST_CASE_P(bar_plot_add_unit_tests,
                        bar_plot_tests,
                        ::testing::ValuesIn(plot_bar_generators));

//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------


plot_regression_test_generator<bar_plot_data> qscore_histogram_regression_test_gen("plot", constants::QHistogramPlot);

INSTANTIATE_TEST_CASE_P(qscore_histogram_regression_test,
                        bar_plot_tests,
                        ProxyValuesIn(qscore_histogram_regression_test_gen, regression_test_data::instance().files()));


plot_regression_test_generator<bar_plot_data> sample_qc_regression_test_gen("plot", constants::QHistogramPlot);

INSTANTIATE_TEST_CASE_P(sample_qc_regression_test,
                        bar_plot_tests,
                        ProxyValuesIn(sample_qc_regression_test_gen, regression_test_data::instance().files()));

