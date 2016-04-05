/** Swig interface to wrap the InterOp library
 *
 */
%include <std_map.i>
%include <std_string.i>
%include <std_vector.i>
%include <stdint.i>
%include "src/ext/swig/exceptions/exceptions_impl.i"
%include "src/ext/swig/extends/extends_impl.i"
%include "src/ext/swig/arrays/arrays_impl.i"

%{
#include "interop/constants/enums.h"
#include "interop/util/time.h"
#include "interop/io/metric_file_stream.h"
%}

%ignore metric_group_iuo;
%ignore set_base(const io::layout::base_metric& base);
%ignore set_base(const io::layout::base_cycle_metric& base);
%ignore set_base(const io::layout::base_read_metric& base);

%include "interop/constants/enums.h"
%include "interop/util/time.h"
%include "interop/model/metric_base/base_metric.h"
%include "interop/model/metric_base/base_cycle_metric.h"
%include "interop/model/metric_base/base_read_metric.h"
%include "interop/model/metric_base/metric_set.h"
%include "interop/io/metric_file_stream.h"

%typemap(in) (const illumina::interop::io::layout::base_cycle_metric& layout)
   "#error \"Please %ignore this constructor\""

%typemap(in) (const illumina::interop::io::layout::base_metric& layout)
   "#error \"Please %ignore this constructor\""

%typemap(in) (const illumina::interop::io::layout::base_read_metric& layout)
   "#error \"Please %ignore this constructor\""

// Template need to be defined before they can be used in SWIG
%define WRAP_TEMPLATE_BASE(metric_t)
    using namespace illumina::interop::model::metrics;
    namespace metric_base = illumina::interop::model::metric_base;

    %apply size_t { std::map< std::size_t, metric_t >::size_type };
    %apply size_t { metric_base::metric_set<metric_t>::size_type };
    %apply uint64_t { metric_base::metric_set<metric_t>::id_t };
    %apply uint64_t { io::layout::base_metric::id_t };

    EXTEND_METRIC_SET(metric_t)
    %template(base_ ## metric_t ## s) metric_base::metric_set<metric_t>;
    %template(vector_ ## metric_t ## s) std::vector<metric_t>;
    %template(write_interop_to_buffer ) illumina::interop::io::write_interop_to_buffer< metric_base::metric_set<metric_t> >;
    %template(read_interop_from_buffer )  illumina::interop::io::read_interop_from_buffer< metric_base::metric_set<metric_t> >;
    %template(read_interop )  illumina::interop::io::read_interop< metric_base::metric_set<metric_t> >;
    %ignore illumina::interop::model::metric_base::metric_set<illumina::interop::model::metrics::metric_t>::begin;
    %ignore illumina::interop::model::metric_base::metric_set<illumina::interop::model::metrics::metric_t>::end;

%enddef

%define WRAP_TEMPLATE_CYCLE_BASE(metric_t)
    using namespace illumina::interop::model::metrics;
    namespace metric_base = illumina::interop::model::metric_base;

    %apply size_t { std::map< std::size_t, metric_t >::size_type };
    %apply size_t { metric_base::metric_set<metric_t>::size_type };
    %apply uint64_t { metric_base::metric_set<metric_t>::id_t };
    %apply uint64_t { io::layout::base_metric::id_t };

    EXTEND_CYCLE_METRIC_SET(metric_t)
    %template(base_ ## metric_t ## s) metric_base::metric_set<metric_t>;
    %template(vector_ ## metric_t ## s) std::vector<metric_t>;
    %template(write_interop_to_buffer ) illumina::interop::io::write_interop_to_buffer< metric_base::metric_set<metric_t> >;
    %template(read_interop_from_buffer )  illumina::interop::io::read_interop_from_buffer< metric_base::metric_set<metric_t> >;
    %template(read_interop )  illumina::interop::io::read_interop< metric_base::metric_set<metric_t> >;
    %ignore illumina::interop::model::metric_base::metric_set<illumina::interop::model::metrics::metric_t>::begin;
    %ignore illumina::interop::model::metric_base::metric_set<illumina::interop::model::metrics::metric_t>::end;

%enddef

%{
#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/metrics/image_metric.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/metrics/index_metric.h"
%}

%template(index_info_vector) std::vector< illumina::interop::model::metrics::index_info  >;
%template(string_vector) std::vector< std::string  >;
%template(ulong_vector) std::vector< uint64_t  >;
%template(ushort_vector) std::vector< uint16_t >;
%template(uint_vector) std::vector< uint32_t >;
%template(float_vector) std::vector< float >;
%template(read_metric_vector) std::vector< illumina::interop::model::metrics::read_metric >;
%template(q_score_bin_vector) std::vector< illumina::interop::model::metrics::q_score_bin >;

%include "interop/model/metrics/corrected_intensity_metric.h"
%include "interop/model/metrics/error_metric.h"
%include "interop/model/metrics/extraction_metric.h"
%include "interop/model/metrics/image_metric.h"
%include "interop/model/metrics/q_metric.h"
%include "interop/model/metrics/tile_metric.h"
%include "interop/model/metrics/index_metric.h"

WRAP_TEMPLATE_CYCLE_BASE(corrected_intensity_metric)
WRAP_TEMPLATE_CYCLE_BASE(error_metric)
WRAP_TEMPLATE_CYCLE_BASE(extraction_metric)
WRAP_TEMPLATE_CYCLE_BASE(image_metric)
WRAP_TEMPLATE_CYCLE_BASE(q_metric)
WRAP_TEMPLATE_BASE(tile_metric)
WRAP_TEMPLATE_BASE(index_metric)

%{
#include "interop/model/metric_sets/corrected_intensity_metric_set.h"
#include "interop/model/metric_sets/error_metric_set.h"
#include "interop/model/metric_sets/extraction_metric_set.h"
#include "interop/model/metric_sets/image_metric_set.h"
#include "interop/model/metric_sets/q_metric_set.h"
#include "interop/model/metric_sets/tile_metric_set.h"
#include "interop/model/metric_sets/index_metric_set.h"
%}

%include "interop/model/metric_sets/corrected_intensity_metric_set.h"
%include "interop/model/metric_sets/error_metric_set.h"
%include "interop/model/metric_sets/extraction_metric_set.h"
%include "interop/model/metric_sets/image_metric_set.h"
%include "interop/model/metric_sets/q_metric_set.h"
%include "interop/model/metric_sets/tile_metric_set.h"
%include "interop/model/metric_sets/index_metric_set.h"

%ignore illumina::interop::model::summary::read_summary::begin;
%ignore illumina::interop::model::summary::read_summary::end;
%ignore illumina::interop::model::summary::read_summary::operator[];
%ignore illumina::interop::model::summary::read_summary::read()const;

%ignore illumina::interop::model::summary::run_summary::begin;
%ignore illumina::interop::model::summary::run_summary::end;
%ignore illumina::interop::model::summary::run_summary::operator[];

%{
#include "interop/model/run/cycle_range.h"
#include "interop/model/summary/cycle_state_summary.h"
#include "interop/model/summary/metric_summary.h"
#include "interop/model/summary/lane_summary.h"
#include "interop/model/summary/metric_stat.h"
#include "interop/model/summary/read_summary.h"
#include "interop/model/summary/run_summary.h"


#include "interop/model/run/flowcell_layout.h"
#include "interop/model/run/image_dimensions.h"
#include "interop/model/run/info.h"
#include "interop/model/run/parameters.h"
#include "interop/model/run/read_info.h"

#include "interop/model/run_metrics.h"
#include "interop/logic/metric/q_metric.h"
#include "interop/logic/summary/run_summary.h"
%}


%include "interop/model/run/cycle_range.h"
%include "interop/model/summary/cycle_state_summary.h"
%include "interop/model/summary/metric_summary.h"
%include "interop/model/summary/lane_summary.h"
%include "interop/model/summary/metric_stat.h"
%include "interop/model/summary/read_summary.h"
%include "interop/model/summary/run_summary.h"

%include "interop/model/run/flowcell_layout.h"
%include "interop/model/run/image_dimensions.h"
%include "interop/model/run/info.h"
%include "interop/model/run/parameters.h"
%include "interop/model/run/read_info.h"

//
// Setup typemaps for summary metrics
//
%apply size_t { std::vector<illumina::interop::model::summary::lane_summary>::size_type };
%apply size_t { std::vector<illumina::interop::model::summary::read_summary>::size_type };
%template(lane_summary_vector) std::vector<illumina::interop::model::summary::lane_summary>;
%template(read_summary_vector) std::vector<illumina::interop::model::summary::read_summary>;
%template(read_info_vector) std::vector<illumina::interop::model::run::read_info>;

%include "interop/model/run_metrics.h"
%include "interop/logic/summary/run_summary.h"
%include "interop/logic/metric/q_metric.h"

