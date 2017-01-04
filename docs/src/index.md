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
| [Issues]                      | Have a question? Need a feature? Found a bug? Interact with the developers here.     |
| [Performance](@ref perf)      | Computation performance of the code                                                  |
| [Changes](@ref changes)       | Changes between releases of the source code                                          |
| [Formats](@ref binary_formats)| Description of the binary formats for each InterOp file                              |

[Source Code]: https://github.com/Illumina/interop
[Issues]: https://github.com/Illumina/interop/issues

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
| [QMetrics2030Out.bin]         | Per tile per cycle Q20/Q30 scores                                                                                                                             |
| [QMetricsByLaneOut.bin]       | Per tile per cycle Q-score histogram per lane                                                                                                                 |

[CorrectedIntMetricsOut.bin]: @ref corrected_intensity "CorrectedIntMetricsOut.bin"
[ErrorMetricsOut.bin]: @ref error_metric "ErrorMetricsOut.bin"
[ExtractionMetricsOut.bin]: @ref extraction_metric "ExtractionMetricsOut.bin"
[ImageMetricsOut.bin]: @ref image_metric "ImageMetricsOut.bin"
[QMetricsOut.bin]: @ref q_metric "QMetricsOut.bin"
[TileMetricsOut.bin]: @ref tile_metric "TileMetricsOut.bin"
[IndexMetricsOut.bin]: @ref index_metric "IndexMetricsOut.bin"
[QMetrics2030Out.bin]: @ref q_collapsed_metric "QMetrics2030Out.bin"
[QMetricsByLaneOut.bin]: @ref q_metric_by_lane "QMetricsByLaneOut.bin"


Known Limitations
-----------------

There are several known limitations to the current library:

  1. You cannot build a shared library on Windows. Our CMake build script disallows it for good reason, it currently
     does not work. You can still use a static library on Windows. There is one caveat, you can build a shared
     library for the SWIG bindings, e.g. C#.
  2. Microsoft .Net less than v4.0.30319 is currently not supported (Same for Mono)
  3. MinGW cannot compile the code when ENABLE_BACKWARDS_COMPATIBILITY=ON
  4. We do not support Mono on Windows
  5. If both Visual Studio and Mono are installed, the build script will only use Visual Studio for C#
  6. We do not support 32-bit builds
  7. MinGW W64 4.9.x and prior will not link properly
  8. Visual Studio 12 2013 is not supported for the C# Bindings (Results in heap corruption)

SAV Analysis Tab
----------------

The following images were generated using the applications provided by
the InterOp package. See [Examples](@ref example_sav_analysis_example)
for more information.

<table class="tg">
  <tr>
    <th class="tg-yw4l" rowspan="2"><img src="MiSeqDemo_flowcell-Intensity.png" alt="MiSeqDemo_flowcell-Intensity.png" width=250 /></th>
    <th class="tg-yw4l"><img src="MiSeqDemo_Intensity-by-cycle_Intensity.png" alt="MiSeqDemo_Intensity-by-cycle_Intensity.png" width=320/></th>
    <th class="tg-yw4l"><img src="MiSeqDemo_q-histogram.png" alt="MiSeqDemo_q-histogram.png" width=320 /></th>
  </tr>
  <tr>
    <td class="tg-yw4l"><img src="MiSeqDemo_ClusterCount-by-lane.png" alt="MiSeqDemo_ClusterCount-by-lane.png" width=320/></td>
    <td class="tg-yw4l"><img src="MiSeqDemo_q-heat-map.png" alt="MiSeqDemo_q-heat-map.png" width=320 /></td>
  </tr>
</table>
