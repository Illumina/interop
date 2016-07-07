/** Model to describe a Run
 */

%include <std_string.i>
%include <std_vector.i>
%include <stdint.i>
%include "src/ext/swig/exceptions/exceptions_impl.i"

%{
#include "interop/interop.h"
#include "interop/model/run/cycle_range.h"
#include "interop/model/run/read_info.h"
#include "interop/model/run/flowcell_layout.h"
#include "interop/model/run/image_dimensions.h"
#include "interop/model/run/info.h"
#include "interop/model/run/parameters.h"
%}

%template(string_vector) std::vector< std::string  >;

%include "interop/interop.h"
%include "interop/constants/enums.h"
%include "interop/model/run/cycle_range.h"
%include "interop/model/run/read_info.h"
%include "interop/model/run/flowcell_layout.h"
%include "interop/model/run/image_dimensions.h"
%include "interop/model/run/info.h"
%include "interop/model/run/parameters.h"

%template(read_info_vector) std::vector<illumina::interop::model::run::read_info>;