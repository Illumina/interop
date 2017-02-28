using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.RunMetrics;
using Illumina.InterOp.Summary;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test plotting % reads identified
	/// </summary>
	[TestFixture]
	public class IndexSummaryLogic
	{
		/// <summary>
		/// Test plotting q-score histogram
		/// </summary>
		[Test]
		public void TestLogic()
		{

			int[] tmp1 = new int[]{1
                          ,1,0,74,47,3,0
                          ,17,0,65,84,67,65,67,71,65,67,45,65,65,71,71,84,84,67,65,218,17,0,0,1,0,49,11,0,84,83,67,65,73,110
                          ,100,101,120,101,115
                          ,2,0,74,47,3,0 // changed to lane 2
                          ,17,0,65,67,65,71,84,71,71,84,45,65,65,71,71,84,84,67,65,125,17,0,0,1,0,50,11,0,84,83,67,65,73,110
                          ,100,101,120,101,115
                          ,3,0,74,47,3,0 // changed to lane 3
                          ,17,0,67,65,71,65,84,67,67,65,45,65,65,71,71,84,84,67,65,226,17,0,0,1,0,51,11,0,84,83,67,65,73,110
                          ,100,101,120,101,115
            			};
            int[] tmp2 = new int[]{2,10
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
            byte[] expected_binary_data1 = new byte[tmp1.Length];
            for(int i=0;i<expected_binary_data1.Length;i++) expected_binary_data1[i] = (byte)tmp1[i];
            byte[] expected_binary_data2 = new byte[tmp2.Length];
            for(int i=0;i<expected_binary_data2.Length;i++) expected_binary_data2[i] = (byte)tmp2[i];
            run_metrics run = new run_metrics();
            c_csharp_comm.read_interop_from_buffer(expected_binary_data1, (uint)expected_binary_data1.Length, run.index_metric_set());
            c_csharp_comm.read_interop_from_buffer(expected_binary_data2, (uint)expected_binary_data2.Length, run.tile_metric_set());

            read_info_vector reads = new read_info_vector();
            reads.Add(new read_info(1, 1, 26));
            reads.Add(new read_info(2, 27, 76));
            run.run_info(new info(new flowcell_layout(2, 2, 2, 16),
                    reads
            ));
            run.legacy_channel_update(instrument_type.HiSeq);

            index_flowcell_summary summary = new index_flowcell_summary();

            c_csharp_summary.summarize_index_metrics(run, summary);
            Assert.AreEqual(summary.size(), 2);
            //Assert.AreEqual(summary.at(0).at(0).total_reads(), 2);

		}
	}

}
