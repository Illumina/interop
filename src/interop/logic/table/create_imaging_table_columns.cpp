/** Logic to populate the imaging table columns
 *
 *  @file
 *  @date  7/20/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/table/create_imaging_table_columns.h"


#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/logic/table/check_imaging_table_column.h"
#include "interop/logic/metric/q_metric.h"

namespace illumina { namespace interop { namespace logic { namespace table
{
    /** Determine whether a column should be filled
     *
     * @param metrics run metrics
     * @param tile_hash map between the tile has and base metric
     * @param filled destination array that indicates whether a column should be filled
     */
    void determine_filled_columns(model::metrics::run_metrics& metrics,
                                         const model::metrics::run_metrics::tile_metric_map_t& tile_hash,
                                         std::vector< bool >& filled)
    {
        typedef model::metrics::run_metrics::tile_metric_map_t tile_metric_map_t;
        filled.assign(model::table::ImagingColumnCount, false);
        check_imaging_table_column::set_filled_for_metric_set(metrics.get<model::metrics::extraction_metric>(), filled);
        check_imaging_table_column::set_filled_for_metric_set(metrics.get<model::metrics::error_metric>(), filled);
        check_imaging_table_column::set_filled_for_metric_set(metrics.get<model::metrics::image_metric>(), filled);
        check_imaging_table_column::set_filled_for_metric_set(metrics.get<model::metrics::corrected_intensity_metric>(), filled);
        check_imaging_table_column::set_filled_for_metric_set(metrics.get<model::metrics::q_metric>(), filled);

        const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics =
                metrics.get<model::metrics::tile_metric>();
        const constants::tile_naming_method naming_method = metrics.run_info().flowcell().naming_method();
        const size_t q20_idx = metric::index_for_q_value(metrics.get<model::metrics::q_metric>(), 20);
        const size_t q30_idx = metric::index_for_q_value(metrics.get<model::metrics::q_metric>(), 30);
        for(tile_metric_map_t::const_iterator it = tile_hash.begin();it != tile_hash.end();++it)
        {
            if(!tile_metrics.has_metric(it->first)) continue;
            for(model::run::info::const_read_iterator read_it = metrics.run_info().reads().begin();read_it != metrics.run_info().reads().end();++read_it)
            {
                check_imaging_table_column::set_filled_for_metric(tile_metrics.get_metric(it->first),
                                                                  read_it->number(),
                                                                  q20_idx,
                                                                  q30_idx,
                                                                  0,
                                                                  naming_method,
                                                                  filled);
            }
        }
        const size_t column_count = std::accumulate(filled.begin(), filled.end(), static_cast<size_t>(0));
        if(column_count>0)
        {
            check_imaging_table_column::set_id_filled(filled);
            // TODO: Create is Selection Logic
            filled[model::table::SectionColumn] = metrics.run_info().flowcell().naming_method() == constants::FiveDigit;
        }
    }
    /** Convert a column type to the data type
     *
     * @param index imaging table column index
     * @return metric data
     */
    constants::metric_data to_data_type(const size_t index)
    {
        typedef std::pair<model::table::column_id, constants::metric_data > mapped_t;
#       define INTEROP_TUPLE7(Name, Ignored2, Ignored3, Ignored4, Ignored5, Data, Ignored6) mapped_t(model::table::Name##Column, constants::Data),
        static const mapped_t name_types[] = {INTEROP_IMAGING_COLUMN_TYPES};// mapped_t(model::table::ImagingColumnCount, constants::MetricDataCount)};
#       undef INTEROP_TUPLE7
        return util::constant_mapping_get(name_types, static_cast<model::table::column_id>(index), constants::UnknownMetricData);
    }
    /** Get the number of digits to round a column
     *
     * @param index imaging table column id
     * @return number of digits to round a column
     */
    size_t get_column_rounding(const model::table::column_id index)
    {
        typedef std::pair<model::table::column_id, size_t > mapped_t;
#       define INTEROP_TUPLE7(Name, Ignored2, Ignored3, Ignored4, Ignored5, Ignored6, RoundDigits) mapped_t(model::table::Name##Column, RoundDigits),
        static const mapped_t name_types[] = {INTEROP_IMAGING_COLUMN_TYPES};// mapped_t(model::table::ImagingColumnCount, constants::MetricDataCount)};
#       undef INTEROP_TUPLE7
        return util::constant_mapping_get(name_types, index, static_cast<size_t>(0));
    }
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
    model::invalid_channel_exception)
    {
        columns.clear();
        columns.reserve(model::table::ImagingColumnCount+channels.size()+constants::NUM_OF_BASES);
        if(filled.size() != model::table::ImagingColumnCount)
            INTEROP_THROW(model::invalid_column_type, "Filled vector does not match number of column names");
        for(size_t i=0, offset=0;i<model::table::ImagingColumnCount;++i)
        {
            if(!filled[i]) continue;
            switch(to_data_type(i))
            {
                case constants::IdType:
                case constants::StructType:
                case constants::ValueType:
                {
                    columns.push_back(model::table::imaging_column(i, offset));
                    offset+=1;
                    break;
                }
                case constants::BaseArray:
                {
                    std::vector<std::string> tmp;
                    constants::list_enum_names<constants::dna_bases>(tmp, 1);
                    tmp.resize(constants::NUM_OF_BASES);
                    columns.push_back(model::table::imaging_column(i, offset, tmp));
                    offset+=tmp.size();
                    break;
                }
                case constants::ChannelArray:
                {
                    columns.push_back(model::table::imaging_column(i, offset, channels));
                    offset+=channels.size();
                    break;
                }
                default:
                {
                    INTEROP_THROW( model::invalid_column_type, "Column index does not have a type");
                }
            }
        }
    }
    /** Populate the value offsets from the column headers
     *
     * @param columns column headers
     */
    void populate_column_offsets(std::vector<model::table::imaging_column>& columns)
    throw(model::invalid_column_type)
    {
        size_t offset = 0;
        for(size_t i=0;i<columns.size();++i)
        {
            columns[i].offset(offset);
            columns[i].parse_header_for_id(columns[i].name());
            offset = columns[i].column_count();
        }
    }

    /** Create a vector of column descriptors
     *
     * @param metrics source collection of InterOp metrics from the run
     * @param columns destination vector of column descriptors
     */
    void create_imaging_table_columns(model::metrics::run_metrics& metrics,
                                      std::vector< model::table::imaging_column >& columns)
                                      throw(model::invalid_column_type,
                                      model::index_out_of_bounds_exception,
                                      model::invalid_channel_exception)
    {
        typedef model::metrics::run_metrics::tile_metric_map_t tile_metric_map_t;
        std::vector< bool > filled;
        tile_metric_map_t tile_hash;
        metrics.populate_id_map(tile_hash);
        determine_filled_columns(metrics, tile_hash, filled);
        create_imaging_table_columns(metrics.run_info().channels(),
                                     filled,
                                     columns);
    }


    /** Get the maximum number of digits to round
     *
     * @return maximum number of rounding digits
     */
    ::uint32_t max_digits()
    {
        return check_imaging_table_column::max_digits();
    }


}}}}

