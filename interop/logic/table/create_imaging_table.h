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

namespace illumina { namespace interop { namespace logic { namespace table
{
    /** Populate the imaging table with all the metrics in the run
     *
     * @param metrics collection of all run metrics
     * @param columns vector of table columns
     * @param data_beg iterator to start of table data
     * @param n number of cells in the data table
     */
    void populate_imaging_table_data(const model::metrics::run_metrics& metrics,
                                   const std::vector<model::table::imaging_column>& columns,
                                   float* data_beg, const size_t n);
    /** Count the number of rows in the imaging table
     *
     * @param metrics collections of InterOp metric sets
     * @return number of tile/cycles
     */
    size_t count_table_rows(const model::metrics::run_metrics& metrics);
    /** Count the total number of columns for the data table
     *
     * @param columns vector of table column descriptions
     * @return total number of columns including sub columns
     */
    size_t count_table_columns(const std::vector<model::table::imaging_column>& columns);
    /** Count the number of cells in the data table
     *
     * @param metrics  collections of InterOp metric sets
     * @param columns vector of table column descriptions
     * @return number of cells in the data table
     */
    size_t count_table_cells(const model::metrics::run_metrics& metrics,
                                    const std::vector<model::table::imaging_column>& columns);
    /** Create an imaging table from run metrics
     *
     * @param metrics source run metrics
     * @param table destination imaging table
     */
    void create_imaging_table(const model::metrics::run_metrics& metrics, model::table::imaging_table& table)
    throw(model::invalid_column_type, model::index_out_of_bounds_exception);

}}}}
