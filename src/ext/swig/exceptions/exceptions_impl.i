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
  %ignore invalid_parameter;
}
#elif defined(SWIGJAVA)
%typemap(javabase) std::runtime_error "java.lang.Exception";
%typemap(javacode) std::runtime_error %{
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
}
#else
namespace std{
        %std_exception_map(bad_alloc, SWIG_MemoryError);
}
#endif
%{
#include "interop/io/stream_exceptions.h"
#include "interop/model/model_exceptions.h"
#include "interop/util/xml_exceptions.h"
#include "interop/util/exception_specification.h"
%}

%define INTEROP_THROW_SPEC(SPEC) throw SPEC
%enddef


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

