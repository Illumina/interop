Summary {#example_summary}
==========================

This section introduces the way SAV calculates the summary tab. This introduces a limited set of functions. The key 
classes used below are:

 - `run_metrics`: model holding the binary InterOp data
 - `run_summary`: model holding the derived summary metrics
 - `valid_to_load`: byte array (`std::vector<unsigned char>` or `uchar_vector`) indicating which InterOp files to load
 
The primary functions used below are:

 - `run_metrics::read`: read the InterOp files from disk
 - `summarize_run_metrics`: summarize the SAV run metrics

C#
---

@snippet src/examples/csharp/SummaryExample.cs Reporting Summary Metrics in CSharp


Python
------


If you have not installed InterOp, see [Install InterOp From a Wheel](@ref install)

@snippet src/examples/python/summary.py Reporting Summary Metrics in Python

C++
---

The following shows how to calculate the summary metrics:

@snippet src/apps/summary.cpp Reporting Summary Metrics in C++

The following shows the implementation of `read_run_metrics` above and how to read the InterOp data from disk:

@snippet src/apps/inc/application.h Reading a subset of run metrics in C++
