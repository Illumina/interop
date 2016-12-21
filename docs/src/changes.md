# Changes                                               {#changes}

## v1.0.14

Date       | Description
---------- | -----------
2016-12-21 | Replace interop2csv with dumptext
2016-12-20 | IPA-5923: Fix bug in legacy q-metric binning
2016-12-19 | Removes the coverity run from the build matrix for every pull request
2016-12-16 | IPA-5885: Ensure error summary matches requirements
2016-12-14 | Enhance C# version information
2016-12-14 | Clean up unused enum types
2016-12-12 | IPA-5153: Ensure Summary tab calculations are consistent with Docs
2016-12-09 | IPA-5883: Fix possible memory issue in MSVC12
2016-12-08 | IPA-4674: Add support for absolute naming convention
2016-12-07 | IPA-5869: Add section filtering
2016-12-06 | IPA-5734: Synchronize master
2016-12-02 | Add Google Analytics tracking to documentation
2016-12-08 | Added admonition to avoid using MSVC 12 (2013) with C# bindings
2016-12-08 | Added support for absolute tile naming
2016-12-08 | Added ability to do section filtering
2016-12-05 | Added regression tests for imaging
2016-12-05 | Added unit tests for format coverage
2016-11-17 | Improved performance of InterOp calculations for C++11 enabled version
2016-11-14 | Added version to Windows DLLs
2016-11-14 | Added regression tests for indexing
2016-11-09 | Added unit tests for clearing logic
2016-11-07 | Added regression tests for analysis plots
2016-11-01 | Improved performance of InterOp loading
2016-10-26 | Added path class to handle RTA output filenames
2016-09-22 | Added regression tests for summary


## v1.0.13

Date       | Description
---------- | -----------
2016-11-30 | Removed Win32 build testing - this is not supported
2016-11-28 | IPA-5830: Fix tile naming bug for on demand datasets
2016-10-19 | Remove time from imaging table
2016-09-27 | IPA-5213: Create per surface summary model/logic/app


## v1.0.12

Date       | Description
---------- | -----------
2016-09-26 | IPA-5210: Create CSV parsable summary output
2016-09-23 | IPA-5194: Create nuspec package file
2016-09-17 | Refactor individual metric unit tests
2016-09-17 | Refactor metric stream tests
2016-09-14 | Reorganized code into own stat util file
2016-09-14 | Fixes error rate at a specific cycle when tiles are at different cycles
2016-09-14 | Add version information for zip archives
2016-09-14 | IPA-5070: Add optional flag to skip median for summary
2016-09-13 | Ensure ANSI build is being tested


## v1.0.11

Date       | Description
---------- | -----------
2016-09-12 | Fixed bug in q-score heatmap and q-score clearing
2016-09-12 | Added summary regression test, fixed regression test framework, fixed runParameters reading
2016-09-09 | Added sentinel for missing values in the heatmap type plots
2016-09-09 | Fixed invalid pointer deference and double free in MSVC
2016-09-08 | Added sentinel for missing values in the imaging table
2016-09-08 | Fixed potential problem found by coverity
2016-09-07 | IPA-5101: Fix MSVC bug in vector type loading (Master)
2016-09-02 | Added buffered reader to improve loading speed
2016-09-01 | Bug fix for SWIG in on-demand loading

## v1.0.10

Date       | Description
---------- | -----------
2016-09-01 | Bug fix clearing the summary stat prior to setting it
2016-09-01 | Fixed a bug in QMetrics V6 Unbinned, added unit test and C# example
2016-08-31 | IPA-5069: Flush out on demand loading further for each tab
2016-08-30 | Various minor fixes to get public working with Clang3.4 and MSVC12
2016-08-30 | Bug fix to RunInfo section validation
2016-08-30 | IPA-5069: Support on demand loading for SAV (Part 1)
2016-08-26 | IPA-5028: Add RunInfo and InterOp validation

## v1.0.9

Date       | Description
---------- | -----------
2016-08-26 | Refactor CSharp code into additional library
2016-08-26 | IPA-5028: Add RunInfo and InterOp validation
2016-08-19 | Fix sorting in imaging table
2016-08-18 | Cleanup code
2016-08-17 | Fix the read ID
2016-08-16 | IPA-5009: Add sorting to imaging table
2016-08-16 | IPA-5007: Fix combo boxes on analysis tab
2016-08-16 | Fix msvc warning and some script differences
2016-08-15 | Add clear function to run metrics
2016-08-15 | IPA-4799: Create Internal Coverity Build
2016-08-11 | Refactor unit tests to make more flexible for regression testing
2016-08-08 | IPA-4922: Move logic code from header to cpp
2016-08-05 | IPA-4827: Make run info parsing more robust
2016-08-03 | Refactor code and add test build script
2016-08-02 | Add big object support for unit tests
2016-08-02 | Clean buffers in the heatmap and flowcell
2016-08-02 | Update travis-ci to GCC-5
2016-07-28 | Added float array backing to heat map
2016-07-25 | IPA-4752: Replace InterOp 1.x Code with InterOp 2.x Code - Imaging Tab
2016-07-22 | Fix camel case
2016-07-22 | IPA-4751: Integrated InterOp2.x with Analysis Tab
2016-07-22 | Github Issue 74: Compiler warning as error unsigned/signed comp
2016-07-21 | Bug fixes for flowcell chart and more recent versions of GCC
2016-07-17 | Added coverity online scan and fixed some issues found

## v1.0.8

Commit  | Description
------- | -----------
b74024d | Added support for paths with spaces in the build system
d76117a | Remove support for 32-bit builds
27c1942 | IPA-4778: Fixed issues found by coverity
ad8cac0 | Updated the documentation

## v1.0.7

Commit  | Description
------- | -----------
2d8480c | Fixed C# binding and filter_option validation
32daedd | Added package target to CMake build script
36ca197 | Add command line parser and update plotting apps
6cc52c2 | Continue refactoring SWIG C# binding and fix exceptions
5e01ff3 | Update tolerances to properly handle floating-point differences due to rounding
5e85cf0 | IPA-4608: Ensure cumulative q-metrics does not overflow
c60caa9 | IPA-4598: Regression test for imaging table
616e6e8 | IPA-4695: Setup framework to reuse unit test code for regression testing
839191c | Quick floor -> round bug fix for indexing plot y-max value
8fa8cfe | Test for larger CollapsedQ size
cbe3540 | IPA-4505: Fixed writing to buffer
76521e3 | IPA-4505: Fix summary tab, index tab and bs q-metrics
03867f0 | IPA-4598: Added imaging metrics and precision of numbers reported
8f5cf70 | IPA-4505: Fix empty PlotQC
fd5e265 | IPA-4505: Replace median, fixed missing percent aligned and fixed collapsed q-metrics
e2d1c80 | IPA-4505: Tile convention checking
e841107 | IPA-4598: Bind imaging logic to C# and test
37e6e21 | IPA-4505: Fix channel order in by cycle
23f1dca | IPA-4505: Fixed by cycle and q-bin index
987d02c | IPA-4595: Add index out of bounds checking to metrics
18e0ef7 | IPA-4505: Fix q-score median
d815021 | IPA-4595: Map plot metric types to input types
01f08c6 | IPA-4497: Fix camel case in metric_format
9cce9b9 | IPA-4486: Refactor code to use INTEROP_TUPLEX
37899c9 | IPA-4493: Replace q-metrics with collapsed q-metrics in logic
cb999ae | IPA-4485: Simplify mapping between constant types
333909f | IPA-4419: Add collapsed q-score logic
68d1b6a | IPA-4286: Add collapsed qscore metrics
2c82c4a | IPA-4292: Add imaging model and logic
c5aad38 | IPA-4292: Refactor to prepare code for imaging model and logic
d9e5a83 | IPA-4290: Add index summary logic
050a9ee | IPA-4447: Add index summary model
fc5d190 | IPA-4408: Plot sample QC for indexing tab
f05a2a3 | IPA-4406: Plot Logic - flowcell map
4eb6568 | IPA-4407: Plot Logic - plot q-score histogram
9706b24 | IPA-4404: Add plot logic - plot by lane
56bb897 | IPA-4384: Prototype PlotDataByCycle to build initial framework
9c8e8d8 | IPA-4384: Model for plot by cycle prototype (Part 1)
6c1fbc5 | IPA-4382: Refactor code to make naming more consistent
9a45340 | IPA-4383: Add filter options to support plot logic
b11e736 | IPA-4382: Create reflection-like interface for enums to support plot logic
d9deeea | IPA-4246: Add search for non Out interop files
3f2f2bf | Reorganize modules in doxygen
202d6ef | IPA-4146: Add summary logic and application
6c5a30d | IPA-4146: Added metric and channel logic and more statistics
735cda2 | IPA-4146: Refactor of the metric set
         
## v1.0.6

Commit  | Description
------- | -----------
57be79c | Update all cycle states and bug fix for legacy q-metrics
0b4304a | Refactor to remove and mark deprecated code
647cc9e | Add utility for creating unit tests
9d82333 | Add search for non Out interop files

## v1.0.5

Commit  | Description
------- | -----------
7380f29 | Bug fixes for q-metrics and compilation error on Mac OSX

## v1.0.4

Commit  | Description
------- | -----------
50e8e2a | Update documentation
50e8e2a | Update documentation
0b22102 | Added summary tests
13f1fb4 | Added summary logic
339a914 | Added summary model
aca0152 | Added run metrics
6516098 | Added metric and channel logic as well as more statistics functions
221028a | Added parsers for RunInfo.xml and RunParameters.xml
5a7ca8f | Remove legacy error metric fields

## v1.0.3

Commit  | Description
------- | -----------
5bf14ca | Fix for reading multiple reads with tile metrics
2cef41a | Fixing tile metric phasing/prephasing scale to match SAV
a11a3e1 | Fixed tile_number_for_lane
29905ea | Fixed clear for all metric sets
9af35c4 | Added simple statistical functions: mean and variance
2f04b3f | Added C# performance test
d8b79b0 | Added example program for simulating InterOps at a specific cycle

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

