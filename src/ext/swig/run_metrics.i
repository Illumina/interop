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
%import "src/ext/swig/comm.i"
%import "src/ext/swig/metrics.i"

// Ensure all the modules import the shared namespace
%pragma(csharp) moduleimports=%{
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
%}

%pragma(csharp) imclassimports=%{
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
using Illumina.InterOp.Comm;
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

RUN_EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)
IO_EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)
METRICS_EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)

// exceptions
%{
#include "interop/model/model_exceptions.h"
%}

%define RUN_METRICS_EXCEPTION_WRAPPER(WRAPPER)
WRAPPER(illumina::interop::model::, invalid_channel_exception, invalid_channel_exception)
WRAPPER(illumina::interop::model::, invalid_metric_type, invalid_metric_type)
WRAPPER(illumina::interop::model::, invalid_parameter, invalid_parameter)
%enddef
RUN_METRICS_EXCEPTION_WRAPPER(WRAP_EXCEPTION)

%include "interop/model/model_exceptions.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Metric Logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The SWIG Python binding does not support polymorphic functions
#if defined(SWIGPYTHON)
    %rename(list_metrics_to_load_metric_group) illumina::interop::logic::utils::list_metrics_to_load(const constants::metric_group group,
                                                                                                     std::vector<unsigned char>& valid_to_load,
                                                                                                     const constants::instrument_type instrument);
    %rename(list_metrics_to_load_metric_groups) illumina::interop::logic::utils::list_metrics_to_load(const std::vector<constants::metric_group>& groups, std::vector<unsigned char>& valid_to_load,
                                                                                                                                   const constants::instrument_type type);
#endif


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
    %template(set_ ## metric_t ## _set) illumina::interop::model::metrics::run_metrics::set< illumina::interop::model::metric_base::metric_set< metric_t> >;
    %template(metric_t ## _set) illumina::interop::model::metrics::run_metrics::get_metric_set< metric_t >;
%enddef
WRAP_METRICS(WRAP_RUN_METRICS)
