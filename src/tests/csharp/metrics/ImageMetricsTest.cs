using System;
using NUnit.Framework;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Confirm that the image metrics InterOp works properly in C#
	/// </summary>
	///
	[TestFixture]
	public abstract class AbstractImageMetricsTest
	{
		/// <summary>
		/// The expected metric set.
		/// </summary>
		protected base_image_metrics expected_metric_set;
		/// <summary>
		/// Vector of the expected metrics
		/// </summary>
		protected vector_image_metrics expected_metrics = new vector_image_metrics();
	    base_image_metrics actual_metric_set = new base_image_metrics();
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
		/// <param name="channelCount">Number of channels</param>
	    protected void SetupBuffers(int[] tmp, short version, ushort channelCount)
	    {
	        expected_binary_data = new byte[tmp.Length];
	        for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
			expected_metric_set = new base_image_metrics(expected_metrics, version, new image_metric_header(channelCount));
	        c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, actual_metric_set);
	        //actual_binary_data = write_metrics(actual_metric_set);
	    }
		/// <summary>
		/// Confirms that reading the hard coded binary data matches the expected values of the metric
		/// </summary>
	    [Test]
	    public void TestHardCodedBinaryData()
	    {
	        Assert.AreEqual(expected_metric_set.version(),  actual_metric_set.version());
	        Assert.AreEqual(expected_metric_set.size(),  actual_metric_set.size());
	        Assert.AreEqual(expected_metric_set.channel_count(),  actual_metric_set.channel_count());

	        for(uint i=0;i<Math.Min(expected_metric_set.size(), actual_metric_set.size());i++)
	        {
	            Assert.AreEqual(expected_metric_set.at(i).lane(), actual_metric_set.at(i).lane());
	            Assert.AreEqual(expected_metric_set.at(i).tile(), actual_metric_set.at(i).tile());
	            Assert.AreEqual(expected_metric_set.at(i).cycle(), actual_metric_set.at(i).cycle());
	            Assert.AreEqual(expected_metric_set.at(i).channel_count(), actual_metric_set.at(i).channel_count());
	            for(uint j=0;j<Math.Min(expected_metric_set.at(i).channel_count(), actual_metric_set.at(i).channel_count());j++)
	            {
	                Assert.AreEqual(expected_metric_set.at(i).min_contrast(j), actual_metric_set.at(i).min_contrast(j));
	                Assert.AreEqual(expected_metric_set.at(i).max_contrast(j), actual_metric_set.at(i).max_contrast(j));
	            }
	        }
	    }
	}
	/// <summary>
	/// Confirm that the image metrics InterOp works properly in C# for Version 1 of the format
	/// </summary>
	///
	[TestFixture]
	public class ImageMetricsTestV1 : AbstractImageMetricsTest
	{
		const int Version = 1;
		/// <summary>
		/// The setup should be overridden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{
            const ushort channel_count = 4;
            ushort[] min_contrast1  = new ushort[]{896, 1725,738,812};
            ushort[] min_contrast2  = new ushort[]{908, 1770,739,806};
            ushort[] min_contrast3  = new ushort[]{923, 1829,734,802};

            ushort[] max_contrast1  = new ushort[]{4840, 8144,3308,4974};
            ushort[] max_contrast2  = new ushort[]{4829, 8159,3302,4985};
            ushort[] max_contrast3  = new ushort[]{4829, 8236,3304,4947};
            expected_metrics.Add(new image_metric(1, 1101, 1, channel_count, (min_contrast1), (max_contrast1)));
            expected_metrics.Add(new image_metric(1, 1102, 1, channel_count, (min_contrast2), (max_contrast2)));
            expected_metrics.Add(new image_metric(1, 1103, 1, channel_count, (min_contrast3), (max_contrast3)));

			int[] tmp = new int[]{
                 1,12,
                 1,0,77,4,1,0,0,0,128,3,232,18,
                 1,0,77,4,1,0,1,0,189,6,208,31,
                 1,0,77,4,1,0,2,0,226,2,236,12,
                 1,0,77,4,1,0,3,0,44,3,110,19,
                 1,0,78,4,1,0,0,0,140,3,221,18,
                 1,0,78,4,1,0,1,0,234,6,223,31,
                 1,0,78,4,1,0,2,0,227,2,230,12,
                 1,0,78,4,1,0,3,0,38,3,121,19,
                 1,0,79,4,1,0,0,0,155,3,221,18,
                 1,0,79,4,1,0,1,0,37,7,44,32,
                 1,0,79,4,1,0,2,0,222,2,232,12,
                 1,0,79,4,1,0,3,0,34,3,83,19
			};
			SetupBuffers (tmp, Version, channel_count);
		}
	}
	/// <summary>
	/// Confirm that the image metrics InterOp works properly in C# for Version 2 of the format
	/// </summary>
	///
	[TestFixture]
	public class ImageMetricsTestV2 : AbstractImageMetricsTest
	{
		const int Version = 2;
		/// <summary>
		/// The setup should be overridden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{
            const ushort channel_count = 2;
            ushort[] min_contrast1  = new ushort[]{231, 207};
            ushort[] min_contrast2  = new ushort[]{229, 205};
            ushort[] min_contrast3  = new ushort[]{231, 222};

            ushort[] max_contrast1  = new ushort[]{462, 387};
            ushort[] max_contrast2  = new ushort[]{457, 387};
            ushort[] max_contrast3  = new ushort[]{473, 416};
            expected_metrics.Add(new image_metric(7, 1114, 1, channel_count, (min_contrast1), (max_contrast1)));
            expected_metrics.Add(new image_metric(7, 1214, 1, channel_count, (min_contrast2), (max_contrast2)));
            expected_metrics.Add(new image_metric(7, 2114, 1, channel_count, (min_contrast3), (max_contrast3)));

			int[] tmp = new int[]{
			                2,14,2
                            ,7,0,90,4,1,0,-25,0,-49,0,-50,1,-125,1
                            ,7,0,-66,4,1,0,-27,0,-51,0,-55,1,-125,1
                            ,7,0,66,8,1,0,-25,0,-34,0,-39,1,-96,1
			};
			SetupBuffers (tmp, Version, channel_count);
		}
	}
}
