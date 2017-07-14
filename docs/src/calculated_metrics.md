# InterOp Calculations      {#calculated_metrics}

This page describes the derived metrics calculated from the InterOp files. 

## Summary Tab

This section describes each metric shown in the SAV summary tab.

 - @subpage q_metrics_requirement_q30 "% >= Q30"
 - @subpage q_metrics_requirement_yield "Yield (G)"
 - @subpage q_metrics_requirement_projected_yield "Projected Yield (G)"
 - @subpage error_metrics_requirement "% Error"

### A note on usable cycles. 

Metrics that average over all cycles in the summary tab, actually average over all "usable cycles".  We define a usable 
cycle as one that is fully corrected in terms of phasing/prephasing. For this reason, we don’t consider the last cycle 
of a run as usable because it is not fully corrected. So, we don’t count this cycle toward yield or q30 or error rate. 
Many aligners drop the last cycle for this reason.

### A note on phasing weights

The phasing weights have a factor of 100x baked in. This means the actual phasing weight can be derived by dividing by 
100. The phasing rate, slope and offset all have this factor baked into the calculation.
