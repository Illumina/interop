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
	/// Test plotting q-score heat map
	/// </summary>
	[TestFixture]
	public class PlotQScoreHeatmap
	{
		/// <summary>
		/// Test plotting q-score heat map
		/// </summary>
		[Test]
		public void Heatmap()
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

            filter_options options = new filter_options(tile_naming_method.FourDigit);
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

            uint row_count = c_csharp_plot.count_rows_for_heatmap(run);
            uint col_count = c_csharp_plot.count_columns_for_heatmap(run);
            var buffer = new float[row_count*col_count];
            heatmap_data data = new heatmap_data();
            c_csharp_plot.plot_qscore_heatmap(run, options, data, buffer);
            Assert.AreEqual(data.row_count(), 3);
            Assert.AreEqual(data.column_count(), 40);
            heatmap_data data2 = new heatmap_data();
            c_csharp_plot.plot_qscore_heatmap(run, options, data2);
            for(uint row=0;row<data.row_count();row++)
            {
                for(uint col=0;col<data.column_count();col++)
                {
                    Assert.AreEqual(buffer[row*data.column_count()+col], data2.at(row,col));
                }
            }

		}
	}

}
