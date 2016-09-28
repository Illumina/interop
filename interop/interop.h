/** InterOp Library
 *
 * This header file provides an interface to the primary InterOp functionality
 *
 *  @file
 *  @date 6/8/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/version.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_by_cycle.h"
#include "interop/logic/plot/plot_by_lane.h"
#include "interop/logic/plot/plot_flowcell_map.h"
#include "interop/logic/plot/plot_qscore_heatmap.h"
#include "interop/logic/plot/plot_qscore_histogram.h"
#include "interop/logic/summary/index_summary.h"
#include "interop/logic/summary/run_summary.h"

namespace illumina { namespace interop
{
    /** Get the version of the library
     *
     * @return library version
     */
    inline const char* library_version(){return INTEROP_VERSION;}
}}

