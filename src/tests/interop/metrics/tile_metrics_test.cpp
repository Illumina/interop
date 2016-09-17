/** Unit tests for the tile metrics
 *
 *
 *  @file
 *  @date 9/6/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <limits>
#include <fstream>
#include <set>
#include <gtest/gtest.h>
#include "interop/util/math.h"
#include "interop/util/statistics.h"
#include "interop/model/run_metrics.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"
#include "src/tests/interop/metrics/inc/tile_metrics_test.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;
using namespace illumina;


typedef metric_set< tile_metric > tile_metric_set;
/** Setup for tests that compare two tile metric sets */
struct tile_metrics_tests : public generic_test_fixture< tile_metric_set > {};


tile_metrics_tests::generator_type tile_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< tile_metric_v2 >) ,
        wrap(new write_read_metric_generator< tile_metric_v2 >)
};

// Setup unit tests for tile_metrics_tests
INSTANTIATE_TEST_CASE_P(tile_metric_unit_test,
                        tile_metrics_tests,
                        ::testing::ValuesIn(tile_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::tile_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 */
TEST_P(tile_metrics_tests, test_read_write)
{
    typedef tile_metric_set::const_iterator const_iterator;
    typedef tile_metric_set::metric_type metric_t;
    if(!test) return;// Disable test for rebaseline
    const float scale = 1;//is_same< TypeParam, write_read_fixture<tile_v2> >::value ? 100 : 1;
    EXPECT_EQ(actual.version(), expected.version());
    ASSERT_EQ(actual.size(), expected.size());

    const float tol = 1e-7f / 0.01f;
    for(const_iterator it_expected=expected.begin(), it_actual = actual.begin();
        it_expected != expected.end() && it_actual != actual.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());

        EXPECT_NEAR(it_expected->cluster_density(), it_actual->cluster_density(), tol);
        EXPECT_NEAR(it_expected->cluster_density_pf(), it_actual->cluster_density_pf(), tol);
        EXPECT_NEAR(it_expected->cluster_count(), it_actual->cluster_count(), tol);
        EXPECT_NEAR(it_expected->cluster_count_pf(), it_actual->cluster_count_pf(), tol);
        EXPECT_EQ(it_expected->read_metrics().size(), it_actual->read_metrics().size());
        for(metric_t::read_metric_vector::const_iterator it_read_expected = it_expected->read_metrics().begin(),
                        it_read_actual = it_actual->read_metrics().begin();
                        it_read_expected != it_expected->read_metrics().end() &&
                        it_read_actual != it_actual->read_metrics().end(); it_read_expected++, it_read_actual++)
        {
            EXPECT_EQ(it_read_expected->read(), it_read_actual->read());
            EXPECT_NEAR(it_read_expected->percent_aligned(), it_read_actual->percent_aligned(), tol);
            if(!std::isnan(it_read_expected->percent_phasing()) && !std::isnan(it_read_actual->percent_phasing()))
                EXPECT_NEAR(it_read_expected->percent_phasing()*scale, it_read_actual->percent_phasing(), tol);
            if(!std::isnan(it_read_expected->percent_prephasing()) && !std::isnan(it_read_actual->percent_prephasing()))
                EXPECT_NEAR(it_read_expected->percent_prephasing()*scale, it_read_actual->percent_prephasing(), tol);
        }
    }
}

TEST(tile_metrics_test, median)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::median(expected.begin(),
                                                                    expected.end(),
                                                                    interop::util::op::const_member_function_less(read, &tile_metric::percent_aligned))->percent_aligned(read);
    EXPECT_NEAR(expected_percent_aligned_avg, 2.6163086891174316, tol);
}

TEST(tile_metrics_test, mean)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::mean<float>(expected.begin(),
                                                                    expected.end(),
                                                                    interop::util::op::const_member_function(read, &tile_metric::percent_aligned));
    EXPECT_NEAR(expected_percent_aligned_avg, 2.5763518810272217, tol);
}

TEST(tile_metrics_test, nan_mean)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::nan_mean<float>(expected.begin(),
                                                                        expected.end(),
                                                                        interop::util::op::const_member_function(read, &tile_metric::percent_aligned));
    EXPECT_NEAR(expected_percent_aligned_avg, 2.5763518810272217, tol);
}


TEST(tile_metrics_test, standard_deviation)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_std = std::sqrt(interop::util::variance<float>(expected.begin(),
                                                                                  expected.end(),
                                                                                  interop::util::op::const_member_function(read, &tile_metric::percent_aligned)));
    EXPECT_NEAR(expected_percent_aligned_std, 0.074578315019607544, tol);
}

TEST(tile_metrics_test, nan_standard_deviation)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_std = std::sqrt(interop::util::nan_variance<float>(expected.begin(),
                                                                                  expected.end(),
                                                                                  interop::util::op::const_member_function(read, &tile_metric::percent_aligned)));
    EXPECT_NEAR(expected_percent_aligned_std, 0.074578315019607544, tol);
}

TEST(tile_metrics_test, standard_deviation_vec)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    std::vector<float> percent_aligned_vec(expected.size());
    for(size_t i=0;i<expected.size();++i) percent_aligned_vec[i] = expected.at(i).percent_aligned(read);
    float expected_percent_aligned_std = std::sqrt(interop::util::variance<float>(percent_aligned_vec.begin(),
                                                                                  percent_aligned_vec.end()));
    EXPECT_NEAR(expected_percent_aligned_std, 0.074578315019607544, tol);
}

TEST(tile_metrics_test, test_unique_id_four_digit)
{
    typedef metric_set<tile_metric>::uint_t uint_t;
    typedef metric_set<tile_metric>::id_t id_t;
    std::set<id_t> ids;
    metric_set<tile_metric> metrics;
    for(uint_t lane=1;lane<=8;++lane)
    {
        for(uint_t surface=1;surface<=2;++surface) {
            for (uint_t swath = 1; swath <= 4; ++swath) {
                for (uint_t tile = 1; tile <= 36; ++tile) {
                    tile_metric metric(lane, surface*1000+swath*100+tile, 0, 0, 0, 0);
                    metrics.insert(metric.id(), metric);
                }
            }
        }
    }
    for(size_t i=0;i<metrics.size();i++)
    {
        EXPECT_TRUE(ids.find(metrics.at(i).id())==ids.end());
        ids.insert(metrics.at(i).id());
    }
}

TEST(tile_metrics_test, test_unique_id_five_digit)
{
    typedef metric_set<tile_metric>::uint_t uint_t;
    typedef metric_set<tile_metric>::id_t id_t;
    std::set<id_t> ids;
    metric_set<tile_metric> metrics;
    for(uint_t lane=1;lane<=8;++lane)
    {
        for(uint_t surface=1;surface<=2;++surface) {
            for (uint_t swath = 1; swath <= 2; ++swath) {
                for(uint_t section=1;section <=4;++section) {
                    for (uint_t tile = 1; tile <= 36; ++tile) {
                        tile_metric metric(lane, surface * 10000 + swath * 1000 + section*100 + tile, 0, 0, 0, 0);
                        metrics.insert(metric);
                    }
                }
            }
        }
    }
    for(size_t i=0;i<metrics.size();i++)
    {
        EXPECT_TRUE(ids.find(metrics.at(i).id())==ids.end());
        ids.insert(metrics.at(i).id());
    }
}

TEST(tile_metrics_test, test_tile_metric_for_lane)
{
    metric_set<tile_metric> metrics;
    tile_metric expected_metric(7, 1114, 2355119.25f,1158081.50f,6470949,3181956,
                tile_metric::read_metric_vector(1, tile_metric::read_metric_type(3, 2.61630869f, 0.0797112584f/100, 0.119908921f/100)));
    metrics.insert(expected_metric.id(), expected_metric);
    metric_set<tile_metric>::metric_array_t tile_lane_metrics = metrics.metrics_for_lane(7);
    tile_metric& actual_metric = tile_lane_metrics[0];

    EXPECT_EQ(expected_metric.lane(), actual_metric.lane());
    EXPECT_EQ(expected_metric.tile(), actual_metric.tile());

    const float tol = 1e-7f / 0.01f;
    EXPECT_NEAR(expected_metric.cluster_density(), actual_metric.cluster_density(), tol);
    EXPECT_NEAR(expected_metric.cluster_density_pf(), actual_metric.cluster_density_pf(), tol);
    EXPECT_NEAR(expected_metric.cluster_count(), actual_metric.cluster_count(), tol);
    EXPECT_NEAR(expected_metric.cluster_count_pf(), actual_metric.cluster_count_pf(), tol);
    EXPECT_EQ(expected_metric.read_metrics().size(), actual_metric.read_metrics().size());


}
TEST(run_metrics_tile_test, test_is_group_empty)
{
    run_metrics metrics;
    EXPECT_TRUE(metrics.is_group_empty(constants::Tile));
    io::read_interop_from_string(tile_v2::binary_data(),
                                 metrics.get_set<tile_metric>());
    EXPECT_FALSE(metrics.is_group_empty(constants::Tile));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<tile_metric_set> tile_regression_gen("metrics");
INSTANTIATE_TEST_CASE_P(tile_metric_regression_test,
                        tile_metrics_tests,
                        ProxyValuesIn(tile_regression_gen, regression_test_data::instance().files()));

