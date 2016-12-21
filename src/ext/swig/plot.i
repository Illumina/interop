/** Plotting model and logic
 */

%include <std_string.i>
%include <stdint.i>
%include <std_vector.i>
%include "util/operator_overload.i"

//////////////////////////////////////////////
// Don't wrap it, just use it with %import
//////////////////////////////////////////////
%import "src/ext/swig/exceptions/exceptions_impl.i"
%include "src/ext/swig/arrays/arrays_impl.i"
%import "src/ext/swig/run.i"
%import "src/ext/swig/metrics.i"


// Ensure all the modules import the shared namespace
%pragma(csharp) moduleimports=%{
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
%}

// Ensure each of the generated C# class import the shared code
%typemap(csimports) SWIGTYPE %{
using System;
using System.Runtime.InteropServices;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Run;
%}

%pragma(java) jniclasscode=%{
  static {
    System.loadLibrary("interop_plot");
  }
%}

// This imports the metrics
WRAP_METRICS(IMPORT_METRIC_WRAPPER)
// This allows exceptions to be imported, but not belong to the module
EXCEPTION_WRAPPER(WRAP_EXCEPTION_IMPORT)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Plotting
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%{
#include "interop/model/plot/axes.h"
#include "interop/model/plot/candle_stick_point.h"
#include "interop/model/plot/bar_point.h"
#include "interop/model/plot/data_point.h"
#include "interop/model/plot/data_point_collection.h"
#include "interop/model/plot/filter_options.h"
#include "interop/model/plot/plot_data.h"
#include "interop/model/plot/series.h"
#include "interop/model/plot/chart_data.h"
#include "interop/model/plot/heatmap_data.h"
#include "interop/model/plot/flowcell_data.h"
%}

%ignore illumina::interop::model::plot::flowcell_data::tile_id(size_t const,size_t const);
RENAME_TEMPLATE_OPERATOR_CONST(illumina::interop::model::plot::heatmap_data);
%ignore illumina::interop::model::plot::filter_options::option_iterator;

%include "interop/model/plot/axes.h"
%include "interop/model/plot/filter_options.h"
%include "interop/model/plot/chart_data.h"
%include "interop/model/plot/heatmap_data.h"
%include "interop/model/plot/flowcell_data.h"


%include "interop/model/plot/data_point.h"

%apply float { illumina::interop::model::plot::data_point<float, float>::x_type };
%apply float { illumina::interop::model::plot::data_point<float, float>::y_type };
%template(float_point) illumina::interop::model::plot::data_point<float, float>;

%include "interop/model/plot/candle_stick_point.h"
%include "interop/model/plot/bar_point.h"
%include "interop/model/plot/data_point_collection.h"

WRAP_VECTOR(std::vector<illumina::interop::model::plot::bar_point>)
%template(bar_vector) std::vector<illumina::interop::model::plot::bar_point>;
WRAP_VECTOR(std::vector<illumina::interop::model::plot::candle_stick_point>)
%template(candle_stick_vector) std::vector<illumina::interop::model::plot::candle_stick_point>;

%template(candle_stick_collection) illumina::interop::model::plot::data_point_collection<illumina::interop::model::plot::candle_stick_point>;
%template(bar_collection) illumina::interop::model::plot::data_point_collection<illumina::interop::model::plot::bar_point>;

%include "interop/model/plot/series.h"

%template(candle_stick_series) illumina::interop::model::plot::series<illumina::interop::model::plot::candle_stick_point>;
%template(bar_series) illumina::interop::model::plot::series<illumina::interop::model::plot::bar_point>;

%include "interop/model/plot/plot_data.h"

%template(candle_stick_plot_data) illumina::interop::model::plot::plot_data<illumina::interop::model::plot::candle_stick_point>;

%template(bar_plot_data) illumina::interop::model::plot::plot_data<illumina::interop::model::plot::bar_point>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Plotting logic
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

%{
#include "interop/logic/plot/plot_by_cycle.h"
#include "interop/logic/plot/plot_by_lane.h"
#include "interop/logic/plot/plot_qscore_histogram.h"
#include "interop/logic/plot/plot_qscore_heatmap.h"
#include "interop/logic/plot/plot_flowcell_map.h"
#include "interop/logic/plot/plot_sample_qc.h"
%}
%include "interop/logic/plot/plot_by_cycle.h"
%include "interop/logic/plot/plot_by_lane.h"
%include "interop/logic/plot/plot_qscore_histogram.h"
%include "interop/logic/plot/plot_qscore_heatmap.h"
%include "interop/logic/plot/plot_flowcell_map.h"
%include "interop/logic/plot/plot_sample_qc.h"
