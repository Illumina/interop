/** Unit tests for enum parsing
 *
 *  @file
 *  @date  4/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/logic/utils/metric_type_ext.h"


using namespace illumina::interop;


TEST(metric_type_ext, is_base_type)
{
    EXPECT_TRUE(logic::utils::is_base_metric(constants::CalledIntensity));
    EXPECT_FALSE(logic::utils::is_base_metric(constants::Intensity));
}

TEST(metric_type_ext, is_channel_metric)
{
    EXPECT_TRUE(logic::utils::is_channel_metric(constants::Intensity));
    EXPECT_FALSE(logic::utils::is_channel_metric(constants::CalledIntensity));
}

TEST(metric_type_ext, is_read_metric)
{
    EXPECT_TRUE(logic::utils::is_read_metric(constants::PercentAligned));
    EXPECT_FALSE(logic::utils::is_read_metric(constants::CalledIntensity));
}

TEST(metric_type_ext, is_cycle_metric)
{
    EXPECT_TRUE(logic::utils::is_cycle_metric(constants::Intensity));
    EXPECT_FALSE(logic::utils::is_cycle_metric(constants::PercentAligned));
}

