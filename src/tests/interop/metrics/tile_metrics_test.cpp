/** Unit tests for the tile metrics
 *
 *
 *  @file
 *
 *  @date 9/6/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <limits>
#include <fstream>
#include <set>
#include <gtest/gtest.h>
#include "interop/model/metric_sets/tile_metric_set.h"
#include "src/tests/interop/metrics/metric_test_utils.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;


namespace illumina{ namespace interop { namespace unittest {
/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * @note Version 2
 */
struct tile_metrics_hardcoded_fixture_v2 : util::fixture_helper<tile_metrics, 2>
{
    enum{
        /** Do not check the expected binary data */
        disable_binary_data=true
    };
    /** Setup fixture */
    tile_metrics_hardcoded_fixture_v2()
    {
        expected_metrics.push_back(metric_type(7, 1114, 2355119.25f,1158081.50f,6470949,3181956,
                                               metric_type::read_metric_vector(1, metric_type::read_metric_type(1, 2.61630869f, 0.0797112584f/100, 0.119908921f/100))));
        expected_metrics.push_back(metric_type(7, 1214, 2355119.25f,1174757.75f,6470949,3227776,
                                               metric_type::read_metric_vector(1, metric_type::read_metric_type(1, 2.62243795f, 0.129267812f/100, 0.135128692f/100))));
        expected_metrics.push_back(metric_type(7, 2114, 2355119.25f,1211592.38f,6470949,3328983,
                                               metric_type::read_metric_vector(1, metric_type::read_metric_type(1, 2.490309f, 0.11908555f/100, 0.092706576f/100))));
        int tmp[] = {
                2,10
                ,7,0,90,4,100,0,-67,-66,15,74
                ,7,0,90,4,102,0,74,122,-59,74
                ,7,0,90,4,101,0,12,94,-115,73
                ,7,0,90,4,103,0,16,54,66,74
                ,7,0,90,4,-56,0,82,-11,80,58
                ,7,0,90,4,-55,0,-62,42,-99,58
                ,7,0,90,4,44,1,-102,113,39,64
                ,7,0,-66,4,100,0,-67,-66,15,74
                ,7,0,-66,4,102,0,74,122,-59,74
                ,7,0,-66,4,101,0,46,103,-113,73
                ,7,0,-66,4,103,0,0,2,69,74
                ,7,0,-66,4,-56,0,21,111,-87,58
                ,7,0,-66,4,-55,0,-86,29,-79,58
                ,7,0,-66,4,44,1,6,-42,39,64
                ,7,0,66,8,100,0,-67,-66,15,74
                ,7,0,66,8,102,0,74,122,-59,74
                ,7,0,66,8,101,0,67,-26,-109,73
                ,7,0,66,8,103,0,92,47,75,74
                ,7,0,66,8,-56,0,123,22,-100,58
                ,7,0,66,8,-55,0,85,6,115,58
                ,7,0,66,8,44,1,57,97,31,64
        };
        setup_hardcoded_binary(tmp, header_type());
    }
};


/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 2
 */
struct tile_metrics_write_read_fixture_v2 : util::fixture_helper<tile_metrics, 2>
{
    enum{
        /** Do not check the expected binary data */
                disable_binary_data=true
    };
    /** Setup fixture */
    tile_metrics_write_read_fixture_v2()
    {
        expected_metrics.push_back(metric_type(7, 1114, 2355119.25f,1158081.50f,6470949,3181956,
                                               metric_type::read_metric_vector(1, metric_type::read_metric_type(3, 2.61630869f, 0.0797112584f/100, 0.119908921f/100))));
        expected_metrics.push_back(metric_type(7, 1214, 2355119.25f,1174757.75f,6470949,3227776,
                                               metric_type::read_metric_vector(1, metric_type::read_metric_type(3, 2.62243795f, 0.129267812f/100, 0.135128692f/100))));
        expected_metrics.push_back(metric_type(7, 2114, 2355119.25f,1211592.38f,6470949,3328983,
                                               metric_type::read_metric_vector(1, metric_type::read_metric_type(3, 2.490309f, 0.11908555f/100, 0.092706576f/100))));
        setup_write_read();
    }
};
/** Interface between fixtures and Google Test */
template<typename TestSetup>
struct tile_metrics_test : public ::testing::Test, public TestSetup { };
}}}
using namespace illumina::interop::unittest;



typedef ::testing::Types<
tile_metrics_hardcoded_fixture_v2,
tile_metrics_write_read_fixture_v2
> Formats;
TYPED_TEST_CASE(tile_metrics_test, Formats);

/**
 * @class illumina::interop::model::metrics::tile_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 */
TYPED_TEST(tile_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metrics.size(), TypeParam::expected_metrics.size());

    const float tol = 1e-7f / 0.01f;
    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metrics.begin(), itActual = TypeParam::actual_metrics.begin();
        itExpected != TypeParam::expected_metrics.end() && itActual != TypeParam::actual_metrics.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());

        EXPECT_NEAR(itExpected->clusterDensity(), itActual->clusterDensity(), tol);
        EXPECT_NEAR(itExpected->clusterDensityPf(), itActual->clusterDensityPf(), tol);
        EXPECT_NEAR(itExpected->clusterCount(), itActual->clusterCount(), tol);
        EXPECT_NEAR(itExpected->clusterCountPf(), itActual->clusterCountPf(), tol);
        EXPECT_EQ(itExpected->read_metrics().size(), itActual->read_metrics().size());
        for(typename TypeParam::metric_type::read_metric_vector::const_iterator itReadExpected = itExpected->read_metrics().begin(),
                        itReadActual = itActual->read_metrics().begin();
                        itReadExpected != itExpected->read_metrics().end() &&
                        itReadActual != itActual->read_metrics().end(); itReadExpected++, itReadActual++)
        {
            EXPECT_EQ(itReadExpected->read(), itReadActual->read());
            EXPECT_NEAR(itReadExpected->percent_aligned(), itReadActual->percent_aligned(), tol);
            EXPECT_NEAR(itReadExpected->percent_phasing(), itReadActual->percent_phasing(), tol);
            EXPECT_NEAR(itReadExpected->percent_prephasing(), itReadActual->percent_prephasing(), tol);
        }
    }
}

TEST(tile_metrics_test, test_unique_id_four_digit)
{
    typedef tile_metrics::uint_t uint_t;
    typedef tile_metrics::uint_t id_t;
    std::set<id_t> ids;
    tile_metrics metrics;
    for(uint_t lane=1;lane<=8;++lane)
    {
        for(uint surface=1;surface<=2;++surface) {
            for (uint swath = 1; swath <= 4; ++swath) {
                for (uint tile = 1; tile <= 36; ++tile) {
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
    typedef tile_metrics::uint_t uint_t;
    std::set<uint_t> ids;
    tile_metrics metrics;
    for(uint_t lane=1;lane<=8;++lane)
    {
        for(uint surface=1;surface<=2;++surface) {
            for (uint swath = 1; swath <= 2; ++swath) {
                for(uint section=1;section <=4;++section) {
                    for (uint tile = 1; tile <= 36; ++tile) {
                        tile_metric metric(lane, surface * 10000 + swath * 1000 + section*100 + tile, 0, 0, 0, 0);
                        metrics.insert(metric.id(), metric);
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
#include "src/tests/interop/metrics/stream_exception_tests.hpp"

