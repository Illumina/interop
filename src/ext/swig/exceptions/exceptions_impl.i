%include "std_except.i"
#if defined(SWIGCSHARP)
namespace std
{
  %ignore runtime_error;
  struct runtime_error {};
}
#endif
%{
#include "interop/io/stream_exceptions.h"
#include "interop/model/model_exceptions.h"
#include "interop/util/xml_exceptions.h"
%}


#if defined(SWIGCSHARP)
%include "src/ext/swig/exceptions/exceptions_csharp.i"

// TODO: use built in c++ stdexcept
%typemap(throws, canthrow=1) illumina::interop::model::index_out_of_bounds_exception {
  SWIG_CSharpSetPendingException(SWIG_CSharpIndexOutOfRangeException, $1.what());
  return $null;
}

// TODO: wrap these
%typemap(throws, canthrow=1) illumina::interop::xml::xml_format_exception {
  SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.what());
  return $null;
}
%typemap(throws, canthrow=1) illumina::interop::xml::xml_file_not_found_exception {
  SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.what());
  return $null;
}
%typemap(throws, canthrow=1) illumina::interop::xml::xml_parse_exception {
  SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.what());
  return $null;
}
%typemap(throws, canthrow=1) illumina::interop::xml::bad_xml_format_exception {
  SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.what());
  return $null;
}
%typemap(throws, canthrow=1) illumina::interop::xml::empty_xml_format_exception {
  SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.what());
  return $null;
}
%typemap(throws, canthrow=1) illumina::interop::xml::missing_xml_element_exception {
  SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, $1.what());
  return $null;
}


#elif defined(SWIGPYTHON)


%extend illumina::interop::io::file_not_found_exception{
    std::string __str__()const{return self->what();}
}

%extend illumina::interop::io::format_exception{
    std::string __str__()const{return self->what();}
}

%extend illumina::interop::io::bad_format_exception{
    std::string __str__()const{return self->what();}
}

%extend illumina::interop::io::incomplete_file_exception{
    std::string __str__()const{return self->what();}
}

%extend  illumina::interop::model::index_out_of_bounds_exception {
    std::string __str__()const{return self->what();}
}

%extend  illumina::interop::xml::xml_format_exception {
    std::string __str__()const{return self->what();}
}
%extend  illumina::interop::xml::xml_file_not_found_exception {
    std::string __str__()const{return self->what();}
}
%extend  illumina::interop::xml::xml_parse_exception {
    std::string __str__()const{return self->what();}
}
%extend  illumina::interop::xml::bad_xml_format_exception {
    std::string __str__()const{return self->what();}
}
%extend  illumina::interop::xml::empty_xml_format_exception {
    std::string __str__()const{return self->what();}
}
%extend  illumina::interop::xml::missing_xml_element_exception {
    std::string __str__()const{return self->what();}
}



#endif


%include "interop/io/stream_exceptions.h"
%include "interop/util/xml_exceptions.h"
%include "interop/model/model_exceptions.h"
