/** Imaging model and logic
 */

%include <std_string.i>
%include <stdint.i>
%include <std_vector.i>
%import "src/ext/swig/metrics.i"

//////////////////////////////////////////////
// Don't wrap it, just use it with %import
//////////////////////////////////////////////
%import "src/ext/swig/exceptions/exceptions_impl.i"
%import "src/ext/swig/interop.i"

%pragma(csharp) moduleimports=%{
using Illumina.InterOp.Interop;
%}

%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;
using Illumina.InterOp.Interop;
%}


// This allows exceptions to be imported, but not belong to the module
WRAP_METRICS(IMPORT_METRIC_WRAPPER)
EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)


// Imaging Model
%{
#include "interop/model/table/imaging_table.h"
#include "interop/logic/table/populate_imaging_table.h"
%}
%include "interop/model/table/column_header.h"
%include "interop/model/table/imaging_table_entry.h"
%include "interop/model/table/imaging_table.h"
%template(imaging_table_entry_vector) std::vector<illumina::interop::model::table::table_entry>;
%template(column_header_vector) std::vector<illumina::interop::model::table::column_header>;


// Imaging Logic
%include "interop/logic/table/populate_imaging_table.h"
