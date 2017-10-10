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
#include "interop/util/length_of.h"
#include "interop/constants/enums.h"
#include "interop/model/run/info.h"
#include "src/tests/interop/run/info_test.h"

using namespace illumina::interop::model;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Fixture to list enum values */
template<typename T>
struct run_info_fixture : public ::testing::Test, public T
{
    /** Constructor */
    run_info_fixture()
    {
    }
};
template<typename T>
struct read_fixture
{
    /** Constructor */
    read_fixture()
    {
        T::create_expected(expected_run_info);
        std::string xml_file;
        T::create_string(xml_file);
        run_info.parse(&xml_file[0]);
    }
    run::info expected_run_info;
    run::info run_info;
};
template<typename T>
struct write_read_fixture
{
    /** Constructor */
    write_read_fixture()
    {
        T::create_expected(expected_run_info);
        std::ostringstream fout;
        expected_run_info.write(fout);
        std::string tmp = fout.str();
        run_info.parse(&tmp[0]);
    }
    run::info expected_run_info;
    run::info run_info;
};
typedef ::testing::Types<
        read_fixture<miseq_run_info>,
        read_fixture<hiseq4k_run_info>,
        read_fixture<miniseq_run_info>,
        read_fixture<nextseq_550_run_info>,
        read_fixture<hiseq2500_run_info>,
        read_fixture<novaseq_run_info>,


        write_read_fixture<miseq_run_info>,
        write_read_fixture<hiseq4k_run_info>,
        write_read_fixture<miniseq_run_info>,
        write_read_fixture<nextseq_550_run_info>,
        write_read_fixture<hiseq2500_run_info>,
        write_read_fixture<novaseq_run_info>
> run_info_list;
TYPED_TEST_CASE(run_info_fixture, run_info_list);

/** Confirm that every enum does not conflict with unknown */
TYPED_TEST(run_info_fixture, run_info_test)
{
    EXPECT_EQ(TestFixture::run_info.name(), TestFixture::expected_run_info.name());
    EXPECT_EQ(TestFixture::run_info.date(), TestFixture::expected_run_info.date());
    EXPECT_EQ(TestFixture::run_info.instrument_name(), TestFixture::expected_run_info.instrument_name());
    EXPECT_EQ(TestFixture::run_info.flowcell_id(), TestFixture::expected_run_info.flowcell_id());
    EXPECT_EQ(TestFixture::run_info.run_number(), TestFixture::expected_run_info.run_number());
    EXPECT_EQ(TestFixture::run_info.version(), TestFixture::expected_run_info.version());
    EXPECT_EQ(TestFixture::run_info.flowcell().lane_count(), TestFixture::expected_run_info.flowcell().lane_count());
    EXPECT_EQ(TestFixture::run_info.flowcell().surface_count(), TestFixture::expected_run_info.flowcell().surface_count());
    EXPECT_EQ(TestFixture::run_info.flowcell().swath_count(), TestFixture::expected_run_info.flowcell().swath_count());
    EXPECT_EQ(TestFixture::run_info.flowcell().tile_count(), TestFixture::expected_run_info.flowcell().tile_count());
    EXPECT_EQ(TestFixture::run_info.flowcell().sections_per_lane(), TestFixture::expected_run_info.flowcell().sections_per_lane());
    EXPECT_EQ(TestFixture::run_info.flowcell().lanes_per_section(), TestFixture::expected_run_info.flowcell().lanes_per_section());
    EXPECT_EQ(TestFixture::run_info.flowcell().naming_method(), TestFixture::expected_run_info.flowcell().naming_method());
    EXPECT_EQ(TestFixture::run_info.dimensions_of_image().width(), TestFixture::expected_run_info.dimensions_of_image().width());
    EXPECT_EQ(TestFixture::run_info.dimensions_of_image().height(), TestFixture::expected_run_info.dimensions_of_image().height());
    EXPECT_EQ(TestFixture::run_info.channels().size(), TestFixture::expected_run_info.channels().size());

    for(size_t i=0;i<std::min(TestFixture::run_info.channels().size(), TestFixture::expected_run_info.channels().size());++i)
        EXPECT_EQ(TestFixture::run_info.channels()[i], TestFixture::expected_run_info.channels()[i]);
    EXPECT_EQ(TestFixture::run_info.reads().size(), TestFixture::expected_run_info.reads().size());
    for(size_t i=0;i<std::min(TestFixture::run_info.reads().size(), TestFixture::expected_run_info.reads().size());++i)
    {
        EXPECT_EQ(TestFixture::run_info.reads()[i].number(), TestFixture::expected_run_info.reads()[i].number());
        EXPECT_EQ(TestFixture::run_info.reads()[i].is_index(), TestFixture::expected_run_info.reads()[i].is_index());
        EXPECT_EQ(TestFixture::run_info.reads()[i].first_cycle(), TestFixture::expected_run_info.reads()[i].first_cycle());
        EXPECT_EQ(TestFixture::run_info.reads()[i].last_cycle(), TestFixture::expected_run_info.reads()[i].last_cycle());
    }
    EXPECT_EQ(TestFixture::run_info.flowcell().tiles().size(), TestFixture::expected_run_info.flowcell().tiles().size());
    for(size_t i=0;i<std::min(TestFixture::run_info.flowcell().tiles().size(), TestFixture::expected_run_info.flowcell().tiles().size());++i)
    {
        EXPECT_EQ(TestFixture::run_info.flowcell().tiles()[i], TestFixture::expected_run_info.flowcell().tiles()[i]);
    }
}

TEST(run_info_test, test_invalid_tile_list_exception)
{
    std::string data= "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<RunInfo>\n"
            "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
            "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
            "\t\t<Instrument>XXXXXX</Instrument>\n"
            "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
            "\t\t<Reads>\n"
            "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t</Reads>\n"
            "\t\t<FlowcellLayout LaneCount=\"2\" SurfaceCount=\"2\" SwathCount=\"4\" TileCount=\"88\" FlowcellSide=\"1\">\n"
            "\t\t\t<TileSet TileNamingConvention=\"FourDigit\">\n"
            "\t\t\t\t<Tiles>\n"
            "\t\t\t\t\t<Tile>1_2101</Tile>\n"
            "\t\t\t\t\t<Tile>3_2102</Tile>\n"
            "\t\t\t\t</Tiles>\n"
            "\t\t\t</TileSet>\n"
            "\t\t</FlowcellLayout>\n"
            "\t\t<AlignToPhiX/>\n"
            "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
            "\t\t<ImageChannels>\n"
            "\t\t\t<Name>RED</Name>\n"
            "\t\t\t<Name>GREEN</Name>\n"
            "\t\t</ImageChannels>\n"
            "\t</Run>\n"
            "</RunInfo>";
    run::info run_info;
    run_info.parse(&data[0]);
    EXPECT_THROW(run_info.validate_tiles(), model::invalid_tile_list_exception);
}

TEST(run_info_test, test_invalid_run_number)
{
    std::string
            data= "<?xml version=\"1.0\"?>\n"
            "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"4\">\n"
            "  <Run Id=\"160630_NS500179_0099_AHW3YTBGXX\" Number=\"99\">\n"
            "    <Flowcell>HW3YTBGXX</Flowcell>\n"
            "    <Instrument>NS500179</Instrument>\n"
            "    <Date>160630</Date>\n"
            "    <Reads>\n"
            "      <Read NumCycles=\"101\" IsIndexedRead=\"N\" />\n"
            "    </Reads>\n"
            "    <FlowcellLayout LaneCount=\"4\" SurfaceCount=\"2\" SwathCount=\"3\" TileCount=\"12\" SectionPerLane=\"3\" LanePerSection=\"2\">\n"
            "      <TileSet TileNamingConvention=\"FiveDigit\">\n"
            "        <Tiles>\n"
            "          <Tile>1_11101</Tile>\n"
            "          <Tile>1_21101</Tile>\n"
            "        </Tiles>\n"
            "      </TileSet>\n"
            "    </FlowcellLayout>\n"
            "    <ImageDimensions Width=\"2592\" Height=\"1944\" />\n"
            "    <ImageChannels>\n"
            "      <Name>Red</Name>\n"
            "      <Name>Green</Name>\n"
            "    </ImageChannels>\n"
            "  </Run>\n"
            "</RunInfo>";
    run::info run_info;
    run_info.parse(&data[0]);
    EXPECT_THROW(run_info.validate(), model::invalid_run_info_exception);
}

TEST(run_info_test, test_zero_lane_count_exception)
{
    std::string data= "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<RunInfo>\n"
            "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
            "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
            "\t\t<Instrument>XXXXXX</Instrument>\n"
            "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
            "\t\t<Reads>\n"
            "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t</Reads>\n"
            "\t\t<FlowcellLayout LaneCount=\"\" SurfaceCount=\"2\" SwathCount=\"4\" TileCount=\"88\" FlowcellSide=\"1\">\n"
            "\t\t\t<TileSet TileNamingConvention=\"FourDigit\">\n"
            "\t\t\t\t<Tiles>\n"
            "\t\t\t\t\t<Tile>1_2101</Tile>\n"
            "\t\t\t\t\t<Tile>3_2102</Tile>\n"
            "\t\t\t\t</Tiles>\n"
            "\t\t\t</TileSet>\n"
            "\t\t</FlowcellLayout>\n"
            "\t\t<AlignToPhiX/>\n"
            "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
            "\t\t<ImageChannels>\n"
            "\t\t\t<Name>RED</Name>\n"
            "\t\t\t<Name>GREEN</Name>\n"
            "\t\t</ImageChannels>\n"
            "\t</Run>\n"
            "</RunInfo>";
    run::info run_info;
    run_info.parse(&data[0]);
    EXPECT_THROW(run_info.validate(), model::invalid_run_info_exception);
}

TEST(run_info_test, test_zero_surface_count_exception)
{
    std::string data= "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<RunInfo>\n"
            "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
            "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
            "\t\t<Instrument>XXXXXX</Instrument>\n"
            "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
            "\t\t<Reads>\n"
            "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t</Reads>\n"
            "\t\t<FlowcellLayout LaneCount=\"4\" SurfaceCount=\"\" SwathCount=\"4\" TileCount=\"88\" FlowcellSide=\"1\">\n"
            "\t\t\t<TileSet TileNamingConvention=\"FourDigit\">\n"
            "\t\t\t\t<Tiles>\n"
            "\t\t\t\t\t<Tile>1_2101</Tile>\n"
            "\t\t\t\t\t<Tile>3_2102</Tile>\n"
            "\t\t\t\t</Tiles>\n"
            "\t\t\t</TileSet>\n"
            "\t\t</FlowcellLayout>\n"
            "\t\t<AlignToPhiX/>\n"
            "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
            "\t\t<ImageChannels>\n"
            "\t\t\t<Name>RED</Name>\n"
            "\t\t\t<Name>GREEN</Name>\n"
            "\t\t</ImageChannels>\n"
            "\t</Run>\n"
            "</RunInfo>";
    run::info run_info;
    run_info.parse(&data[0]);
    EXPECT_THROW(run_info.validate(), model::invalid_run_info_exception);
}


TEST(run_info_test, test_zero_swath_count_exception)
{
    std::string data= "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<RunInfo>\n"
            "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
            "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
            "\t\t<Instrument>XXXXXX</Instrument>\n"
            "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
            "\t\t<Reads>\n"
            "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t</Reads>\n"
            "\t\t<FlowcellLayout LaneCount=\"4\" SurfaceCount=\"2\" SwathCount=\"\" TileCount=\"88\" FlowcellSide=\"1\">\n"
            "\t\t\t<TileSet TileNamingConvention=\"FourDigit\">\n"
            "\t\t\t\t<Tiles>\n"
            "\t\t\t\t\t<Tile>1_2101</Tile>\n"
            "\t\t\t\t\t<Tile>3_2102</Tile>\n"
            "\t\t\t\t</Tiles>\n"
            "\t\t\t</TileSet>\n"
            "\t\t</FlowcellLayout>\n"
            "\t\t<AlignToPhiX/>\n"
            "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
            "\t\t<ImageChannels>\n"
            "\t\t\t<Name>RED</Name>\n"
            "\t\t\t<Name>GREEN</Name>\n"
            "\t\t</ImageChannels>\n"
            "\t</Run>\n"
            "</RunInfo>";
    run::info run_info;
    run_info.parse(&data[0]);
    EXPECT_THROW(run_info.validate(), model::invalid_run_info_exception);
}


TEST(run_info_test, test_zero_tile_count_exception)
{
    std::string data= "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<RunInfo>\n"
            "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
            "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
            "\t\t<Instrument>XXXXXX</Instrument>\n"
            "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
            "\t\t<Reads>\n"
            "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t</Reads>\n"
            "\t\t<FlowcellLayout LaneCount=\"4\" SurfaceCount=\"2\" SwathCount=\"4\" TileCount=\"\" FlowcellSide=\"1\">\n"
            "\t\t\t<TileSet TileNamingConvention=\"FourDigit\">\n"
            "\t\t\t\t<Tiles>\n"
            "\t\t\t\t\t<Tile>1_2101</Tile>\n"
            "\t\t\t\t\t<Tile>3_2102</Tile>\n"
            "\t\t\t\t</Tiles>\n"
            "\t\t\t</TileSet>\n"
            "\t\t</FlowcellLayout>\n"
            "\t\t<AlignToPhiX/>\n"
            "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
            "\t\t<ImageChannels>\n"
            "\t\t\t<Name>RED</Name>\n"
            "\t\t\t<Name>GREEN</Name>\n"
            "\t\t</ImageChannels>\n"
            "\t</Run>\n"
            "</RunInfo>";
    run::info run_info;
    run_info.parse(&data[0]);
    EXPECT_THROW(run_info.validate(), model::invalid_run_info_exception);
}

TEST(run_info_test, test_zero_sections_per_lane_count_exception)
{
    std::string data= "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<RunInfo>\n"
            "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
            "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
            "\t\t<Instrument>XXXXXX</Instrument>\n"
            "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
            "\t\t<Reads>\n"
            "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t</Reads>\n"
            "\t\t<FlowcellLayout LaneCount=\"4\" SurfaceCount=\"2\" SwathCount=\"4\" TileCount=\"88\" SectionPerLane=\"\" LanePerSection=\"2\" FlowcellSide=\"1\">\n"
            "\t\t\t<TileSet TileNamingConvention=\"FiveDigit\">\n"
            "\t\t\t\t<Tiles>\n"
            "\t\t\t\t\t<Tile>1_21101</Tile>\n"
            "\t\t\t\t\t<Tile>3_21402</Tile>\n"
            "\t\t\t\t</Tiles>\n"
            "\t\t\t</TileSet>\n"
            "\t\t</FlowcellLayout>\n"
            "\t\t<AlignToPhiX/>\n"
            "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
            "\t\t<ImageChannels>\n"
            "\t\t\t<Name>RED</Name>\n"
            "\t\t\t<Name>GREEN</Name>\n"
            "\t\t</ImageChannels>\n"
            "\t</Run>\n"
            "</RunInfo>";
    run::info run_info;
    run_info.parse(&data[0]);
    EXPECT_THROW(run_info.validate(), model::invalid_run_info_exception);
}

TEST(run_info_test, test_zero_lanes_per_section_count_exception)
{
    std::string data= "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
            "<RunInfo>\n"
            "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
            "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
            "\t\t<Instrument>XXXXXX</Instrument>\n"
            "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
            "\t\t<Reads>\n"
            "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
            "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
            "\t\t</Reads>\n"
            "\t\t<FlowcellLayout LaneCount=\"4\" SurfaceCount=\"2\" SwathCount=\"4\" TileCount=\"88\" SectionPerLane=\"3\" LanePerSection=\"\" FlowcellSide=\"1\">\n"
            "\t\t\t<TileSet TileNamingConvention=\"FiveDigit\">\n"
            "\t\t\t\t<Tiles>\n"
            "\t\t\t\t\t<Tile>1_21101</Tile>\n"
            "\t\t\t\t\t<Tile>3_21402</Tile>\n"
            "\t\t\t\t</Tiles>\n"
            "\t\t\t</TileSet>\n"
            "\t\t</FlowcellLayout>\n"
            "\t\t<AlignToPhiX/>\n"
            "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
            "\t\t<ImageChannels>\n"
            "\t\t\t<Name>RED</Name>\n"
            "\t\t\t<Name>GREEN</Name>\n"
            "\t\t</ImageChannels>\n"
            "\t</Run>\n"
            "</RunInfo>";
    run::info run_info;
    run_info.parse(&data[0]);
    EXPECT_THROW(run_info.validate(), model::invalid_run_info_exception);
}