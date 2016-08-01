/** Unit tests for enum parsing
 *
 *  @file
 *  @date  4/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/logic/utils/enums.h"
#include "interop/logic/utils/metric_type_ext.h"


using namespace illumina::interop;


TEST(enum_parsing_test, parse_metric_type)
{
    EXPECT_EQ(constants::parse<constants::metric_type>("Intensity"), constants::Intensity);
    EXPECT_EQ(constants::parse<constants::metric_type>("FWHM"), constants::FWHM);
    EXPECT_EQ(constants::parse<constants::metric_type>("Gobble"), constants::UnknownMetricType);
}

TEST(enum_parsing_test, list_enums_intensity)
{
    std::vector<constants::metric_type> types;
    constants::list_enums(types);
    EXPECT_EQ(types[0], constants::Intensity);
}

TEST(enum_parsing_test, metric_type_to_string)
{
    EXPECT_EQ(constants::to_string(constants::Intensity), std::string("Intensity"));
}

TEST(enum_parsing_test, metric_type_to_group)
{
    EXPECT_EQ(logic::utils::to_group(constants::Intensity), constants::Extraction);
}
