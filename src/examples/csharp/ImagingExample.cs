
// @ [Create Imaging Table from a Run Folder]
using System;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.RunMetrics;
using Illumina.InterOp.Comm;
using Illumina.InterOp.Table;

class ImagingTableExample
{
	static int Main(string[] args)
	{
		if (args.Length != 1)
		{
			if (args.Length < 1)
				Console.WriteLine ("No run folder");
			else
				Console.WriteLine ("Too many arguments");
			return 1;
		}

		run_metrics metrics = new run_metrics();
		uchar_vector interopsToLoad = new uchar_vector();

		// Load only the metrics required by imaging table
        c_csharp_table.list_imaging_table_metrics_to_load(interopsToLoad);
		metrics.read(args[0], interopsToLoad);


        imaging_column_vector columnVector = new imaging_column_vector();
        map_id_offset rowOffsets = new map_id_offset();

        c_csharp_table.create_imaging_table_columns(metrics, columnVector);
        c_csharp_table.count_table_rows(metrics, rowOffsets);
        int rowCount = rowOffsets.Count;
        int columnCount = (int)c_csharp_table.count_table_columns(columnVector);
        float[] data = new float[rowCount * columnCount];

        c_csharp_table.populate_imaging_table_data(metrics, columnVector, rowOffsets, data, (uint)data.Length);

        for(int rowIndex=0;rowIndex<rowCount;++rowIndex)
        {
            for(int groupIndex=0;groupIndex<columnVector.Count;++groupIndex)
            {
                if(columnVector[groupIndex].has_children())
                {
                    for(int subColumnIndex=0;subColumnIndex<columnVector[groupIndex].subcolumns().Count;++subColumnIndex)
                    {
                        int columnIndex = (int)(columnVector[groupIndex].offset()+subColumnIndex);
                        Console.Write("{0},", data[rowIndex*columnCount + columnIndex]);
                    }
                }
                else
                {
                    int columnIndex = (int)(columnVector[groupIndex].offset());
                    Console.Write("{0},", data[rowIndex*columnCount + columnIndex]);
                }
            }
            Console.WriteLine();
        }
		return 0;
	}
}
// @ [Create Imaging Table from a Run Folder]
