/** Include all logic functions
 *
 *  @file
 *  @date  6/1/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/logic/table/populate_imaging_table.h"
#include "interop/logic/summary/run_summary.h"
#include "interop/logic/summary/index_summary.h"
#include "interop/logic/plot/plot_by_cycle.h"
#include "interop/logic/plot/plot_by_lane.h"
#include "interop/logic/plot/plot_flowcell_map.h"
#include "interop/logic/plot/plot_qscore_heatmap.h"
#include "interop/logic/plot/plot_qscore_histogram.h"
#include "interop/logic/plot/plot_sample_qc.h"


/** @defgroup logic Logic for populating summaries, tables and plots
 *
 * This group defines the interface functions for computing the SAV:
 *  1. Summary tab
 *  2. Analysis tab
 *  3. Imaging tab
 *  4. Indexing tab
 */

/** @defgroup plot_logic Logic for populating plots in the SAV Analysis and Indexing tabs
 *
 * @ingroup logic
 */

/** @defgroup summary_logic Logic for populating the SAV Summary and Indexing tabs
 *
 * @ingroup logic
 */

/** @defgroup table_logic Logic for populating the SAV Imaging tab
 *
 * @ingroup logic
 */

