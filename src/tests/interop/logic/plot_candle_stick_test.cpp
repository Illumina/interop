/** Unit tests for candle stick type plot logic
 *
 *
 *  @file
 *  @date 10/26/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "interop/model/plot/candle_stick_point.h"
#include "interop/model/plot/plot_data.h"
#include "interop/model/plot/filter_options.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"
#include "src/tests/interop/metrics/inc/extraction_metrics_test.h"
#include "src/tests/interop/logic/inc/empty_plot_test_generator.h"
#include "src/tests/interop/logic/inc/plot_regression_test_generator.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::plot;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;

/** Define the plot data */
typedef plot_data<model::plot::candle_stick_point> candle_stick_plot_data;
/** Setup for tests that compare two candle stick plots */
struct candle_stick_tests : public generic_test_fixture<candle_stick_plot_data> {};

/** Test that the filter iterator works */
TEST(candle_stick_tests, test_filter_iterator_by_cycle)
{
    const std::string channels[] = {"Red", "Green"};
    const model::run::read_info reads[] = {model::run::read_info(1, 3, false)};
    model::run::info run_info(model::run::flowcell_layout(2, 2), util::to_vector(channels), util::to_vector(reads));
    run_info.set_naming_method(constants::FourDigit);
    metric_filter_iterator metric_iterator;
    metric_iterator.reset(run_info, constants::ByCyclePlot);
    while(!metric_iterator.is_done())
    {
        ASSERT_NO_THROW(metric_iterator.options().validate(metric_iterator.metric(), run_info, true));
        if(metric_iterator.advance()) break;
    }
}

/** Test if candle stick plot matchs expected data
 */
TEST_P(candle_stick_tests, plot_data)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    const float tol = 1e-4f;
    EXPECT_EQ(actual.title(), expected.title());
    EXPECT_EQ(actual.x_axis().min(), expected.x_axis().min());
    EXPECT_EQ(actual.x_axis().max(), expected.x_axis().max()) << actual.empty() << ", " << expected.empty();
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
            INTEROP_ASSERT_NEAR(actual[i][j].p25(), expected[i][j].p25(), tol);
            INTEROP_ASSERT_NEAR(actual[i][j].p50(), expected[i][j].p50(), tol);
            INTEROP_ASSERT_NEAR(actual[i][j].p75(), expected[i][j].p75(), tol);
            INTEROP_ASSERT_NEAR(actual[i][j].lower(), expected[i][j].lower(), tol);
            INTEROP_ASSERT_NEAR(actual[i][j].upper(), expected[i][j].upper(), tol);
            EXPECT_EQ(actual[i][j].data_point_count(), expected[i][j].data_point_count());
            ASSERT_EQ(actual[i][j].outliers().size(), expected[i][j].outliers().size());
            for(size_t k=0;k<actual[i][j].outliers().size();++k)
                INTEROP_ASSERT_NEAR(actual[i][j].outliers()[k], expected[i][j].outliers()[k], tol);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Unit test parameter section
//---------------------------------------------------------------------------------------------------------------------

empty_plot_test_generator<candle_stick_plot_data> plot_by_cycle_gen;
const constants::plot_types candle_stick_types[] = {constants::ByCyclePlot,
                                                    constants::ByLanePlot};
std::vector<constants::plot_types> plot_by_cycle_gen_data(candle_stick_types,
                                                          candle_stick_types+util::length_of(candle_stick_types));
// Setup unit tests for run summary tests
INSTANTIATE_TEST_CASE_P(candle_stick_unit_test,
                        candle_stick_tests,
        ProxyValuesIn(plot_by_cycle_gen, plot_by_cycle_gen_data));

class candle_stick_read_generator
{
public:
    /** Define parameter type */
    typedef int parameter_type;
public:
    /** Constructor */
    candle_stick_read_generator(const int){}
    /** Generate the expected and actual metric sets
     *
     * @param expected expected plot data
     * @param actual actual plot data
     */
    ::testing::AssertionResult generate(candle_stick_plot_data& expected, candle_stick_plot_data &actual) const
    {
        expected.set_title("Test Plot");
        expected.set_xlabel("X-axis");
        expected.set_ylabel("Y-axis");
        expected.set_range(0, 1, 0, 2);
        expected.resize(2);
        expected.push_back(series<candle_stick_point>("Q Score",
                                                      "DarkGreen",
                                                      model::plot::series<candle_stick_point>::Candlestick));
        expected.push_back(model::plot::series<candle_stick_point>("Threshold(>=Q30)",
                                                                   "Green",
                                                                   model::plot::series<candle_stick_point>::Line));
        expected[0].resize(3);
        expected[1].resize(3);
        expected[0][0] = candle_stick_point(std::numeric_limits<float>::quiet_NaN(), 2, 3, 4, 5, 6, 7, std::vector<float>(1, 44));
        expected[0][1] = candle_stick_point(11, 12, 31, 41, 51, 61, 71, std::vector<float>(1, 3));
        expected[0][2] = candle_stick_point(12, 22, 23, 24, 25, 26, 27, std::vector<float>(1, 4));
        expected[1][0].set(9, 10);
        expected[1][1].set(12, 13);
        expected[1][2].set(15, 16);
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
        out << "candle_stick_read_generator";
    }
};

candle_stick_tests::generator_type plot_candle_stick_generators[] = {
        wrap(new standard_parameter_generator<candle_stick_plot_data, candle_stick_read_generator>(0)),
};

// Setup unit tests for extended_tile_metrics_tests
INSTANTIATE_TEST_CASE_P(candle_stick_unit_tests,
                        candle_stick_tests,
                        ::testing::ValuesIn(plot_candle_stick_generators));

//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------



plot_regression_test_generator<candle_stick_plot_data> by_cycle_regression_test_gen("plot", constants::ByCyclePlot);

INSTANTIATE_TEST_CASE_P(plot_by_cycle_regression_test,
                        candle_stick_tests,
                        ProxyValuesIn(by_cycle_regression_test_gen, regression_test_data::instance().files()));


plot_regression_test_generator<candle_stick_plot_data>  by_lane_regression_test_gen("plot", constants::ByLanePlot);

INSTANTIATE_TEST_CASE_P(plot_by_lane_regression_test,
                        candle_stick_tests,
                        ProxyValuesIn(by_lane_regression_test_gen, regression_test_data::instance().files()));

