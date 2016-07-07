/** Imaging model and logic
 */

%include <std_string.i>
%include <stdint.i>
%include <std_vector.i>

//////////////////////////////////////////////
// Don't wrap it, just use it with %import
//////////////////////////////////////////////
%import "src/ext/swig/exceptions/exceptions_impl.i"
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


// This imports the metrics
WRAP_METRICS(IMPORT_METRIC_WRAPPER)
// This allows exceptions to be imported, but not belong to the module
EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)


%template(ushort_vector) std::vector< uint16_t >;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Imaging model
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%{
#include "interop/model/table/imaging_table.h"
#include "interop/logic/table/populate_imaging_table.h"
%}

%template(size_vector) std::vector< size_t >;
%template(size_vector_2d) std::vector< std::vector< size_t > >;
WRAP_VECTOR(illumina::interop::model::table::imaging_table);

%include "interop/model/table/column_header.h"
%include "interop/model/table/imaging_table_entry.h"
%include "interop/model/table/imaging_table.h"

%template(imaging_table_entry_vector) std::vector<illumina::interop::model::table::table_entry>;
%template(column_header_vector) std::vector<illumina::interop::model::table::column_header>;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Imaging Logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%include "interop/logic/table/populate_imaging_table.h"
