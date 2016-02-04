Illumina InterOp Documentation                         {#mainpage}
==============================

Purpose
-------

The Illumina InterOp libraries are a set of common routines used for reading and writing InterOp metric files.  These
metric files are binary files produced during a run providing detailed statistics about a run.  In a few cases, the 
metric files are produced after a run during secondary analysis (index metrics) or for faster display of a subset of 
the original data (collapsed quality scores).

Compatibility
-------------

There are a wide number of platforms that Illumina supports and releases.  The stance has always been to keep SAV 
backwards compatible with prior releases.  This means that these common InterOp libraries need to be backwards 
compatible as well.  There is one exception to this rule: GA systems have been excluded and are not supported with 
these common InterOp libraries.

Table of Contents
-----------------

| Content                       | Description                                                                          |
| ----------------------------- | -------------------------------------------------------------------------------------|
| [Install](@ref install)       | Describes how to install Illumina InterOp from binaries and the source code          |
| [Applications](@ref apps)     | Describes applications that transform binary InterOp data into text                  |
| [Examples](@ref examples)     | How to examples showing how to use the Illumina InterOp C++ API                      |
| [Modules](modules.html)       | Short descriptions of the most useful code in the Illumina InterOp API               |
| [Source Code]                 | Git repository hosted on Github.com                                                  |
| [Changes](@ref changes)       | Changes between releases of the source code                                          |

[Source Code]: https://github.com/Illumina/interop

List of InterOp Metric Files
----------------------------

| Filename                      | Description                                                                                                                                                   |
| ----------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [CorrectedIntMetricsOut.bin]  | Per tile per cycle per channel average intensity values                                                                                                       |
| [ErrorMetricsOut.bin]         | Per tile per cycle (after alignment cycle, e.g. 25) percentages of incorrect calls for aligned clusters                                                       |
| [ExtractionMetricsOut.bin]    | Per tile per cycle per channel 90th percentile of intensity and FWHM values                                                                                   |
| [ImageMetricsOut.bin]         | Per tile per cycle per channel percentiles of pixel values used to autocontrast thumbnail images                                                              |
| [QMetricsOut.bin]             | Per tile per cycle subsampled basecall quality score histogram, also contains QScore Binning information in header as of version 5                            |
| [TileMetricsOut.bin]          | Per tile information about raw density, cluster count, cluster density passed filter, cluster count passed filter, percent aligned, phasing and prephasing    |
| [IndexMetricsOut.bin]         | Per tile per lane index sequence information                                                                                                                  |

[CorrectedIntMetricsOut.bin]: @ref corrected_intensity "CorrectedIntMetricsOut.bin"
[ErrorMetricsOut.bin]: @ref error_metric "ErrorMetricsOut.bin"
[ExtractionMetricsOut.bin]: @ref extraction_metric "ExtractionMetricsOut.bin"
[ImageMetricsOut.bin]: @ref image_metric "ImageMetricsOut.bin"
[QMetricsOut.bin]: @ref q_metric "QMetricsOut.bin"
[TileMetricsOut.bin]: @ref tile_metric "TileMetricsOut.bin"
[IndexMetricsOut.bin]: @ref index_metric "IndexMetricsOut.bin"


Known Limitations
-----------------

There are several known limitations to the current library:

  1. You cannot build a shared library on Windows. Our CMake build script disallows it for good reason, it currently
     does not work. You can still use a static library on Windows. There is one caveat, you can build a shared
     library for the SWIG bindings, e.g. C#.
  2. The full logic of SAV including certain metrics and plots are not yet implemented.
  3. Microsoft .Net less than v4.0.30319 is currently not supported (Same for Mono)
  4. MinGW cannot compile the code when ENABLE_BACKWARDS_COMPATIBILITY=ON
  5. We do not support Mono on Windows
  6. If both Visual Studio and Mono are installed, the build script will only use Visual Studio for C#
