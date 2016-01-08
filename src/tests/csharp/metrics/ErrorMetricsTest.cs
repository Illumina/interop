using System;
using NUnit.Framework;
using System.IO;

namespace illumina.interop.csharp.unittest
{
	/// <summary>
	/// Confirm that the Error metrics InterOp works properly in C#
	/// </summary>
	[TestFixture]
	public class ErrorMetricsTestV3
	{
		const int Version = 3;
		error_metrics expected_metric_set;
		error_metrics actual_metric_set = new error_metrics();
		vector_error_metrics expected_metrics = new vector_error_metrics();
		vector_error_metrics actual_metrics;
		byte[] expected_binary_data;

		/// <summary>
		/// Sets up the expected and actual metric sets for the test
		/// </summary>
		[SetUp]
		protected void SetUp()
		{
			expected_metrics.Add(new error_metric(7, 1114, 1, 0.450100899f));
			expected_metrics.Add(new error_metric(7, 1114, 2, 0.900201797f));
			expected_metrics.Add(new error_metric(7, 1114, 3, 0.465621591f));

			int[] tmp = new int[]{3,30,7,0,90,4,1,0,-96,115,-26,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
				,7,0,90,4,2,0,-96,115,102,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
				,7,0,90,4,3,0,-12,101,-18,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			};
			expected_binary_data = new byte[tmp.Length];
			for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
			expected_metric_set = new error_metrics(expected_metrics, Version);
			c_csharp_interop.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, actual_metric_set);
			actual_metrics = actual_metric_set.metrics();
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

			for(int i=0;i<Math.Min(expected_metrics.Count, actual_metrics.Count);i++)
			{
				Assert.AreEqual(expected_metrics[i].lane(), actual_metrics[i].lane());
				Assert.AreEqual(expected_metrics[i].tile(), actual_metrics[i].tile());
				Assert.AreEqual(expected_metrics[i].cycle(), actual_metrics[i].cycle());
				Assert.AreEqual(expected_metrics[i].errorRate(), actual_metrics[i].errorRate(), 1e-7);
				Assert.AreEqual(expected_metrics[i].mismatch_count(), actual_metrics[i].mismatch_count());
				for(uint j=0;j<expected_metrics[i].mismatch_count();j++)
					Assert.AreEqual(expected_metrics[i].mismatch_cluster_count(j), actual_metrics[i].mismatch_cluster_count(j));
			}
		}
	}
}