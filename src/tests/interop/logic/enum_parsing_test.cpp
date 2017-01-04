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

/* Tests to ensure that each metric group is accurately represented by at least 1 member of the group */
TEST(enum_parsing_test, metric_type_to_string)
{
    EXPECT_EQ(constants::to_string(constants::Intensity), std::string("Intensity"));
}

TEST(enum_parsing_test, metric_type_to_group_Intensity)
{
    EXPECT_EQ(logic::utils::to_group(constants::Intensity), constants::Extraction);
}

TEST(enum_parsing_test, metric_type_to_group_Q20Percent)
{
    EXPECT_EQ(logic::utils::to_group(constants::Q20Percent), constants::Q);
}

TEST(enum_parsing_test, metric_type_to_group_PercentBase)
{
    EXPECT_EQ(logic::utils::to_group(constants::BasePercent), constants::CorrectedInt);
}

TEST(enum_parsing_test, metric_type_to_group_Clusters)
{
    EXPECT_EQ(logic::utils::to_group(constants::Clusters), constants::Tile);
}

TEST(enum_parsing_test, metric_type_to_group_ErrorRate)
{
    EXPECT_EQ(logic::utils::to_group(constants::ErrorRate), constants::Error);
}

/** Fixture to list enum values */
template<typename Enum>
struct enum_unknown_test : public ::testing::Test
{
    /** Constructor */
    enum_unknown_test()
    {
        constants::list_enums(features);
    }
    /** List of all available enum values */
    std::vector< Enum > features;
};
typedef ::testing::Types<
        constants::metric_feature_type,
        constants::metric_type,
        constants::metric_group,
        constants::tile_naming_method,
        constants::dna_bases,
        constants::surface_type,
        constants::instrument_type,
        constants::metric_base_type,
        constants::plot_colors,
        constants::bar_plot_options,
        constants::metric_data,
        constants::metric_feature_type,
        constants::plot_types
> all_enums_t;
TYPED_TEST_CASE(enum_unknown_test, all_enums_t);

/** Confirm that every enum does not conflict with unknown */
TYPED_TEST(enum_unknown_test, format)
{
    constants::list_enums(TestFixture::features);
    for(size_t i=0, n=TestFixture::features.size()-1;i<n;++i)
        ASSERT_NE(TestFixture::features[i], static_cast<TypeParam>(INTEROP_UNKNOWN))
                                    << constants::to_string(TestFixture::features[i]);
    EXPECT_EQ(TestFixture::features.back(), static_cast<TypeParam>(INTEROP_UNKNOWN));
}

