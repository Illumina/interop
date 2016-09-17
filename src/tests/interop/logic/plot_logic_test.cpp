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

using namespace illumina::interop;

TEST(plot_logic, check_plot_by_cycle_list)
{
    std::vector<logic::utils::metric_type_description_t> types;
    logic::plot::list_by_cycle_metrics(types);
    for(size_t i=0;i<types.size();++i)
    {
        EXPECT_FALSE(logic::utils::is_read_metric(types[i]));
        EXPECT_FALSE(logic::utils::is_tile_metric(types[i]));
        EXPECT_TRUE(logic::utils::is_cycle_metric(types[i]));
    }
}

TEST(plot_logic, check_plot_by_lane_list)
{
    std::vector<logic::utils::metric_type_description_t> types;
    logic::plot::list_by_lane_metrics(types);
    for(size_t i=0;i<types.size();++i)
    {
        EXPECT_TRUE(logic::utils::is_read_metric(types[i]) || logic::utils::is_tile_metric(types[i]));
        EXPECT_FALSE(logic::utils::is_cycle_metric(types[i]));
    }
}

TEST(plot_logic, check_plot_flowcell_list)
{
    std::vector< logic::utils::metric_type_description_t > types;
    logic::plot::list_flowcell_metrics(types);
    for(size_t i=0;i<types.size();++i)
    {
        EXPECT_TRUE(logic::utils::is_read_metric(types[i])
                    || logic::utils::is_tile_metric(types[i])
                    || logic::utils::is_cycle_metric(types[i]));
    }
}

/** @test Confirm invalid metric thrown */
TEST(plot_logic, failure_mode_bad_metric_plot_by_cycle)
{
    model::metrics::run_metrics metrics;
    model::plot::filter_options options(constants::FourDigit);
    model::plot::plot_data<model::plot::candle_stick_point> data;
    EXPECT_THROW(logic::plot::plot_by_cycle(metrics, "NoSuchMetric", options, data), model::invalid_metric_type);
}

TEST(plot_logic, intensity_by_cycle)
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
            model::run::flowcell_layout(2, 2, 2, 16),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    metrics.set_naming_method(constants::FourDigit);
    metrics.legacy_channel_update(constants::HiSeq);

    unittest::extraction_metric_v2::create_metric_set(metrics.get_set<model::metrics::extraction_metric>());

    model::plot::plot_data<model::plot::candle_stick_point> data;
    logic::plot::plot_by_cycle(metrics, constants::Intensity, options, data);
    ASSERT_EQ(data.size(), 4u);
    EXPECT_EQ(data.x_axis().label(), "Cycle");
    EXPECT_EQ(data.y_axis().label(), "Intensity");
    EXPECT_EQ(data.title(), "All Lanes All Channels");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 3.0f, tol);
    EXPECT_NEAR(data.y_axis().max(), 353.1f, tol);
    for(size_t channel=0;channel<4u;++channel)
    {
        for(size_t i=0;i<data[channel].size();++i)
        {
            EXPECT_EQ(data[channel][i].x(), i+1);
            // Could check y, but ...
        }
    }
}


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

    unittest::tile_metric_v2::create_metric_set(metrics.get_set<model::metrics::tile_metric>());

    model::plot::plot_data<model::plot::candle_stick_point> data;
    logic::plot::plot_by_lane(metrics, constants::ClusterCountPF, options, data);
    ASSERT_EQ(data.size(), 1u);
    EXPECT_EQ(data.title(), "");
    EXPECT_EQ(data.x_axis().label(), "Lane");
    EXPECT_EQ(data.y_axis().label(), "Clusters PF");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 8.0f, tol);
    EXPECT_NEAR(data.y_axis().max(),  3.9946798252130975, tol);

}

TEST(plot_logic, q_score_histogram)
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
    metrics.legacy_channel_update(constants::HiSeq);
    metrics.set_naming_method(constants::FourDigit);

    unittest::q_metric_v6::create_metric_set(metrics.get_set<model::metrics::q_metric>());
    metrics.finalize_after_load();

    model::plot::plot_data<model::plot::bar_point> data;
    logic::plot::plot_qscore_histogram(metrics, options, data);
    ASSERT_EQ(data.size(), 1u);
    EXPECT_EQ(data.title(), "All Lanes");
    EXPECT_EQ(data.x_axis().label(), "Q Score");
    EXPECT_EQ(data.y_axis().label(), "Total (million)");
    EXPECT_NEAR(data.x_axis().min(), 1.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 38.5f, tol);
    EXPECT_NEAR(data.y_axis().max(), 9.4780035018920898f, tol);
}

TEST(plot_logic, q_score_heatmap)
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
    metrics.legacy_channel_update(constants::HiSeq);
    metrics.set_naming_method(constants::FourDigit);

    unittest::q_metric_v6::create_metric_set(metrics.get_set<model::metrics::q_metric>());
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
            {0,0,0,0,0,0,0,0,0,9.1749,9.1749,9.1749,9.1749,9.1749,9.1749,9.1749,9.1749,9.1749,9.1749,4.06434,4.06434,
             4.06434,4.06434,4.06434,0.146682,0.146682,0.146682,0.146682,0.146682,95.7376,95.7376,95.7376,95.7376,
             95.7376,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8.26827,8.26827,8.26827,8.26827,8.26827,8.26827,8.26827,8.26827,
             8.26827,8.26827,1.53941,1.53941,1.53941,1.53941,1.53941,0.0376635,0.0376635,0.0376635,0.0376635,0.0376635,
             99.2799,99.2799,99.2799,99.2799,99.2799,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7.26372,7.26372,7.26372,7.26372,
             7.26372,7.26372,7.26372,7.26372,7.26372,7.26372,1.84695,1.84695,1.84695,1.84695,1.84695,0.0146203,
             0.0146203,0.0146203,0.0146203,0.0146203,100,100,100,100,100,0,0,0,0,0,0};

    for(size_t row=0, k=0;row<data.row_count();++row)
    {
        for(size_t col=0;col<data.column_count();++col,++k)
        {
            EXPECT_NEAR(static_cast<float>(expected[k]), data(row,col), 1e-3f);
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

    unittest::q_metric_v6::create_metric_set(metrics.get_set<model::metrics::q_metric>());
    metrics.finalize_after_load();

    model::plot::heatmap_data data1;
    logic::plot::plot_qscore_heatmap(metrics, options, data1);

    size_t row_count = logic::plot::count_rows_for_heatmap(metrics);
    size_t col_count = logic::plot::count_columns_for_heatmap(metrics);
    std::vector<float> buffer(row_count*col_count, 1);
    model::plot::heatmap_data data2;
    logic::plot::plot_qscore_heatmap(metrics, options, data2, &buffer.front());

    for(size_t row=0;row<data2.row_count();row++)
    {
        for(size_t col=0;col<data2.column_count();col++)
        {
            if(std::isnan(buffer[row*data2.column_count()+col]) && std::isnan(data1.at(row,col))) continue;
            EXPECT_NEAR(buffer[row*data2.column_count()+col], data1.at(row,col), tol) << data1.at(row,col);
        }
    }
}

TEST(plot_logic, flowcell_map)
{
    const model::plot::filter_options::id_t ALL_IDS = model::plot::filter_options::ALL_IDS;
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info(
            "",
            "",
            1,
            model::run::flowcell_layout(8, 2, 2, 36, 1, 1, std::vector<std::string>(), constants::FourDigit),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    model::plot::filter_options options(constants::FourDigit, ALL_IDS, 0, constants::A, ALL_IDS, 1, 1);
    metrics.legacy_channel_update(constants::HiSeq);

    unittest::extraction_metric_v2::create_metric_set(metrics.get_set<model::metrics::extraction_metric>());
    ASSERT_GT(metrics.extraction_metric_set().size(), 0u);

    model::plot::flowcell_data data;
    logic::plot::plot_flowcell_map(metrics, constants::Intensity, options, data);
    ASSERT_EQ(data.row_count(), 8u);
    EXPECT_EQ(data.title(), "Intensity");
    EXPECT_EQ(data.saxis().label(), "Intensity");
    EXPECT_NEAR(data.saxis().min(), 302.0f, tol);
    EXPECT_NEAR(data.saxis().max(), 349.0f, tol);
}

TEST(plot_logic, sample_qc)
{
    const model::plot::filter_options::id_t ALL_IDS = model::plot::filter_options::ALL_IDS;
    const float tol = 1e-3f;
    model::metrics::run_metrics metrics;
    std::vector<model::run::read_info> reads(2);
    reads[0] = model::run::read_info(1, 1, 26);
    reads[1] = model::run::read_info(2, 27, 76);
    metrics.run_info(model::run::info(
            "",
            "",
            1,
            model::run::flowcell_layout(8, 2, 2, 36, 1, 1, std::vector<std::string>(), constants::FourDigit),
            std::vector<std::string>(),
            model::run::image_dimensions(),
            reads
    ));
    model::plot::filter_options options(constants::FourDigit, ALL_IDS, 0, constants::A, ALL_IDS, 1, 1);
    metrics.legacy_channel_update(constants::HiSeq);

    unittest::index_metric_v1::create_metric_set(metrics.get_set<model::metrics::index_metric>());
    unittest::tile_metric_v2::create_metric_set(metrics.get_set<model::metrics::tile_metric>());
    ASSERT_GT(metrics.index_metric_set().size(), 0u);

    model::plot::plot_data<model::plot::bar_point> data;
    logic::plot::plot_sample_qc(metrics, 1, data);
    ASSERT_EQ(data.size(), 1u);
    EXPECT_EQ(data[0].title(), "% reads");
    EXPECT_EQ(data.x_axis().label(), "Index Number");
    EXPECT_EQ(data.y_axis().label(), "% Reads Identified (PF)");
    EXPECT_NEAR(data.x_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.y_axis().min(), 0.0f, tol);
    EXPECT_NEAR(data.x_axis().max(), 1, tol);
    EXPECT_NEAR(data.y_axis().max(), 5, tol);
}
