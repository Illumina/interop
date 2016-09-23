/** Unit tests for option parsing
 *
 *  @file
 *  @date  7/3/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/constants/enums.h"
#include "interop/util/option_parser.h"
#include "interop/model/plot/filter_options.h"

using namespace illumina::interop;


TEST(option_parser_test, parse_int)
{
    int actual_value=0;
    int expected_value = 3;
    const char* argv[] = {"program", "file", "--actual=3"};
    int argc = 3;

    util::option_parser description;
    description(actual_value, "actual", "Help description");
    description.parse(argc, argv);

    EXPECT_EQ(expected_value, actual_value);
    EXPECT_EQ(argc, 2);
}

TEST(option_parser_test, parse_lane_wrapper)
{
    model::plot::filter_options options(constants::UnknownTileNamingMethod);
    unsigned int expected_value = 3;
    const char* argv[] = {"program", "file", "--actual=3"};
    int argc = 3;

    util::option_parser description;
    description(util::wrap_setter(options, &model::plot::filter_options::lane), "actual", "Help description");
    description.parse(argc, argv);

    EXPECT_EQ(expected_value, options.lane());
    EXPECT_EQ(argc, 2);
}

TEST(option_parser_test, invalid_option_exception)
{
    int actual_value=0;
    const char* argv[] = {"program", "file", "--actual", "3"};
    int argc = 3;

    util::option_parser description;
    description(actual_value, "actual", "Help description");
    EXPECT_THROW(description.parse(argc, argv), util::invalid_option_exception);
}

TEST(option_parser_test, invalid_option_value_exception)
{
    int actual_value=0;
    const char* argv[] = {"program", "file", "--actual=3b"};
    int argc = 3;

    util::option_parser description;
    description(actual_value, "actual", "Help description");
    EXPECT_THROW(description.parse(argc, argv), util::invalid_option_value_exception);
}
TEST(option_parser_test, no_invalid_option_value_exception)
{
    int actual_value=0;
    const char* argv[] = {"program", "file", "--actual=3  "};
    int argc = 3;

    util::option_parser description;
    description(actual_value, "actual", "Help description");
    EXPECT_NO_THROW(description.parse(argc, argv));
}
