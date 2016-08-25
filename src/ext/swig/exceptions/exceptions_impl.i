%include "std_except.i"
#if defined(SWIGCSHARP)
namespace std
{
  %ignore runtime_error;
  struct runtime_error {};
  %ignore out_of_range;
  struct out_of_range {};
  %ignore invalid_argument;
  struct invalid_argument {};
}
#endif
%{
#include "interop/io/stream_exceptions.h"
#include "interop/model/model_exceptions.h"
#include "interop/util/xml_exceptions.h"
%}


#if defined(SWIGCSHARP)

    %include "src/ext/swig/exceptions/exceptions_csharp.i"

#elif defined(SWIGPYTHON)

    %define WRAP_EXCEPTION_IMPORT(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
    %enddef

    %define WRAP_EXCEPTION(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
        %extend NAMESPACE EXCEPTION_CPLUS_PLUS {
            std::string __str__()const{return self->what();}
        }
    %enddef

#endif

%define EXCEPTION_WRAPPER(WRAPPER)

// IO
WRAPPER(illumina::interop::io::, file_not_found_exception, file_not_found_exception)
WRAPPER(illumina::interop::io::, bad_format_exception, bad_format_exception)
WRAPPER(illumina::interop::io::, incomplete_file_exception, incomplete_file_exception)

// Model
WRAPPER(illumina::interop::model::, index_out_of_bounds_exception, index_out_of_bounds_exception)
WRAPPER(illumina::interop::model::, invalid_channel_exception, invalid_channel_exception)
WRAPPER(illumina::interop::model::, invalid_read_exception, invalid_read_exception)
WRAPPER(illumina::interop::model::, invalid_metric_type, invalid_metric_type)
WRAPPER(illumina::interop::model::, invalid_filter_option, invalid_filter_option)
WRAPPER(illumina::interop::model::, invalid_run_info_exception, invalid_run_info_exception)

// XML
WRAPPER(illumina::interop::xml::, xml_file_not_found_exception, xml_file_not_found_exception)
WRAPPER(illumina::interop::xml::, xml_parse_exception, xml_parse_exception)
WRAPPER(illumina::interop::xml::, bad_xml_format_exception, bad_xml_format_exception)
WRAPPER(illumina::interop::xml::, empty_xml_format_exception, empty_xml_format_exception)
WRAPPER(illumina::interop::xml::, missing_xml_element_exception, missing_xml_element_exception)

// Imaging Logic
WRAPPER(illumina::interop::model::, invalid_column_type, invalid_column_type)
%enddef

EXCEPTION_WRAPPER(WRAP_EXCEPTION)


%include "interop/io/stream_exceptions.h"
%include "interop/util/xml_exceptions.h"
%include "interop/model/model_exceptions.h"
