/** Unit tests for image table logic
 *
 *  @file
 *  @date 5/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <math.h>
#include "interop/logic/table/populate_imaging_table.h"
#include "interop/io/table/csv_format.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"
#include "src/tests/interop/inc/regression_fixture.h"
#include "interop/model/table/imaging_table.h"
#include "interop/util/length_of.h"

#define INTEROP_EXPECT_EQ_UIntIdType(EXPECTED, ACTUAL, TOL, MSG) ASSERT_EQ(EXPECTED, ACTUAL) << MSG
#define INTEROP_EXPECT_EQ_FloatValueType(EXPECTED, ACTUAL, TOL, MSG) if(!std::isnan(EXPECTED) && !std::isnan(ACTUAL)) \
    EXPECT_NEAR(EXPECTED, ACTUAL, TOL) << MSG
#define INTEROP_EXPECT_EQ_DateTimeStructType(EXPECTED, ACTUAL, TOL, MSG) EXPECT_TRUE(true)
#define INTEROP_EXPECT_EQ_UShortChannelArray(EXPECTED, ACTUAL, TOL, MSG) EXPECT_THAT(EXPECTED, ::testing::ElementsAreArray(ACTUAL)) << MSG
#define INTEROP_EXPECT_EQ_UShortBaseArray(EXPECTED, ACTUAL, TOL, MSG) EXPECT_THAT(EXPECTED, ::testing::ElementsAreArray(ACTUAL)) << MSG
#define INTEROP_EXPECT_EQ_FloatChannelArray(EXPECTED, ACTUAL, TOL, MSG) \
    ASSERT_EQ(EXPECTED.size(), ACTUAL.size()) << to_string(EXPECTED) << " - " << MSG;\
    for(size_t k=0;k<EXPECTED.size();++k) \
        if(!std::isnan(EXPECTED[k]) && !std::isnan(ACTUAL[k])) EXPECT_NEAR(EXPECTED[k], ACTUAL[k], TOL) << MSG
#define INTEROP_EXPECT_EQ_FloatBaseArray(EXPECTED, ACTUAL, TOL, MSG) \
    ASSERT_EQ(EXPECTED.size(), ACTUAL.size()) << to_string(EXPECTED) << " - " << MSG;\
    for(size_t k=0;k<EXPECTED.size();++k) \
        if(!std::isnan(EXPECTED[k]) && !std::isnan(ACTUAL[k])) EXPECT_NEAR(EXPECTED[k], ACTUAL[k], TOL) << MSG

using namespace illumina::interop;
using namespace illumina::interop::unittest;

typedef std::vector< model::table::column_header > column_header_vector_t;

template<typename T>
std::string to_string(const std::vector<T>& values)
{
    std::ostringstream out;
    for(size_t i=0;i<values.size();++i)
        out << values[i] << ",";
    return out.str();
}

// This test is disabled because it was designed to fail, in order to show the framework works.
TEST(imaging_table, DISABLED_table_entry)
{
    model::table::table_entry expected;
    expected.Lane = 1;

    model::table::table_entry actual;
    actual.Lane = 2;
    const float tol = 1e-6f;

#   define INTEROP_TUPLE7(Id, Ignored1, Ignored2, Ignored3, Data, Type, Ignored5) \
        INTEROP_EXPECT_EQ_##Data##Type(expected.Id, actual.Id, tol, "");
    INTEROP_IMAGING_COLUMN_TYPES
#   undef INTEROP_TUPLE7


}

TEST(imaging_table, name_to_header)
{
    const std::vector<std::string>& names = logic::table::imaging_table_column_names();
    for(size_t i=0;i<names.size();++i)
    {
        const model::table::column_header header = logic::table::to_header(names[i]);
        const std::string actual_name = logic::table::to_name(header);
        EXPECT_EQ(names[i], actual_name);
    }
}



TEST(imaging_table, test_header_names)
{
    EXPECT_EQ(logic::table::imaging_table_column_names()[0], "Lane");
    EXPECT_EQ(logic::table::imaging_table_column_names()[16], "P90");
}


TEST(imaging_table, test_column_headers)
{
    typedef model::table::column_header column_header;
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<column_header> headers;
    std::vector<bool> filled(model::table::ImagingColumnCount, false);
    filled[model::table::LaneColumn] = true;
    filled[model::table::DensityKPermm2Column] = true;
    filled[model::table::P90Column] = true;
    filled[model::table::CycleWithinReadColumn] = true;
    logic::table::populate_column_headers(headers, channels, filled);
    const column_header expected_headers[] = {
                column_header("", "Lane"),
                column_header("", "Cycle Within Read"),
                column_header("", "Density(k/mm2)"),
                column_header("P90", "Red"),
                column_header("P90", "Green")};
    EXPECT_THAT(to_vector(expected_headers), ::testing::ElementsAreArray(headers));
}

TEST(imaging_table, test_row0)
{
    const float tol = 1e-6f;
    model::metrics::run_metrics metrics;
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info(
            "",
            "",
            1,
            model::run::flowcell_layout(2, 2, 2, 16),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    metrics.legacy_channel_update(constants::HiSeq);

    std::istringstream iss(unittest::error_v3::binary_data());
    io::read_metrics(iss, metrics.error_metric_set());

    std::vector<model::table::table_entry> table;
    std::vector< bool > filled_columns;
    logic::table::populate_imaging_table(metrics, table, filled_columns);
    ASSERT_EQ(table.size(), 3u);
    ASSERT_EQ(filled_columns.size(), model::table::ImagingColumnCount);
    EXPECT_TRUE(filled_columns[0]);
    EXPECT_TRUE(filled_columns[1]);
    EXPECT_TRUE(filled_columns[model::table::ErrorRateColumn]);
    EXPECT_FALSE(filled_columns[model::table::PercentPhasingColumn]);
    EXPECT_NEAR(table[0].ErrorRate, 0.450f, tol);
}

TEST(imaging_table, write_read_headers)
{
    typedef model::table::column_header column_header;
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<column_header> headers;
    logic::table::populate_column_headers(headers, channels);

    std::ostringstream out;
    io::table::write_csv_line(out, headers);
    std::istringstream in(out.str());

    std::vector<column_header> actual_headers;
    io::table::read_csv_line(in, actual_headers);
    EXPECT_THAT(headers, ::testing::ElementsAreArray(actual_headers));

}
TEST(imaging_table, populate_column_offsets)
{
    typedef model::table::column_header column_header;
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<column_header> headers;
    logic::table::populate_column_headers(headers, channels);

    std::ostringstream out;
    io::table::write_csv_line(out, headers);
    std::istringstream in(out.str());

    std::vector<column_header> actual_headers;
    io::table::read_csv_line(in, actual_headers);
    logic::table::table_fill_vector_t offsets;
    logic::table::populate_column_offsets(offsets, actual_headers);
    EXPECT_EQ(offsets[model::table::P90Column].size(), 2u);
}

TEST(imaging_table, copy_to_vector)
{
    typedef model::table::column_header column_header;
    const float tol = 1e-6f;
    model::metrics::run_metrics metrics;
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info(
            "",
            "",
            1,
            model::run::flowcell_layout(2, 2, 2, 16),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    metrics.legacy_channel_update(constants::HiSeq);
    metrics.set_naming_method(constants::FourDigit);

    std::istringstream iss(unittest::error_v3::binary_data());
    io::read_metrics(iss, metrics.error_metric_set());


    std::vector<model::table::table_entry> table;
    std::vector< bool > filled_columns;
    logic::table::populate_imaging_table(metrics, table, filled_columns);

    std::vector<float> values;
    table[0].copy_to_vector(values, filled_columns);;
    std::vector<column_header> headers;
    logic::table::populate_column_headers(headers, metrics.run_info().channels(), filled_columns);

    const model::metrics::error_metric metric = unittest::error_v3::metrics()[0];
    const float expected_values[] ={(float)metric.lane(), (float)metric.tile(), (float)metric.cycle(), 1, 1, roundf(metric.error_rate()*100)/100, 1, 1, 14};
    ASSERT_EQ(headers.size(), values.size());
    ASSERT_EQ(util::length_of(expected_values), values.size());
    for(size_t i=0;i<values.size();++i)
    {
        EXPECT_NEAR(expected_values[i], values[i], tol) << headers[i] << " | " << constants::to_string(metrics.run_info().flowcell().naming_method());
    }
}

TEST(imaging_table, copy_from_vector)
{

    const float expected_values[] ={1, 255, 256};
    model::table::table_entry actual;

    logic::table::table_fill_vector_t offsets(model::table::ImagingColumnCount);
    offsets[0] = (std::vector<size_t>(1, 0));
    std::vector<size_t> tmp;
    tmp.push_back(1u);tmp.push_back(2u);
    offsets[model::table::P90Column] = tmp;
    actual.copy_from_vector(std::vector<float>(expected_values, expected_values+util::length_of(expected_values)), offsets);

    EXPECT_EQ(static_cast<size_t>(expected_values[0]), actual.Lane);
    EXPECT_EQ(static_cast<size_t>(expected_values[1]), actual.P90[0]);
    EXPECT_EQ(static_cast<size_t>(expected_values[2]), actual.P90[1]);
}


TEST(imaging_table, read_csv_line )
{
    std::istringstream in("1,2,,4\n");
    std::vector<unsigned int> values;
    io::table::read_csv_line(in, values);
    EXPECT_EQ(values[0], 1u);
    EXPECT_EQ(values[1], 2u);
    EXPECT_EQ(values[2], 0u);
    EXPECT_EQ(values[3], 4u);
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
struct image_table_regression_test : public regression_test_fixture< image_table_regression_test, model::table::imaging_table>
{
    image_table_regression_test() : regression_test_fixture< image_table_regression_test, model::table::imaging_table>("image_table"){}
    /** Populate the actual imaging table using the given run_metrics
     *
     * @param actual_metrics run_metrics read in from a run_folder
     * @param actual imaging table constructed from the run_metrics
     */
    static void populate_actual(model::metrics::run_metrics& actual_metrics, model::table::imaging_table& actual)
    {
        logic::table::populate_imaging_table(actual_metrics, actual);
        logic::table::populate_column_headers(actual_metrics.run_info().channels(), actual);
    }
};

//@todo remove this function
bool compare_rows(const model::table::table_entry& lhs, const model::table::table_entry& rhs)
{
    if(lhs.Lane == rhs.Lane)
    {
        if(lhs.Tile == rhs.Tile) return lhs.Cycle < rhs.Cycle;
        return lhs.Tile < rhs.Tile;
    }
    return lhs.Lane < rhs.Lane;
}

TEST_P(image_table_regression_test, compare_to_baseline)
{
    if(!test) return;
    ASSERT_EQ(expected.column_count(), actual.column_count()) << "Baseline: " << to_string(expected.headers()) << "\n   Actual: " << to_string(actual.headers());
    ASSERT_EQ(expected.row_count(), actual.row_count());
    EXPECT_THAT(expected.headers(), ::testing::ElementsAreArray(actual.headers()));
    // @todo remove the following sort
    std::stable_sort(expected.begin(), expected.end(), compare_rows);
    std::stable_sort(actual.begin(), actual.end(), compare_rows);

    // Account for differences in rounding due to writing floating-point numbers as text (and double/float issues)
    // C# rounds to nearest even
    // InterOp rounds using std::floor(val+0.5)
    const float round_tol = 0.101f;
    const float round_tols[] = {1e-5f, round_tol, round_tol/10, round_tol/100};

    for(size_t i=0;i<expected.row_count();++i)
    {
#       define INTEROP_TUPLE7(Id, Ignored1, Ignored2, Ignored3, Data, Type, NUM_DECIMALS) \
        INTEROP_ASSERT(NUM_DECIMALS < util::length_of(round_tols));\
        INTEROP_EXPECT_EQ_##Data##Type(expected[i].Id, actual[i].Id, round_tols[NUM_DECIMALS], i \
                << " - " << expected[i].Lane << "_" << expected[i].Tile << "(" << expected[i].Cycle\
                << ") == " << actual[i].Lane << "_" << actual[i].Tile << "(" << actual[i].Cycle << ") "\
                << io::basename(run_folder));
        INTEROP_IMAGING_COLUMN_TYPES
#       undef INTEROP_TUPLE7
    }
}

INSTANTIATE_TEST_CASE_P(regression_input,
                        image_table_regression_test,
                        PersistentValuesIn(regression_test_data::instance().files()));
