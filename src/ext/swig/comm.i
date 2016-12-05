/** Plotting model and logic
 */

%include <std_string.i>
%include <stdint.i>
%include <std_vector.i>
%include "util/operator_overload.i"

//////////////////////////////////////////////
// Don't wrap it, just use it with %import
//////////////////////////////////////////////
%import "src/ext/swig/exceptions/exceptions_impl.i"
%include "src/ext/swig/arrays/arrays_impl.i"
%import "src/ext/swig/run.i"
%import "src/ext/swig/metrics.i"

// Ensure all the modules import the shared namespace
%pragma(csharp) moduleimports=%{
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

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("interop_comm");
  }
%}

// This imports the metrics
WRAP_METRICS(IMPORT_METRIC_WRAPPER)
// This allows exceptions to be imported, but not belong to the module
EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)

%{
#include "interop/io/metric_file_stream.h"
#include "interop/io/paths.h"
%}


%include "interop/io/metric_file_stream.h"
%include "interop/io/paths.h"


%define WRAP_METRIC_IO(metric_t)
    %template(compute_buffer_size ) illumina::interop::io::compute_buffer_size< metric_base::metric_set<metric_t> >;
    %template(write_interop_to_buffer ) illumina::interop::io::write_interop_to_buffer< metric_base::metric_set<metric_t> >;
    %template(read_interop_from_buffer )  illumina::interop::io::read_interop_from_buffer< metric_base::metric_set<metric_t> >;
    %template(read_interop )  illumina::interop::io::read_interop< metric_base::metric_set<metric_t> >;
    %template(write_interop )  illumina::interop::io::write_interop< metric_base::metric_set<metric_t> >;
%enddef


WRAP_METRICS(WRAP_METRIC_IO)

