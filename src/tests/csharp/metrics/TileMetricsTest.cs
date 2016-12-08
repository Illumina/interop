using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Confirm that the tile metrics InterOp works properly in C#
	/// </summary>
	[TestFixture]
	public class TileMetricsTestV2
	{
		const int Version = 2;
		base_tile_metrics expected_metric_set;
		base_tile_metrics actual_metric_set = new base_tile_metrics();
		vector_tile_metrics expected_metrics = new vector_tile_metrics();
		byte[] expected_binary_data;

		/// <summary>
		/// Sets up the expected and actual metric sets for the test
		/// </summary>
		[SetUp]
		protected void SetUp()
		{

		    read_metric_vector read1 = new read_metric_vector();
		    read1.Add(new read_metric(1, 2.61630869f, 0.0797112584f, 0.119908921f));
			expected_metrics.Add(new tile_metric(7, 1114, 2355119.25f,1158081.50f,6470949,3181956, read1));
		    read_metric_vector read2 = new read_metric_vector();
		    read2.Add(new read_metric(1, 2.62243795f, 0.129267812f, 0.135128692f));
			expected_metrics.Add(new tile_metric(7, 1214, 2355119.25f,1174757.75f,6470949,3227776, read2));
		    read_metric_vector read3 = new read_metric_vector();
		    read3.Add(new read_metric(1, 2.490309f, 0.11908555f, 0.092706576f));
			expected_metrics.Add(new tile_metric(7, 2114, 2355119.25f,1211592.38f,6470949,3328983, read3));

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
			expected_binary_data = new byte[tmp.Length];
			for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
			expected_metric_set = new base_tile_metrics(expected_metrics, Version, new base_metric_header());
			c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, actual_metric_set);
		}

		/// <summary>
		/// Confirms that the data was properly parsed and matches the expected model.
		/// This test also confirms that the binding gives the expected results.
		/// </summary>
		[Test]
		public void TestHardCodedBinaryData()
		{
			Assert.AreEqual(expected_metric_set.version(),  actual_metric_set.version());
			Assert.AreEqual(expected_metric_set.size(),  actual_metric_set.size());

			for(uint i=0;i<Math.Min(expected_metric_set.size(), actual_metric_set.size());i++)
			{
				Assert.AreEqual(expected_metric_set.at(i).lane(), actual_metric_set.at(i).lane());
				Assert.AreEqual(expected_metric_set.at(i).tile(), actual_metric_set.at(i).tile());
				Assert.AreEqual(expected_metric_set.at(i).clusterDensity(), actual_metric_set.at(i).clusterDensity());
				Assert.AreEqual(expected_metric_set.at(i).clusterDensityPf(), actual_metric_set.at(i).clusterDensityPf());
				Assert.AreEqual(expected_metric_set.at(i).clusterCount(), actual_metric_set.at(i).clusterCount());
				Assert.AreEqual(expected_metric_set.at(i).clusterCountPf(), actual_metric_set.at(i).clusterCountPf());
				Assert.AreEqual(expected_metric_set.at(i).read_metrics().Count, actual_metric_set.at(i).read_metrics().Count);
				for(int j=0;j<Math.Min(expected_metric_set.at(i).read_metrics().Count, actual_metric_set.at(i).read_metrics().Count);j++)
				{
					Assert.AreEqual(expected_metric_set.at(i).read_metrics()[j].read(), actual_metric_set.at(i).read_metrics()[j].read());
					Assert.AreEqual(expected_metric_set.at(i).read_metrics()[j].percent_aligned(), actual_metric_set.at(i).read_metrics()[j].percent_aligned(), 1e-7);
					Assert.AreEqual(expected_metric_set.at(i).read_metrics()[j].percent_phasing(), actual_metric_set.at(i).read_metrics()[j].percent_phasing(), 1e-7);
					Assert.AreEqual(expected_metric_set.at(i).read_metrics()[j].percent_prephasing(), actual_metric_set.at(i).read_metrics()[j].percent_prephasing(), 1e-7);
                }
			}
		}
	}
}
