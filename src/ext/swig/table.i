/** Imaging model and logic
 */

%include <std_string.i>
%include <stdint.i>
%include <std_vector.i>
%include <std_map.i>
%include "util/operator_overload.i"

//////////////////////////////////////////////
// Don't wrap it, just use it with %import
//////////////////////////////////////////////
%import "src/ext/swig/exceptions/exceptions_impl.i" // Import is used when the file wraps code avoiding duplicates
%include "src/ext/swig/arrays/arrays_impl.i"
%import "src/ext/swig/run.i"
%import "src/ext/swig/metrics.i"

// Ensure all the modules import the shared namespace
%pragma(csharp) moduleimports=%{
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
%}

// Ensure each of the generated C# class imports the shared namespaces
%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
%}

// Ensure the C++ shared library is loaded by the Java interface file
%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("interop_table");
  }
%}

// This imports the metrics
WRAP_METRICS(IMPORT_METRIC_WRAPPER)
// This allows exceptions to be imported, but not belong to the module
EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)

// Map an ID to an offset in a table
%template(map_id_offset) std::map<uint64_t, uint64_t>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Imaging model
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%{
#include "interop/logic/table/create_imaging_table_columns.h"
#include "interop/logic/table/create_imaging_table.h"
%}

%include "interop/model/table/imaging_column.h"
%include "interop/model/table/imaging_table.h"


%template(imaging_column_vector) std::vector< illumina::interop::model::table::imaging_column >;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Imaging Logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%include "interop/logic/table/create_imaging_table.h"
%include "interop/logic/table/create_imaging_table_columns.h"

