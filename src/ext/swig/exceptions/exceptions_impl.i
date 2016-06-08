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

WRAP_EXCEPTION(illumina::interop::io::, file_not_found_exception, FileNotFoundException)
WRAP_EXCEPTION(illumina::interop::io::, bad_format_exception, BadFormatException)
WRAP_EXCEPTION(illumina::interop::io::, incomplete_file_exception, IncompleteFileException)

//WRAP_EXCEPTION(std::, invalid_argument, invalid_argument)

WRAP_EXCEPTION(illumina::interop::model::, index_out_of_bounds_exception, IndexOutOfBoundsException)
WRAP_EXCEPTION(illumina::interop::model::, invalid_channel_exception, InvalidChannelException)
WRAP_EXCEPTION(illumina::interop::model::, invalid_read_exception, InvalidReadException)
WRAP_EXCEPTION(illumina::interop::model::, invalid_metric_type, InvalidMetricType)
// todo enable with imaging logic
//WRAP_EXCEPTION(illumina::interop::model::, invalid_column_type, InvalidColumnTypeException)


// TODO: support proper inheritance of exceptions for c#
//WRAP_EXCEPTION(illumina::interop::xml::, xml_format_exception, XmlFormatException)
WRAP_EXCEPTION(illumina::interop::xml::, xml_file_not_found_exception, XmlFileNotFoundException)
WRAP_EXCEPTION(illumina::interop::xml::, xml_parse_exception, XmlParseException)
WRAP_EXCEPTION(illumina::interop::xml::, bad_xml_format_exception, BadXmlFormatException)
WRAP_EXCEPTION(illumina::interop::xml::, empty_xml_format_exception, EmptyXmlFormatException)
WRAP_EXCEPTION(illumina::interop::xml::, missing_xml_element_exception, MissingXmlElementException)

%include "interop/io/stream_exceptions.h"
%include "interop/util/xml_exceptions.h"
%include "interop/model/model_exceptions.h"
