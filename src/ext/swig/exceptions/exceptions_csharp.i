/** Catch the C++ exception and throw a C# exception
 */
%define WRAP_EXCEPTION_IMPORT1(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP, ENTRY_POINT)

%insert(runtime) %{
  // Code to handle throwing of C# CustomApplicationException from C/C++ code.
  // The equivalent delegate to the callback, CSharpExceptionCallback_t, is CustomExceptionDelegate
  // and the equivalent customExceptionCallback instance is customDelegate
  typedef void (SWIGSTDCALL* CSharpExceptionCallback_t)(const char *);
  static CSharpExceptionCallback_t customExceptionCallback_##EXCEPTION_CPLUS_PLUS = NULL;

  extern "C" SWIGEXPORT
  void SWIGSTDCALL ENTRY_POINT(CSharpExceptionCallback_t customCallback) {
    customExceptionCallback_##EXCEPTION_CPLUS_PLUS = customCallback;
  }

  // Note that SWIG detects any method calls named starting with
  // SWIG_CSharpSetPendingException for warning 845
  static void SWIGUNUSED SWIG_CSharpSetPendingExceptionCustom_##EXCEPTION_CPLUS_PLUS##_##ENTRY_POINT(const char *msg) {
    customExceptionCallback_##EXCEPTION_CPLUS_PLUS(msg);
  }
%}

%pragma(csharp) imclasscode=%{
  class CustomExceptionHelper_##EXCEPTION_CPLUS_PLUS {
    // C# delegate for the C/C++ customExceptionCallback
    public delegate void CustomExceptionDelegate(string message);
    static CustomExceptionDelegate customDelegate =
                                   new CustomExceptionDelegate(SetPendingCustomException);

    [global::System.Runtime.InteropServices.DllImport("$dllimport", EntryPoint=#ENTRY_POINT)]
    public static extern
           void ENTRY_POINT(CustomExceptionDelegate customCallback);

    static void SetPendingCustomException(string message) {
      SWIGPendingException.Set(new Illumina.InterOp.Run.EXCEPTION_CSHARP(message, SWIGPendingException.Retrieve()));
    }

    static CustomExceptionHelper_##EXCEPTION_CPLUS_PLUS() {
      ENTRY_POINT(customDelegate);
    }
  }
  #pragma warning disable 0414 // Need to keep a reference to this object
  static CustomExceptionHelper_##EXCEPTION_CPLUS_PLUS exceptionHelper_##EXCEPTION_CPLUS_PLUS = new CustomExceptionHelper_##EXCEPTION_CPLUS_PLUS();
  #pragma warning restore 0414
%}


%typemap(throws, canthrow=1) NAMESPACE EXCEPTION_CPLUS_PLUS {
  SWIG_CSharpSetPendingExceptionCustom_##EXCEPTION_CPLUS_PLUS##_##ENTRY_POINT($1.what());
  return $null;
}

%enddef

%define WRAP_EXCEPTION_HELPER(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP, ENTRY_POINT)
WRAP_EXCEPTION_IMPORT1(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP, ENTRY_POINT)


%ignore EXCEPTION_CPLUS_PLUS(const std::string &mesg);
%typemap(csinterfaces) NAMESPACE EXCEPTION_CPLUS_PLUS ""
%typemap(csbase, replace="1") NAMESPACE EXCEPTION_CPLUS_PLUS "global::System.ApplicationException"
%typemap(csbody) NAMESPACE EXCEPTION_CPLUS_PLUS ""
%typemap(csdestruct) NAMESPACE EXCEPTION_CPLUS_PLUS ""
%typemap(csfinalize) NAMESPACE EXCEPTION_CPLUS_PLUS ""
%typemap(cscode) NAMESPACE EXCEPTION_CPLUS_PLUS %{
    public $csclassname(string mesg) : base(mesg){}
    public $csclassname(string mesg, global::System.Exception inner) : base(mesg, inner){}
%}

%enddef


%define WRAP_EXCEPTION_IMPORT(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
WRAP_EXCEPTION_IMPORT1(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP, ImportCustomExceptionRegisterCallback_##EXCEPTION_CPLUS_PLUS)
%enddef
%define WRAP_EXCEPTION(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP)
WRAP_EXCEPTION_HELPER(NAMESPACE, EXCEPTION_CPLUS_PLUS, EXCEPTION_CSHARP, CustomExceptionRegisterCallback_##EXCEPTION_CPLUS_PLUS)
%enddef


