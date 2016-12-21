/** Unit tests for plot logic
 *
 *  @file
 *  @date  4/29/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/model/plot/filter_options.h"
#include "interop/logic/plot/plot_by_cycle.h"
#include "interop/logic/plot/plot_by_lane.h"
#include "interop/logic/plot/plot_qscore_histogram.h"
#include "interop/logic/plot/plot_qscore_heatmap.h"
#include "interop/logic/plot/plot_flowcell_map.h"
#include "interop/logic/plot/plot_sample_qc.h"
#include "src/tests/interop/metrics/inc/extraction_metrics_test.h"
#include "src/tests/interop/metrics/inc/tile_metrics_test.h"
#include "src/tests/interop/metrics/inc/q_metrics_test.h"
#include "src/tests/interop/metrics/inc/index_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/logic/inc/metric_filter_iterator.h"

using namespace illumina::interop;
using namespace illumina::interop::unittest;

/** @test Confirm invalid metric thrown */
TEST(plot_logic, failure_mode_bad_metric_plot_by_cycle)
{
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    model::plot::plot_data<model::plot::candle_stick_point> data;
    EXPECT_THROW(logic::plot::plot_by_cycle(metrics, "NoSuchMetric", options, data), model::invalid_metric_type);
}

//Check that reading in an interop and then plotting by cycle graph works
TEST(plot_logic, intensity_by_cycle)
{
    const float tol = 1e-3f;
    const size_t channel_count = 4;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info("", "", 1, model::run::flowcell_layout(2, 2, 2, 16),
                                      std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.set_naming_method(constants::FourDigit);
    metrics.legacy_channel_update(constants::HiSeq);

    unittest::extraction_metric_v2::create_expected(metrics.get<model::metrics::extraction_metric>());

    model::plot::plot_data<model::plot::candle_stick_point> data;
    logic::plot::plot_by_cycle(metrics, constants::Intensity, options, data);
    const double expected[] = {321, 282.666656, 0, 0};
    ASSERT_EQ(data.size(), channel_count);
    EXPECT_EQ(data.x_axis().label(), "Cycle");
    EXPECT_EQ(data.y_axis().label(), "Intensity");
    EXPECT_EQ(data.title(), "All Lanes All Channels");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 3.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 353.1f, tol);
    size_t k = 0;
    for (size_t channel = 0; channel < data.size(); ++channel)
    {
        for (size_t i = 0; i < data[channel].size(); ++i)
        {
            EXPECT_EQ(data[channel][i].x(), i + 1);
            EXPECT_NEAR(static_cast<float> (expected[k]), data[channel][i].y(), tol);
            k++;
        }
    }
}

//Check that reading in an empty interop sets values to 0 etc for plot_by_cycle
TEST(plot_logic, intensity_by_cycle_empty_interop)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info("", "", 1, model::run::flowcell_layout(2, 2, 2, 16),
                                      std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.set_naming_method(constants::FourDigit);
    metrics.legacy_channel_update(constants::HiSeq);

    model::plot::plot_data<model::plot::candle_stick_point> data;
    logic::plot::plot_by_cycle(metrics, constants::Intensity, options, data);
    ASSERT_EQ(data.size(), 0u);
    EXPECT_EQ(data.x_axis().label(), "");
    EXPECT_EQ(data.y_axis().label(), "");
    EXPECT_EQ(data.title(), "");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 0.0f, tol);
}

//Check that plotting by lane gives you accurate values - valid entry test
TEST(plot_logic, pf_clusters_by_lane)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info(
            "",
            "",
            1,
            model::run::flowcell_layout(8, 2, 2, 16),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    metrics.set_naming_method(constants::FourDigit);
    metrics.legacy_channel_update(constants::HiSeq);

    unittest::tile_metric_v2::create_expected(metrics.get<model::metrics::tile_metric>());

    model::plot::plot_data<model::plot::candle_stick_point> data;
    logic::plot::plot_by_lane(metrics, constants::ClusterCountPF, options, data);

    const double expected_val = 3.22777605;
    EXPECT_NEAR(expected_val, data[0][0].y(), tol);
    //data.size() refers to the number of series in the collection
    ASSERT_EQ(data.size(), 1u);
    EXPECT_EQ(data.title(), "");
    EXPECT_EQ(data.x_axis().label(), "Lane");
    EXPECT_EQ(data.y_axis().label(), "Clusters PF");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 8.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 3.9946798252130975, tol);

}

//Check that reading in no interop and plotting by lane prints out 0's and doesn't crash badly
TEST(plot_logic, pf_clusters_by_lane_empty_interop)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info("", "", 1, model::run::flowcell_layout(8, 2, 2, 16),
                                      std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.set_naming_method(constants::FourDigit);
    metrics.legacy_channel_update(constants::HiSeq);

    model::plot::plot_data<model::plot::candle_stick_point> data;
    logic::plot::plot_by_lane(metrics, constants::ClusterCountPF, options, data);

    //data.size() refers to the number of series, so this should still be 1
    ASSERT_EQ(data.size(), 0u);
    EXPECT_EQ(data.title(), "");

    EXPECT_EQ(data.x_axis().label(), "");
    EXPECT_EQ(data.y_axis().label(), "");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 0.0f, tol);
}

//Checks that plotting q_score histogram works correctly - checks values
TEST(plot_logic, q_score_histogram)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info("", "", 1, model::run::flowcell_layout(8, 2, 2, 16),
                                      std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.legacy_channel_update(constants::HiSeq);
    metrics.set_naming_method(constants::FourDigit);

    unittest::q_metric_v6::create_expected(metrics.get<model::metrics::q_metric>());
    metrics.finalize_after_load();

    model::plot::plot_data<model::plot::bar_point> data;
    logic::plot::plot_qscore_histogram(metrics, options, data);
    const double expected_y[] = {0.721589, 0.217605, 0.005811, 8.61628};
    const size_t expected_x[] = {10, 20, 25, 30};
    size_t k = 0;

    //data.size() refers to the number of series in the collection
    ASSERT_EQ(data.size(), 1u);
    for (size_t i = 0; i < data.size(); i++)
    {
        for (size_t j = 0; j < data[i].size(); j++)
        {
            EXPECT_NEAR(data[i][j].y(), static_cast<float>(expected_y[k]), tol);
            EXPECT_EQ(data[i][j].x(), expected_x[k]);
            k++;
        }
    }

    EXPECT_EQ(data.title(), "All Lanes");
    EXPECT_EQ(data.x_axis().label(), "Q Score");
    EXPECT_EQ(data.y_axis().label(), "Total (million)");
    EXPECT_NEAR(data.x_axis().min(), 1.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 38.5f, tol);
    EXPECT_NEAR(data.y_axis().max(), 9.4780035018920898f, tol);
}

//Not reading an interop in for Q-score Hist causes the title and labels to go away (only here, not in cycle/lane)
TEST(plot_logic, q_score_histogram_empty_interop)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info("", "", 1, model::run::flowcell_layout(8, 2, 2, 16),
                                      std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.legacy_channel_update(constants::HiSeq);
    metrics.set_naming_method(constants::FourDigit);

    metrics.finalize_after_load();

    model::plot::plot_data<model::plot::bar_point> data;
    logic::plot::plot_qscore_histogram(metrics, options, data);

    //data.size() refers to the number of series in the collection
    ASSERT_EQ(data.size(), 0u);
    EXPECT_EQ(data.title(), "");
    EXPECT_EQ(data.x_axis().label(), "");
    EXPECT_EQ(data.y_axis().label(), "");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 0.0f, tol);
}

//Checks that q-score heatmap works as intended
TEST(plot_logic, q_score_heatmap)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info("", "", 1, model::run::flowcell_layout(8, 2, 2, 16),
                                      std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.legacy_channel_update(constants::HiSeq);
    metrics.set_naming_method(constants::FourDigit);

    unittest::q_metric_v6::create_expected(metrics.get<model::metrics::q_metric>());
    metrics.finalize_after_load();

    model::plot::heatmap_data data;
    logic::plot::plot_qscore_heatmap(metrics, options, data);
    ASSERT_EQ(data.row_count(), 3u);
    ASSERT_EQ(data.column_count(), 40u);
    EXPECT_EQ(data.title(), "All Lanes");
    EXPECT_EQ(data.x_axis().label(), "Cycle");
    EXPECT_EQ(data.y_axis().label(), "Q Score");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 3.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 40.0f, tol);
    double expected[] =
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 9.1749, 9.1749, 9.1749, 9.1749, 9.1749, 9.1749, 9.1749, 9.1749, 9.1749, 9.1749,
             4.06434, 4.06434,
             4.06434, 4.06434, 4.06434, 0.146682, 0.146682, 0.146682, 0.146682, 0.146682, 95.7376, 95.7376, 95.7376,
             95.7376,
             95.7376, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8.26827, 8.26827, 8.26827, 8.26827, 8.26827, 8.26827,
             8.26827, 8.26827,
             8.26827, 8.26827, 1.53941, 1.53941, 1.53941, 1.53941, 1.53941, 0.0376635, 0.0376635, 0.0376635, 0.0376635,
             0.0376635,
             99.2799, 99.2799, 99.2799, 99.2799, 99.2799, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7.26372, 7.26372,
             7.26372, 7.26372,
             7.26372, 7.26372, 7.26372, 7.26372, 7.26372, 7.26372, 1.84695, 1.84695, 1.84695, 1.84695, 1.84695,
             0.0146203,
             0.0146203, 0.0146203, 0.0146203, 0.0146203, 100, 100, 100, 100, 100, 0, 0, 0, 0, 0, 0};

    for (size_t row = 0, k = 0; row < data.row_count(); ++row)
    {
        for (size_t col = 0; col < data.column_count(); ++col, ++k)
        {
            EXPECT_NEAR(static_cast<float>(expected[k]), data(row, col), 1e-3f);
        }
    }
}

//Checks that q-score heatmap works as intended
TEST(plot_logic, q_score_heatmap_empty_interop)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info("", "", 1, model::run::flowcell_layout(8, 2, 2, 16),
                                      std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.legacy_channel_update(constants::HiSeq);
    metrics.set_naming_method(constants::FourDigit);

    metrics.finalize_after_load();

    model::plot::heatmap_data data;
    logic::plot::plot_qscore_heatmap(metrics, options, data);
    ASSERT_EQ(data.row_count(), 0u);
    ASSERT_EQ(data.column_count(), 0u);
    EXPECT_EQ(data.title(), "");
    EXPECT_EQ(data.x_axis().label(), "");
    EXPECT_EQ(data.y_axis().label(), "");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 0.0f, tol);
}

TEST(plot_logic, q_score_heatmap_buffer)
{
    const float tol = 1e-5f;
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info(
            "",
            "",
            1,
            model::run::flowcell_layout(8, 2, 2, 16),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    metrics.legacy_channel_update(constants::HiSeq);
    metrics.set_naming_method(constants::FourDigit);

    unittest::q_metric_v6::create_expected(metrics.get<model::metrics::q_metric>());
    metrics.finalize_after_load();

    model::plot::heatmap_data data1;
    logic::plot::plot_qscore_heatmap(metrics, options, data1);

    size_t row_count = logic::plot::count_rows_for_heatmap(metrics);
    size_t col_count = logic::plot::count_columns_for_heatmap(metrics);
    std::vector<float> buffer(row_count * col_count, 1);
    model::plot::heatmap_data data2;
    logic::plot::plot_qscore_heatmap(metrics, options, data2, &buffer.front());

    for (size_t row = 0; row < data2.row_count(); row++)
    {
        for (size_t col = 0; col < data2.column_count(); col++)
        {
            INTEROP_ASSERT_NEAR(buffer[row * data2.column_count() + col], data1(row, col), tol) << data1(row, col);

        }
    }
}

//Tests that plot_flowcell_map works normally with interop read in
TEST(plot_logic, flowcell_map)
{
    const model::plot::filter_options::id_t ALL_IDS = model::plot::filter_options::ALL_IDS;
    const float tol = 1e-3f;
    const size_t num_lanes = 8;
    model::metrics::run_metrics metrics;
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info(
            "",
            "",
            1,
            model::run::flowcell_layout(num_lanes, 2, 2, 36, 1, 1, std::vector<std::string>(), constants::FourDigit),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    model::plot::filter_options options(constants::FourDigit, ALL_IDS, 0, constants::A, ALL_IDS, 1, 1);
    metrics.legacy_channel_update(constants::HiSeq);

    unittest::extraction_metric_v2::create_expected(metrics.get<model::metrics::extraction_metric>());
    ASSERT_GT(metrics.get< model::metrics::extraction_metric >().size(), 0u);

    model::plot::flowcell_data data;
    logic::plot::plot_flowcell_map(metrics, constants::Intensity, options, data);
    std::vector<float> actual_values;
    actual_values.reserve(data.row_count() * data.column_count());
    for (size_t val = 0; val < data.row_count() * data.column_count(); val++)
    {
        actual_values.push_back(std::numeric_limits<float>::quiet_NaN());
    }
    float expected_vals[] = {302, 312, 349};
    std::pair<size_t, size_t> indices[] = {std::make_pair(13, 6), std::make_pair(49, 6),
                                                      std::make_pair(85, 6)};
    size_t k = 0;
    for (size_t i = 0; i < data.row_count(); i++)
    {
        for (size_t j = 0; j < data.column_count(); j++)
        {
            if (!std::isnan(data(i, j)))
            {
                actual_values[i * data.column_count() + j] = data(i, j);
                EXPECT_NEAR(expected_vals[k], data(i, j), tol);
                k++;
            }
        }
    }

    size_t m = 0;
    for (size_t val = 0; val < actual_values.size(); val++)
    {
        if (!std::isnan(actual_values[val]))
        {
            ASSERT_EQ(val / data.column_count(), indices[m].second);
            ASSERT_EQ(val % data.column_count(), indices[m].first);
            m++;
        }
    }

    ASSERT_EQ(data.row_count(), num_lanes);
    EXPECT_EQ(data.title(), "Intensity");
    EXPECT_EQ(data.saxis().label(), "Intensity");
    EXPECT_NEAR(data.saxis().min(), 302.0f, tol);
    EXPECT_NEAR(data.saxis().max(), 349.0f, tol);
}

//Checks that plot_flowcell_map with no interop read sets axis values cleanly (no crash)
TEST(plot_logic, flowcell_map_empty_interop)
{
    const model::plot::filter_options::id_t ALL_IDS = model::plot::filter_options::ALL_IDS;
    const float tol = 1e-3f;
    const size_t num_lanes = 8;
    model::metrics::run_metrics metrics;
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info(
            "",
            "",
            1,
            model::run::flowcell_layout(num_lanes, 2, 2, 36, 1, 1, std::vector<std::string>(), constants::FourDigit),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    model::plot::filter_options options(constants::FourDigit, ALL_IDS, 0, constants::A, ALL_IDS, 1, 1);
    metrics.legacy_channel_update(constants::HiSeq);

    model::plot::flowcell_data data;
    logic::plot::plot_flowcell_map(metrics, constants::Intensity, options, data);
    ASSERT_EQ(data.row_count(), 0u);
    ASSERT_EQ(data.column_count(), 0u);
    EXPECT_EQ(data.title(), "");
    EXPECT_EQ(data.saxis().label(), "");
    EXPECT_NEAR(data.saxis().min(), 0.0f, tol);
    EXPECT_NEAR(data.saxis().max(), 0.0f, tol);
}

//Test to ensure that plot_sample_qc works as intended
TEST(plot_logic, sample_qc)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(
            model::run::info("", "", 1, model::run::flowcell_layout(8, 2, 2, 36, 1, 1, std::vector<std::string>(),
                                                                    constants::FourDigit),
                             std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.legacy_channel_update(constants::HiSeq);

    unittest::index_metric_v1::create_expected(metrics.get<model::metrics::index_metric>());
    unittest::tile_metric_v2::create_expected(metrics.get<model::metrics::tile_metric>());
    ASSERT_GT(metrics.get< model::metrics::index_metric >().size(), 0u);

    model::plot::plot_data<model::plot::bar_point> data;
    logic::plot::plot_sample_qc(metrics, 7, data);

    ASSERT_EQ(data.size(), 1u);
    ASSERT_EQ(data[0].size(), 3u);
    ASSERT_EQ(data[0].color(), "Green");
    EXPECT_EQ(data[0].title(), "% reads");
    EXPECT_NEAR(data[0][0].x(), 1, tol);
    EXPECT_NEAR(data[0][0].y(), 0.04597f, tol);
    EXPECT_NEAR(data[0][1].x(), 2, tol);
    EXPECT_NEAR(data[0][1].y(), 0.04693f, tol);
    EXPECT_NEAR(data[0][2].x(), 3, tol);
    EXPECT_NEAR(data[0][2].y(), 0.04701f, tol);
    EXPECT_EQ(data.x_axis().label(), "Index Number");
    EXPECT_EQ(data.y_axis().label(), "% Reads Identified (PF)");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 4, tol);
    EXPECT_NEAR(data.y_axis().max(), 5, tol);
}

//Test to make sure that plot_sample_qc stores 0's for axis values with no interop being read
TEST(plot_logic, sample_qc_empty_interop)
{
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(
            model::run::info("", "", 1, model::run::flowcell_layout(8, 2, 2, 36, 1, 1, std::vector<std::string>(),
                                                                    constants::FourDigit),
                             std::vector<std::string>(), model::run::image_dimensions(), reads));
    metrics.legacy_channel_update(constants::HiSeq);

    model::plot::plot_data<model::plot::bar_point> data;
    logic::plot::plot_sample_qc(metrics, 1, data);
    ASSERT_EQ(data.size(), 0u);
    EXPECT_EQ(data.x_axis().label(), "");
    EXPECT_EQ(data.y_axis().label(), "");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 0.0f, tol);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Lists
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Test that all metrics printed out by list_by_cycle_metrics are actually cycle metrics
TEST(plot_logic, check_plot_by_cycle_list)
{
    std::vector<logic::utils::metric_type_description_t> types;
    logic::plot::list_by_cycle_metrics(types);
    for (size_t i = 0; i < types.size(); ++i)
    {
        EXPECT_FALSE(logic::utils::is_read_metric(types[i]));
        EXPECT_FALSE(logic::utils::is_tile_metric(types[i]))
                            << types[i] << " " << constants::to_string(types[i].value());
        EXPECT_TRUE(logic::utils::is_cycle_metric(types[i]))
                            << types[i] << " " << constants::to_string(types[i].value());
    }
}

//Test that all cycle metrics are actually printed out by list_by_cycle_metrics
TEST(plot_logic, check_all_cycle_list)
{
    //Test all cycle metrics are actually printed out by list_by_cycle_metrics
    std::vector<constants::metric_type> metrics;
    std::vector<constants::metric_type> cycle_metrics;
    std::vector<logic::utils::metric_type_description_t> types;
    constants::list_enums(metrics);

    for (size_t i = 0; i < metrics.size(); i++)
    {
        if ((logic::utils::is_cycle_metric(metrics[i]) &&
            std::find(cycle_metrics.begin(), cycle_metrics.end(), metrics[i]) == cycle_metrics.end()))
        {
            cycle_metrics.push_back(metrics[i]);
        }
    }

    logic::plot::list_by_cycle_metrics(types);
    EXPECT_TRUE(types.size() == cycle_metrics.size());
}

//Test that all metrics printed out by list_by_lane_metrics are actually lane metrics
TEST(plot_logic, check_plot_by_lane_list)
{
    std::vector<logic::utils::metric_type_description_t> types;
    logic::plot::list_by_lane_metrics(types);
    for (size_t i = 0; i < types.size(); ++i)
    {
        EXPECT_TRUE(logic::utils::is_read_metric(types[i]) || logic::utils::is_tile_metric(types[i]));
        EXPECT_FALSE(logic::utils::is_cycle_metric(types[i]));
    }
}

//Test that all lane metrics are actually printed out by list_by_lane_metrics
TEST(plot_logic, check_all_lane_list)
{
    //Test all lane (read / tile) metrics are actually printed out by list_by_lane_metrics
    std::vector<constants::metric_type> metrics;
    std::vector<constants::metric_type> lane_metrics;
    std::vector<logic::utils::metric_type_description_t> types;
    constants::list_enums(metrics);

    for (size_t i = 0; i < metrics.size(); i++)
    {
        if (((logic::utils::is_read_metric(metrics[i]) || logic::utils::is_tile_metric(metrics[i]))
             && !logic::utils::is_cycle_metric(metrics[i])) &&
            std::find(lane_metrics.begin(), lane_metrics.end(), metrics[i]) == lane_metrics.end())
        {
            lane_metrics.push_back(metrics[i]);
        }
    }

    logic::plot::list_by_lane_metrics(types);
    EXPECT_TRUE(types.size() == lane_metrics.size());
}

//Test that all metrics printed out by list_flowcell_metrics are actually flowcell metrics
TEST(plot_logic, check_plot_flowcell_list)
{
    std::vector<logic::utils::metric_type_description_t> types;
    logic::plot::list_flowcell_metrics(types);
    for (size_t i = 0; i < types.size(); ++i)
    {
        EXPECT_TRUE(logic::utils::is_read_metric(types[i])
                    || logic::utils::is_tile_metric(types[i])
                    || logic::utils::is_cycle_metric(types[i]));
    }
}

//Test all flowcell metrics are actually printed out by list_flowcell_metrics
TEST(plot_logic, check_all_flowcell_list)
{
    std::vector<constants::metric_type> metrics;
    std::vector<constants::metric_type> flowcell_metrics;
    std::vector<logic::utils::metric_type_description_t> types;
    constants::list_enums(metrics);

    for (size_t i = 0; i < metrics.size(); i++)
    {
        if (logic::utils::is_read_metric(metrics[i]) ||
            logic::utils::is_tile_metric(metrics[i]) ||
            logic::utils::is_cycle_metric(metrics[i]))
        {
            {
                if (std::find(flowcell_metrics.begin(), flowcell_metrics.end(), metrics[i]) == flowcell_metrics.end())
                {
                    flowcell_metrics.push_back(metrics[i]);
                }
            }
        }
    }

    logic::plot::list_flowcell_metrics(types);
    EXPECT_TRUE(types.size() == flowcell_metrics.size());
}




