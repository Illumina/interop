# Changes                                               {#changes}

The change list on the website is likely not the most up to date for trunk. This set
of changes can be found in [Recent Changes].

[Recent Changes]: https://github.com/Illumina/interop/blob/master/docs/src/changes.md

## Trunk

## v1.0.3

Commit  | Description
------- | -----------
5bf14ca | Fix for reading multiple reads with tile metrics
2cef41a | Fixing tile metric phasing/prephasing scale to match SAV
a11a3e1 | Fixed tile_number_for_lane
29905ea | Fixed clear for all metric sets

## v1.0.2

Commit  | Description
------- | -----------
27a7eab | Fixed unclear error message when InterOp is empty
7f93166 | Improvements to the documentation website theme
ade1187 | Ensure CMake configure script throws error of system does not support 32-bit float
096c817 | Made CMake configure script more robust to missing dependencies
498efc3 | Ensure proper cast to record_size_t
286c66c | Added header size calculation
06f567d | Added record size calculation
d35f6bf | Added new filename function without assuming directory
fc94e9f | Added links to Issues on webpage
08f579c | Updated documentation and build script building and deployment

## v1.0.1

Commit  | Description
------- | -----------
450b7e6 | Bundle RapidXML and update deployment scripts
b9cc4b0 | Add change log
f74a702 | Deploy binaries for Windows/Linux/OSX and major compilers
    -   | Ensure only Mono .Net 4.x compiler is used
  -     | Ensure only SWIG 3.x or later is used
3958559 | Setup Appveyor build script
f063b81 | Setup TravisCI build script
586f9d1 | Build C# examples by default
553856c | Updated documentation
03522ff | Fixed build issues
78881f4 | Disable C# examples when swig is disabled
b70f775 | Clean up documentation
216ea62 | Ensure C# compiler matches C++ 32/64-bit
6871246 | Switch to bootstrap theme for documentation

