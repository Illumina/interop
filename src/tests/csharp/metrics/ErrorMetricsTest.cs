using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Confirm that the Error metrics InterOp works properly in C#
	/// </summary>
	[TestFixture]
	public class ErrorMetricsTestV3
	{
		const int Version = 3;
		base_error_metrics expected_metric_set;
		base_error_metrics actual_metric_set = new base_error_metrics();
		vector_error_metrics expected_metrics = new vector_error_metrics();
		byte[] expected_binary_data;

		/// <summary>
		/// Sets up the expected and actual metric sets for the test
		/// </summary>
		[SetUp]
		protected void SetUp()
		{
	        base_cycle_metric_header header = new base_cycle_metric_header();
			expected_metrics.Add(new error_metric(7, 1114, 1, 0.450100899f));
			expected_metrics.Add(new error_metric(7, 1114, 2, 0.900201797f));
			expected_metrics.Add(new error_metric(7, 1114, 3, 0.465621591f));

			int[] tmp = new int[]{3,30,7,0,90,4,1,0,-96,115,-26,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
				,7,0,90,4,2,0,-96,115,102,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
				,7,0,90,4,3,0,-12,101,-18,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			};
			expected_binary_data = new byte[tmp.Length];
			for(int i=0;i<expected_binary_data.Length;i++)
			{
			    expected_binary_data[i] = (byte)tmp[i];
			}
			expected_metric_set = new base_error_metrics(expected_metrics, Version, header);
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
				Assert.AreEqual(expected_metric_set.at(i).cycle(), actual_metric_set.at(i).cycle());
				Assert.AreEqual(expected_metric_set.at(i).errorRate(), actual_metric_set.at(i).errorRate(), 1e-7);
				Assert.AreEqual(expected_metric_set.at(i).mismatch_count(), actual_metric_set.at(i).mismatch_count());
				for(uint j=0;j<expected_metric_set.at(i).mismatch_count();j++)
					Assert.AreEqual(expected_metric_set.at(i).mismatch_cluster_count(j), actual_metric_set.at(i).mismatch_cluster_count(j));
			}
            byte[] newBuffer = new byte[c_csharp_comm.compute_buffer_size(expected_metric_set)];
			c_csharp_comm.write_interop_to_buffer(expected_metric_set, newBuffer, (uint)newBuffer.Length);
			Assert.AreEqual(newBuffer, expected_binary_data);
		}
	}
}
