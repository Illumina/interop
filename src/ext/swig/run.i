/** Model to describe a Run
 */

%include <std_string.i>
%include <std_vector.i>
%include <stdint.i>
%include "src/ext/swig/exceptions/exceptions_impl.i"
%include "util/operator_overload.i"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ignore methods that should not be wrapped
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%ignore illumina::interop::model::metrics::q_score_bin::operator=;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("interop_run");
  }
%}


%{
#include "interop/interop.h"
#include "interop/model/run/cycle_range.h"
#include "interop/model/run/read_info.h"
#include "interop/model/run/flowcell_layout.h"
#include "interop/model/run/image_dimensions.h"
#include "interop/model/run/info.h"
#include "interop/model/run/run_exceptions.h"
#include "interop/model/run/parameters.h"
#include "interop/logic/metric/q_metric.h"
#include "interop/logic/utils/enums.h"
#include "interop/util/xml_exceptions.h"
%}

// Exceptions

%define RUN_EXCEPTION_WRAPPER(WRAPPER)
WRAPPER(illumina::interop::util::, base_exception, base_exception)
// XML
WRAPPER(illumina::interop::xml::, xml_file_not_found_exception, xml_file_not_found_exception)
WRAPPER(illumina::interop::xml::, xml_format_exception, xml_format_exception)
WRAPPER(illumina::interop::xml::, xml_parse_exception, xml_parse_exception)
WRAPPER(illumina::interop::xml::, bad_xml_format_exception, bad_xml_format_exception)
WRAPPER(illumina::interop::xml::, empty_xml_format_exception, empty_xml_format_exception)
WRAPPER(illumina::interop::xml::, missing_xml_element_exception, missing_xml_element_exception)
// RunInfo
WRAPPER(illumina::interop::model::, invalid_read_exception, invalid_read_exception)
WRAPPER(illumina::interop::model::, invalid_tile_naming_method, invalid_tile_naming_method)
WRAPPER(illumina::interop::model::, invalid_run_info_exception, invalid_run_info_exception)
%enddef

RUN_EXCEPTION_WRAPPER(WRAP_EXCEPTION)
%include "interop/util/base_exception.h"
%include "interop/util/xml_exceptions.h"
%include "interop/model/run/run_exceptions.h"


// Primitive array types
%template(string_vector) std::vector< std::string  >;
%template(ulong_vector) std::vector< uint64_t  >;
%template(ushort_vector) std::vector< uint16_t >;
%template(uint_vector) std::vector< uint32_t >;
%template(float_vector) std::vector< float >;
%template(bool_vector) std::vector< bool >;
%template(uchar_vector) std::vector< uint8_t >;

#if !defined(SWIGPYTHON)
%template(size_vector) std::vector< size_t  >;
#endif

%include "interop/interop.h"
%include "interop/constants/enums.h"
%include "interop/constants/enum_description.h"
%include "interop/model/run/cycle_range.h"
%include "interop/model/run/read_info.h"
%include "interop/model/run/flowcell_layout.h"
%include "interop/model/run/image_dimensions.h"
%include "interop/model/run/info.h"
%include "interop/model/run/parameters.h"
%include "interop/logic/utils/enums.h"

%template(read_info_vector) std::vector<illumina::interop::model::run::read_info>;



%define WRAP_ENUM(ENUM)
%template(list_##ENUM) illumina::interop::constants::list_enum_names< illumina::interop::constants:: ENUM >;
%template(parse_##ENUM) illumina::interop::constants::parse< illumina::interop::constants:: ENUM >;
#if defined(SWIGPYTHON)
%template(to_string_##ENUM) illumina::interop::constants::to_string< illumina::interop::constants:: ENUM >;
#else
%template(to_string) illumina::interop::constants::to_string< illumina::interop::constants:: ENUM >;
#endif
%enddef

WRAP_ENUM(metric_type)
WRAP_ENUM(metric_group)
WRAP_ENUM(tile_naming_method)
WRAP_ENUM(dna_bases)
WRAP_ENUM(surface_type)
WRAP_ENUM(instrument_type)
WRAP_ENUM(metric_base_type)
WRAP_ENUM(plot_colors)
WRAP_ENUM(bar_plot_options)
WRAP_ENUM(metric_data)
WRAP_ENUM(metric_feature_type)

