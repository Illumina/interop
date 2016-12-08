%{
#define SWIG_FILE_WITH_INIT
%}
%include "numpy.i"
%init
%{
import_array();
%}

%apply (unsigned char* INPLACE_ARRAY1, int DIM1) {(::uint8_t* buffer, size_t buffer_size)}
%apply (unsigned char* INPLACE_ARRAY1, int DIM1) {(unsigned char* buffer, size_t buffer_size)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float* buffer, size_t buffer_size)}
%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(::uint32_t* id_buffer, size_t id_buffer_size)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float* data_beg, const size_t n)}

