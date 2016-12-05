/** Logic to populate the imaging table columns
 *
 *  @file
 *  @date  7/20/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/run_metrics.h"
#include "interop/model/table/imaging_column.h"
#include "interop/logic/table/table_util.h"


namespace illumina { namespace interop { namespace logic { namespace table
{

    /** Get the maximum number of digits to round
     *
     * @return maximum number of rounding digits
     */
    ::uint32_t max_digits();
    /** Get the number of digits to round a column
     *
     * @param index imaging table column id
     * @return number of digits to round a column
     */
    size_t get_column_rounding(const model::table::column_id index);
    /** Create the imaging table columns
     *
     * @param channels names of each channel
     * @param filled boolean array indicating whether to include the column
     * @param columns destination column vector
     */
    void create_imaging_table_columns(const std::vector<std::string>& channels,
                                      const std::vector<bool>& filled,
                                      std::vector< model::table::imaging_column >& columns)
                                        throw(model::invalid_column_type,
                                        model::index_out_of_bounds_exception,
                                        model::invalid_channel_exception);
    /** Populate the value offsets from the column headers
     *
     * @param columns column headers
     */
    void populate_column_offsets(std::vector<model::table::imaging_column>& columns)
    throw(model::invalid_column_type);
    /** Create a vector of column descriptors
     *
     * @param metrics source collection of InterOp metrics from the run
     * @param columns destination vector of column descriptors
     */
    void create_imaging_table_columns(model::metrics::run_metrics& metrics,
                                      std::vector< model::table::imaging_column >& columns)
    throw(model::invalid_column_type,
    model::index_out_of_bounds_exception,
    model::invalid_channel_exception);


}}}}

