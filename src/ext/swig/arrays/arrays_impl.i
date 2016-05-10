
#if defined(SWIGCSHARP)
%include "arrays_csharp_impl.i"
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
%enddef


%define WRAP_AS_VECTOR(value_t)
WRAP_VECTOR(std::vector<value_t>);
%enddef

