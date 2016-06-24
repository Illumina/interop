%include "std_except.i"
#if defined(SWIGCSHARP)
namespace std
{
  %ignore runtime_error;
  struct runtime_error {};
  %ignore out_of_range;
  struct out_of_range {};
}
#endif
%{
#include "interop/io/stream_exceptions.h"
#include "interop/model/model_exceptions.h"
#include "interop/util/xml_exceptions.h"
%}
//%include "interop/model/model_exceptions.h"


#if defined(SWIGCSHARP)

    %include "src/ext/swig/exceptions/exceptions_csharp.i"

    %typemap(throws, canthrow=1) std::invalid_argument {
        SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.what());
        return $null;
    }

#elif defined(SWIGPYTHON)

    %define WRAP_EXCEPTION(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
        %extend NAMESPACE EXCEPTION_CPLUS_PLUS {
            std::string __str__()const{return self->what();}
        }
%enddef

#endif

WRAP_EXCEPTION(illumina::interop::io::, file_not_found_exception, file_not_found_exception)
WRAP_EXCEPTION(illumina::interop::io::, bad_format_exception, bad_format_exception)
WRAP_EXCEPTION(illumina::interop::io::, incomplete_file_exception, incomplete_file_exception)

//WRAP_EXCEPTION(std::, invalid_argument, invalid_argument)

WRAP_EXCEPTION(illumina::interop::model::, index_out_of_bounds_exception, index_out_of_bounds_exception)
WRAP_EXCEPTION(illumina::interop::model::, invalid_channel_exception, invalid_channel_exception)
WRAP_EXCEPTION(illumina::interop::model::, invalid_read_exception, invalid_read_exception)
WRAP_EXCEPTION(illumina::interop::model::, invalid_metric_type, invalid_metric_type)
// todo enable with imaging logic
//WRAP_EXCEPTION(illumina::interop::model::, invalid_column_type, invalid_column_type)


// TODO: support proper inheritance of exceptions for c#
//WRAP_EXCEPTION(illumina::interop::xml::, xml_format_exception, xml_format_exception)
WRAP_EXCEPTION(illumina::interop::xml::, xml_file_not_found_exception, xml_file_not_found_exception)
WRAP_EXCEPTION(illumina::interop::xml::, xml_parse_exception, xml_parse_exception)
WRAP_EXCEPTION(illumina::interop::xml::, bad_xml_format_exception, bad_xml_format_exception)
WRAP_EXCEPTION(illumina::interop::xml::, empty_xml_format_exception, empty_xml_format_exception)
WRAP_EXCEPTION(illumina::interop::xml::, missing_xml_element_exception, missing_xml_element_exception)

%include "interop/io/stream_exceptions.h"
%include "interop/util/xml_exceptions.h"
%include "interop/model/model_exceptions.h"
