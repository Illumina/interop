using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Metrics;

namespace Illumina.InterOp.UnitTest
{
	/// <summary>
	/// Test the performance of tabulating a large number of extraction metrics
	/// </summary>
	[TestFixture]
	public class ExtractionMetricsPerformanceTestV2
	{
		const int Version = 2;
		base_extraction_metrics extraction_metric_set;
		vector_extraction_metrics metrics = new vector_extraction_metrics();
		const int TileCount=500;
		/// <summary>
		/// Build a large extraction metric set
		/// </summary>
		[SetUp]
		protected void SetUp()
		{
	        extraction_metric_header header = new extraction_metric_header(2);
		    if(metrics.Count == 0)
		    {
                System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
                timer.Start();
                float[] focus1 = new float[]{2.24664021f, 2.1896739f, 0, 0};
                ushort[] p90_1  = new ushort[]{302, 273, 0, 0};
                for(uint lane = 1;lane <=8;lane++)
                {
                    for(uint tile = 1;tile <=TileCount;tile++)
                    {
                        for(uint cycle = 1;cycle <=318;cycle++)
                        {
                            metrics.Add(new extraction_metric(lane, tile, cycle, new csharp_date_time(9859129975844165472ul), (p90_1), (focus1), 4));
                        }
                    }
                }
                extraction_metric_set = new base_extraction_metrics(metrics, Version, header);
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
		        sum += extraction_metric_set.at(i).focus_score(0);
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
		        for(int tile = 1;tile <=TileCount;tile++)
		        {
		            for(int cycle = 1;cycle <=318;cycle++)
		            {
		                extraction_metric metric = extraction_metric_set.get_metric((uint)lane, (uint)tile, (uint)cycle);
		                sum += metric.focus_score(0);
		            }
                }
            }
		    timer.Stop();
		    System.Console.WriteLine("GetMetric - Sum focus: " + timer.Elapsed.Hours +" : " + timer.Elapsed.Minutes +" : " + timer.Elapsed.Seconds);
		}
		/// <summary>
		/// Test performance of getting the focus values. This method is at least 20x faster than the above
		/// </summary>
		[Test]
		public void Test_CopyFocus()
		{
		    System.Diagnostics.Stopwatch timer = new System.Diagnostics.Stopwatch();
		    timer.Start();
		    float[] focusVals = new float[extraction_metric_set.size()];
            c_csharp_metrics.copy_focus(extraction_metric_set, focusVals, 0, (uint)focusVals.Length);
		    double sum = 0.0;
		    for(int i=0;i<focusVals.Length;i++) sum+=focusVals[i];
		    timer.Stop();
		    System.Console.WriteLine("CopyFocus - Sum focus: " + timer.Elapsed.Hours +" : " + timer.Elapsed.Minutes +" : " + timer.Elapsed.Seconds);
		}
	}

}
