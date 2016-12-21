/** Summary model and logic
 */

%include <std_string.i>
%include <stdint.i>
%include <std_vector.i>
%include "util/operator_overload.i"

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

// Ensure each of the generated C# class import the shared code
%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
%}

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("interop_summary");
  }
%}

// This imports the metrics
WRAP_METRICS(IMPORT_METRIC_WRAPPER)
// This allows exceptions to be imported, but not belong to the module
EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Summary model
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%{
#include "interop/model/summary/cycle_state_summary.h"
#include "interop/model/summary/stat_summary.h"
#include "interop/model/summary/surface_summary.h"
#include "interop/model/summary/metric_summary.h"
#include "interop/model/summary/lane_summary.h"
#include "interop/model/summary/metric_stat.h"
#include "interop/model/summary/read_summary.h"
#include "interop/model/summary/run_summary.h"
#include "interop/logic/metric/q_metric.h"
%}

%ignore illumina::interop::model::summary::read_summary::read()const;
%ignore illumina::interop::model::summary::lane_summary::cycle_state()const;
%ignore illumina::interop::model::summary::read_summary::summary()const;
%ignore illumina::interop::model::summary::run_summary::total_summary()const;
%ignore illumina::interop::model::summary::run_summary::nonindex_summary()const;
%ignore illumina::interop::model::summary::run_summary::cycle_state()const;

%include "interop/model/summary/cycle_state_summary.h"
%include "interop/model/summary/stat_summary.h"
%include "interop/model/summary/surface_summary.h"
%include "interop/model/summary/metric_summary.h"
%include "interop/model/summary/lane_summary.h"
%include "interop/model/summary/metric_stat.h"
%include "interop/model/summary/read_summary.h"
%include "interop/model/summary/run_summary.h"

//
// Setup typemaps for summary metrics
//


%template(surface_summary_vector) std::vector<illumina::interop::model::summary::surface_summary>;
%template(lane_summary_vector) std::vector<illumina::interop::model::summary::lane_summary>;
%template(read_summary_vector) std::vector<illumina::interop::model::summary::read_summary>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Summary logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%{
#include "interop/logic/summary/run_summary.h"
%}
%include "interop/logic/summary/run_summary.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Index summary model
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%{
#include "interop/model/summary/index_count_summary.h"
#include "interop/model/summary/index_lane_summary.h"
#include "interop/model/summary/index_flowcell_summary.h"
%}

%include "interop/model/summary/index_count_summary.h"
%include "interop/model/summary/index_lane_summary.h"
%include "interop/model/summary/index_flowcell_summary.h"

%template(index_count_summary_vector) std::vector<illumina::interop::model::summary::index_count_summary>;
%template(index_lane_summary_vector) std::vector<illumina::interop::model::summary::index_lane_summary>;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Summary Index Logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%{
#include "interop/logic/summary/index_summary.h"
%}

%include "interop/logic/summary/index_summary.h"
