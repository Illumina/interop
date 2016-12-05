using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Plot;
using Illumina.InterOp.Table;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test C# Swig Exception Mapping
	/// </summary>
	[TestFixture]
	public class ExceptionTests
	{
		/// <summary>
		/// Test FileNotFoundException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.file_not_found_exception")]
		public void TestFileNotFoundException()
		{
            base_corrected_intensity_metrics metrics = new base_corrected_intensity_metrics();
            c_csharp_comm.read_interop("/NO/FILE/EXISTS", metrics);
		}
		/// <summary>
		/// Test XmlFileNotFoundException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.xml_file_not_found_exception")]
		public void TestXMLFileNotFoundException()
		{
            run_metrics metrics = new run_metrics();
            metrics.read("/nofilecanexist/test/seef");
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.index_out_of_bounds_exception")]
		public void TestIndexOutOfBoundsException()
		{
            base_corrected_intensity_metrics metrics = new base_corrected_intensity_metrics();
            System.Console.WriteLine(metrics.at(0).signal_to_noise());
		}
		/// <summary>
		/// Test invalid_filter_option
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.invalid_filter_option")]
		public void TestInvalidFilterOption()
		{
            run_metrics metrics = new run_metrics();
            filter_options options = new filter_options(tile_naming_method.FourDigit);
            metrics.extraction_metric_set().insert(new extraction_metric(1,1101,1, 0L, new ushort_vector(), new float_vector()));
            candle_stick_plot_data data = new candle_stick_plot_data();
            options.cycle(1);
            c_csharp_plot.plot_by_cycle(metrics, metric_type.Intensity, options, data);
		}
		/// <summary>
		/// Test invalid_metric_type
		/// </summary>
		[Test]
	    [ExpectedException("Illumina.InterOp.Run.invalid_metric_type")]
		public void TestInvalidMetricName()
		{
            run_metrics metrics = new run_metrics();
            filter_options options = new filter_options(tile_naming_method.FourDigit);
            candle_stick_plot_data data = new candle_stick_plot_data();
            c_csharp_plot.plot_by_cycle(metrics, "NoMetric", options, data);
		}
		/// <summary>
		/// Test invalid_metric_type
		/// </summary>
		/*[Test]
	    [ExpectedException("Illumina.InterOp.Run.invalid_column_type")]
		public void TestInvalidColumnType()
		{
            size_vector_2d offsets = new size_vector_2d();
            column_header_vector headers = new column_header_vector();
            headers.Add(new column_header("NoColumn", column_data_type.UnknownDataType));
            c_csharp_table.populate_column_offsets(offsets, headers);
		}*/
	}
}
