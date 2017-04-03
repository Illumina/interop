/** Plotting model and logic
 */

%include <std_string.i>
%include <stdint.i>
%include <std_vector.i>
%include "util/operator_overload.i"

//////////////////////////////////////////////
// Don't wrap it, just use it with %import
//////////////////////////////////////////////
%include "src/ext/swig/exceptions/exceptions_impl.i"
%include "src/ext/swig/arrays/arrays_impl.i"
%import "src/ext/swig/run.i"
%import "src/ext/swig/metrics.i"

// Ensure all the modules import the shared namespace
%pragma(csharp) moduleimports=%{
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
%}
%pragma(csharp) imclassimports=%{
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
%}

// Ensure each of the generated C# class import the shared code
%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
%}

// Ensure the C++ shared library is loaded by the Java interface file
%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("interop_comm");
  }
%}

// This imports the metrics
WRAP_METRICS(IMPORT_METRIC_WRAPPER)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exceptions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%define IO_EXCEPTION_WRAPPER(WRAPPER)
WRAPPER(illumina::interop::io::, file_not_found_exception, file_not_found_exception)
WRAPPER(illumina::interop::io::, format_exception, format_exception)
WRAPPER(illumina::interop::io::, bad_format_exception, bad_format_exception)
WRAPPER(illumina::interop::io::, incomplete_file_exception, incomplete_file_exception)
WRAPPER(illumina::interop::io::, invalid_argument, invalid_argument)
%enddef


IO_EXCEPTION_WRAPPER(WRAP_EXCEPTION)
%include "interop/io/stream_exceptions.h"
// This allows exceptions to be imported, but not belong to the module
RUN_EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)
METRICS_EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IO
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%{
#include "interop/io/metric_file_stream.h"
#include "interop/io/paths.h"
#include "interop/model/run/run_exceptions.h"
#include "interop/util/xml_exceptions.h"
%}

%include "interop/io/metric_file_stream.h"
%include "interop/io/paths.h"

// Wrap all the interface functions
%define WRAP_METRIC_IO(metric_t)
    %template(compute_buffer_size ) illumina::interop::io::compute_buffer_size< metric_base::metric_set<metric_t> >;
    %template(write_interop_to_buffer ) illumina::interop::io::write_interop_to_buffer< metric_base::metric_set<metric_t> >;
    %template(read_interop_from_buffer )  illumina::interop::io::read_interop_from_buffer< metric_base::metric_set<metric_t> >;
    %template(read_interop )  illumina::interop::io::read_interop< metric_base::metric_set<metric_t> >;
    %template(write_interop )  illumina::interop::io::write_interop< metric_base::metric_set<metric_t> >;
%enddef

WRAP_METRICS(WRAP_METRIC_IO)

