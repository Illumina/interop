using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Plot;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test plotting q-score flowcell map
	/// </summary>
	[TestFixture]
	public class FlowcellMap
	{
		/// <summary>
		/// Test plotting q-score flowcell map
		/// </summary>
		[Test]
		public void PlotFlowcellMap()
		{
			int[] tmp = new int[]{
                6,34,1,7,2,10,20,25,30,35,40,9,19,24,29,34,39,40,2,14,21,27,32,36,40
                ,7,0,90,4,1,0,0,0,0,0,-70,22,4,0,-81,-49,1,0,-68,16,0,0,78,-86,42,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,2,0,0,0,0,0,75,-81,3,0,-96,-81,0,0,76,4,0,0,112,62,44,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,3,0,0,0,0,0,-80,60,3,0,-74,-46,0,0,-85,1,0,0,-106,-112,44,0,0,0,0,0,0,0,0,0
			};
            byte[] expected_binary_data = new byte[tmp.Length];
            for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
            run_metrics run = new run_metrics();
            c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, run.q_metric_set());
            uint ALL_IDS = (uint)filter_options.UseAll.ALL_IDS;
            filter_options options = new filter_options(tile_naming_method.FourDigit, ALL_IDS, 0, dna_bases.A, ALL_IDS, 1, 1);
            read_info_vector reads = new read_info_vector();
            reads.Add(new read_info(1, 1, 26));
            reads.Add(new read_info(2, 27, 76));
            run.run_info(new info(
                    "",
                    "",
                    1,
                    new flowcell_layout(8, 2, 2, 36, 1, 1, new string_vector(), tile_naming_method.FourDigit),
                    new string_vector(),
                    new image_dimensions(),
                    reads
            ));
            run.legacy_channel_update(instrument_type.HiSeq);
            run.finalize_after_load();

            var layout = run.run_info().flowcell();
            uint lane_count = layout.lane_count();
            uint swath_count = layout.total_swaths(layout.surface_count() > 1 && !options.is_specific_surface());
            uint tile_count = layout.tiles_per_lane();
            float[] data_buffer = new float[lane_count*swath_count*tile_count];
            uint[] tile_buffer  = new uint[lane_count*swath_count*tile_count];
            flowcell_data data = new flowcell_data();
            c_csharp_plot.plot_flowcell_map(run,  metric_type.QScore, options, data, data_buffer, tile_buffer);
            Assert.AreEqual(data.row_count(), 8);
		}
		/// <summary>
		/// Test bad metric name exception
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.invalid_metric_type")]
		public void TestBadMetricException()
		{
			int[] tmp = new int[]{
                6,34,1,7,2,10,20,25,30,35,40,9,19,24,29,34,39,40,2,14,21,27,32,36,40
                ,7,0,90,4,1,0,0,0,0,0,-70,22,4,0,-81,-49,1,0,-68,16,0,0,78,-86,42,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,2,0,0,0,0,0,75,-81,3,0,-96,-81,0,0,76,4,0,0,112,62,44,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,3,0,0,0,0,0,-80,60,3,0,-74,-46,0,0,-85,1,0,0,-106,-112,44,0,0,0,0,0,0,0,0,0
			};
            byte[] expected_binary_data = new byte[tmp.Length];
            for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
            run_metrics run = new run_metrics();
            c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, run.q_metric_set());
            uint ALL_IDS = (uint)filter_options.UseAll.ALL_IDS;
            filter_options options = new filter_options(tile_naming_method.FourDigit, ALL_IDS, 0, dna_bases.A, ALL_IDS, 1, 1);
            read_info_vector reads = new read_info_vector();
            reads.Add(new read_info(1, 1, 26));
            reads.Add(new read_info(2, 27, 76));
            run.run_info(new info(
                    "",
                    "",
                    1,
                    new flowcell_layout(8, 2, 2, 36, 1, 1, new string_vector(), tile_naming_method.FourDigit),
                    new string_vector(),
                    new image_dimensions(),
                    reads
            ));
            run.legacy_channel_update(instrument_type.HiSeq);
            run.finalize_after_load();

            flowcell_data data = new flowcell_data();
            c_csharp_plot.plot_flowcell_map(run, "NoMetric", options, data);
		}
		/// <summary>
		/// Test whether you can load metrics specific to flowcell layout
		/// </summary>
		[Test]
		public void TestListFlowMetrics()
		{
		    var interopsToLoad = new uchar_vector();
		    c_csharp_metrics.list_analysis_metrics_to_load(interopsToLoad);
		}
	}
}

