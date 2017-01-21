/** Run Metrics model and metric logic
 */
%include <std_vector.i>
%include <stdint.i>
%include <std_map.i>
%include <std_pair.i>
%import "src/ext/swig/exceptions/exceptions_impl.i"
%include "src/ext/swig/arrays/arrays_impl.i"
%include "util/operator_overload.i"

//////////////////////////////////////////////
// Don't wrap it, just use it with %import
//////////////////////////////////////////////
%import "src/ext/swig/exceptions/exceptions_impl.i"
%import "src/ext/swig/run.i"
%import "src/ext/swig/metrics.i"


// Ensure all the modules import the shared namespace
%pragma(csharp) moduleimports=%{
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
%}

// Ensure each of the generated C# class imports the shared namespaces
%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
%}

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("interop_run_metrics");
  }
%}

EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Metric Logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%{
#include "interop/logic/metric/extraction_metric.h"
#include "interop/logic/metric/q_metric.h"
#include "interop/logic/utils/metric_type_ext.h"
#include "interop/logic/utils/metrics_to_load.h"
%}

%include "interop/logic/metric/extraction_metric.h"
%include "interop/logic/metric/q_metric.h"
%include "interop/logic/utils/metric_type_ext.h"
%include "interop/logic/utils/metrics_to_load.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Run Metrics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%{
#include "interop/model/run_metrics.h"
%}
%include "interop/model/run_metrics.h"

%define WRAP_RUN_METRICS(metric_t)
    %template(list_ ## metric_t ## _filenames) illumina::interop::model::metrics::run_metrics::list_filenames< metric_t >;
    %template(set_ ## metric_t ## _set) illumina::interop::model::metrics::run_metrics::set< illumina::interop::model::metric_base::metric_set< metric_t> >;
    %template(metric_t ## _set) illumina::interop::model::metrics::run_metrics::get_metric_set< metric_t >;
%enddef
WRAP_METRICS(WRAP_RUN_METRICS)


