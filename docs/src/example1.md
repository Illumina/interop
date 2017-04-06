Reading a binary InterOp file {#reading}
=============================

Reading a binary Interop file is really just two lines of code, but all sorts of unexpected situations may arise. This 
example shows you how to write bullet proof code to read an Illumina InterOp file.

## Simple method

C++
---

Shows how to read a single metric set using run_metrics

@snippet src/examples/example_populate_qmetricset.cpp Reading a single metric set using run_metrics

C#
--

Shows how to read all metrics required by the imaging tab in SAV

@snippet src/examples/csharp/ImagingExample.cs Reading only metrics required by the Imaging Tab

## Advanced method

Not recommneded for most users.

### C++

@snippet src/examples/example1.cpp Reading a binary InterOp file

### C#

@snippet src/examples/csharp/Example1.cs Reading a binary InterOp file in CSharp