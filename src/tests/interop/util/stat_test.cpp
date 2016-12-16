/** Unit tests for the stat utility
*
*
*  @file
*  @date 9/18/2016
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
#include "src/tests/interop/metrics/inc/tile_metrics_test.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;
using namespace illumina;
typedef metric_set< tile_metric > tile_metric_set;

TEST(stat_test, median)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::median(expected.begin(),
                                                               expected.end(),
                                                               interop::util::op::const_member_function_less(read,
                                                                                                             &tile_metric::percent_aligned))->percent_aligned(
            read);
    EXPECT_NEAR(expected_percent_aligned_avg, 2.6163086891174316, tol);
}

TEST(stat_test, mean)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::mean<float>(expected.begin(),
                                                                    expected.end(),
                                                                    interop::util::op::const_member_function(read,
                                                                                                             &tile_metric::percent_aligned));
    EXPECT_NEAR(expected_percent_aligned_avg, 2.5763518810272217, tol);
}

TEST(stat_test, nan_mean)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_avg = interop::util::nan_mean<float>(expected.begin(),
                                                                        expected.end(),
                                                                        interop::util::op::const_member_function(read,
                                                                                                                 &tile_metric::percent_aligned));
    EXPECT_NEAR(expected_percent_aligned_avg, 2.5763518810272217, tol);
}


TEST(stat_test, standard_deviation)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_std = std::sqrt(interop::util::variance<float>(expected.begin(),
                                                                                  expected.end(),
                                                                                  interop::util::op::const_member_function(
                                                                                          read,
                                                                                          &tile_metric::percent_aligned)));
    EXPECT_NEAR(expected_percent_aligned_std, 0.074578315019607544, tol);
}

TEST(stat_test, nan_standard_deviation)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    float expected_percent_aligned_std = std::sqrt(interop::util::nan_variance<float>(expected.begin(),
                                                                                      expected.end(),
                                                                                      interop::util::op::const_member_function(
                                                                                              read,
                                                                                              &tile_metric::percent_aligned)));
    EXPECT_NEAR(expected_percent_aligned_std, 0.074578315019607544, tol);
}

TEST(stat_test, standard_deviation_vec)
{
    tile_metric_set expected;
    tile_metric_v2::create_expected(expected);
    const float tol = 1e-7f / 0.01f;
    const size_t read = 0;
    std::vector<float> percent_aligned_vec(expected.size());
    for (size_t i = 0; i < expected.size(); ++i) percent_aligned_vec[i] = expected[i].percent_aligned(read);
    float expected_percent_aligned_std = std::sqrt(interop::util::variance<float>(percent_aligned_vec.begin(),
                                                                                  percent_aligned_vec.end()));
    EXPECT_NEAR(expected_percent_aligned_std, 0.074578315019607544, tol);
}


