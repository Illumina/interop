%{
#define SWIG_FILE_WITH_INIT
%}
%include "numpy.i"
%init
%{
import_array();
%}

%apply (unsigned char* INPLACE_ARRAY1, int DIM1) {(unsigned char* buffer, size_t buffer_size)}