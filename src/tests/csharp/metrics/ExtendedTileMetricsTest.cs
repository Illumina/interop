using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Comm;

namespace illumina.interop.csharp.unittest
{
	/// <summary>
	/// Confirm that the Extended Tile metrics InterOp works properly in C#
	/// </summary>
	public class ExtendedTileMetricsTestV2
	{
		const int Version = 2;
		base_extended_tile_metrics expected_metric_set;
		base_extended_tile_metrics actual_metric_set = new base_extended_tile_metrics();
		vector_extended_tile_metrics expected_metrics = new vector_extended_tile_metrics();
		byte[] expected_binary_data;

		/// <summary>
		/// Sets up the expected and actual metric sets for the test
		/// </summary>
		[SetUp]
		protected void SetUp()
		{
			expected_metrics.Add(new extended_tile_metric(7, 111014, 6309736));
			expected_metrics.Add(new extended_tile_metric(7, 121014, 6260919));
			expected_metrics.Add(new extended_tile_metric(7, 211014, 6232864));

			int[] tmp = new int[]{
                2,10,7,0,-90,-79,1,0,-48,-114,-64,74,7,0,-74,-40,1,0,110,17,-65,74,7,0,70,56,3,0,64,54,-66,74
			};
			expected_binary_data = new byte[tmp.Length];
			for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
			expected_metric_set = new base_extended_tile_metrics(expected_metrics, Version, new base_metric_header());
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
				Assert.AreEqual(expected_metric_set.at(i).cluster_count_occupied(), actual_metric_set.at(i).cluster_count_occupied(), 1e-7);
			}
		}
	}

}
