%include "std_except.i"
%include "exception.i"
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
#elif defined(SWIGJAVA)
%typemap(javabase) std::runtime_error "java.lang.Exception";
%typemap(javacode) std::runtime_error %{
  public String getMessage() {
    return what();
  }
%}
%typemap(javabase) std::out_of_range "java.lang.Exception";
%typemap(javacode) std::out_of_range %{
  public String getMessage() {
    return what();
  }
%}
namespace std {
  struct runtime_error
  {
    runtime_error(const string& msg);
    virtual ~runtime_error() throw();
    virtual const char* what() const throw();
  };
  struct out_of_range
  {
    out_of_range(const string& msg);
    virtual ~out_of_range() throw();
    virtual const char* what() const throw();
  };
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

%pythoncode %{
 class exceptionMetaclass(type):

    def __new__(self, name, bases, classdict):
        if Exception not in bases:
            return type(name, (Exception, )+bases, classdict)
        return type(name, bases, classdict)
%}
    %define WRAP_EXCEPTION_IMPORT(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
    %enddef

    %define WRAP_EXCEPTION(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
        %extend NAMESPACE EXCEPTION_CPLUS_PLUS {
            %pythoncode { __metaclass__ = exceptionMetaclass }
            std::string __str__()const{return self->what();}
        }
    %enddef

#elif defined(SWIGJAVA)

    %define WRAP_EXCEPTION_IMPORT(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
    %enddef

    %define WRAP_EXCEPTION(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_JAVA)
        %typemap(throws, throws="EXCEPTION_CPLUS_PLUS") NAMESPACE EXCEPTION_CPLUS_PLUS {
            // the following namespace thing is hacky
          jclass excep = jenv->FindClass("com/illumina/interop/EXCEPTION_CPLUS_PLUS");
          if (excep)
            jenv->ThrowNew(excep, $1.what());
          return $null;
        }
    %enddef

#else

    %define WRAP_EXCEPTION_IMPORT(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
    %enddef

    %define WRAP_EXCEPTION(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
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
