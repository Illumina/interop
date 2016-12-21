/** Unit tests for flowcell type plot logic
 *
 *
 *  @file
 *  @date 11/3/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "interop/model/plot/flowcell_data.h"
#include "interop/model/plot/filter_options.h"
#include "src/tests/interop/logic/inc/empty_plot_test_generator.h"
#include "src/tests/interop/logic/inc/plot_regression_test_generator.h"


using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::plot;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Setup for tests that compare two flowcell map plots */
struct flowcell_plot_tests : public generic_test_fixture<flowcell_data> {};


/** Test if flowcell matchs expected data
 */
TEST_P(flowcell_plot_tests, plot_data)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    const float tol = 1e-4f;
    EXPECT_EQ(actual.title(), expected.title());
    EXPECT_EQ(actual.subtitle(), expected.subtitle()) << GetParam();
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
        ASSERT_EQ(actual.tile_at(i), expected.tile_at(i));
    }
}

//---------------------------------------------------------------------------------------------------------------------
// Unit test parameter section
//---------------------------------------------------------------------------------------------------------------------

empty_plot_test_generator<flowcell_data> plot_flowcell_gen;
const constants::plot_types flowcell_types[] = {constants::FlowcellPlot};
std::vector<constants::plot_types> flowcell_gen_data(flowcell_types,
                                                    flowcell_types+util::length_of(flowcell_types));
// Setup unit tests for run summary tests
INSTANTIATE_TEST_CASE_P(plot_flowcell_unit_test,
                        flowcell_plot_tests,
                        ProxyValuesIn(plot_flowcell_gen, flowcell_gen_data));

class flowcell_write_read_generator
{
public:
    /** Define parameter type */
    typedef int parameter_type;
public:
    /** Constructor */
    flowcell_write_read_generator(const int){}
    /** Generate the expected and actual metric sets
     *
     * @param expected expected plot data
     * @param actual actual plot data
     */
    ::testing::AssertionResult generate(flowcell_data& expected, flowcell_data &actual) const
    {
        expected.set_title("Test Plot");
        expected.set_xlabel("X-axis");
        expected.set_ylabel("Y-axis");
        expected.set_range(0, 1);
        expected.resize(2, 3, 1);
        size_t i=0;
        expected(0,0) = std::numeric_limits<float>::quiet_NaN();
        expected(0,1) = static_cast<float>(i++);
        expected(0,2) = static_cast<float>(i++);
        expected(1,0) = static_cast<float>(i++);
        expected(1,1) = static_cast<float>(i++);
        expected(1,2) = static_cast<float>(i++);
        expected.tile_id(0,0) = static_cast< ::uint32_t >(i++);
        expected.tile_id(0,1) = static_cast< ::uint32_t >(i++);
        expected.tile_id(0,2) = static_cast< ::uint32_t >(i++);
        expected.tile_id(1,0) = static_cast< ::uint32_t >(i++);
        expected.tile_id(1,1) = static_cast< ::uint32_t >(i++);
        expected.tile_id(1,2) = static_cast< ::uint32_t >(i++);
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
        out << "flowcell_read_generator";
    }
};

flowcell_plot_tests::generator_type plot_flowcell_generators[] = {
        wrap(new standard_parameter_generator<flowcell_data, flowcell_write_read_generator>(0)),
};

// Setup unit tests for extended_tile_metrics_tests
INSTANTIATE_TEST_CASE_P(flowcell_unit_tests,
                        flowcell_plot_tests,
                        ::testing::ValuesIn(plot_flowcell_generators));

//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------


plot_regression_test_generator<flowcell_data> flowcell_regression_test_gen("plot", constants::FlowcellPlot);

INSTANTIATE_TEST_CASE_P(flowcell_regression_test,
                        flowcell_plot_tests,
                        ProxyValuesIn(flowcell_regression_test_gen, regression_test_data::instance().files()));
