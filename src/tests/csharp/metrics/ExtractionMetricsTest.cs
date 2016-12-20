using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Confirm that the extraction metrics InterOp works properly in C#
	/// </summary>
	[TestFixture]
	public class ExtractionMetricsTestV2
	{
		const int Version = 2;
		base_extraction_metrics expected_metric_set;
		base_extraction_metrics actual_metric_set = new base_extraction_metrics();
		vector_extraction_metrics expected_metrics = new vector_extraction_metrics();
		byte[] expected_binary_data;

		/// <summary>
		/// Sets up the expected and actual metric sets for the test
		/// </summary>
		[SetUp]
		protected void SetUp()
		{
	        extraction_metric_header header = new extraction_metric_header(2);
            float[] focus1 = new float[]{2.24664021f, 2.1896739f, 0, 0};
            ushort[] p90_1  = new ushort[]{302, 273, 0, 0};
			expected_metrics.Add(new extraction_metric(7, 1114, 1, new csharp_date_time(9859129975844165472ul), (p90_1), (focus1), 4));
			float[] focus2 = new float[]{2.23177338f, 2.20616174f, 0, 0};
            ushort[] p90_2  = new ushort[]{312, 273, 0, 0};
			expected_metrics.Add(new extraction_metric(7, 1214, 1, new csharp_date_time(9859129975872781680ul), (p90_2), (focus2), 4));
			float[] focus3 = new float[]{2.10524225f, 2.14023066f, 0, 0};
            ushort[] p90_3  = new ushort[]{349, 302, 0, 0};
			expected_metrics.Add(new extraction_metric(7, 2114, 1, new csharp_date_time(9859129975901427921ul), (p90_3), (focus3), 4));

			int[] tmp = new int[]{
                2,38
                ,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
                ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
                ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120
			};
			expected_binary_data = new byte[tmp.Length];
			for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
			expected_metric_set = new base_extraction_metrics(expected_metrics, Version, header);
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
				Assert.AreEqual(expected_metric_set.at(i).date_time_csharp().value, actual_metric_set.at(i).date_time_csharp().value);
				for(uint j=0;j<Math.Min(expected_metric_set.at(i).channel_count(), actual_metric_set.at(i).channel_count());j++)
				{
				    Assert.AreEqual(expected_metric_set.at(i).max_intensity(j), actual_metric_set.at(i).max_intensity(j));
				    Assert.AreEqual(expected_metric_set.at(i).focusScore(j), actual_metric_set.at(i).focusScore(j));
				}
			}
		}
	}

}
