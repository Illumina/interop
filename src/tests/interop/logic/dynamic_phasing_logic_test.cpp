/** Unit tests for dynamic phasing metric logic
 *
 *  @file
 *  @date  11/28/16
 *  @version 1.0
 *  @copyright GNU Public License
 */
#include <gtest/gtest.h>
#if (defined(__cplusplus) && __cplusplus >= 201103L)
#   include <random>
#endif
#include <algorithm>
#include "interop/util/length_of.h"
#include "interop/logic/metric/dynamic_phasing_metric.h"
#include "interop/io/metric_file_stream.h"
#include "src/tests/interop/metrics/inc/metric_test.h"


using namespace illumina::interop;
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::logic::metric;
using namespace illumina::interop::unittest;

TEST(dynamic_phasing_logic, populate_dynamic_phasing_metrics)
{
    typedef model::metric_base::base_metric::uint_t uint_t;
    model::metric_base::metric_set<model::metrics::phasing_metric> phasing_metrics_set;
    const float R1_phasing_slope = 1, R1_phasing_offset = 0;
    const float R2_phasing_slope = 10, R2_phasing_offset = -2.23f;
    const float R1_prephasing_slope = 0, R1_prephasing_offset = 0;
    const float R2_prephasing_slope = -1, R2_prephasing_offset = 178;
    const size_t read_length = 26;
    const uint_t lane = 1, tile = 1101;
    logic::summary::read_cycle_vector_t cycle_to_read;
    for(size_t cycle = 1; cycle <= read_length; ++cycle)
    {
        cycle_to_read.push_back(logic::summary::read_cycle(1, cycle));
        phasing_metrics_set.insert(phasing_metric(lane, tile, static_cast<uint_t>(cycle),
                                               cycle * R1_phasing_slope + R1_phasing_offset,
                                               cycle * R1_prephasing_slope + R1_prephasing_offset));
    }
    for(size_t cycle = read_length+1; cycle <= 2*read_length; ++cycle)
    {
        cycle_to_read.push_back(logic::summary::read_cycle(2, cycle - read_length));
        phasing_metrics_set.insert(phasing_metric(lane, tile, static_cast<uint_t>(cycle),
                                               (cycle - read_length) * R2_phasing_slope + R2_phasing_offset,
                                               (cycle - read_length) * R2_prephasing_slope + R2_prephasing_offset));
    }

    model::metric_base::metric_set<model::metrics::dynamic_phasing_metric> dynamic_phasing_metrics;
    model::metric_base::metric_set<model::metrics::tile_metric> tile_metrics;
    populate_dynamic_phasing_metrics(phasing_metrics_set, cycle_to_read, dynamic_phasing_metrics, tile_metrics);

    const float tol = 1e-4f;
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 1).phasing_slope(), R1_phasing_slope, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 1).phasing_offset(), R1_phasing_offset, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 1).prephasing_slope(), R1_prephasing_slope, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 1).prephasing_offset(), R1_phasing_offset, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 2).phasing_slope(), R2_phasing_slope, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 2).phasing_offset(), R2_phasing_offset, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 2).prephasing_slope(), R2_prephasing_slope, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 2).prephasing_offset(), R2_prephasing_offset, tol);
}

TEST(dynamic_phasing_logic, populate_dynamic_phasing_metrics_reorder)
{
    typedef model::metric_base::base_metric::uint_t uint_t;
    model::metric_base::metric_set<model::metrics::phasing_metric> phasing_metrics_set;
    const float R1_phasing_slope = 1, R1_phasing_offset = 0;
    const float R2_phasing_slope = 10, R2_phasing_offset = -2.23f;
    const float R1_prephasing_slope = 0, R1_prephasing_offset = 0;
    const float R2_prephasing_slope = -1, R2_prephasing_offset = 178;
    const size_t read_length = 26;
    const uint_t lane = 1, tile = 1101;
    logic::summary::read_cycle_vector_t cycle_to_read;
    for(size_t cycle = 1; cycle <= read_length; ++cycle)
    {
        cycle_to_read.push_back(logic::summary::read_cycle(1, cycle));
        phasing_metrics_set.insert(phasing_metric(lane, tile, static_cast<uint_t>(cycle),
                                                  cycle * R1_phasing_slope + R1_phasing_offset,
                                                  cycle * R1_prephasing_slope + R1_prephasing_offset));
    }
    for(size_t cycle = read_length+1; cycle <= 2*read_length; ++cycle)
    {
        cycle_to_read.push_back(logic::summary::read_cycle(2, cycle - read_length));
        phasing_metrics_set.insert(phasing_metric(lane, tile, static_cast<uint_t>(cycle),
                                                  (cycle - read_length) * R2_phasing_slope + R2_phasing_offset,
                                                  (cycle - read_length) * R2_prephasing_slope + R2_prephasing_offset));
    }
#if (defined(__cplusplus) && __cplusplus >= 201103L)
    // Randomize metrics
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(phasing_metrics_set.begin(), phasing_metrics_set.end(), g);
// #else TODO: Test me in this case too!
#endif

    model::metric_base::metric_set<model::metrics::dynamic_phasing_metric> dynamic_phasing_metrics;
    model::metric_base::metric_set<model::metrics::tile_metric> tile_metrics;
    populate_dynamic_phasing_metrics(phasing_metrics_set, cycle_to_read, dynamic_phasing_metrics, tile_metrics);

    const float tol = 1e-3f;
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 1).phasing_slope(), R1_phasing_slope, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 1).phasing_offset(), R1_phasing_offset, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 1).prephasing_slope(), R1_prephasing_slope, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 1).prephasing_offset(), R1_phasing_offset, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 2).phasing_slope(), R2_phasing_slope, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 2).phasing_offset(), R2_phasing_offset, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 2).prephasing_slope(), R2_prephasing_slope, tol);
    EXPECT_NEAR(dynamic_phasing_metrics.get_metric(lane, tile, 2).prephasing_offset(), R2_prephasing_offset, tol);
}