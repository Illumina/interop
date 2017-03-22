// Efficient wrapping of arrays for Java
// Downside of this "fix" upcasting is not currently supported!

// Do not use the following:
//  1. It is inefficient, creating temporary arrays
//  2. It leaks memory in certain situations
//%include "arrays_java.i"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Big Integer Support
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(SWIGWORDSIZE64)

%typemap(jtype) unsigned long, const unsigned long & "java.math.BigInteger"
%typemap(jstype) unsigned long, const unsigned long & "java.math.BigInteger"


%typemap(in) unsigned long {
  jclass clazz;
  jmethodID mid;
  jbyteArray ba;
  jbyte* bae;
  jsize sz;
  int i;

  if (!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "BigInteger null");
    return $null;
  }
  clazz = JCALL1(GetObjectClass, jenv, reinterpret_cast<jobject>($input));
  mid = JCALL3(GetMethodID, jenv, clazz, "toByteArray", "()[B");
  ba = (jbyteArray)JCALL2(CallObjectMethod, jenv, reinterpret_cast<jobject>($input), mid);
  bae = JCALL2(GetByteArrayElements, jenv, ba, 0);
  sz = JCALL1(GetArrayLength, jenv, ba);
  $1 = 0;
  if (sz > 0) {
    $1 = ($1_type)(signed char)bae[0];
    for(i=1; i<sz; i++) {
      $1 = ($1 << 8) | ($1_type)(unsigned char)bae[i];
    }
  }
  JCALL3(ReleaseByteArrayElements, jenv, ba, bae, 0);
}

%typemap(directorout) unsigned long {
  jclass clazz;
  jmethodID mid;
  jbyteArray ba;
  jbyte* bae;
  jsize sz;
  int i;

  if (!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "BigInteger null");
    return $null;
  }
  clazz = JCALL1(GetObjectClass, jenv, reinterpret_cast<jobject>($input));
  mid = JCALL3(GetMethodID, jenv, clazz, "toByteArray", "()[B");
  ba = (jbyteArray)JCALL2(CallObjectMethod, jenv, reinterpret_cast<jobject>($input), mid);
  bae = JCALL2(GetByteArrayElements, jenv, ba, 0);
  sz = JCALL1(GetArrayLength, jenv, ba);
  $result = 0;
  if (sz > 0) {
    $result = ($1_type)(signed char)bae[0];
    for(i=1; i<sz; i++) {
      $result = ($result << 8) | ($1_type)(unsigned char)bae[i];
    }
  }
  JCALL3(ReleaseByteArrayElements, jenv, ba, bae, 0);
}


%typemap(out) unsigned long {
  jbyteArray ba = JCALL1(NewByteArray, jenv, 9);
  jbyte* bae = JCALL2(GetByteArrayElements, jenv, ba, 0);
  jclass clazz = JCALL1(FindClass, jenv, "java/math/BigInteger");
  jmethodID mid = JCALL3(GetMethodID, jenv, clazz, "<init>", "([B)V");
  jobject bigint;
  int i;

  bae[0] = 0;
  for(i=1; i<9; i++ ) {
    bae[i] = (jbyte)($1>>8*(8-i));
  }

  JCALL3(ReleaseByteArrayElements, jenv, ba, bae, 0);
  bigint = JCALL3(NewObject, jenv, clazz, mid, ba);
  $result = reinterpret_cast<jlong>(bigint);
}

%typemap(directorin, descriptor="Ljava/math/BigInteger;") unsigned long, const unsigned long & {
  jbyteArray ba = JCALL1(NewByteArray, jenv, 9);
  jbyte* bae = JCALL2(GetByteArrayElements, jenv, ba, 0);
  jclass clazz = JCALL1(FindClass, jenv, "java/math/BigInteger");
  jmethodID mid = JCALL3(GetMethodID, jenv, clazz, "<init>", "([B)V");
  jobject bigint;
  int swig_i;

  bae[0] = 0;
  for(swig_i=1; swig_i<9; swig_i++ ) {
    bae[swig_i] = (jbyte)($1>>8*(8-swig_i));
  }

  JCALL3(ReleaseByteArrayElements, jenv, ba, bae, 0);
  bigint = JCALL3(NewObject, jenv, clazz, mid, ba);
  $input = reinterpret_cast<jlong>(bigint);
}

%typemap(in) const unsigned long & ($*1_ltype temp) {
  jclass clazz;
  jmethodID mid;
  jbyteArray ba;
  jbyte* bae;
  jsize sz;
  int i;

  if (!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "BigInteger null");
    return $null;
  }
  clazz = JCALL1(GetObjectClass, jenv, reinterpret_cast<jobject>($input));
  mid = JCALL3(GetMethodID, jenv, clazz, "toByteArray", "()[B");
  ba = (jbyteArray)JCALL2(CallObjectMethod, jenv, reinterpret_cast<jobject>($input), mid);
  bae = JCALL2(GetByteArrayElements, jenv, ba, 0);
  sz = JCALL1(GetArrayLength, jenv, ba);
  $1 = &temp;
  temp = 0;
  if (sz > 0) {
    temp = ($*1_ltype)(signed char)bae[0];
    for(i=1; i<sz; i++) {
      temp = (temp << 8) | ($*1_ltype)(unsigned char)bae[i];
    }
  }
  JCALL3(ReleaseByteArrayElements, jenv, ba, bae, 0);
}

%typemap(directorout,warning=SWIGWARN_TYPEMAP_THREAD_UNSAFE_MSG) const unsigned long & {
  static $*1_ltype temp;
  jclass clazz;
  jmethodID mid;
  jbyteArray ba;
  jbyte* bae;
  jsize sz;
  int i;

  if (!$input) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "BigInteger null");
    return $null;
  }
  clazz = JCALL1(GetObjectClass, jenv, reinterpret_cast<jobject>($input));
  mid = JCALL3(GetMethodID, jenv, clazz, "toByteArray", "()[B");
  ba = (jbyteArray)JCALL2(CallObjectMethod, jenv, reinterpret_cast<jobject>($input), mid);
  bae = JCALL2(GetByteArrayElements, jenv, ba, 0);
  sz = JCALL1(GetArrayLength, jenv, ba);
  $result = &temp;
  temp = 0;
  if (sz > 0) {
    temp = ($*1_ltype)(signed char)bae[0];
    for(i=1; i<sz; i++) {
      temp = (temp << 8) | ($*1_ltype)(unsigned char)bae[i];
    }
  }
  JCALL3(ReleaseByteArrayElements, jenv, ba, bae, 0);
}

%typemap(out) const unsigned long & {
  jbyteArray ba = JCALL1(NewByteArray, jenv, 9);
  jbyte* bae = JCALL2(GetByteArrayElements, jenv, ba, 0);
  jclass clazz = JCALL1(FindClass, jenv, "java/math/BigInteger");
  jmethodID mid = JCALL3(GetMethodID, jenv, clazz, "<init>", "([B)V");
  jobject bigint;
  int i;

  bae[0] = 0;
  for(i=1; i<9; i++ ) {
    bae[i] = (jbyte)(*$1>>8*(8-i));
  }

  JCALL3(ReleaseByteArrayElements, jenv, ba, bae, 0);
  bigint = JCALL3(NewObject, jenv, clazz, mid, ba);
  $result = reinterpret_cast<jlong>(bigint);
}

#endif


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Arrays of primitive types use the following macro. The array typemaps use support functions. */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
%define JAVA_ARRAY_TYPEMAPS(CTYPE, JTYPE, JNITYPE, JFUNCNAME)

    %typemap(jni) CTYPE[ANY], CTYPE[]               %{JNITYPE##Array%}
    %typemap(jtype) CTYPE[ANY], CTYPE[]             %{JTYPE[]%}
    %typemap(jstype) CTYPE[ANY], CTYPE[]            %{JTYPE[]%}

    %typemap(in) CTYPE[] (JNITYPE *)
    %{  $1 = reinterpret_cast<CTYPE*>(JCALL2(Get##JFUNCNAME##ArrayElements, jenv, reinterpret_cast<JNITYPE##Array>($input), 0)); %}
    %typemap(in) CTYPE[ANY] (JNITYPE *)
    %{  if ($input && JCALL1(GetArrayLength, jenv, reinterpret_cast<JNITYPE##Array>($input)) != $1_size) {
        SWIG_JavaThrowException(jenv, SWIG_JavaIndexOutOfBoundsException, "incorrect array size");
        return $null;
      }
      $1 = JCALL2(Get##JFUNCNAME##ArrayElements, jenv, reinterpret_cast<JNITYPE##Array>($input), 0); %}
    %typemap(argout) CTYPE[ANY], CTYPE[]
    %{
      JCALL3(Release##JFUNCNAME##ArrayElements, jenv, reinterpret_cast<JNITYPE##Array>($input), reinterpret_cast<JNITYPE*>($1), 0);
      $1 = NULL;
      %}

    %typemap(freearg) CTYPE[ANY], CTYPE[]

    %{
        if ($1)
        {
           JCALL3(Release##JFUNCNAME##ArrayElements, jenv, reinterpret_cast<JNITYPE##Array>($input), reinterpret_cast<JNITYPE*>($1), JNI_ABORT);
        }
    %}


    %typemap(javain) CTYPE[ANY], CTYPE[] "$javainput"
    %typemap(javaout) CTYPE[ANY], CTYPE[] {
        return $jnicall;
      }

    %typemap(memberin) CTYPE[ANY], CTYPE[];
    %typemap(globalin) CTYPE[ANY], CTYPE[];

%enddef

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Arrays of unsigned primitive types use the following macro. The array typemaps use support functions. */
/* Todo figure out how to upcast without leaking memory */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%define JAVA_UPCAST_ARRAY_TYPEMAPS(CTYPE, JTYPE, JNITYPE, JFUNCNAME)

    %typemap(jni) CTYPE[ANY], CTYPE[]               %{JNITYPE##Array%}
    %typemap(jtype) CTYPE[ANY], CTYPE[]             %{JTYPE[]%}
    %typemap(jstype) CTYPE[ANY], CTYPE[]            %{JTYPE[]%}

    %typemap(in) CTYPE[] (JNITYPE *)
    %{
        $1 = reinterpret_cast<CTYPE*>(JCALL2(Get##JFUNCNAME##ArrayElements, jenv, reinterpret_cast<JNITYPE##Array>($input), 0));
    %}

    %typemap(in) CTYPE[ANY] (JNITYPE *)
    %{
        if ($input && JCALL1(GetArrayLength, jenv, reinterpret_cast<JNITYPE##Array>($input)) != $1_size)
        {
            SWIG_JavaThrowException(jenv, SWIG_JavaIndexOutOfBoundsException, "incorrect array size");
            return $null;
        }
        $1 = JCALL2(Get##JFUNCNAME##ArrayElements, jenv, reinterpret_cast<JNITYPE##Array>($input), 0);
    %}

    %typemap(argout) CTYPE[ANY], CTYPE[]
    %{
        JCALL3(Release##JFUNCNAME##ArrayElements, jenv, reinterpret_cast<JNITYPE##Array>($input), reinterpret_cast<JNITYPE*>($1), 0);
        $1 = NULL;
    %}

    %typemap(freearg) CTYPE[ANY], CTYPE[]
    %{
        if ($1)
        {
           JCALL3(Release##JFUNCNAME##ArrayElements, jenv, reinterpret_cast<JNITYPE##Array>($input), reinterpret_cast<JNITYPE*>($1), JNI_ABORT);
        }
    %}


    %typemap(javain) CTYPE[ANY], CTYPE[] "$javainput"
    %typemap(javaout) CTYPE[ANY], CTYPE[]
    {
        return $jnicall;
    }

    %typemap(memberin) CTYPE[ANY], CTYPE[];
    %typemap(globalin) CTYPE[ANY], CTYPE[];
%enddef


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Define the typemaps */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

JAVA_ARRAY_TYPEMAPS(bool, boolean, jboolean, Boolean)
JAVA_ARRAY_TYPEMAPS(signed char, byte, jbyte, Byte)
JAVA_ARRAY_TYPEMAPS(unsigned char, byte, jbyte, Byte)
JAVA_ARRAY_TYPEMAPS(short,  short, jshort, Short)
JAVA_ARRAY_TYPEMAPS(int, int, jint, Int)


JAVA_UPCAST_ARRAY_TYPEMAPS(unsigned short, short, jshort, Short) // TODO: Upcast to int
JAVA_UPCAST_ARRAY_TYPEMAPS(unsigned int, int, jint, Int) // TODO: Upcast to jlong


#if defined(SWIGWORDSIZE64)
JAVA_ARRAY_TYPEMAPS(long, long, jlong, Long)
JAVA_ARRAY_TYPEMAPS(long long, long, jlong, Long)

JAVA_UPCAST_ARRAY_TYPEMAPS(unsigned long long, long, jlong, Long) // TODO: Upcast to BigInteger
JAVA_UPCAST_ARRAY_TYPEMAPS(unsigned long, long, jlong, Long) // TODO: Upcast to BigInteger


#else
JAVA_ARRAY_TYPEMAPS(long, int, jint, Int)
JAVA_ARRAY_TYPEMAPS(long long, long, jlong, Long)

JAVA_UPCAST_ARRAY_TYPEMAPS(unsigned long, int, jint, Int)// TODO: Upcast to jlong
JAVA_UPCAST_ARRAY_TYPEMAPS(unsigned long long, long, jlong, Long) // TODO: Upcast to BigInteger
#endif
JAVA_ARRAY_TYPEMAPS(float, float, jfloat, Float)
JAVA_ARRAY_TYPEMAPS(double, double, jdouble, Double)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


%apply unsigned char[] { uint8_t *buffer};// Allow unsigned char to be wrapped as a byte
%apply (char *STRING, size_t LENGTH) { (uint8_t *buffer, size_t buffer_size) };
%apply unsigned short[] {uint16_t *}
%apply unsigned int[] {uint32_t *}
#if defined(SWIGWORDSIZE64)
%apply unsigned long[] {uint64_t *}
#else
%apply unsigned long long[] {uint64_t *}
#endif

%apply float[] {float *};
