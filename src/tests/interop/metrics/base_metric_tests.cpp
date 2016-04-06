/** Unit tests for base metric operations
 *
 *
 *  @file
 *  @date  3/24/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/io/layout/base_metric.h"

#ifdef _MSC_VER
#pragma warning(push)
    #pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

using namespace illumina::interop::io::layout;


TEST(base_metric_test, lane_from_id)
{
    base_metric::id_t id = base_metric::id(8, 1323);
    EXPECT_EQ(base_metric::lane_from_id(id), 8u);


    base_metric::id_t id2 = base_metric::id(1, 1);
    EXPECT_EQ(base_metric::lane_from_id(id2), 1u);
}
