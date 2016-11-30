
#if defined(SWIGCSHARP)
%include "arrays_csharp_impl.i"
#elif defined(SWIGJAVA)

%include "arrays_java.i"

%apply signed char[] { uint8_t *buffer};// Allow unsigned char to be wrapped as a byte
%apply (char *STRING, size_t LENGTH) { (uint8_t *buffer, size_t buffer_size) };
%apply int[] {uint16_t *}
%apply long[] {uint32_t *}
%apply long[] {uint64_t *}

%apply float[] {float *};

#elif defined(SWIGPYTHON)
%include "arrays_numpy_impl.i"
#endif

// Collection of methods we need to ignore in a common vector interface
// Collection of typedefs we need to make SWIG aware of in a common vector interface
%define WRAP_VECTOR(vector_t)
%ignore vector_t::iterator;
%ignore vector_t::begin;
%ignore vector_t::end;
%ignore vector_t::operator[];
%apply size_t { vector_t::size_type };
%ignore vector_t::at(const size_type) const;
%enddef


%define WRAP_AS_VECTOR(value_t)
%apply value_t& { std::vector<value_t>::reference };
//%apply value_t& const  { std::vector<value_t>::const_reference };
WRAP_VECTOR(std::vector<value_t>);
%enddef

