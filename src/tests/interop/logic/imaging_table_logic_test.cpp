/** Unit tests for image table logic
 *
 *  @file
 *  @date 5/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "interop/util/length_of.h"
#include "interop/logic/table/create_imaging_table.h"
#include "interop/io/table/imaging_table_csv.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"

using namespace illumina::interop;
using namespace illumina::interop::unittest;

namespace illumina{ namespace interop {namespace model {namespace table
{
    /** Compare whether two imaging columns are equal
     *
     * @param lhs an imaging column
     * @param rhs an imaging column
     * @return true if the columns are equal
     */
    bool operator==(const imaging_column& lhs, const imaging_column& rhs)
    {
        if(lhs.subcolumns().size() != rhs.subcolumns().size()) return false;
        for(size_t i=0;i<lhs.subcolumns().size();++i)
            if(lhs.subcolumns()[i] != rhs.subcolumns()[i]) return false;
        return lhs.id() == rhs.id() &&
                lhs.offset() == rhs.offset() &&
                lhs.name() == rhs.name();
    }
}}}}

/** Simulate reading error metrics
 *
 * @param metrics run metrics
 */
void simulate_read_error_metrics(model::metrics::run_metrics& metrics)
{
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

    unittest::error_metric_v3::create_expected(metrics.get<model::metrics::error_metric>());
}
/**
 * @class illumina::interop::model::table::imaging_table
 * @test Confirm to_header and to_name properly convert header names
 */
TEST(imaging_table, convert_name_to_header)
{
    for(size_t i=0;i<model::table::ImagingColumnCount;++i)
    {
        const std::string expected_name = constants::to_string(static_cast<model::table::column_id>(i));
        const std::string header = model::table::imaging_column::to_header(expected_name);
        const std::string actual_name = model::table::imaging_column::to_name(header);
        EXPECT_EQ(expected_name, actual_name);
    }
}

TEST(imaging_table, base_header_test)
{
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<bool> filled(model::table::ImagingColumnCount, false);

    filled[model::table::PercentBaseColumn] = true;
    std::vector< model::table::imaging_column > columns;
    logic::table::create_imaging_table_columns(channels, filled, columns);

    const std::string expected_subcolumns[] = {
            "A", "C", "G", "T"
    };
    EXPECT_THAT(util::to_vector(expected_subcolumns), ::testing::ElementsAreArray(columns[0].subcolumns()));
}

TEST(imaging_table, max_digits)
{
    EXPECT_EQ(logic::table::max_digits(), 3u);
}

TEST(imaging_table, test_write_column_name)
{
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<bool> filled(model::table::ImagingColumnCount, false);

    filled[model::table::PercentBaseColumn] = true;
    std::vector< model::table::imaging_column > columns;
    logic::table::create_imaging_table_columns(channels, filled, columns);

    std::ostringstream oss;
    oss << columns[0];
    EXPECT_EQ(oss.str(), "% Base<A;C;G;T>");
}

TEST(imaging_table, test_read_column_name)
{
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<bool> filled(model::table::ImagingColumnCount, false);

    filled[model::table::PercentBaseColumn] = true;
    std::vector< model::table::imaging_column > columns;
    logic::table::create_imaging_table_columns(channels, filled, columns);

    std::vector< model::table::imaging_column > actual_columns(1);
    std::istringstream iss("% Base<A;C;G;T>");
    iss >> actual_columns[0];
    logic::table::populate_column_offsets(actual_columns);
    EXPECT_EQ(actual_columns[0], columns[0]);
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


TEST(imaging_table, create_imaging_table_columns_channel)
{
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<bool> filled(model::table::ImagingColumnCount, false);

    filled[model::table::P90Column] = true;
    std::vector< model::table::imaging_column > columns;
    logic::table::create_imaging_table_columns(channels, filled, columns);

    std::vector< model::table::imaging_column > expected_columns;
    expected_columns.push_back(model::table::imaging_column(model::table::P90Column,
                                                            0,
                                                            channels));

    EXPECT_THAT(expected_columns, ::testing::ElementsAreArray(columns));
}

TEST(imaging_table, create_imaging_table_columns_error_metrics)
{
    model::metrics::run_metrics metrics;
    simulate_read_error_metrics(metrics);

    std::vector<model::table::imaging_column> columns;
    logic::table::create_imaging_table_columns(metrics, columns);

    const model::table::imaging_column expected_columns[] = {
            model::table::imaging_column(model::table::LaneColumn, 0),
            model::table::imaging_column(model::table::TileColumn, 1),
            model::table::imaging_column(model::table::CycleColumn, 2),
            model::table::imaging_column(model::table::ReadColumn, 3),
            model::table::imaging_column(model::table::CycleWithinReadColumn, 4),
            model::table::imaging_column(model::table::ErrorRateColumn, 5),
            model::table::imaging_column(model::table::SurfaceColumn, 6),
            model::table::imaging_column(model::table::SwathColumn, 7),
            model::table::imaging_column(model::table::TileNumberColumn, 8)
    };

    EXPECT_THAT(util::to_vector(expected_columns), ::testing::ElementsAreArray(columns));
}
TEST(imaging_table, create_imaging_table_error_metrics)
{
    model::metrics::run_metrics metrics;
    simulate_read_error_metrics(metrics);

    std::vector<model::table::imaging_column> columns;
    logic::table::row_offset_map_t row_offsets;
    logic::table::create_imaging_table_columns(metrics, columns);
    const size_t column_count = logic::table::count_table_columns(columns);
    logic::table::count_table_rows(metrics, row_offsets);
    std::vector<float> data(row_offsets.size()*column_count);
    ASSERT_TRUE(data.size() > 0);
    logic::table::populate_imaging_table_data(metrics, columns, row_offsets, &data[0], data.size());
    EXPECT_EQ(data[0], 7u);
}
TEST(imaging_table, create_imaging_table_error_metrics_tile_test)
{
    model::metrics::run_metrics metrics;
    simulate_read_error_metrics(metrics);

    model::table::imaging_table table;
    logic::table::create_imaging_table(metrics, table);
    EXPECT_EQ(table(0, model::table::LaneColumn), 7.0f);
    EXPECT_EQ(table(0, model::table::TileColumn), 1114.0f);
    EXPECT_EQ(table(0, model::table::CycleColumn), 1.0f);
    EXPECT_EQ(table(0, model::table::TileNumberColumn), 14.0f);
    EXPECT_EQ(table(0, model::table::SurfaceColumn), 1.0f);
    EXPECT_EQ(table(0, model::table::SwathColumn), 1.0f);


    EXPECT_EQ(table(1, model::table::LaneColumn), 7.0f);
    EXPECT_EQ(table(1, model::table::TileColumn), 1114.0f);
    EXPECT_EQ(table(1, model::table::CycleColumn), 2.0f);
    EXPECT_EQ(table(1, model::table::TileNumberColumn), 14.0f);
    EXPECT_EQ(table(1, model::table::SurfaceColumn), 1.0f);
    EXPECT_EQ(table(1, model::table::SwathColumn), 1.0f);
}


