using System;
using NUnit.Framework;
using System.IO;
using Illumina.InterOp.Table;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
using Illumina.InterOp.Comm;

namespace Illumina.InterOp.Interop.UnitTest
{
	/// <summary>
	/// Test plotting data by cycle
	/// </summary>
	[TestFixture]
	public class ImagingTableLogic
	{
		/// <summary>
		/// Test building a float backed image table
		/// </summary>
		[Test]
		public void PopulateFloatBackedTableTest()
		{
            int[] tmp = new int[]{
                 2,38
                 ,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
                 ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
                 ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120
            };
            byte[] expected_binary_data = new byte[tmp.Length];
            for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
            run_metrics run = new run_metrics();
            c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, run.extraction_metric_set());

            var run_param = new parameters();
            Assert.AreEqual(run_param.version(), 0);
            Assert.AreEqual(run.extraction_metric_set().max_cycle(), 1);

            read_info_vector reads = new read_info_vector();
            reads.Add(new read_info(1, 1, 26));
            reads.Add(new read_info(2, 27, 76));
            run.run_info(new info(
                    "",
                    "",
                    1,
                    new flowcell_layout(2, 2, 2, 16),
                    new string_vector(),
                    new image_dimensions(),
                    reads
            ));
            run.legacy_channel_update(instrument_type.HiSeq);

            imaging_column_vector columnVector = new imaging_column_vector();
            c_csharp_table.create_imaging_table_columns(run, columnVector);
            map_id_offset rowOffsets = new map_id_offset();
            c_csharp_table.count_table_rows(run, rowOffsets);
            uint columnCount = c_csharp_table.count_table_columns(columnVector);
            var data = new float[rowOffsets.Count*columnCount];
            c_csharp_table.populate_imaging_table_data(run, columnVector, rowOffsets, data, (uint)data.Length);
            Assert.AreEqual(rowOffsets.Count, 3);
            Assert.AreEqual(data[0], 7);

		}
		/// <summary>
		/// Test building a simple image table
		/// </summary>
		[Test]
		public void PopulateTableTest()
		{
            int[] tmp = new int[]{
                 2,38
                 ,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
                 ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
                 ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120
            };
            byte[] expected_binary_data = new byte[tmp.Length];
            for(int i=0;i<expected_binary_data.Length;i++) expected_binary_data[i] = (byte)tmp[i];
            run_metrics run = new run_metrics();
            c_csharp_comm.read_interop_from_buffer(expected_binary_data, (uint)expected_binary_data.Length, run.extraction_metric_set());

            var run_param = new parameters();
            Assert.AreEqual(run_param.version(), 0);
            Assert.AreEqual(run.extraction_metric_set().max_cycle(), 1);

            read_info_vector reads = new read_info_vector();
            reads.Add(new read_info(1, 1, 26));
            reads.Add(new read_info(2, 27, 76));
            run.run_info(new info(
                    "",
                    "",
                    1,
                    new flowcell_layout(2, 2, 2, 16),
                    new string_vector(),
                    new image_dimensions(),
                    reads
            ));
            run.legacy_channel_update(instrument_type.HiSeq);

            imaging_table table = new imaging_table();
            c_csharp_table.create_imaging_table(run, table);
            Assert.AreEqual(table.row_count(), 3);
            Assert.AreEqual(table.at(0u, 0u), 7);

		}
		[Test]
		public void IsBaseMetric()
		{
		    var metricType = metric_type.BasePercent;
		    Assert.IsTrue(c_csharp_metrics.is_base_metric(metricType));
		}
	}

}
