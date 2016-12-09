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
#include "interop/io/table/imaging_table_csv.h"
#include "src/tests/interop/inc/abstract_regression_test_generator.h"

using namespace illumina::interop;
using namespace illumina::interop::unittest;

namespace illumina{ namespace interop {namespace model {namespace table
{
    bool operator==(const imaging_column& lhs, const imaging_column& rhs);
}}}}

/** Simulate reading error metrics
 *
 * @param metrics run metrics
 */
void simulate_read_error_metrics(model::metrics::run_metrics& metrics);

template<typename T>
std::string to_string(const std::vector<T>& values)
{
    std::ostringstream out;
    for(size_t i=0;i<values.size();++i)
        out << values[i] << ",";
    return out.str();
}

/** Setup for tests that compare two run summaries */
struct imaging_table_tests : public generic_test_fixture< model::table::imaging_table > {};

TEST_P(imaging_table_tests, compare_to_baseline)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
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
    std::vector<float> round_tols(logic::table::max_digits()+1);
    round_tols[0] = 1e-5f;
    round_tols[1] = round_tol;
    for(size_t i=2;i<round_tols.size();++i)
        round_tols[i] = round_tols[i-1] / 10.0f;

// @todo remove the following sort
    std::stable_sort(expected_idx.begin(), expected_idx.end(), model::table::imaging_table_id_less(expected));
    std::stable_sort(actual_idx.begin(), actual_idx.end(), model::table::imaging_table_id_less(actual));
    for(size_t row_index=0;row_index<expected.row_count();++row_index)
    {
        for(size_t col_index=0;col_index<expected.column_count();++col_index)
        {
            const size_t digit_count = logic::table::get_column_rounding(expected.column_at(col_index).id());
            INTEROP_ASSERT(digit_count < util::length_of(round_tols));
            const float eps = round_tols[digit_count];
            for(size_t sub_index=0;sub_index<expected.column_at(col_index).size();++sub_index)
            {
                const float expected_value = expected(expected_idx[row_index], col_index, sub_index);
                const float actual_value = actual(actual_idx[row_index], col_index, sub_index);
                INTEROP_ASSERT_NEAR(expected_value,
                            actual_value, eps)
                                            << expected.column_at(col_index).full_name(sub_index) << " | "
                                            << expected(expected_idx[row_index], model::table::LaneColumn)
                                            << "_" << expected(expected_idx[row_index], model::table::TileColumn)
                                            << "@" << expected(expected_idx[row_index], model::table::CycleColumn)
                                            << " == "  << actual(actual_idx[row_index], model::table::LaneColumn)
                                            << "_" << actual(actual_idx[row_index], model::table::TileColumn)
                                            << "@" << actual(actual_idx[row_index], model::table::CycleColumn)
                                            << " digit_count: " << digit_count;
            }
        }
    }
}


//---------------------------------------------------------------------------------------------------------------------
// Unit test section
//---------------------------------------------------------------------------------------------------------------------

class imaging_write_read_generator
{
public:
    /** Define parameter type */
    typedef int parameter_type;
public:
    /** Constructor */
    imaging_write_read_generator(const int){}
    /** Generate the expected and actual metric sets
     *
     * @param expected expected plot data
     * @param actual actual plot data
     */
    ::testing::AssertionResult generate(model::table::imaging_table& expected,
                                        model::table::imaging_table &actual) const
    {
        model::metrics::run_metrics metrics;
        simulate_read_error_metrics(metrics);
        logic::table::create_imaging_table(metrics, expected);


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
        out << "imaging_write_read_generator";
    }
};

imaging_table_tests::generator_type imaging_io_generators[] = {
        wrap(new standard_parameter_generator<model::table::imaging_table, imaging_write_read_generator>(0)),
};

// Setup unit tests for extended_tile_metrics_tests
INSTANTIATE_TEST_CASE_P(imaging_unit_tests,
                        imaging_table_tests,
                        ::testing::ValuesIn(imaging_io_generators));

//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------

/** Imaging regression test fixture
 *
 * This provides the test group name, provides the tested type as imaging_table, ensures the output file is prefixed with
 * image_table and finally properly sets up the expected imaging_table object.
 */
class imaging_table_regression_test : public abstract_regression_test_generator< model::table::imaging_table >
{
    typedef abstract_regression_test_generator< model::table::imaging_table > parent_t;
public:
    /** Constructor
     *
     * @param test_dir sub directory for baseline
     */
    imaging_table_regression_test(const std::string& test_dir) : parent_t(test_dir){}
    /** Constructor
     *
     * @param run_folder run folder
     * @param test_dir sub directory for baseline
     */
    imaging_table_regression_test(const std::string& run_folder, const std::string& test_dir) : parent_t(run_folder, test_dir){}

protected:
    /** Read the expected data from the baseline file into the model
     *
     * @param baseline_file baseline file
     * @param expected expected model data
     * @return true if the file was found, and the read completed without failure
     */
    bool read_expected(const std::string& baseline_file, model::table::imaging_table& expected)const
    {
        std::ifstream fin(baseline_file.c_str());
        if( !fin.good() ) return false;
        fin >> expected;
        if( fin.eof() ) return true;
        return !fin.fail();
    }
    /** Read the actual data from the run folder
     *
     * @param run_folder run folder
     * @param actual actual model data
     * @return true if data was generated
     */
    bool generate_actual(const std::string& run_folder,  model::table::imaging_table& actual)const
    {
        model::metrics::run_metrics actual_metrics;
        actual_metrics.read(run_folder);
        if( actual_metrics.empty() ) return false;
        logic::table::create_imaging_table(actual_metrics, actual);
        return actual.column_count()*actual.row_count() > 0;
    }
    /** Write the actual data to the run folder
     *
     * @param baseline_file baseline file
     * @param actual actual model data
     */
    bool write_actual(const std::string& baseline_file,  const model::table::imaging_table& actual)const
    {
        std::ofstream fout(baseline_file.c_str());
        fout << actual;
        return fout.good();
    }
    /** Create a copy of the current object
     *
     * @return pointer to new copy
     */
    base_t clone()const
    {
        return new imaging_table_regression_test(*this);
    }
    /** Create a copy of the current object with the given run folder
     *
     * @param run_folder run folder
     * @return pointer to new copy
     */
    base_t clone(const std::string& run_folder)const
    {
        return new imaging_table_regression_test(run_folder, m_test_dir);
    }
    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream& out)const
    {
        out << "imaging_table_regression_test - " << io::basename(m_run_folder);
    }
};


imaging_table_regression_test imaging_table_regression_gen("imaging");

INSTANTIATE_TEST_CASE_P(imaging_table_regression_test,
                        imaging_table_tests,
                        ProxyValuesIn(imaging_table_regression_gen, regression_test_data::instance().files()));


