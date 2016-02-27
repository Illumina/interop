using System;
using NUnit.Framework;
using System.IO;

namespace illumina.interop.csharp.unittest
{
	/// <summary>
	/// Confirm that the extraction metrics InterOp works properly in C#
	/// </summary>
	[TestFixture]
	public class ExtractionMetricsTestV2
	{
		const int Version = 2;
		extraction_metrics expected_metric_set;
		extraction_metrics actual_metric_set = new extraction_metrics();
		vector_extraction_metrics expected_metrics = new vector_extraction_metrics();
		vector_extraction_metrics actual_metrics;
		byte[] expected_binary_data;

		/// <summary>
		/// Sets up the expected and actual metric sets for the test
		/// </summary>
		[SetUp]
		protected void SetUp()
		{
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
			expected_metric_set = new extraction_metrics(expected_metrics, Version);
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
				Assert.AreEqual(expected_metrics[i].date_time_csharp().value, actual_metrics[i].date_time_csharp().value);
				for(uint j=0;j<Math.Min(expected_metrics[i].channel_count(), actual_metrics[i].channel_count());j++)
				{
				    Assert.AreEqual(expected_metrics[i].max_intensity(j), actual_metrics[i].max_intensity(j));
				    Assert.AreEqual(expected_metrics[i].focusScore(j), actual_metrics[i].focusScore(j));
				}
			}
		}
	}


	/// <summary>
	/// Test the performance of tabulating a large number of extraction metrics
	/// </summary>
	[TestFixture]
	public class ExtractionMetricsPerformanceTestV2
	{
		const int Version = 2;
		extraction_metrics extraction_metric_set;
		vector_extraction_metrics metrics = new vector_extraction_metrics();
		/// <summary>
		/// Build a large extraction metric set
		/// </summary>
		[SetUp]
		protected void SetUp()
		{

		    if(metrics.Count == 0)
		    {
                System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
                timer.Start();
                float[] focus1 = new float[]{2.24664021f, 2.1896739f, 0, 0};
                ushort[] p90_1  = new ushort[]{302, 273, 0, 0};
                for(uint lane = 1;lane <=8;lane++)
                {
                    for(uint tile = 1;tile <=2000;tile++)
                    {
                        for(uint cycle = 1;cycle <=318;cycle++)
                        {
                            metrics.Add(new extraction_metric(lane, tile, cycle, new csharp_date_time(9859129975844165472ul), (p90_1), (focus1), 4));
                        }
                    }
                }
                extraction_metric_set = new extraction_metrics(metrics, Version);
                timer.Stop();
                System.Console.WriteLine("Setup: " + timer.Elapsed.Hours +" : " + timer.Elapsed.Minutes +" : " + timer.Elapsed.Seconds);
                System.Console.WriteLine("Size: " + metrics.Count + " - " + extraction_metric_set.size());
		    }
		}
		/// <summary>
		/// Test performance of getting the focus values
		/// </summary>
		[Test]
		public void Test_At()
		{
		    System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
		    timer.Start();
		    double sum = 0.0;
		    for(uint i=0;i<extraction_metric_set.size();i++)
		        sum += extraction_metric_set.at(i).focusScore(0);
		    timer.Stop();
		    System.Console.WriteLine("At - Sum focus: " + timer.Elapsed.Hours +" : " + timer.Elapsed.Minutes +" : " + timer.Elapsed.Seconds);
		}
		/// <summary>
		/// Test performance of getting the focus values
		/// </summary>
		[Test]
		public void Test_GetMetric()
		{
		    System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
		    timer.Start();
		    double sum = 0.0;
		    for(int lane = 1;lane <=8;lane++)
		    {
		        for(int tile = 1;tile <=2000;tile++)
		        {
		            for(int cycle = 1;cycle <=318;cycle++)
		            {

		                extraction_metric metric = extraction_metric_set.GetMetric(lane, tile, cycle);
		                sum += metric.focusScore(0);
		            }
                }
            }
		    timer.Stop();
		    System.Console.WriteLine("GetMetric - Sum focus: " + timer.Elapsed.Hours +" : " + timer.Elapsed.Minutes +" : " + timer.Elapsed.Seconds);
		}
		/// <summary>
		/// Test performance of getting the focus values
		/// </summary>
		[Test]
		public void Test_CopyFocus()
		{
		    System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
		    timer.Start();
		    float[] focusVals = new float[extraction_metric_set.size()];
            extraction_metric_set.copy_focus(focusVals, 0, (uint)focusVals.Length);
		    double sum = 0.0;
		    for(int i=0;i<focusVals.Length;i++) sum+=focusVals[i];
		    timer.Stop();
		    System.Console.WriteLine("CopyFocus - Sum focus: " + timer.Elapsed.Hours +" : " + timer.Elapsed.Minutes +" : " + timer.Elapsed.Seconds);
		}
	}

}
