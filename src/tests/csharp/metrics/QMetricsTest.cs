using System;
using NUnit.Framework;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Comm;

using QVal = System.Collections.Generic.KeyValuePair<uint, uint>;
namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Confirm that the q-metrics InterOp works properly in C#
	/// </summary>
	///
	[TestFixture]
	public abstract class AbstractQMetricsTest
	{
		/// <summary>
		/// The expected metric set.
		/// </summary>
		protected base_q_metrics expected_metric_set;
		/// <summary>
		/// Vector of the expected metrics
		/// </summary>
		protected vector_q_metrics expected_metrics = new vector_q_metrics();
	    base_q_metrics actual_metric_set = new base_q_metrics();
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
		/// <param name="header">Header for the file format</param>
	    protected void SetupBuffers(int[] tmp, short version, q_score_header header)
	    {
	        expected_binary_data = new byte[tmp.Length];
	        for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
			expected_metric_set = new base_q_metrics(expected_metrics, version, header);
	        c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, actual_metric_set);

	    }
		/// <summary>
		/// Confirms that reading the hard coded binary data matches the expected values of the metric
		/// </summary>
	    [Test]
	    public void TestHardCodedBinaryData()
	    {
	        Assert.AreEqual(expected_metric_set.version(),  actual_metric_set.version());
	        Assert.AreEqual(expected_metric_set.size(),  actual_metric_set.size());

	        Assert.AreEqual(c_csharp_metrics.count_q_metric_bins(expected_metric_set),  c_csharp_metrics.count_q_metric_bins(actual_metric_set));
	        Assert.AreEqual(expected_metric_set.bin_count(),  actual_metric_set.bin_count());

	        for(uint i=0;i<Math.Min(expected_metric_set.size(), actual_metric_set.size());i++)
	        {
	            Assert.AreEqual(expected_metric_set.at(i).lane(), actual_metric_set.at(i).lane());
	            Assert.AreEqual(expected_metric_set.at(i).tile(), actual_metric_set.at(i).tile());
	            Assert.AreEqual(expected_metric_set.at(i).cycle(), actual_metric_set.at(i).cycle());
	            Assert.AreEqual(expected_metric_set.at(i).size(), actual_metric_set.at(i).size());
	            for(uint j=0;j<Math.Min(expected_metric_set.at(i).size(), actual_metric_set.at(i).size());j++)
	            {
	                Assert.AreEqual(expected_metric_set.at(i).qscoreHist(j), actual_metric_set.at(i).qscoreHist(j));
	            }
	        }
	        for(uint i=0;i<Math.Min(expected_metric_set.bin_count(), actual_metric_set.bin_count());i++)
	        {
	            Assert.AreEqual(expected_metric_set.bin_at(i).lower(), actual_metric_set.bin_at(i).lower());
	            Assert.AreEqual(expected_metric_set.bin_at(i).upper(), actual_metric_set.bin_at(i).upper());
	            Assert.AreEqual(expected_metric_set.bin_at(i).value(), actual_metric_set.bin_at(i).value());
	        }
	    }
		/// <summary>
		/// Convert sparse representation of histogram to to dense vector
		/// </summary>
		protected uint[] ToHist(QVal[] hist)
		{
		    uint[] bins = new uint[50];
		    for(int i=0;i<hist.Length;i++) bins[hist[i].Key] = hist[i].Value;
		    return bins;
		}
	}
	/// <summary>
	/// Confirm that the q-metrics InterOp works properly in C# for Version 4 of the format
	/// </summary>
	///
	[TestFixture]
	public class QMetricsTestV4 : AbstractQMetricsTest
	{
		const int Version = 4;
		/// <summary>
		/// The setup should be overridden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{
		    const uint bin_count = 50;
		    QVal[] hist1 = new QVal[]{new QVal(14,21208), new QVal(21,8227), new QVal(26,73051), new QVal(32,2339486)};
		    QVal[] hist2 = new QVal[]{new QVal(14,22647), new QVal(21,9570), new QVal(26,81839), new QVal(32,2413227)};
		    QVal[] hist3 = new QVal[]{new QVal(14,18878), new QVal(21,8168), new QVal(26,72634), new QVal(32,2342292)};

            expected_metrics.Add(new q_metric(1, 1104, 1, ToHist(hist1), bin_count));
            expected_metrics.Add(new q_metric(1, 1106, 1, ToHist(hist2), bin_count));
            expected_metrics.Add(new q_metric(1, 1104, 2, ToHist(hist3), bin_count));

			int[] tmp = new int[]{
                 4,206,
                 1,0,80,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                 ,0,0,0,0,0,216,82,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,91
                 ,29,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,158,178,35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                 ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                 1,0,82,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                 ,0,0,0,0,0,0,0,0,0,0,0,119,88,0,0,0,0,0,0,0,0
                 ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,98,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,63,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                 ,0,0,0,0,0,0,0,171,210,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                 ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                 1,0,80,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                 ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,190,73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                 ,0,232,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,186,27,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,148,189,35,0,0,0,0
                 ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                 ,0,0,0,0,0,0,0
			};
			SetupBuffers (tmp, Version, new q_score_header());
		}
	}
	/// <summary>
	/// Confirm that the q-metrics InterOp works properly in C# for Version 5 of the format
	/// </summary>
	///
	[TestFixture]
	public class QMetricsTestV5 : AbstractQMetricsTest
	{
		const int Version = 5;
		/// <summary>
		/// The setup should be overriden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{
		    const uint bin_count = 7;
		    QVal[] hist1 = new QVal[]{new QVal(1,45272), new QVal(3,33369), new QVal(4,1784241)};
		    QVal[] hist2 = new QVal[]{new QVal(1,45229), new QVal(3,34304), new QVal(4,1792186)};
		    QVal[] hist3 = new QVal[]{new QVal(1,49152), new QVal(3,37440), new QVal(4,1806479)};

            expected_metrics.Add(new q_metric(1, 1103, 1, ToHist(hist1), bin_count));
            expected_metrics.Add(new q_metric(1, 1104, 1, ToHist(hist2), bin_count));
            expected_metrics.Add(new q_metric(1, 1108, 1, ToHist(hist3), bin_count));


            ushort[] lower = new ushort[]{1, 10, 20, 25, 30, 35, 40};
            ushort[] upper = new ushort[]{9, 19, 24, 29, 34, 39, 41};
            ushort[] value = new ushort[]{1, 14, 22, 27, 33, 37, 40};
            q_score_bin_vector headervec = new q_score_bin_vector();
            for(int i=0;i<lower.Length;i++)
                headervec.Add(new q_score_bin(lower[i], upper[i], value[i]));

			int[] tmp = new int[]{
                5,206,
                1,7,
                1,10,20,25,30,35,40,9,19,24,29,34,39,41,1,14,22,27,33,37,40,
                1,0,79,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,216,176,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,89,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,177,57,27,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,
                1,0,80,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,173,176,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,186,88,27,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,
                1,0,84,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,64,146,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,143,144,27,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
			};
			SetupBuffers (tmp, Version, new q_score_header(headervec));
		}
	}
	/// <summary>
	/// Confirm that the q-metrics InterOp works properly in C# for Version 6 of the format
	/// </summary>
	///
	[TestFixture]
	public class QMetricsTestV6 : AbstractQMetricsTest
	{
		const int Version = 6;
		/// <summary>
		/// The setup should be overriden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{
		    uint[] hist1 = new uint[]{0, 267962, 118703, 4284, 2796110, 0, 0};
		    uint[] hist2 = new uint[]{0,241483, 44960, 1100, 2899568, 0 ,0};
		    uint[] hist3 = new uint[]{0,212144, 53942, 427, 2920598, 0, 0};

            expected_metrics.Add(new q_metric(7, 1114, 1, hist1, (uint)hist1.Length));
            expected_metrics.Add(new q_metric(7, 1114, 2, hist2, (uint)hist2.Length));
            expected_metrics.Add(new q_metric(7, 1114, 3, hist3, (uint)hist3.Length));


            ushort[] lower = new ushort[]{2, 10, 20, 25, 30, 35, 40};
            ushort[] upper = new ushort[]{9, 19, 24, 29, 34, 39, 40};
            ushort[] value = new ushort[]{2, 14, 21, 27, 32, 36, 40};
            q_score_bin_vector headervec = new q_score_bin_vector();
            for(int i=0;i<lower.Length;i++)
                headervec.Add(new q_score_bin(lower[i], upper[i], value[i]));

			int[] tmp = new int[]{
                6,34,1,7,2,10,20,25,30,35,40,9,19,24,29,34,39,40,2,14,21,27,32,36,40
                ,7,0,90,4,1,0,0,0,0,0,-70,22,4,0,-81,-49,1,0,-68,16,0,0,78,-86,42,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,2,0,0,0,0,0,75,-81,3,0,-96,-81,0,0,76,4,0,0,112,62,44,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,3,0,0,0,0,0,-80,60,3,0,-74,-46,0,0,-85,1,0,0,-106,-112,44,0,0,0,0,0,0,0,0,0
			};
			SetupBuffers (tmp, Version, new q_score_header(headervec));
		}
	}
}
