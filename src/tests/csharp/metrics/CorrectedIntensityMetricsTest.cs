using System;
using NUnit.Framework;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Confirm that the corrected intensity metrics InterOp works properly in C#
	/// </summary>
	///
	[TestFixture]
	public abstract class AbstractCorrectedIntensityMetricsTest
	{
		/// <summary>
		/// The expected metric set.
		/// </summary>
		protected base_corrected_intensity_metrics expected_metric_set;
		/// <summary>
		/// Vector of the expected metrics
		/// </summary>
		protected vector_corrected_intensity_metrics expected_metrics = new vector_corrected_intensity_metrics();
	    base_corrected_intensity_metrics actual_metric_set = new base_corrected_intensity_metrics();
	    byte[] expected_binary_data;
		/// <summary>
		/// The setup should be overridden by the specific version of the format
		/// </summary>
	    [SetUp]
		protected abstract void SetUp();

		/// <summary>
		/// Setup the expected and actual metric sets
		/// </summary>
		/// <param name="tmp">Hard coded binary data</param>
		/// <param name="version">Version of the format</param>
	    protected void SetupBuffers(int[] tmp, short version)
	    {
	        base_cycle_metric_header header = new base_cycle_metric_header();
	        expected_binary_data = new byte[tmp.Length];
	        for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
			expected_metric_set = new base_corrected_intensity_metrics(expected_metrics, version, header);
	        c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, actual_metric_set);
	        //actual_binary_data = write_metrics(actual_metric_set);
	    }
		/// <summary>
		/// Confirms that reading the hard coded binary data matches the expected values of the metric
		/// </summary>
	    [Test]
	    public void TestHardCodedBinaryData()
	    {
			int Version = expected_metric_set.version ();
	        Assert.AreEqual(expected_metric_set.version(),  actual_metric_set.version());
	        Assert.AreEqual(expected_metric_set.size(),  actual_metric_set.size());

	        for(uint i=0;i<Math.Min((uint)expected_metrics.Count, actual_metric_set.size());i++)
	        {
	            Assert.AreEqual(expected_metric_set.at(i).lane(), actual_metric_set.at(i).lane());
	            Assert.AreEqual(expected_metric_set.at(i).tile(), actual_metric_set.at(i).tile());
	            Assert.AreEqual(expected_metric_set.at(i).cycle(), actual_metric_set.at(i).cycle());
	            if(Version < 3)
	                Assert.AreEqual(expected_metric_set.at(i).averageCycleIntensity(), actual_metric_set.at(i).averageCycleIntensity());
	            if(Version == 2)
	                Assert.AreEqual(expected_metric_set.at(i).signalToNoise(), actual_metric_set.at(i).signalToNoise(), 1e-7);
	            for(int j=-1;j<(int)dna_bases.NUM_OF_BASES;j++)
	            {
	                Assert.AreEqual(expected_metric_set.at(i).calledCounts(j), actual_metric_set.at(i).calledCounts(j));
	            }
	            for(uint j=0;j<(uint)dna_bases.NUM_OF_BASES;j++)
	            {
	                Assert.AreEqual(expected_metric_set.at(i).correctedIntCalled(j), actual_metric_set.at(i).correctedIntCalled(j));
	                if(Version<3)
	                Assert.AreEqual(expected_metric_set.at(i).correctedIntAll(j), actual_metric_set.at(i).correctedIntAll(j));
	            }
	        }
	    }
	}
	/// <summary>
	/// Confirm that the corrected intensity metrics InterOp works properly in C# for Version 2 of the format
	/// </summary>
	///
	[TestFixture]
	public class CorrectedIntensityMetricsTestV2 : AbstractCorrectedIntensityMetricsTest
	{
		const int Version = 2;
		/// <summary>
		/// The setup should be overridden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{

			ushort[] correctedIntAll1 = new ushort[]{1213, 966, 960, 1095};
			ushort[] correctedIntCalled1 = new ushort[]{4070, 4074, 4029, 3972};
			uint[] calledCounts1 = new uint[]{0, 698433, 548189, 548712, 646638};
			expected_metrics.Clear();
			expected_metrics.Add(
				new corrected_intensity_metric(1, 1104, 25, 1063, 11.9458876f, correctedIntCalled1, correctedIntAll1,
					calledCounts1));
			ushort[] correctedIntAll2 = (new ushort[]{1558, 1151, 1158, 1293});
			uint[] calledCounts2 = (new uint[]{10938, 733661, 537957, 543912, 615504});
			ushort[] correctedIntCalled2 = (new ushort[]{5013, 4983, 4915, 4932});
			expected_metrics.Add(
				new corrected_intensity_metric(1, 1104, 1, 1295, 13.3051805f, correctedIntCalled2, correctedIntAll2,
					calledCounts2));
			ushort[] correctedIntAll3 = (new ushort[]{1171, 932, 912, 1069});
			uint[] calledCounts3 = (new uint[]{0, 706987, 556441, 556067, 653959});
			ushort[] correctedIntCalled3 = (new ushort[]{3931, 3931, 3923, 3878});
			expected_metrics.Add(
				new corrected_intensity_metric(1, 1105, 25, 1025, 11.7396259f, correctedIntCalled3, correctedIntAll3,
					calledCounts3));

			int[] tmp = new int[]{2, 48, 1, 0, 80, 4, 25, 0, 39, 4, 189, 4, 198, 3, 192, 3, 71, 4, 230, 15, 234, 15, 189, 15, 132,
				15, 0, 0, 0, 0, 65, 168, 10, 0, 93, 93, 8, 0, 104, 95, 8, 0, 238, 221, 9, 0, 91, 34, 63, 65, 1, 0,
				80, 4, 1, 0, 15, 5, 22, 6, 127, 4, 134, 4, 13, 5, 149, 19, 119, 19, 51, 19, 68, 19, 186, 42, 0, 0,
				221, 49, 11, 0, 101, 53, 8, 0, 168, 76, 8, 0, 80, 100, 9, 0, 5, 226, 84, 65, 1, 0, 81, 4, 25, 0, 1,
				4, 147, 4, 164, 3, 144, 3, 45, 4, 91, 15, 91, 15, 83, 15, 38, 15, 0, 0, 0, 0, 171, 201, 10, 0, 153,
				125, 8, 0, 35, 124, 8, 0, 135, 250, 9, 0, 130, 213, 59, 65
			};
			SetupBuffers (tmp,Version);
		}
	}
	/// <summary>
	/// Confirm that the corrected intensity metrics InterOp works properly in C# for Version 3 of the format
	/// </summary>
	///
	[TestFixture]
	public class CorrectedIntensityMetricsTestV3 : AbstractCorrectedIntensityMetricsTest
	{
		const int Version = 3;
		/// <summary>
		/// The setup should be overridden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{
			uint_vector calledCounts1 = new uint_vector(new uint[]{52, 1049523, 654071, 500476, 982989});
			ushort_vector correctedIntCalled1 = new ushort_vector(new ushort[]{245, 252, 61, 235});
			expected_metrics.Clear();
			expected_metrics.Add(new corrected_intensity_metric(7, 1114, 1, correctedIntCalled1, (calledCounts1)));
			uint_vector calledCounts2 = new uint_vector(new uint[]{0, 1063708, 582243, 588028, 953132});
			ushort_vector correctedIntCalled2 = new ushort_vector(new ushort[]{232, 257, 68, 228});
			expected_metrics.Add(new corrected_intensity_metric(7, 1114, 2, (correctedIntCalled2), (calledCounts2)));
			uint_vector calledCounts3 = new uint_vector(new uint[]{0, 1022928, 617523, 594836, 951825});
			ushort_vector correctedIntCalled3 = new ushort_vector(new ushort[]{227, 268, 68, 229});
			expected_metrics.Add(new corrected_intensity_metric(7, 1114, 3, (correctedIntCalled3), (calledCounts3)));

			int[] tmp = new int[]{3, 34, 7, 0, 90, 4, 1, 0, -11, 0, -4, 0, 61, 0, -21, 0, 52, 0, 0, 0, -77, 3, 16, 0, -9, -6, 9, 0,
				-4, -94, 7, 0, -51, -1, 14, 0, 7, 0, 90, 4, 2, 0, -24, 0, 1, 1, 68, 0, -28, 0, 0, 0, 0, 0, 28, 59,
				16, 0, 99, -30, 8, 0, -4, -8, 8, 0, 44, -117, 14, 0, 7, 0, 90, 4, 3, 0, -29, 0, 12, 1, 68, 0, -27,
				0, 0, 0, 0, 0, -48, -101, 15, 0, 51, 108, 9, 0, -108, 19, 9, 0, 17, -122, 14, 0
			};
			SetupBuffers (tmp,Version);
		}
	}
}
