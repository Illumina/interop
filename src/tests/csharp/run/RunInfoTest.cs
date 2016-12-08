using System;
using NUnit.Framework;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using Illumina.InterOp.Run;

namespace illumina.interop.csharp.unittest
{
	/// <summary>
	/// Confirm that the run info XML parsing works properly in C#
	/// </summary>
	///
	[TestFixture]
	public abstract class AbstractRunInfoTest
	{
		/// <summary>
		/// The expected run info
		/// </summary>
		protected info expected_run_info;
		/// <summary>
		/// The actual run info
		/// </summary>
		protected info run_info = new info();
		/// <summary>
		/// The setup should be overridden by the specific configuration file
		/// </summary>
	    [SetUp]
		protected abstract void SetUp();

		/// <summary>
		/// Confirms that reading the hard coded binary data matches the expected values of the metric
		/// </summary>
	    [Test]
	    public void CompareRunInfo()
	    {
	        Assert.AreEqual(run_info.name(), expected_run_info.name());
            Assert.AreEqual(run_info.date(), expected_run_info.date());
            Assert.AreEqual(run_info.version(), expected_run_info.version());
            Assert.AreEqual(run_info.flowcell().lane_count(), expected_run_info.flowcell().lane_count());
            Assert.AreEqual(run_info.flowcell().surface_count(), expected_run_info.flowcell().surface_count());
            Assert.AreEqual(run_info.flowcell().swath_count(), expected_run_info.flowcell().swath_count());
            Assert.AreEqual(run_info.flowcell().tile_count(), expected_run_info.flowcell().tile_count());
            Assert.AreEqual(run_info.flowcell().sections_per_lane(), expected_run_info.flowcell().sections_per_lane());
            Assert.AreEqual(run_info.flowcell().lanes_per_section(), expected_run_info.flowcell().lanes_per_section());
            Assert.AreEqual(run_info.flowcell().naming_method(), expected_run_info.flowcell().naming_method());
            Assert.AreEqual(run_info.dimensions_of_image().width(), expected_run_info.dimensions_of_image().width());
            Assert.AreEqual(run_info.dimensions_of_image().height(), expected_run_info.dimensions_of_image().height());
            Assert.AreEqual(run_info.channels().Count, expected_run_info.channels().Count);

            for(int i=0;i<(int)Math.Min(run_info.channels().Count, expected_run_info.channels().Count);++i)
                    Assert.AreEqual(run_info.channels()[i], expected_run_info.channels()[i]);
            Assert.AreEqual(run_info.reads().Count, expected_run_info.reads().Count);
            for(int i=0;i<Math.Min(run_info.reads().Count, expected_run_info.reads().Count);++i)
            {
                Assert.AreEqual(run_info.reads()[i].number(), expected_run_info.reads()[i].number());
                Assert.AreEqual(run_info.reads()[i].is_index(), expected_run_info.reads()[i].is_index());
                Assert.AreEqual(run_info.reads()[i].first_cycle(), expected_run_info.reads()[i].first_cycle());
                Assert.AreEqual(run_info.reads()[i].last_cycle(), expected_run_info.reads()[i].last_cycle());
            }
            Assert.AreEqual(run_info.flowcell().tiles().Count, expected_run_info.flowcell().tiles().Count);
            for(int i=0;i<Math.Min(run_info.flowcell().tiles().Count, expected_run_info.flowcell().tiles().Count);++i)
            {
                Assert.AreEqual(run_info.flowcell().tiles()[i], expected_run_info.flowcell().tiles()[i]);
            }
    }
	}
	/// <summary>
	/// Confirm that the run info XML parsing for MiSeq works properly in C#
	/// </summary>
	///
	[TestFixture]
	public class RunInfoMiSeqTest : AbstractRunInfoTest
	{
		/// <summary>
		/// The setup should be overridden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{
            string xml_file = "<?xml version=\"1.0\"?>\n"+
                    "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"2\">\n"+
                    "  <Run Id=\"120705_M00903_0009_A000000000-A12V4\" Number=\"8\">\n"+
                    "    <Flowcell>000000000-A12V4</Flowcell>\n"+
                    "    <Instrument>M00903</Instrument>\n"+
                    "    <Date>120705</Date>\n"+
                    "    <Reads>\n"+
                    "      <Read NumCycles=\"251\" Number=\"1\" IsIndexedRead=\"N\" />\n"+
                    "      <Read NumCycles=\"12\" Number=\"2\" IsIndexedRead=\"Y\" />\n"+
                    "      <Read NumCycles=\"251\" Number=\"3\" IsIndexedRead=\"N\" />\n"+
                    "    </Reads>\n"+
                    "    <FlowcellLayout LaneCount=\"1\" SurfaceCount=\"2\" SwathCount=\"1\" TileCount=\"14\" />\n"+
                    "  </Run>\n"+
                    "</RunInfo>";

            string_vector Tiles = new string_vector();
            string_vector ImageChannels = new string_vector();
            read_info[] ReadsArr = new read_info[]{
                          /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
                          new read_info(1,          1,        251,          false),
                          new read_info(2,         252,      263,          true),
                          new read_info(3,         264,      514,          false)
            };
            read_info_vector Reads = new read_info_vector();
            for(int i=0;i<ReadsArr.Length;i++) Reads.Add(ReadsArr[i]);
            tile_naming_method TileNamingConvention=tile_naming_method.UnknownTileNamingMethod;
            uint LaneCount = 1;
            uint SurfaceCount = 2;
            uint SwathCount = 1;
            uint TileCount=14;
            uint SectionPerLane = 1;
            uint LanePerSection = 1;
            flowcell_layout FlowcellLayout = new flowcell_layout(
                                                LaneCount,
                                                SurfaceCount,
                                                SwathCount,
                                                TileCount,
                                                SectionPerLane,
                                                LanePerSection,
                                                Tiles,
                                                TileNamingConvention);

            string Run_Id = "120705_M00903_0009_A000000000-A12V4";
            string Date = "120705";
            uint version = 2;
            image_dimensions ImageDimensions = new image_dimensions(/*Width*/ 0, /*Height*/ 0);
            expected_run_info = new info(Run_Id,
                                        Date,
                                        version,
                                        FlowcellLayout,
                                        ImageChannels,
                                        ImageDimensions,
                                        Reads);
            run_info.parse(xml_file);

		}
	}
}
