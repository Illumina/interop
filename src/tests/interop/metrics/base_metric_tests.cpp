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
#include "interop/model/metric_base/base_read_metric.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

using namespace illumina::interop::model;
using namespace illumina::interop::model::metric_base;

TEST(base_metric_test, lane_from_id)
{
    const base_metric::id_t id = base_metric::create_id(8, 1323);
    EXPECT_EQ(base_metric::lane_from_id(id), 8u);

    const base_metric::id_t id2 = base_metric::create_id(1, 1);
    EXPECT_EQ(base_metric::lane_from_id(id2), 1u);
}

TEST(base_metric_test, tile_from_id)
{
    const base_metric::id_t id = base_metric::create_id(8, 1323);
    EXPECT_EQ(base_metric::tile_from_id(id), 1323u);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_cycle_metric_test, lane_from_id)
{
    const base_metric::id_t id = base_cycle_metric::create_id(8, 1323, 2);
    EXPECT_EQ(base_cycle_metric::lane_from_id(id), 8u);

    const base_metric::id_t id2 = base_cycle_metric::create_id(1, 1, 2);
    EXPECT_EQ(base_cycle_metric::lane_from_id(id2), 1u);
}

TEST(base_cycle_metric_test, tile_from_id)
{
    const base_metric::id_t id = base_cycle_metric::create_id(8, 1323, 2);
    EXPECT_EQ(base_cycle_metric::tile_from_id(id), 1323u);
    EXPECT_EQ(base_cycle_metric::reserved_from_id(id), 0u);
}

TEST(base_cycle_metric_test, cycle_from_id)
{
    const base_metric::id_t id = base_cycle_metric::create_id(8, 1323, 10);
    EXPECT_EQ(base_cycle_metric::cycle_from_id(id), 10u);
    EXPECT_EQ(base_cycle_metric::reserved_from_id(id), 0u);
}
TEST(base_cycle_metric_test, tile_hash_from_id)
{
    const base_metric::id_t id = base_cycle_metric::create_id(8, 1323, 10);
    EXPECT_EQ(base_cycle_metric::tile_hash_from_id(id), base_metric::create_id(8, 1323));
    EXPECT_EQ(base_cycle_metric::reserved_from_id(id), 0u);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST(base_read_metric_test, lane_from_id)
{
    const base_metric::id_t id = base_read_metric::create_id(8, 1323, 2);
    EXPECT_EQ(base_read_metric::lane_from_id(id), 8u);

    const base_metric::id_t id2 = base_read_metric::create_id(1, 1, 2);
    EXPECT_EQ(base_read_metric::lane_from_id(id2), 1u);
}

TEST(base_read_metric_test, tile_from_id)
{
    const base_metric::id_t id = base_read_metric::create_id(8, 1323, 2);
    EXPECT_EQ(base_read_metric::tile_from_id(id), 1323u);
}

TEST(base_read_metric_test, read_from_id)
{
    const base_metric::id_t id = base_read_metric::create_id(8, 1323, 3);
    EXPECT_EQ(base_read_metric::read_from_id(id), 3u);
}

TEST(base_read_metric_test, tile_hash_from_id)
{
    const base_metric::id_t id = base_read_metric::create_id(8, 1323, 10);
    EXPECT_EQ(base_read_metric::tile_hash_from_id(id), base_metric::create_id(8, 1323));
}



