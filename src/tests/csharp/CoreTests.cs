using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Plot;
using Illumina.InterOp.Table;
using Illumina.InterOp.Comm;
using Illumina.InterOp.Summary;
using Illumina.InterOp.RunMetrics;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test C# Swig Wrapping
	/// </summary>
	[TestFixture]
	public class CoreTests
	{
		/// <summary>
		/// Test Enum parsing
		/// </summary>
		[Test]
		public void ParseMetricTypeEnum()
		{
		    Assert.AreEqual(c_csharp_run.parse_metric_type("Intensity"), metric_type.Intensity);
		}
		/// <summary>
		/// Test Enum parsing
		/// </summary>
		[Test]
		public void ToStringMetricTypeEnum()
		{
		    Assert.AreEqual(c_csharp_run.to_string(metric_type.Intensity), "Intensity");
		}
		/// <summary>
		/// Test Enum parsing
		/// </summary>
		[Test]
		public void ToStringMetricGroupEnum()
		{
		    Assert.AreEqual(c_csharp_run.to_string(metric_group.Error), "Error");
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestHeatMapIndexOutOfBoundsException()
		{
            heatmap_data heatmap = new heatmap_data();
            System.Console.WriteLine(heatmap.at(0,0));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestImagingTableIndexOutOfBoundsException()
		{
            imaging_table table = new imaging_table();
            System.Console.WriteLine(table.at(0u,0u));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestIndexLaneSummaryIndexOutOfBoundsException()
		{
            index_lane_summary summary = new index_lane_summary();
            System.Console.WriteLine(summary.at(0u));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestIndexFlowcellSummaryIndexOutOfBoundsException()
		{
            index_flowcell_summary summary = new index_flowcell_summary();
            System.Console.WriteLine(summary.at(0u));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestRunSummaryIndexOutOfBoundsException()
		{
            run_summary summary = new run_summary();
            System.Console.WriteLine(summary.at(0u));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestReadSummaryIndexOutOfBoundsException()
		{
            read_summary summary = new read_summary();
            System.Console.WriteLine(summary.at(0u));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestLaneSummaryIndexOutOfBoundsException()
		{
            lane_summary summary = new lane_summary();
            System.Console.WriteLine(summary.at(0u));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestTileMetricsIndexOutOfBoundsException()
		{
            base_tile_metrics summary = new base_tile_metrics();
            System.Console.WriteLine(summary.at(0u));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("System.ArgumentOutOfRangeException")]
		public void TestBarVectorIndexOutOfBoundsException()
		{
            bar_vector points = new bar_vector();
            System.Console.WriteLine(points[0]);
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("System.ArgumentOutOfRangeException")]
		public void TestCandleStickVectorIndexOutOfBoundsException()
		{
            candle_stick_vector points = new candle_stick_vector();
            System.Console.WriteLine(points[0]);
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestCandleStickCollectionIndexOutOfBoundsException()
		{
            candle_stick_collection points = new candle_stick_collection();
            System.Console.WriteLine(points.at(0));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestBarCollectionIndexOutOfBoundsException()
		{
            bar_collection points = new bar_collection();
            System.Console.WriteLine(points.at(0));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestCandleStickSeriesIndexOutOfBoundsException()
		{
            candle_stick_series points = new candle_stick_series();
            System.Console.WriteLine(points.at(0));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestBarSeriesIndexOutOfBoundsException()
		{
            bar_series points = new bar_series();
            System.Console.WriteLine(points.at(0));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestCandleStickDataIndexOutOfBoundsException()
		{
            candle_stick_plot_data points = new candle_stick_plot_data();
            System.Console.WriteLine(points.at(0));
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestBarDataIndexOutOfBoundsException()
		{
            bar_plot_data points = new bar_plot_data();
            System.Console.WriteLine(points.at(0));
		}
		[Test]
		public void TestToFeatureForMetricType()
		{
            var metricType = metric_type.Intensity;
            Assert.AreEqual((int)c_csharp_run_metrics.to_feature(metricType), 18);
		}
		[Test]
		public void TestDispose()
		{
            run_metrics tmp = new run_metrics();
            using(var set = tmp.extraction_metric_set())
            {
                ushort_vector tmp1 = new ushort_vector();
                float_vector tmp2 = new float_vector();
                set.insert(new extraction_metric(1,1,1, tmp1, tmp2));
            }
            using(var set = tmp.extraction_metric_set())
            {
                Assert.AreEqual((int)set.size(), 1);
            }
            using(var set = tmp.extraction_metric_set())
            {
                Assert.AreEqual((int)set.at(0).lane(), 1);
            }
		}
		[Test]
		public void TestReadFromBinaryBuffer()
		{
			int[] tmp = new int[]{2, 48, 1, 0, 80, 4, 25, 0, 39, 4, 189, 4, 198, 3, 192, 3, 71, 4, 230, 15, 234, 15, 189, 15, 132,
				15, 0, 0, 0, 0, 65, 168, 10, 0, 93, 93, 8, 0, 104, 95, 8, 0, 238, 221, 9, 0, 91, 34, 63, 65, 1, 0,
				80, 4, 1, 0, 15, 5, 22, 6, 127, 4, 134, 4, 13, 5, 149, 19, 119, 19, 51, 19, 68, 19, 186, 42, 0, 0,
				221, 49, 11, 0, 101, 53, 8, 0, 168, 76, 8, 0, 80, 100, 9, 0, 5, 226, 84, 65, 1, 0, 81, 4, 25, 0, 1,
				4, 147, 4, 164, 3, 144, 3, 45, 4, 91, 15, 91, 15, 83, 15, 38, 15, 0, 0, 0, 0, 171, 201, 10, 0, 153,
				125, 8, 0, 35, 124, 8, 0, 135, 250, 9, 0, 130, 213, 59, 65
			};
	        byte[] expected_binary_data = new byte[tmp.Length];
	        for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
	        run_metrics metrics = new run_metrics();
	        metrics.read_metrics_from_buffer(metric_group.CorrectedInt, expected_binary_data, (uint)expected_binary_data.Length);
	        Assert.AreEqual(metrics.corrected_intensity_metric_set().size(), 3u);
		}
		[Test]
		public void TestWriteToBinaryBuffer()
		{
		    run_metrics metrics = new run_metrics();
		    base_corrected_intensity_metrics expected_metrics = metrics.corrected_intensity_metric_set();

            expected_metrics.set_version(3);
			uint_vector calledCounts1 = new uint_vector(new uint[]{52, 1049523, 654071, 500476, 982989});
			float_vector correctedIntCalled1 = new float_vector(new float[]{245, 252, 61, 235});
			expected_metrics.insert(new corrected_intensity_metric(7, 1114, 1, correctedIntCalled1, (calledCounts1)));
			uint_vector calledCounts2 = new uint_vector(new uint[]{0, 1063708, 582243, 588028, 953132});
			float_vector correctedIntCalled2 = new float_vector(new float[]{232, 257, 68, 228});
			expected_metrics.insert(new corrected_intensity_metric(7, 1114, 2, (correctedIntCalled2), (calledCounts2)));
			uint_vector calledCounts3 = new uint_vector(new uint[]{0, 1022928, 617523, 594836, 951825});
			float_vector correctedIntCalled3 = new float_vector(new float[]{227, 268, 68, 229});
			expected_metrics.insert(new corrected_intensity_metric(7, 1114, 3, (correctedIntCalled3), (calledCounts3)));

		    byte[] expected_binary_data = new byte[metrics.calculate_buffer_size(metric_group.CorrectedInt)];
		    metrics.write_metrics_to_buffer(metric_group.CorrectedInt, expected_binary_data, (uint)expected_binary_data.Length);
		}

	}
}
