/** Logic to populate the imaging table
 *
 *  @file
 *  @date  7/20/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/table/create_imaging_table.h"

#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/logic/table/create_imaging_table_columns.h"
#include "interop/logic/table/table_populator.h"
#include "interop/logic/metric/q_metric.h"
#include "interop/logic/utils/metric_type_ext.h"

namespace illumina { namespace interop { namespace logic { namespace table
{
    /** Populate the imaging table with a by cycle InterOp metric set
     *
     * @param beg iterator to start of the metric set
     * @param end iterator to end of the metric set
     * @param q20_idx index of the q20 value
     * @param q30_idx index of the q30_idxindex value
     * @param naming_method tile naming method enum
     * @param cycle_to_read map cycle to read/cycle within read
     * @param columns vector of table columns
     * @param row_offset offset for each metric into the sorted table
     * @param column_count number of data columns including sub columns
     * @param data_beg iterator to start of table data
     * @param data_end iterator to end of table data
     */
    template<typename InputIterator, typename OutputIterator>
    void populate_imaging_table_data_by_cycle(InputIterator beg,
                                              InputIterator end,
                                              const size_t q20_idx,
                                              const size_t q30_idx,
                                              const constants::tile_naming_method naming_method,
                                              const summary::read_cycle_vector_t& cycle_to_read,
                                              const std::vector<size_t>& columns,
                                              const row_offset_map_t& row_offset,
                                              const size_t column_count,
                                              OutputIterator data_beg,
                                              OutputIterator data_end)
    {
        typedef model::metric_base::base_metric::id_t id_t;
        for(;beg != end;++beg)
        {
            const id_t id = beg->cycle_hash();
            typename row_offset_map_t::const_iterator row_it = row_offset.find(id);
            const ::uint64_t row = row_it->second;
            const summary::read_cycle& read = cycle_to_read[beg->cycle()-1];
            if(data_beg[row*column_count]==0)
            {
                if((beg->cycle()-1) >= cycle_to_read.size())
                    INTEROP_THROW(model::index_out_of_bounds_exception, "Cycle exceeds total cycles from Reads in the RunInfo.xml");


                INTEROP_ASSERTMSG(row_it != row_offset.end(), "Bug with row offset");
                INTEROP_ASSERT(row<row_offset.size())
                INTEROP_ASSERTMSG(columns[model::table::ReadColumn] < static_cast<size_t>(model::table::ImagingColumnCount), columns[model::table::ReadColumn] );
                INTEROP_ASSERTMSG(columns[model::table::CycleWithinReadColumn] < static_cast<size_t>(model::table::ImagingColumnCount), columns[model::table::CycleWithinReadColumn] );
                INTEROP_ASSERTMSG(data_beg+row*column_count+columns[model::table::ReadColumn] < data_end, columns[model::table::ReadColumn]
                        << " - " <<  row*column_count+columns[model::table::ReadColumn] << " < " << std::distance(data_beg, data_end) << " "
                        << "row: " << row << " < " << row_offset.size());
                // TODO: Only populate Id once!
                table_populator::populate_id(*beg,
                                             read,
                                             q20_idx,
                                             q30_idx,
                                             0,
                                             naming_method,
                                             columns,
                                             data_beg+row*column_count,
                                             data_end);
            }
            table_populator::populate(*beg,
                                      read.number,
                                      q20_idx,
                                      q30_idx,
                                      0,
                                      naming_method,
                                      columns,
                                      data_beg+row*column_count,
                                      data_end);
        }
    }
    /** Populate the imaging table with a by cycle InterOp metric set
     *
     * @param metrics InterOp metric set
     * @param q20_idx index of the q20 value
     * @param q30_idx index of the q30 value
     * @param naming_method tile naming method enum
     * @param cycle_to_read map cycle to read/cycle within read
     * @param columns vector of table columns
     * @param row_offset offset for each metric into the sorted table
     * @param column_count number of data columns including sub columns
     * @param data_beg iterator to start of table data
     * @param data_end iterator to end of table data
     */
    template<class MetricSet, typename OutputIterator>
    void populate_imaging_table_data_by_cycle(const MetricSet& metrics,
                                              const size_t q20_idx,
                                              const size_t q30_idx,
                                              const constants::tile_naming_method naming_method,
                                              const summary::read_cycle_vector_t& cycle_to_read,
                                              const std::vector<size_t>& columns,
                                              const row_offset_map_t& row_offset,
                                              const size_t column_count,
                                              OutputIterator data_beg, OutputIterator data_end)
    {
        populate_imaging_table_data_by_cycle(metrics.begin(),
                                             metrics.end(),
                                             q20_idx,
                                             q30_idx,
                                             naming_method,
                                             cycle_to_read,
                                             columns,
                                             row_offset,
                                             column_count,
                                             data_beg, data_end);
    }
    /** Zero out first column of every row
     *
     * @param beg iterator to start of colleciton
     * @param end iterator to end of collection
     * @param column_count number of columns
     */
    template<typename I>
    void zero_first_column(I beg, I end, size_t column_count)
    {
        for(;beg != end;beg+=column_count) *beg = 0;
    }
    /** Populate the imaging table with all the metrics in the run
     *
     * @param metrics collection of all run metrics
     * @param columns vector of table columns
     * @param row_offset offset for each metric into the sorted table
     * @param data_beg iterator to start of table data
     * @param data_end iterator to end of table data
     */
    template<typename I>
    void create_imaging_table_data(const model::metrics::run_metrics& metrics,
                                   const std::vector<model::table::imaging_column>& columns,
                                   const row_offset_map_t& row_offset,
                                   I data_beg,
                                   I data_end)
    {
        typedef typename model::metrics::run_metrics::id_t id_t;
        typedef model::metric_base::metric_set< model::metrics::tile_metric > tile_metric_set_t;

        if(columns.empty())return;
        const size_t column_count = columns.back().column_count();
        const constants::tile_naming_method naming_method = metrics.run_info().flowcell().naming_method();
        const size_t q20_idx = metric::index_for_q_value(metrics.get<model::metrics::q_metric>(), 20);
        const size_t q30_idx = metric::index_for_q_value(metrics.get<model::metrics::q_metric>(), 30);
        std::vector<size_t> cmap(model::table::ImagingColumnCount, std::numeric_limits<size_t>::max());
        for(size_t i=0;i<columns.size();++i) cmap[columns[i].id()] = columns[i].offset();
        summary::read_cycle_vector_t cycle_to_read;
        summary::map_read_to_cycle_number(metrics.run_info().reads().begin(),
                                          metrics.run_info().reads().end(),
                                          cycle_to_read);
        if(data_beg+column_count*row_offset.size() > data_end)
            INTEROP_THROW(model::index_out_of_bounds_exception, "Table is larger than buffer: "
                    << (column_count*row_offset.size()) << " > " << std::distance(data_beg, data_end)
                    << " column_count: " << column_count << " row_offset.size()=" << row_offset.size());
        zero_first_column(data_beg, data_beg+column_count*row_offset.size(), column_count);
        populate_imaging_table_data_by_cycle(metrics.get<model::metrics::extraction_metric>(),
                                             q20_idx,
                                             q30_idx,
                                             naming_method,
                                             cycle_to_read,
                                             cmap,
                                             row_offset,
                                             column_count,
                                             data_beg, data_end);
        populate_imaging_table_data_by_cycle(metrics.get<model::metrics::error_metric>(),
                                             q20_idx,
                                             q30_idx,
                                             naming_method,
                                             cycle_to_read,
                                             cmap,
                                             row_offset,
                                             column_count,
                                             data_beg, data_end);
        populate_imaging_table_data_by_cycle(metrics.get<model::metrics::image_metric>(),
                                             q20_idx,
                                             q30_idx,
                                             naming_method,
                                             cycle_to_read,
                                             cmap,
                                             row_offset,
                                             column_count,
                                             data_beg, data_end);
        populate_imaging_table_data_by_cycle(metrics.get<model::metrics::corrected_intensity_metric>(),
                                             q20_idx,
                                             q30_idx,
                                             naming_method,
                                             cycle_to_read,
                                             cmap,
                                             row_offset,
                                             column_count,
                                             data_beg, data_end);
        populate_imaging_table_data_by_cycle(metrics.get<model::metrics::q_metric>(),
                                             q20_idx,
                                             q30_idx,
                                             naming_method,
                                             cycle_to_read,
                                             cmap,
                                             row_offset,
                                             column_count,
                                             data_beg, data_end);
        const tile_metric_set_t& tile_metrics = metrics.get<model::metrics::tile_metric>();
        for(typename row_offset_map_t::const_iterator it = row_offset.begin();it != row_offset.end();++it)
        {
            const id_t tid = model::metric_base::base_cycle_metric::tile_hash_from_id(it->first);
            if (!tile_metrics.has_metric(tid)) continue;
            const id_t cycle = model::metric_base::base_cycle_metric::cycle_from_id(it->first);
            const ::uint64_t row = it->second;
            INTEROP_ASSERTMSG(cycle <= cycle_to_read.size(),
                              cycle << " <= " << cycle_to_read.size()
                                    <<  " tile: " << model::metric_base::base_cycle_metric::tile_from_id(it->first));
            const summary::read_cycle& read = cycle_to_read[static_cast<size_t>(cycle-1)];
            table_populator::populate(tile_metrics.get_metric(tid),
                                      read.number,
                                      q20_idx,
                                      q30_idx,
                                      0,
                                      naming_method,
                                      cmap,
                                      data_beg+row*column_count, data_end);
        }
    }
    /** Populate the imaging table with all the metrics in the run
     *
     * @param metrics collection of all run metrics
     * @param columns vector of table columns
     * @param row_offset row offset map
     * @param data_beg iterator to start of table data
     * @param n number of cells in the data table
     */
    template<typename I>
    void populate_imaging_table_data_t(const model::metrics::run_metrics& metrics,
                                       const std::vector<model::table::imaging_column>& columns,
                                       const row_offset_map_t& row_offset,
                                     I data_beg, const size_t n)
    {
        create_imaging_table_data(metrics, columns, row_offset, data_beg, data_beg+n);
    }
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
                                     float* data_beg,
                                     const size_t n)
    {
        std::fill(data_beg, data_beg+n, std::numeric_limits<float>::quiet_NaN());
        create_imaging_table_data(metrics, columns, row_offset, data_beg, data_beg+n);
    }
    /** Count the number of rows in the imaging table and setup an ordering
     *
     * @param metrics collections of InterOp metric sets
     * @param row_offset ordering for the rows
     */
    void count_table_rows(const model::metrics::run_metrics& metrics,
                          row_offset_map_t& row_offset)
    {
        typedef model::metrics::run_metrics::cycle_metric_map_t cycle_metric_map_t;
        cycle_metric_map_t hash_set;
        metrics.populate_id_map(hash_set);
        row_offset.clear();
        ::uint64_t row = 0;
        for(cycle_metric_map_t::const_iterator it = hash_set.begin();it != hash_set.end();++it,++row)
            row_offset[it->first]=row;
    }
    /** Count the total number of columns for the data table
     *
     * @param columns vector of table column descriptions
     * @return total number of columns including sub columns
     */
    size_t count_table_columns(const std::vector<model::table::imaging_column>& columns)
    {
        if(columns.empty()) return 0;
        return columns.back().column_count();
    }
    /** Create an imaging table from run metrics
     *
     * @param metrics source run metrics
     * @param table destination imaging table
     */
    void create_imaging_table(model::metrics::run_metrics& metrics, model::table::imaging_table& table)
                                        throw(model::invalid_column_type, model::index_out_of_bounds_exception)
    {
        typedef model::table::imaging_table::column_vector_t column_vector_t;
        typedef model::table::imaging_table::data_vector_t data_vector_t;

        row_offset_map_t row_offset;
        column_vector_t columns;
        create_imaging_table_columns(metrics, columns);
        if(columns.empty())return;
        count_table_rows(metrics, row_offset);
        data_vector_t data(row_offset.size()*count_table_columns(columns), std::numeric_limits<float>::quiet_NaN());
        create_imaging_table_data(metrics, columns, row_offset, data.begin(), data.end());
        table.set_data(row_offset.size(), columns, data);
    }


    /** Convert metric type to metric group
     *
     * @param type metric type
     * @return metric group
     */
    inline constants::metric_group to_group(const model::table::column_id type)
    {
        using namespace constants;
        using namespace model::table;
        using namespace model;
        using namespace model::metric_base;
        // TODO: This can be reduced to a single macro define
        typedef std::pair<column_id, metric_group > mapped_t;
#       define INTEROP_TUPLE7(Id, Metric, Ignore2, Ignore3, Ignore4, Ignore5, Ignored6) \
        mapped_t(Id##Column,static_cast<metric_group>(metric_attributes<Metric>::TYPE)),
        static const mapped_t name_types[] = {INTEROP_IMAGING_COLUMN_TYPES mapped_t(ImagingColumnCount, UnknownMetricGroup)};
#       undef INTEROP_TUPLE7
        return util::constant_mapping_get(name_types, type, UnknownMetricGroup);
    }

    /** List the required on demand metrics
     *
     * @param valid_to_load list of metrics to load on demand
     */
    void list_imaging_table_metrics_to_load(std::vector<unsigned char>& valid_to_load)
    {
        if(valid_to_load.size() != constants::MetricCount) valid_to_load.assign(constants::MetricCount, 0);
        std::vector<model::table::column_id> columns;
        constants::list_enums<model::table::column_id>(columns);
        for(size_t i=0;i<columns.size();++i)
        {
            const constants::metric_group group = to_group(columns[i]);
            if(group >= constants::MetricCount)continue;
            valid_to_load[group] = static_cast<unsigned char>(1);
        }
    }

}}}}

