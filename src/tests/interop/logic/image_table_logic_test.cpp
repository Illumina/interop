//
// Created by dev on 5/12/16.
//

#include <gtest/gtest.h>
#include "interop/logic/table/populate_imaging_table.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"

using namespace illumina::interop;

TEST(imaging_table, test_header_names)
{
    EXPECT_EQ(logic::table::imaging_table_column_names()[0], "Lane");
    EXPECT_EQ(logic::table::imaging_table_column_names()[16], "P90");
}
TEST(imaging_table, test_column_headers)
{
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<model::table::column_header> headers = logic::table::imaging_column_headers(channels);
    EXPECT_EQ(headers[0].title(), "");
    EXPECT_EQ(headers[0].subtitle(), "Lane");
    EXPECT_EQ(headers[5].subtitle(), "Density(k/mm2)");
    EXPECT_EQ(headers[8].subtitle(), "Cluster Count Pf (k)");
    EXPECT_EQ(headers[16].title(), "P90");
    EXPECT_EQ(headers[16].subtitle(), "Red");
    EXPECT_EQ(headers[16].title(), "P90");
    EXPECT_EQ(headers[17].subtitle(), "Green");
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
