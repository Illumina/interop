%include <arrays_csharp.i>
%include <typemaps.i>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The CSharp Wrapper does not properly handle 64-bit integer types
// This is a workaround that is consistent with the SWIG interface: stdint.i
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(SWIGWORDSIZE64)


%define INPUT_TYPEMAP(TYPE, CTYPE, CSTYPE)
%typemap(ctype, out="void *") TYPE *INPUT, TYPE &INPUT "CTYPE"
%typemap(imtype, out="global::System.IntPtr") TYPE *INPUT, TYPE &INPUT "CSTYPE"
%typemap(cstype, out="$csclassname") TYPE *INPUT, TYPE &INPUT "CSTYPE"
%typemap(csin) TYPE *INPUT, TYPE &INPUT "$csinput"

%typemap(in) TYPE *INPUT, TYPE &INPUT
%{ $1 = ($1_ltype)&$input; %}

%typemap(typecheck) TYPE *INPUT = TYPE;
%typemap(typecheck) TYPE &INPUT = TYPE;
%enddef

%define OUTPUT_TYPEMAP(TYPE, CTYPE, CSTYPE, TYPECHECKPRECEDENCE)
%typemap(ctype, out="void *") TYPE *OUTPUT, TYPE &OUTPUT "CTYPE *"
%typemap(imtype, out="global::System.IntPtr") TYPE *OUTPUT, TYPE &OUTPUT "out CSTYPE"
%typemap(cstype, out="$csclassname") TYPE *OUTPUT, TYPE &OUTPUT "out CSTYPE"
%typemap(csin) TYPE *OUTPUT, TYPE &OUTPUT "out $csinput"

%typemap(in) TYPE *OUTPUT, TYPE &OUTPUT
%{ $1 = ($1_ltype)$input; %}

%typecheck(SWIG_TYPECHECK_##TYPECHECKPRECEDENCE) TYPE *OUTPUT, TYPE &OUTPUT ""
%enddef

%define INOUT_TYPEMAP(TYPE, CTYPE, CSTYPE, TYPECHECKPRECEDENCE)
%typemap(ctype, out="void *") TYPE *INOUT, TYPE &INOUT "CTYPE *"
%typemap(imtype, out="global::System.IntPtr") TYPE *INOUT, TYPE &INOUT "ref CSTYPE"
%typemap(cstype, out="$csclassname") TYPE *INOUT, TYPE &INOUT "ref CSTYPE"
%typemap(csin) TYPE *INOUT, TYPE &INOUT "ref $csinput"

%typemap(in) TYPE *INOUT, TYPE &INOUT
%{ $1 = ($1_ltype)$input; %}

%typecheck(SWIG_TYPECHECK_##TYPECHECKPRECEDENCE) TYPE *INOUT, TYPE &INOUT ""
%enddef

INPUT_TYPEMAP(long int,               long int,                 long)
INPUT_TYPEMAP(unsigned long int,      unsigned long int,        ulong)

OUTPUT_TYPEMAP(long int,               long int,                 long,      INT64_PTR)
OUTPUT_TYPEMAP(unsigned long int,      unsigned long int,        ulong,     UINT64_PTR)

INOUT_TYPEMAP(long int,               long int,                 long,      INT64_PTR)
INOUT_TYPEMAP(unsigned long int,      unsigned long int,        ulong,     UINT64_PTR)


#undef INPUT_TYPEMAP
#undef OUTPUT_TYPEMAP
#undef INOUT_TYPEMAP

%typemap(imtype) long,          const long &          "long"
%typemap(imtype) unsigned long, const unsigned long & "ulong"
%typemap(cstype) long,          const long &          "long"
%typemap(cstype) unsigned long, const unsigned long & "ulong"

%typemap(csout, excode=SWIGEXCODE) long, const long & {
    long ret = $imcall;$excode
    return ret;
    }
%typemap(csvarout, excode=SWIGEXCODE2) long, const long & %{
    get {
        long ret = $imcall;$excode
        return ret;
        } %}

%typemap(csout, excode=SWIGEXCODE) unsigned long, const unsigned long & {
    ulong ret = $imcall;$excode
    return ret;
    }
%typemap(csvarout, excode=SWIGEXCODE2) unsigned long, const unsigned long & %{
    get {
        ulong ret = $imcall;$excode
        return ret;
        } %}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fix bug in older versions of SWIG
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%define CSHARP_ARRAYS_FIXED2( CTYPE, CSTYPE )

%typemap(ctype)   CSTYPE FIXED[] "CTYPE*"
%typemap(imtype)  CSTYPE FIXED[] "global::System.IntPtr"
%typemap(cstype)  CSTYPE FIXED[] "CSTYPE[]"
%typemap(csin,
           pre=       "    unsafe{ fixed ( CSTYPE* swig_ptrTo_$csinput = $csinput ) {",
           terminator="    }}")
                  CSTYPE FIXED[] "(global::System.IntPtr)swig_ptrTo_$csinput"

%typemap(in)      CSTYPE FIXED[] "$1 = $input;"
%typemap(freearg) CSTYPE FIXED[] ""
%typemap(argout)  CSTYPE FIXED[] ""


%enddef // CSHARP_ARRAYS_FIXED

CSHARP_ARRAYS_FIXED2(uint8_t, byte)
CSHARP_ARRAYS_FIXED2(uint16_t, ushort)
CSHARP_ARRAYS_FIXED2(uint32_t, uint)
CSHARP_ARRAYS_FIXED2(uint64_t, ulong)
CSHARP_ARRAYS_FIXED2(float, float)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%apply byte FIXED[] {uint8_t *buffer} // Confine this to only variables named buffer for now
%apply ushort FIXED[] {uint16_t *}
%apply uint FIXED[] {uint32_t *}
%apply ulong FIXED[] {uint64_t *}

%apply float FIXED[] {float *}

//%apply byte {unsigned char};



