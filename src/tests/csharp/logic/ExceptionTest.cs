using System;
using NUnit.Framework;
using System.IO;

namespace Illumina.InterOp.UnitTest
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
	    [ExpectedException("file_not_found_exception")]
		public void TestFileNotFoundException()
		{
            corrected_intensity_metrics metrics = new corrected_intensity_metrics();
            c_csharp_interop.read_interop("/nofilecanexist/test/seef", metrics);
		}
		/// <summary>
		/// Test XmlFileNotFoundException
		/// </summary>
		[Test]
	    [ExpectedException("xml_file_not_found_exception")]
		public void TestXMLFileNotFoundException()
		{
            run_metrics metrics = new run_metrics();
            metrics.read("/nofilecanexist/test/seef");
		}
		/// <summary>
		/// Test IndexOutOfBoundsException
		/// </summary>
		[Test]
	    [ExpectedException("index_out_of_bounds_exception")]
		public void TestIndexOutOfBoundsException()
		{
            corrected_intensity_metrics metrics = new corrected_intensity_metrics();
            System.Console.WriteLine(metrics.at(0).signal_to_noise());
		}
	}
}
