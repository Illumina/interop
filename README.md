Quick Start
===========

[![Build Status] [tcistatus]] [tcihome]
[![Build status] [acistatus]] [acihome]
[![Coverity status] [covstatus]] [covhome]

The Illumina InterOp libraries are a set of common routines used for reading InterOp metric files produced by
Illumina sequencers. These libraries are backwards compatible and capable of supporting prior releases of the software,
with one exception: GA systems have been excluded.

The InterOp files supported by this library include:
  - CorrectedIntMetricsOut.bin
  - ErrorMetricsOut.bin
  - ExtractionMetricsOut.bin
  - ImageMetricsOut.bin
  - QMetricsOut.bin
  - TileMetricsOut.bin
  - IndexMetricsOut.bin
  - QMetrics2030Out.bin
  - QMetricsByLaneOut.bin

This library is written in C++98 and provides bindings for C# using the SWIG interface. This
support includes examples and unit tests for both languages.

[tcistatus]:https://travis-ci.org/Illumina/interop.svg?branch=master
[tcihome]:https://travis-ci.org/Illumina/interop

[acistatus]:https://ci.appveyor.com/api/projects/status/5hwirymkc10uf13d/branch/master?svg=true
[acihome]:https://ci.appveyor.com/project/ezralanglois/interop/branch/master


[covstatus]:https://scan.coverity.com/projects/7744/badge.svg
[covhome]:https://scan.coverity.com/projects/illumina-interop


Documentation
-------------

| Content                       | Description                                                                          |
| ----------------------------- | -------------------------------------------------------------------------------------|
| [Main][MainPage]              | Index page for the documentation                                                     |
| [Install][InstallPage]        | Describes how to install Illumina InterOp from binaries and the source code          |
| [Applications][AppsPage]      | Describes applications that transform binary InterOp data into text                  |
| [Examples][ExamplesPage]      | How to examples showing how to use the Illumina InterOp C++ API                      |
| [Modules][ModulesPage]        | Short descriptions of the most useful code in the Illumina InterOp API               |
| [Changes][ChangesPage]        | Changes between releases of the source code                                          |
| [Issues]                      | Have a question? Need a feature? Found a bug? Interact with the developers here.     |
| [Performance][PerfPage]       | Computation performance of the code                                                  |
| [Formats][BinaryFormats]      | Description of the binary formats for each InterOp file                              |

[MainPage]: http://illumina.github.io/interop/index.html
[InstallPage]: http://illumina.github.io/interop/install.html
[AppsPage]: http://illumina.github.io/interop/apps.html
[ExamplesPage]: http://illumina.github.io/interop/examples.html
[ModulesPage]: http://illumina.github.io/interop/modules.html
[ChangesPage]: http://illumina.github.io/interop/changes.html
[Issues]: https://github.com/Illumina/interop/issues
[PerfPage]: http://illumina.github.io/interop/perf.html
[BinaryFormats]: http://illumina.github.io/interop/binary_formats.html

Install
-------

### From Source

~~~~~~~~~~~~~{.sh}
$ git clone https://github.com/Illumina/interop.git
$ mkdir build
$ cd build
$ cmake ../interop
$ cmake --build .
~~~~~~~~~~~~~

Building with InterOp
---------------------

### GCC/Clang

~~~~~~~~~~~~~{.sh}
g++ example1.cpp -l interop_lib -L <install_dir>/lib64 -I <install_dir>/include
~~~~~~~~~~~~~

For CLang, just substitute g++ with clang++. 


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
the InterOp package.

![Example Plots] [example_image]
[example_image]:docs/images/example_plots.png
