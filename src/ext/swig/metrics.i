/** Metrics model and logic
 */

%include <std_vector.i>
%include <stdint.i>
%include <std_map.i>
%import "src/ext/swig/exceptions/exceptions_impl.i"
%include "src/ext/swig/extends/extends_impl.i"
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

EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ignore methods
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%ignore confusion_matrix;
%ignore focus_score_matrix;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Wrap the base metrics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%{
#include "interop/util/time.h"
%}

%ignore metric_group_iuo;
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

%define WRAP_TYPES(metric_t)
    using namespace illumina::interop::model::metrics;
    namespace metric_base = illumina::interop::model::metric_base;

    %apply size_t { std::map< std::size_t, metric_t >::size_type };
    %apply uint64_t { metric_base::metric_set<metric_t>::id_t };
    WRAP_VECTOR(illumina::interop::model::metric_base::metric_set<metric_t>);

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
%template(ulong_vector) std::vector< uint64_t  >;
%template(ushort_vector) std::vector< uint16_t >;
%template(uint_vector) std::vector< uint32_t >;
%template(float_vector) std::vector< float >;
%template(bool_vector) std::vector< bool >;
%template(uchar_vector) std::vector< uint8_t >;
%template(tile_metric_map) std::map< uint64_t, illumina::interop::model::metric_base::base_metric >;
%template(cycle_metric_map) std::map< uint64_t, illumina::interop::model::metric_base::base_cycle_metric >;
%template(read_metric_vector) std::vector< illumina::interop::model::metrics::read_metric >;
%template(q_score_bin_vector) std::vector< illumina::interop::model::metrics::q_score_bin >;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Wrap metrics
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


WRAP_METRICS(INCLUDE_METRIC_WRAPPER)
WRAP_METRICS(WRAP_TYPES)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Deprecated
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EXTEND_CYCLE_METRIC_SET(corrected_intensity_metric)
EXTEND_CYCLE_METRIC_SET(error_metric)
EXTEND_CYCLE_METRIC_SET(extraction_metric)
EXTEND_CYCLE_METRIC_SET(image_metric)
EXTEND_Q_METRIC(q_metric)
EXTEND_TILE_METRIC(tile_metric)
EXTEND_METRIC_SET(index_metric)
EXTEND_Q_METRIC(q_collapsed_metric)
EXTEND_Q_METRIC(q_by_lane_metric)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


WRAP_METRICS(WRAP_METRIC_SET)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Metric Logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%{
#include "interop/logic/metric/extraction_metric.h"
#include "interop/logic/metric/q_metric.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/utils/metric_type_ext.h"
#include "interop/logic/utils/metrics_to_load.h"
%}

%include "interop/logic/metric/extraction_metric.h"
%include "interop/logic/metric/q_metric.h"
%include "interop/model/run_metrics.h"
%include "interop/logic/utils/metric_type_ext.h"
%include "interop/logic/utils/metrics_to_load.h"



