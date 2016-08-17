/** Regression tests for image table logic
 *
 *  @file
 *  @date 7/21/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "interop/util/math.h"
#include "interop/util/length_of.h"
#include "interop/logic/table/create_imaging_table.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"
#include "src/tests/interop/inc/regression_fixture.h"
#include "interop/io/table/imaging_table_csv.h"

using namespace illumina::interop;
using namespace illumina::interop::unittest;

namespace illumina{ namespace interop {namespace model {namespace table
{
    bool operator==(const imaging_column& lhs, const imaging_column& rhs);
}}}}

template<typename T>
std::string to_string(const std::vector<T>& values)
{
    std::ostringstream out;
    for(size_t i=0;i<values.size();++i)
        out << values[i] << ",";
    return out.str();
}


//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------
/** Imaging regression test fixture.
 *
 * This provides the test group name, provides the tested type as imaging_table, ensures the output file is prefixed with
 * image_table and finally properly sets up the expected imaging_table object.
 *
 */
struct imaging_table_regression_test : public regression_test_fixture< imaging_table_regression_test, model::table::imaging_table>
{
    /** Constructor */
    imaging_table_regression_test() : regression_test_fixture< imaging_table_regression_test, model::table::imaging_table>("image_table"){}
    /** Populate the actual imaging table using the given run_metrics
     *
     * @param actual_metrics run_metrics read in from a run_folder
     * @param actual imaging table constructed from the run_metrics
     */
    static void populate_actual(model::metrics::run_metrics& actual_metrics, model::table::imaging_table& actual)
    {
        logic::table::create_imaging_table(actual_metrics, actual);
    }
};


TEST_P(imaging_table_regression_test, compare_to_baseline)
{
    if(!test) return;
    ASSERT_EQ(expected.column_count(), actual.column_count()) << "Baseline: " << to_string(expected.columns()) << "\n   Actual: " << to_string(actual.columns());
    ASSERT_EQ(expected.row_count(), actual.row_count());
    EXPECT_THAT(expected.columns(), ::testing::ElementsAreArray(actual.columns()));
    std::vector<size_t> expected_idx(expected.row_count(), expected.row_count());
    for(size_t i=0;i<expected_idx.size();++i) expected_idx[i] = i;
    std::vector<size_t> actual_idx(expected_idx);


    // Account for differences in rounding due to writing floating-point numbers as text (and double/float issues)
    // C# rounds to nearest even
    // InterOp rounds using std::floor(val+0.5)
    const float round_tol = 0.101f;
    const float round_tols[] = {1e-5f, round_tol, round_tol/10, round_tol/100};

// @todo remove the following sort
    std::stable_sort(expected_idx.begin(), expected_idx.end(), model::table::imaging_table_id_less(expected));
    std::stable_sort(actual_idx.begin(), actual_idx.end(), model::table::imaging_table_id_less(actual));
    for(size_t row_index=0;row_index<expected.row_count();++row_index)
    {
        for(size_t col_index=0;col_index<expected.column_count();++col_index)
        {

            if(expected.column_at(col_index).id() == model::table::TimeColumn) continue;
            const size_t digit_count = logic::table::get_column_rounding(expected.column_at(col_index).id());
            INTEROP_ASSERT(digit_count < util::length_of(round_tols));
            const float eps = round_tols[digit_count];
            for(size_t sub_index=0;sub_index<expected.column_at(col_index).size();++sub_index)
            {
                const float expected_value = expected(expected_idx[row_index], col_index, sub_index);
                const float actual_value = actual(actual_idx[row_index], col_index, sub_index);
                if(std::isnan(expected_value) && std::isnan(actual_value)) continue;
                EXPECT_NEAR(expected_value,
                            actual_value, eps)
                                    << expected.column_at(col_index).full_name(sub_index) << " | "
                                    << expected(expected_idx[row_index], model::table::LaneColumn)
                                    << "_" << expected(expected_idx[row_index], model::table::TileColumn)
                                    << "@" << expected(expected_idx[row_index], model::table::CycleColumn)
                                    << " == "  << actual(actual_idx[row_index], model::table::LaneColumn)
                                    << "_" << actual(actual_idx[row_index], model::table::TileColumn)
                                    << "@" << actual(actual_idx[row_index], model::table::CycleColumn);
            }
        }
    }
}

INSTANTIATE_TEST_CASE_P(regression_input,
                        imaging_table_regression_test, PersistentValuesIn(regression_test_data::instance().files()));
