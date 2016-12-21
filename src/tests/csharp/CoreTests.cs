using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Plot;
using Illumina.InterOp.Table;
using Illumina.InterOp.Comm;
using Illumina.InterOp.Summary;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test C# Swig Wrapping
	/// </summary>
	[TestFixture]
	public class CoreTests
	{
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
	}
}
