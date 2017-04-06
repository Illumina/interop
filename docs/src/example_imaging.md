Imaging {#example_imaging}
==========================

This section introduces the way SAV calculates the Imaging tab. This introduces a limited set of functions. The key 
classes used below are:

 - `run_metrics`: model holding the binary InterOp data
 - `imaging_column_vector`: Vector of columns for the imaging table
 - `map_id_offset`: map row ID to the offset in the float array
 - `valid_to_load`: byte array (`std::vector<unsigned char>` or `uchar_vector`) indicating which InterOp files to load
 
The primary functions used below are:

 - `run_metrics::read`: read the InterOp files from disk
 - `create_imaging_table_columns`: Generate the available column groups from the metric set
 - `count_table_rows`: Count the number of necessary rows from the metric set
 - `count_table_columns`: Count the number of subcolumns for the table
 - `populate_imaging_table_data`: Populate the array of data for the table
 - `list_imaging_table_metrics_to_load`: list which imaging metrics will be loaded

C#
--

@snippet src/examples/csharp/ImagingExample.cs Create Imaging Table from a Run Folder

C++
---

The following shows how to calculate the Imaging metrics:

@snippet src/apps/imaging_table.cpp Reporting Imaging Metrics in C++

The following shows the implementation of `read_run_metrics` above and how to read the InterOp data from disk:

@snippet src/apps/inc/application.h Reading a subset of run metrics in C++
