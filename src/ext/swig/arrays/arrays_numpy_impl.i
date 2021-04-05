%{
#define SWIG_FILE_WITH_INIT
%}
%include "numpy.i"
%init
%{
import_array();
%}

%apply (unsigned char* INPLACE_ARRAY1, int DIM1) {(::uint8_t* buffer, const size_t buffer_size)}
%apply (unsigned char* INPLACE_ARRAY1, int DIM1) {(unsigned char* buffer, const size_t buffer_size)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float* buffer, size_t buffer_size)}
%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(::uint32_t* id_buffer, size_t id_buffer_size)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float* data_beg, const size_t n)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float* input, size_t input_size)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float* output, size_t output_size)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(float* matrix, size_t matrix_size)}

%apply (unsigned int* INPLACE_ARRAY1, int DIM1) {(const ::uint32_t* called_counts, const size_t num_of_counts)}
%apply (unsigned short* INPLACE_ARRAY1, int DIM1) {(const ::uint16_t* intensity_values, const size_t intensity_count)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(const float* focus_scores, const size_t focus_count)}

%apply (unsigned short* INPLACE_ARRAY1, int DIM1) {(const ::uint16_t* buffer, const size_t count)}
%apply (unsigned char* INPLACE_ARRAY1, int DIM1) {(const ::uint8_t* buffer, const size_t count)}
%apply (int* INPLACE_ARRAY1, int DIM1) {(const ::int32_t* buffer, const size_t num_of_counts)}
%apply (short* INPLACE_ARRAY1, int DIM1) {(const ::int16_t* buffer, const size_t count)}
%apply (char* INPLACE_ARRAY1, int DIM1) {(const ::int8_t* buffer, const size_t count)}
%apply (float* INPLACE_ARRAY1, int DIM1) {(const float* buffer, const size_t count)}
%apply (double* INPLACE_ARRAY1, int DIM1) {(const double* buffer, const size_t count)}

#if defined(SWIGWORDSIZE64)
%apply (unsigned long* INPLACE_ARRAY1, int DIM1) {(::uint64_t* data_int_beg, const size_t n)}
%apply (unsigned long* INPLACE_ARRAY1, int DIM1) {(::uint64_t* buffer, const size_t count)}
%apply (long* INPLACE_ARRAY1, int DIM1) {(::int64_t* buffer, const size_t count)}
#else
%apply (unsigned long long* INPLACE_ARRAY1, int DIM1) {(::uint64_t* data_int_beg, const size_t n)}
%apply (unsigned long long* INPLACE_ARRAY1, int DIM1) {(::uint64_t* buffer, const size_t count)}
%apply (long long* INPLACE_ARRAY1, int DIM1) {(const ::int64_t* buffer, const size_t count)}
#endif

