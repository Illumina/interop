
%define IMPORT_METRIC_WRAPPER(METRIC_NAME)
    %{
    // using wrapper
    #include "interop/model/metrics/METRIC_NAME.h"
    %}
%enddef
%define INCLUDE_METRIC_WRAPPER(METRIC_NAME)
    %{
        // using wrapper
        #include "interop/model/metrics/METRIC_NAME.h"
    %}
    %include "interop/model/metrics/METRIC_NAME.h"
%enddef

%define WRAP_METRICS(WRAPPER)
    WRAPPER(corrected_intensity_metric)
    WRAPPER(error_metric)
    WRAPPER(extraction_metric)
    WRAPPER(image_metric)
    WRAPPER(q_metric)
    WRAPPER(tile_metric)
    WRAPPER(index_metric)
    WRAPPER(q_collapsed_metric)
    WRAPPER(q_by_lane_metric)
%enddef

WRAP_METRICS(INCLUDE_METRIC_WRAPPER)
