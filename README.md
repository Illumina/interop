Quick Start
===========

The Illumina InterOp libraries are a set of common routines used for reading InterOp metric files produced by
Illumina sequencers. These libraries backwards compatible and capable of supporting prior releases of the software,
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

Install
-------

### From Binaries

TODO

### From Source

~~~~~~~~~~~~~{.sh}
$ git clone https://github.com/illumina/ipa.git
$ mkdir build
$ cd build
$ cmake ../ipa
$ cmake --build .
~~~~~~~~~~~~~

Building with InterOp
---------------------

### GCC/Clang

~~~~~~~~~~~~~{.sh}
g++ example1.cpp -l interop_lib -L <install_dir>/lib64 -I <install_dir>/include
~~~~~~~~~~~~~

For CLang, just substitute g++ with clang++. 

### CMake

See `cmake/FindInterop.cmake`


Known Limitations
-----------------

There are several known limitations to the current library:

  1. You cannot build a shared library on Windows. Our CMake build script disallows it for good reason, it currently
     does not work. You can still use a static library on Windows. There is one caveat, you can build a shared
     library for the SWIG bindings, e.g. C#.
  2. The full logic of SAV including certain metrics and plots are not yet implemented.


