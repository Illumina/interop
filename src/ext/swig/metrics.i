/** Metrics model and metric logic
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


// Ensure all the modules import the shared namespace
%pragma(csharp) moduleimports=%{
using Illumina.InterOp.Run;
%}

// Ensure each of the generated C# class imports the shared namespaces
%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;
using Illumina.InterOp.Run;
%}

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("interop_metrics");
  }
%}

EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Wrap the base metrics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%{
#include "interop/util/time.h"
#include "interop/constants/enum_description.h"
%}

%ignore set_base(const io::layout::base_metric& base);
%ignore set_base(const io::layout::base_cycle_metric& base);
%ignore set_base(const io::layout::base_read_metric& base);

%include "interop/util/time.h"
%include "interop/model/metric_base/base_metric.h"
%include "interop/model/metric_base/base_cycle_metric.h"
%include "interop/model/metric_base/base_read_metric.h"
%include "interop/model/metric_base/metric_set.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define shared macros
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%apply uint64_t { io::layout::base_metric::id_t };
%define WRAP_TYPES(metric_t)
    using namespace illumina::interop::model::metrics;
    namespace metric_base = illumina::interop::model::metric_base;

    %ignore illumina::interop::model::metric_base::metric_set<metric_t>::populate_tile_numbers_for_lane;
    %ignore illumina::interop::model::metric_base::metric_set<metric_t>::populate_tile_numbers_for_lane_surface;
    %ignore illumina::interop::model::metric_base::metric_set<metric_t>::offset_map;

    %apply size_t { std::map< std::size_t, metric_t >::size_type };
    %apply uint64_t { metric_base::metric_set<metric_t>::id_t };

%enddef

%define WRAP_METRIC_SET(metric_t)
    using namespace illumina::interop::model::metrics;
    namespace metric_base = illumina::interop::model::metric_base;

    %template(base_ ## metric_t ## s) metric_base::metric_set<metric_t>;
    %template(vector_ ## metric_t ## s) std::vector<metric_t>;

%enddef

%define IMPORT_METRIC_WRAPPER(METRIC_NAME)
    %{
    // using wrapper
    #include "interop/model/metrics/METRIC_NAME.h"
    %}
%enddef

%define INCLUDE_METRIC_WRAPPER(METRIC_NAME)
    %include "interop/model/metrics/METRIC_NAME.h"
%enddef

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// List metrics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%define WRAP_METRICS(WRAPPER)
    WRAPPER(corrected_intensity_metric)
    WRAPPER(error_metric)
    WRAPPER(extraction_metric)
    WRAPPER(image_metric)
    WRAPPER(q_metric)
    WRAPPER(tile_metric)
    WRAPPER(index_metric)
    WRAPPER(q_collapsed_metric)
    WRAPPER(q_by_lane_metric)
%enddef


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Import metrics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WRAP_METRICS(IMPORT_METRIC_WRAPPER)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Wrap vectors
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


%template(index_info_vector) std::vector< illumina::interop::model::metrics::index_info  >;

%template(tile_metric_map) std::map< uint64_t, illumina::interop::model::metric_base::base_metric >;
%template(cycle_metric_map) std::map< uint64_t, illumina::interop::model::metric_base::base_cycle_metric >;
%template(read_metric_vector) std::vector< illumina::interop::model::metrics::read_metric >;
%template(q_score_bin_vector) std::vector< illumina::interop::model::metrics::q_score_bin >;


%template(metric_type_name_pair) std::pair< illumina::interop::constants::metric_type, std::string>;
%template(metric_type_description) illumina::interop::constants::enum_description< illumina::interop::constants::metric_type>;
%template(metric_type_description_vector) std::vector< illumina::interop::constants::enum_description< illumina::interop::constants::metric_type> >;

// std::vector of enums causes a crash in Visual Studio C#
%template(metric_type_vector) std::vector< illumina::interop::constants::metric_type>;
%template(metric_group_vector) std::vector< illumina::interop::constants::metric_group>;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Wrap metrics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


WRAP_METRICS(INCLUDE_METRIC_WRAPPER)
WRAP_METRICS(WRAP_TYPES)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


WRAP_METRICS(WRAP_METRIC_SET)




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
