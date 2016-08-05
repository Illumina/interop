/** Unit tests for base metric operations
 *
 *
 *  @file
 *  @date  3/24/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/model/metric_base/base_metric.h"
#include "interop/model/metric_base/base_cycle_metric.h"

#ifdef _MSC_VER
#pragma warning(push)
    #pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

using namespace illumina::interop::model::metric_base;

TEST(base_metric_test, lane_from_id)
{
    base_metric::id_t id = base_metric::create_id(8, 1323);
    EXPECT_EQ(base_metric::lane_from_id(id), 8u);


    base_metric::id_t id2 = base_metric::create_id(1, 1);
    EXPECT_EQ(base_metric::lane_from_id(id2), 1u);
}
TEST(base_metric_test, cycle_from_id)
{
    base_metric::id_t id = base_cycle_metric::create_id(8, 1323, 10);
    EXPECT_EQ(base_cycle_metric::cycle_from_id(id), 10u);
}
TEST(base_metric_test, tile_has_from_id)
{
    base_metric::id_t id = base_cycle_metric::create_id(8, 1323, 10);
    EXPECT_EQ(base_cycle_metric::tile_hash_from_id(id), base_metric::create_id(8, 1323));
}

