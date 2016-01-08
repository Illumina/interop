
%define WRAP_EXCEPTION_HELPER(EXCEPTION_CSHARP, EXCEPTION_CPLUS_PLUS, ENTRY_POINT)

%insert(runtime) %{
  // Code to handle throwing of C# CustomApplicationException from C/C++ code.
  // The equivalent delegate to the callback, CSharpExceptionCallback_t, is CustomExceptionDelegate
  // and the equivalent customExceptionCallback instance is customDelegate
  typedef void (SWIGSTDCALL* CSharpExceptionCallback_t)(const char *);
  CSharpExceptionCallback_t customExceptionCallback_##EXCEPTION_CSHARP = NULL;

  extern "C" SWIGEXPORT
  void SWIGSTDCALL CustomExceptionRegisterCallback_##EXCEPTION_CSHARP(CSharpExceptionCallback_t customCallback) {
    customExceptionCallback_##EXCEPTION_CSHARP = customCallback;
  }

  // Note that SWIG detects any method calls named starting with
  // SWIG_CSharpSetPendingException for warning 845
  static void SWIG_CSharpSetPendingExceptionCustom_##EXCEPTION_CSHARP(const char *msg) {
    customExceptionCallback_##EXCEPTION_CSHARP(msg);
  }
%}


%pragma(csharp) imclasscode=%{
  class CustomExceptionHelper_##EXCEPTION_CSHARP {
    // C# delegate for the C/C++ customExceptionCallback
    public delegate void CustomExceptionDelegate(string message);
    static CustomExceptionDelegate customDelegate =
                                   new CustomExceptionDelegate(SetPendingCustomException);

    [System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint=#ENTRY_POINT)]
    public static extern
           void CustomExceptionRegisterCallback_##EXCEPTION_CSHARP(CustomExceptionDelegate customCallback);

    static void SetPendingCustomException(string message) {
      SWIGPendingException.Set(new EXCEPTION_CSHARP(message));
    }

    static CustomExceptionHelper_##EXCEPTION_CSHARP() {
      CustomExceptionRegisterCallback_##EXCEPTION_CSHARP(customDelegate);
    }
  }
  static CustomExceptionHelper_##EXCEPTION_CSHARP exceptionHelper_##EXCEPTION_CSHARP = new CustomExceptionHelper_##EXCEPTION_CSHARP();
%}


%typemap(throws, canthrow=1) EXCEPTION_CPLUS_PLUS {
  SWIG_CSharpSetPendingExceptionCustom_##EXCEPTION_CSHARP($1.what());
  return $null;
}

%ignore EXCEPTION_CSHARP(const std::string &mesg);
%typemap(csinterfaces) EXCEPTION_CPLUS_PLUS ""
%typemap(csbase, replace="1") EXCEPTION_CPLUS_PLUS "System.ApplicationException"
%typemap(csbody) EXCEPTION_CPLUS_PLUS ""
%typemap(csdestruct) EXCEPTION_CPLUS_PLUS ""
%typemap(csfinalize) EXCEPTION_CPLUS_PLUS ""
%typemap(cscode) EXCEPTION_CPLUS_PLUS %{
    public EXCEPTION_CSHARP(string mesg) : base(mesg){}
%}

%enddef


%define WRAP_EXCEPTION(EXCEPTION_CSHARP, EXCEPTION_CPLUS_PLUS)
WRAP_EXCEPTION_HELPER(EXCEPTION_CSHARP, EXCEPTION_CPLUS_PLUS, CustomExceptionRegisterCallback_##EXCEPTION_CSHARP)
%enddef


WRAP_EXCEPTION(file_not_found_exception, illumina::interop::io::file_not_found_exception)
WRAP_EXCEPTION(bad_format_exception, illumina::interop::io::bad_format_exception)
WRAP_EXCEPTION(incomplete_file_exception, illumina::interop::io::incomplete_file_exception)

// TODO: find work around for unused function (because this is in a method, not a global function)
//WRAP_EXCEPTION(xml_file_not_found_exception, illumina::interop::xml::xml_file_not_found_exception)


