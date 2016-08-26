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
	/// Test plotting data by lane
	/// </summary>
	[TestFixture]
	public class PlotDataByLaneTest
	{
		/// <summary>
		/// Test plotting ClusterCountPF by lane
		/// </summary>
		[Test]
		public void IntensityTest()
		{
            int[] tmp = new int[]{2,10
                        ,7,0,90,4,100,0,-67,-66,15,74
                        ,7,0,90,4,102,0,74,122,-59,74
                        ,7,0,90,4,101,0,12,94,-115,73
                        ,7,0,90,4,103,0,16,54,66,74
                        ,7,0,90,4,-56,0,82,-11,80,58
                        ,7,0,90,4,-55,0,-62,42,-99,58
                        ,7,0,90,4,44,1,-102,113,39,64
                        ,7,0,-66,4,100,0,-67,-66,15,74
                        ,7,0,-66,4,102,0,74,122,-59,74
                        ,7,0,-66,4,101,0,46,103,-113,73
                        ,7,0,-66,4,103,0,0,2,69,74
                        ,7,0,-66,4,-56,0,21,111,-87,58
                        ,7,0,-66,4,-55,0,-86,29,-79,58
                        ,7,0,-66,4,44,1,6,-42,39,64
                        ,7,0,66,8,100,0,-67,-66,15,74
                        ,7,0,66,8,102,0,74,122,-59,74
                        ,7,0,66,8,101,0,67,-26,-109,73
                        ,7,0,66,8,103,0,92,47,75,74
                        ,7,0,66,8,-56,0,123,22,-100,58
                        ,7,0,66,8,-55,0,85,6,115,58
                        ,7,0,66,8,44,1,57,97,31,64
            			};
            byte[] expected_binary_data = new byte[tmp.Length];
            for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
            run_metrics run = new run_metrics();
            c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, run.tile_metric_set());

            filter_options options = new filter_options(tile_naming_method.FourDigit);
            read_info_vector reads = new read_info_vector();
            reads.Add(new read_info(1, 1, 26));
            reads.Add(new read_info(2, 27, 76));
            run.run_info(new info(
                    "",
                    "",
                    1,
                    new flowcell_layout(2, 2, 2, 16),
                    new string_vector(),
                    new image_dimensions(),
                    reads
            ));
            run.set_naming_method(tile_naming_method.FourDigit);
            run.legacy_channel_update(instrument_type.HiSeq);

            candle_stick_plot_data data = new candle_stick_plot_data();
            c_csharp_plot.plot_by_lane(run, metric_type.ClusterCountPF, options, data);
            Assert.AreEqual(data.size(), 1);

		}
	}

}
