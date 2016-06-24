
%define WRAP_EXCEPTION_HELPER(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP, ENTRY_POINT)

%insert(runtime) %{
  // Code to handle throwing of C# CustomApplicationException from C/C++ code.
  // The equivalent delegate to the callback, CSharpExceptionCallback_t, is CustomExceptionDelegate
  // and the equivalent customExceptionCallback instance is customDelegate
  typedef void (SWIGSTDCALL* CSharpExceptionCallback_t)(const char *);
  CSharpExceptionCallback_t customExceptionCallback_##EXCEPTION_CPLUS_PLUS = NULL;

  extern "C" SWIGEXPORT
  void SWIGSTDCALL CustomExceptionRegisterCallback_##EXCEPTION_CPLUS_PLUS(CSharpExceptionCallback_t customCallback) {
    customExceptionCallback_##EXCEPTION_CPLUS_PLUS = customCallback;
  }

  // Note that SWIG detects any method calls named starting with
  // SWIG_CSharpSetPendingException for warning 845
  static void SWIG_CSharpSetPendingExceptionCustom_##EXCEPTION_CPLUS_PLUS(const char *msg) {
    customExceptionCallback_##EXCEPTION_CPLUS_PLUS(msg);
  }
%}


%pragma(csharp) imclasscode=%{
  class CustomExceptionHelper_##EXCEPTION_CPLUS_PLUS {
    // C# delegate for the C/C++ customExceptionCallback
    public delegate void CustomExceptionDelegate(string message);
    static CustomExceptionDelegate customDelegate =
                                   new CustomExceptionDelegate(SetPendingCustomException);

    [System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint=#ENTRY_POINT)]
    public static extern
           void CustomExceptionRegisterCallback_##EXCEPTION_CPLUS_PLUS(CustomExceptionDelegate customCallback);

    static void SetPendingCustomException(string message) {
      SWIGPendingException.Set(new EXCEPTION_CSHARP(message));
    }

    static CustomExceptionHelper_##EXCEPTION_CPLUS_PLUS() {
      CustomExceptionRegisterCallback_##EXCEPTION_CPLUS_PLUS(customDelegate);
    }
  }
  static CustomExceptionHelper_##EXCEPTION_CPLUS_PLUS exceptionHelper_##EXCEPTION_CPLUS_PLUS = new CustomExceptionHelper_##EXCEPTION_CPLUS_PLUS();
%}


%typemap(throws, canthrow=1) NAMESPACE EXCEPTION_CPLUS_PLUS {
  SWIG_CSharpSetPendingExceptionCustom_##EXCEPTION_CPLUS_PLUS($1.what());
  return $null;
}

%ignore EXCEPTION_CPLUS_PLUS(const std::string &mesg);
%typemap(csinterfaces) NAMESPACE EXCEPTION_CPLUS_PLUS ""
%typemap(csbase, replace="1") NAMESPACE EXCEPTION_CPLUS_PLUS "System.ApplicationException"
%typemap(csbody) NAMESPACE EXCEPTION_CPLUS_PLUS ""
%typemap(csdestruct) NAMESPACE EXCEPTION_CPLUS_PLUS ""
%typemap(csfinalize) NAMESPACE EXCEPTION_CPLUS_PLUS ""
%typemap(cscode) NAMESPACE EXCEPTION_CPLUS_PLUS %{
    public $csclassname(string mesg) : base(mesg){}
%}

%enddef


%define WRAP_EXCEPTION(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
//%rename("%(camelcase)s", %$isclass) EXCEPTION_CPLUS_PLUS;
WRAP_EXCEPTION_HELPER(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP, CustomExceptionRegisterCallback_##EXCEPTION_CPLUS_PLUS)
%enddef


