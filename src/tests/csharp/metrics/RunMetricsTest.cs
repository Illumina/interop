using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test C# Swig run_metrics Mapping
	/// </summary>
	[TestFixture]
	public class RunMetricsTests
	{
		/// <summary>
		/// Test list_filenames
		/// </summary>
		[Test]
		public void TestListErrorMetricFilenames()
		{
		    run_metrics run = new run_metrics();

            read_info_vector reads = new read_info_vector();
            reads.Add(new read_info(1, 1, 3));
            run.run_info(new info(
                    "",
                    "",
                    1,
                    new flowcell_layout(2, 2, 2, 16),
                    new string_vector(),
                    new image_dimensions(),
                    reads
            ));
            run.set_naming_method(tile_naming_method.FourDigit);
            run.legacy_channel_update(instrument_type.HiSeq);

            string_vector filenames = new string_vector();
            run.list_error_metric_filenames(filenames, "RunFolder");
            Assert.AreEqual(filenames.Count, 1);
            Assert.AreEqual(filenames[0], Path.Combine("RunFolder", "InterOp", "ErrorMetricsOut.bin"));

		}
		[Test]
		public void RunInfoPath()
		{
		    Assert.AreEqual(Path.Combine("RunFolder", "RunInfo.xml"), paths.run_info("RunFolder"));
		}
	}
}


