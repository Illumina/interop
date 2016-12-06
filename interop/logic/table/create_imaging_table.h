/** Logic to populate the imaging table
 *
 *  @file
 *  @date  7/20/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "interop/model/run_metrics.h"
#include "interop/model/table/imaging_column.h"
#include "interop/model/table/imaging_table.h"
#include "interop/logic/table/table_util.h"

namespace illumina { namespace interop { namespace logic { namespace table
{
    // TODO: Make unordered? - need to sort later
    // Workaround for SWIG not understanding the macro
    /** Define a row offset map */
    typedef std::map<model::metric_base::base_metric::id_t, ::uint64_t> row_offset_map_t;
    /** Populate the imaging table with all the metrics in the run
     *
     * @param metrics collection of all run metrics
     * @param columns vector of table columns
     * @param row_offset ordering for the rows
     * @param data_beg iterator to start of table data
     * @param n number of cells in the data table
     */
    void populate_imaging_table_data(const model::metrics::run_metrics& metrics,
                                     const std::vector<model::table::imaging_column>& columns,
                                     const row_offset_map_t& row_offset,
                                     float* data_beg, const size_t n);
    /** Count the number of rows in the imaging table and setup an ordering
     *
     * @param metrics collections of InterOp metric sets
     * @param row_offset ordering for the rows
     */
    void count_table_rows(const model::metrics::run_metrics& metrics,
                          row_offset_map_t& row_offset);
    /** Count the total number of columns for the data table
     *
     * @param columns vector of table column descriptions
     * @return total number of columns including sub columns
     */
    size_t count_table_columns(const std::vector<model::table::imaging_column>& columns);
    /** Create an imaging table from run metrics
     *
     * @param metrics source run metrics
     * @param table destination imaging table
     */
    void create_imaging_table(model::metrics::run_metrics& metrics, model::table::imaging_table& table)
    throw(model::invalid_column_type, model::index_out_of_bounds_exception);

    /** List the required on demand metrics
     *
     * @param valid_to_load list of metrics to load on demand
     */
    void list_imaging_table_metrics_to_load(std::vector<unsigned char>& valid_to_load);

}}}}

