
#if defined(SWIGCSHARP)
    %include "extends_csharp.i"
#else
    %define EXTEND_METRIC_SET(metric_t)
    %enddef
    %define EXTEND_CYCLE_METRIC_SET(metric_t)
    %enddef
    %define EXTEND_Q_METRIC(metric_t)
    %enddef
    %define EXTEND_TILE_METRIC(metric_t)
    %enddef
#endif
