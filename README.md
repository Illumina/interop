Quick Start
===========

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

This library is written in C++98 and provides bindings for C# using the SWIG interface. This
support includes examples and unit tests for both languages.

Documentation
-------------


| Content                       | Description                                                                          |
| ----------------------------- | -------------------------------------------------------------------------------------|
| [Main][MainPage]              | Index page for the documentation                                                     |
| [Install][InstallPage]        | Describes how to install Illumina InterOp from binaries and the source code          |
| [Applications][AppsPage]      | Describes applications that transform binary InterOp data into text                  |
| [Examples][ExamplesPage]      | How to examples showing how to use the Illumina InterOp C++ API                      |
| [Modules][ModulesPage]        | Short descriptions of the most useful code in the Illumina InterOp API               |



[MainPage]: http://illumina.github.io/interop/index.html
[InstallPage]: http://illumina.github.io/interop/install.html
[AppsPage]: http://illumina.github.io/interop/apps.html
[ExamplesPage]: http://illumina.github.io/interop/examples.html
[ModulesPage]: http://illumina.github.io/interop/modules.html

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
  2. The full logic of SAV including certain metrics and plots are not yet implemented.
  3. Microsoft Dot Net less than v4.0.30319 is currently not supported
  4. MinGW cannot compile the code when ENABLE_BACKWARDS_COMPATIBILITY=ON
  5. Currently, the minimum DotNet version is v4.0.30319


