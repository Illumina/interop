
/** Unit tests for CSV format parsing and wirting
 *
 *  @file
 *  @date  9/12/2017
 *  @version 1.0
 *  @copyright GNU Public License
 */
#include <gtest/gtest.h>
#include <vector>
#include "interop/io/table/csv_format.h"
#include "src/tests/interop/inc/generic_fixture.h"

using namespace illumina::interop;


TEST(csv_format_test, read_value)
{
    std::string line = "3.0,-inf,1.0";
    //void read_csv_line(std::istream& in, std::vector<T>& values, const T missing=T())
    std::vector<float> vals;
    std::istringstream sin(line);
    io::table::read_csv_line<float>(sin, vals);
    const float eps = 1e-3f;
    INTEROP_EXPECT_NEAR(3.0f, vals[0], eps);
    INTEROP_EXPECT_NEAR(std::numeric_limits<float>::infinity(), vals[1], eps);
    INTEROP_EXPECT_NEAR(1.0f, vals[2], eps);
}