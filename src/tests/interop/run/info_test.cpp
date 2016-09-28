/** Test parsing of the run info XML file
 *
 *  @file
 *  @date 11/7/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include "interop/constants/enums.h"
#include "interop/model/run/info.h"

using namespace illumina::interop::model;
using namespace illumina::interop;

/** Convert an array to a vector
 *
 * Determines the length of the stack array automatically.
 *
 * @param vals array pointer
 * @return vector of values
 */
template<typename T, size_t N>
std::vector<T> to_vector(const T (&vals)[N])
{
    return std::vector<T>(vals, vals + N);
}

static void test_helper_run_info_parse_xml(const run::info& expected_run_info,
                                                std::string& xml_file)
{
    run::info run_info;
    run_info.parse(&xml_file[0]);

    EXPECT_EQ(run_info.name(), expected_run_info.name());
    EXPECT_EQ(run_info.date(), expected_run_info.date());
    EXPECT_EQ(run_info.version(), expected_run_info.version());
    EXPECT_EQ(run_info.flowcell().lane_count(), expected_run_info.flowcell().lane_count());
    EXPECT_EQ(run_info.flowcell().surface_count(), expected_run_info.flowcell().surface_count());
    EXPECT_EQ(run_info.flowcell().swath_count(), expected_run_info.flowcell().swath_count());
    EXPECT_EQ(run_info.flowcell().tile_count(), expected_run_info.flowcell().tile_count());
    EXPECT_EQ(run_info.flowcell().sections_per_lane(), expected_run_info.flowcell().sections_per_lane());
    EXPECT_EQ(run_info.flowcell().lanes_per_section(), expected_run_info.flowcell().lanes_per_section());
    EXPECT_EQ(run_info.flowcell().naming_method(), expected_run_info.flowcell().naming_method());
    EXPECT_EQ(run_info.dimensions_of_image().width(), expected_run_info.dimensions_of_image().width());
    EXPECT_EQ(run_info.dimensions_of_image().height(), expected_run_info.dimensions_of_image().height());
    EXPECT_EQ(run_info.channels().size(), expected_run_info.channels().size());

    for(size_t i=0;i<std::min(run_info.channels().size(), expected_run_info.channels().size());++i)
            EXPECT_EQ(run_info.channels()[i], expected_run_info.channels()[i]);
    EXPECT_EQ(run_info.reads().size(), expected_run_info.reads().size());
    for(size_t i=0;i<std::min(run_info.reads().size(), expected_run_info.reads().size());++i)
    {
        EXPECT_EQ(run_info.reads()[i].number(), expected_run_info.reads()[i].number());
        EXPECT_EQ(run_info.reads()[i].is_index(), expected_run_info.reads()[i].is_index());
        EXPECT_EQ(run_info.reads()[i].first_cycle(), expected_run_info.reads()[i].first_cycle());
        EXPECT_EQ(run_info.reads()[i].last_cycle(), expected_run_info.reads()[i].last_cycle());
    }
    EXPECT_EQ(run_info.flowcell().tiles().size(), expected_run_info.flowcell().tiles().size());
    for(size_t i=0;i<std::min(run_info.flowcell().tiles().size(), expected_run_info.flowcell().tiles().size());++i)
    {
        EXPECT_EQ(run_info.flowcell().tiles()[i], expected_run_info.flowcell().tiles()[i]);
    }
}
/**120705_11618Unbin1R2I
 * @class illumina::interop::model::run::info
 * @test Confirm MiSeq version of the run info XML format is properly parsed
 *
 */
TEST(RunInfo, ParseXML_MiSeq)
{
    std::string xml_file = "<?xml version=\"1.0\"?>\n"
            "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"2\">\n"
            "  <Run Id=\"120705_M00903_0009_A000000000-A12V4\" Number=\"8\">\n"
            "    <Flowcell>000000000-A12V4</Flowcell>\n"
            "    <Instrument>M00903</Instrument>\n"
            "    <Date>120705</Date>\n"
            "    <Reads>\n"
            "      <Read NumCycles=\"251\" Number=\"1\" IsIndexedRead=\"N\" />\n"
            "      <Read NumCycles=\"12\" Number=\"2\" IsIndexedRead=\"Y\" />\n"
            "      <Read NumCycles=\"251\" Number=\"3\" IsIndexedRead=\"N\" />\n"
            "    </Reads>\n"
            "    <FlowcellLayout LaneCount=\"1\" SurfaceCount=\"2\" SwathCount=\"1\" TileCount=\"14\" />\n"
            "  </Run>\n"
            "</RunInfo>";

    //std::string Tiles[] = {};
    const std::vector<std::string> Tiles;
    //std::string ImageChannels[] = {};
    const std::vector<std::string> ImageChannels;
    const run::read_info Reads[] = {
                  /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
            run::read_info(1,         1,       251,          false),
            run::read_info(2,         252,      263,          true),
            run::read_info(3,         264,      514,          false)
    };
    const constants::tile_naming_method TileNamingConvention= constants::UnknownTileNamingMethod;
    const size_t LaneCount = 1;
    const size_t SurfaceCount = 2;
    const size_t SwathCount = 1;
    const size_t TileCount=14;
    const size_t SectionPerLane = 1;
    const size_t LanePerSection = 1;
    const run::flowcell_layout FlowcellLayout(LaneCount,
                                        SurfaceCount,
                                        SwathCount,
                                        TileCount,
                                        SectionPerLane,
                                        LanePerSection,
                                        Tiles,
                                        TileNamingConvention);

    const std::string Run_Id = "120705_M00903_0009_A000000000-A12V4";
    const std::string Date = "120705";
    const run::info::uint_t version = 2;
    const run::image_dimensions ImageDimensions(/*Width*/ 0, /*Height*/ 0);
    const run::info expected_run_info(Run_Id,
                                Date ,
                                version,
                                FlowcellLayout,
                                ImageChannels,
                                ImageDimensions,
                                to_vector(Reads));

    test_helper_run_info_parse_xml(expected_run_info, xml_file);
}

/**131212_221Bin1R0I
 * @class illumina::interop::model::run::info
 * @test Confirm HiSeq version of the run info XML format is properly parsed
 *
 */
TEST(RunInfo, ParseXML_HiSeq)
{
    std::string xml_file = "<?xml version=\"1.0\"?>\n"
            "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"2\">\n"
            "  <Run Id=\"131212_1270_0870_A02D224\" Number=\"870\">\n"
            "    <Flowcell>02D224DRR</Flowcell>\n"
            "    <Instrument>1270</Instrument>\n"
            "    <Date>131212</Date>\n"
            "    <Reads>\n"
            "      <Read Number=\"1\" NumCycles=\"30\" IsIndexedRead=\"N\" />\n"
            "    </Reads>\n"
            "    <FlowcellLayout LaneCount=\"8\" SurfaceCount=\"2\" SwathCount=\"2\" TileCount=\"24\">\n"
            "      <TileSet TileNamingConvention=\"FourDigit\">\n"
            "        <Tiles>\n"
            "          <Tile>1_1203</Tile>\n"
            "\t</Tiles>\n"
            "      </TileSet>\n"
            "    </FlowcellLayout>\n"
            "    <ImageDimensions Width=\"3188\" Height=\"7244\" />\n"
            "    <ImageChannels>\n"
            "      <Name>a</Name>\n"
            "      <Name>c</Name>\n"
            "      <Name>g</Name>\n"
            "      <Name>t</Name>\n"
            "    </ImageChannels>\n"
            "    <AlignToPhiX>\n"
            "      <Lane>1</Lane>\n"
            "      <Lane>2</Lane>\n"
            "      <Lane>3</Lane>\n"
            "      <Lane>4</Lane>\n"
            "      <Lane>5</Lane>\n"
            "      <Lane>6</Lane>\n"
            "      <Lane>7</Lane>\n"
            "      <Lane>8</Lane>\n"
            "    </AlignToPhiX>\n"
            "  </Run>\n"
            "</RunInfo>";



    const std::string Tiles[] = {"1_1203"};
    const std::string ImageChannels[] = {"a", "c", "g", "t"};
    const run::read_info Reads[] = {
            /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
            run::read_info(1,          1,       30,          false)
    };
    const constants::tile_naming_method TileNamingConvention= constants::FourDigit;
    const size_t LaneCount = 8;
    const size_t SurfaceCount = 2;
    const size_t SwathCount = 2;
    const size_t TileCount=24;
    const size_t SectionPerLane = 1;
    const size_t LanePerSection = 1;
    const run::flowcell_layout FlowcellLayout(LaneCount,
                                        SurfaceCount,
                                        SwathCount,
                                        TileCount,
                                        SectionPerLane,
                                        LanePerSection,
                                        to_vector(Tiles),
                                        TileNamingConvention);

    const std::string Run_Id = "131212_1270_0870_A02D224";
    const std::string Date = "131212";
    const run::info::uint_t version = 2;
    const run::image_dimensions ImageDimensions(/*Width*/ 3188, /*Height*/ 7244);
    const run::info expected_run_info(Run_Id,
                                Date ,
                                version,
                                FlowcellLayout,
                                to_vector(ImageChannels),
                                ImageDimensions,
                                to_vector(Reads));

    test_helper_run_info_parse_xml(expected_run_info, xml_file);
}

/**6471489_213Bin1R0I
 * @class illumina::interop::model::run::info
 * @test Confirm NexSeq version of the run info XML format is properly parsed
 *
 */
TEST(RunInfo, ParseXML_NextSeq)
{
    std::string xml_file = "<?xml version=\"1.0\"?>\n"
            "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"2\">\n"
            "  <Run Id=\"140502_NS500128_0072_AH0MNBAGXX_TsCancer_1.6pM\" Number=\"72\">\n"
            "    <Flowcell>XXXXXXXXX</Flowcell>\n"
            "    <Instrument>NS500128</Instrument>\n"
            "    <Date>140502</Date>\n"
            "    <Reads>\n"
            "      <Read Number=\"1\" NumCycles=\"75\" IsIndexedRead=\"N\" />\n"
            "    </Reads>\n"
            "    <FlowcellLayout LaneCount=\"4\" SurfaceCount=\"2\" SwathCount=\"3\" TileCount=\"12\" SectionPerLane=\"3\" LanePerSection=\"2\" />\n"
            "  </Run>\n"
            "</RunInfo>";



    //std::string Tiles[] = {};
    const std::vector<std::string> Tiles;
    //std::string ImageChannels[] = {};
    const std::vector<std::string> ImageChannels;
    const run::read_info Reads[] = {
            /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
            run::read_info(1,          1,       75,          false)
    };
    const constants::tile_naming_method TileNamingConvention= constants::UnknownTileNamingMethod;
    const size_t LaneCount = 4;
    const size_t SurfaceCount = 2;
    const size_t SwathCount = 3;
    const size_t TileCount=12;
    const size_t SectionPerLane = 3;
    const size_t LanePerSection = 2;
    const run::flowcell_layout FlowcellLayout(LaneCount,
                                        SurfaceCount,
                                        SwathCount,
                                        TileCount,
                                        SectionPerLane,
                                        LanePerSection,
                                        Tiles,
                                        TileNamingConvention);

    const std::string Run_Id = "140502_NS500128_0072_AH0MNBAGXX_TsCancer_1.6pM";
    const std::string Date = "140502";
    const run::info::uint_t version = 2;
    const run::image_dimensions ImageDimensions(/*Width*/ 0, /*Height*/ 0);
    const run::info expected_run_info(Run_Id,
                                Date ,
                                version,
                                FlowcellLayout,
                                ImageChannels,
                                ImageDimensions,
                                to_vector(Reads));

    test_helper_run_info_parse_xml(expected_run_info, xml_file);
}


