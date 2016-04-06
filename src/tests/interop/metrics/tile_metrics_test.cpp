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
#include "inc/tile_metrics_test.h"
#include "interop/util/statistics.h"
#include "interop/util/type_traits.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;
using namespace illumina;



typedef ::testing::Types<
        hardcoded_fixture<tile_v2>,
        write_read_fixture<tile_v2>
> Formats;
TYPED_TEST_CASE(tile_metrics_test, Formats);

/**
 * @class illumina::interop::model::metrics::tile_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 */
TYPED_TEST(tile_metrics_test, test_read_write)
{
    const float scale = is_same< TypeParam, write_read_fixture<tile_v2> >::value ? 100 : 1;
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metric_set.size(), TypeParam::expected_metric_set.size());

    const float tol = 1e-7f / 0.01f;
    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metric_set.begin(), itActual = TypeParam::actual_metric_set.begin();
        itExpected != TypeParam::expected_metric_set.end() && itActual != TypeParam::actual_metric_set.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());

        EXPECT_NEAR(itExpected->clusterDensity(), itActual->clusterDensity(), tol);
        EXPECT_NEAR(itExpected->clusterDensityPf(), itActual->clusterDensityPf(), tol);
        EXPECT_NEAR(itExpected->clusterCount(), itActual->clusterCount(), tol);
        EXPECT_NEAR(itExpected->clusterCountPf(), itActual->clusterCountPf(), tol);
        EXPECT_EQ(itExpected->read_metrics().size(), itActual->read_metrics().size());
        for(typename TypeParam::metric_t::read_metric_vector::const_iterator itReadExpected = itExpected->read_metrics().begin(),
                        itReadActual = itActual->read_metrics().begin();
                        itReadExpected != itExpected->read_metrics().end() &&
                        itReadActual != itActual->read_metrics().end(); itReadExpected++, itReadActual++)
        {
            EXPECT_EQ(itReadExpected->read(), itReadActual->read());
            EXPECT_NEAR(itReadExpected->percent_aligned(), itReadActual->percent_aligned(), tol);
            EXPECT_NEAR(itReadExpected->percent_phasing()*scale, itReadActual->percent_phasing(), tol);
            EXPECT_NEAR(itReadExpected->percent_prephasing()*scale, itReadActual->percent_prephasing(), tol);
        }
    }
}

TYPED_TEST(tile_metrics_test, median)
{
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::median(TypeParam::expected_metric_set.begin(),
                                                                    TypeParam::expected_metric_set.end(),
                                                                    interop::util::op::const_member_function_less(read, &tile_metric::percent_aligned))->percent_aligned(read);
    EXPECT_NEAR(expected_percent_aligned_avg, 2.6163086891174316, tol);
}

TYPED_TEST(tile_metrics_test, mean)
{
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::mean<float>(TypeParam::expected_metric_set.begin(),
                                                                    TypeParam::expected_metric_set.end(),
                                                                    interop::util::op::const_member_function(read, &tile_metric::percent_aligned));
    EXPECT_NEAR(expected_percent_aligned_avg, 2.5763518810272217, tol);
}

TYPED_TEST(tile_metrics_test, nan_mean)
{
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::nan_mean<float>(TypeParam::expected_metric_set.begin(),
                                                                        TypeParam::expected_metric_set.end(),
                                                                        interop::util::op::const_member_function(read, &tile_metric::percent_aligned));
    EXPECT_NEAR(expected_percent_aligned_avg, 2.5763518810272217, tol);
}


TYPED_TEST(tile_metrics_test, standard_deviation)
{
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_std = std::sqrt(interop::util::variance<float>(TypeParam::expected_metric_set.begin(),
                                                                                  TypeParam::expected_metric_set.end(),
                                                                                  interop::util::op::const_member_function(read, &tile_metric::percent_aligned)));
    EXPECT_NEAR(expected_percent_aligned_std, 0.074578315019607544, tol);
}

TYPED_TEST(tile_metrics_test, nan_standard_deviation)
{
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_std = std::sqrt(interop::util::nan_variance<float>(TypeParam::expected_metric_set.begin(),
                                                                                  TypeParam::expected_metric_set.end(),
                                                                                  interop::util::op::const_member_function(read, &tile_metric::percent_aligned)));
    EXPECT_NEAR(expected_percent_aligned_std, 0.074578315019607544, tol);
}

TYPED_TEST(tile_metrics_test, standard_deviation_vec)
{
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    std::vector<float> percent_aligned_vec(TypeParam::expected_metric_set.size());
    for(size_t i=0;i<TypeParam::expected_metric_set.size();++i) percent_aligned_vec[i] = TypeParam::expected_metric_set.at(i).percent_aligned(read);
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
    EXPECT_NEAR(expected_metric.clusterDensity(), actual_metric.clusterDensity(), tol);
    EXPECT_NEAR(expected_metric.clusterDensityPf(), actual_metric.clusterDensityPf(), tol);
    EXPECT_NEAR(expected_metric.clusterCount(), actual_metric.clusterCount(), tol);
    EXPECT_NEAR(expected_metric.clusterCountPf(), actual_metric.clusterCountPf(), tol);
    EXPECT_EQ(expected_metric.read_metrics().size(), actual_metric.read_metrics().size());


}

#define FIXTURE tile_metrics_test
/**
 * @class illumina::interop::model::metrics::tile_metric
 * @test Confirm binary write matches expected binary data
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "inc/stream_tests.hpp"

