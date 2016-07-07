/** Logic to populate the imaging table
 *
 *  @file
 *  @date  5/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include <string>
#include "interop/util/exception.h"
#include "interop/util/constant_mapping.h"
#include "interop/util/string.h"
#include "interop/util/length_of.h"
#include "interop/constants/enums.h"
#include "interop/logic/utils/enums.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/run_metrics.h"
#include "interop/model/model_exceptions.h"
#include "interop/model/table/column_header.h"
#include "interop/model/table/imaging_table_entry.h"
#include "interop/model/table/imaging_table.h"
#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/io/table/csv_format.h"

namespace illumina { namespace interop { namespace logic { namespace table {


    /** Convert a column type to the data type
     *
     * @param type imaging table column type
     * @return metric data
     */
    inline constants::metric_data to_data_type(const model::table::column_type type)
    {
        typedef std::pair<model::table::column_type, constants::metric_data > mapped_t;
#       define INTEROP_TUPLE7(Name, Ignored2, Ignored3, Ignored4, Ignored5, Data, Ignored6) mapped_t(model::table::Name##Column, constants::Data),
        static const mapped_t name_types[] = {INTEROP_IMAGING_COLUMN_TYPES};// mapped_t(model::table::ImagingColumnCount, constants::MetricDataCount)};
#       undef INTEROP_TUPLE7
        return util::constant_mapping_get(name_types, type, constants::UnknownMetricData);
    }
    /** Convert a column index to the data type
     *
     * @param index imaging table column index
     * @return metric data
     */
    inline constants::metric_data to_data_type(const size_t index)
    {
        return to_data_type(static_cast<model::table::column_type>(index));
    }

    /** Defines a table_entry map, a temporary data structure used in logic */
    typedef std::map< model::metric_base::base_metric::id_t, model::table::table_entry > table_entry_map_t;
    /** Define a map from the current cycle to the read information */
    typedef summary::read_cycle_vector_t read_cycle_vector_t;
    /** Define an offset vector for filling a table */
    typedef model::table::table_entry::table_fill_vector_t table_fill_vector_t;

    /** Get the internal names of each column
     *
     * @return collection of strings representing a column identifier
     */
    inline const std::vector<std::string>& imaging_table_column_names()
    {
        /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
         *
         * Convert each entry to a string name
         *
         * Example:
         * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
         * "Lane"
         *
         * @see INTEROP_IMAGING_COLUMN_TYPES
         */
#       define INTEROP_TUPLE7(Id, Ignored1, Ignored2, Ignored3, Ignored4, Ignored5, Ignored6) #Id,
        // The list of strings below ends with a ',', e.g. "Lane", "Tile", "Cycle",
        // This is not allowed in C++, so I added a dummy string to the end.
        static const std::string name_types[] = {INTEROP_IMAGING_COLUMN_TYPES "Dummy"};// Added dummy as a work around
#       undef INTEROP_TUPLE7

        static const std::vector<std::string> tmp(name_types, name_types+util::length_of(name_types)-1);// remove dummy
        return tmp;
    }
    /** Get the internal names of each column
     *
     * @todo keep or remove?
     *
     * @param filled boolean array indicating filled columns
     * @return collection of strings representing a column identifier
     */
    inline std::vector<std::string> imaging_table_column_names(const std::vector<bool>& filled)
    {
        std::vector<std::string> tmp = imaging_table_column_names();
        size_t k=0;
        for(size_t i=0;i<tmp.size();++i)
        {
            if(filled[i])
            {
                tmp[k] = tmp[i];
                ++k;
            }
        }
        tmp.resize(k);
        return tmp;
    }
    /** Convert name to header
     *
     * @param name column name
     * @return header representation
     */
    inline std::string to_header(const std::string& name)
    {
        std::string header = name;
        util::replace(header, "Percent", "%");
        util::replace(header, "GreaterThan", ">=");
        util::replace(header, "KPermm2", "(k/mm2)");
        util::replace(header, "K", " (k)");
        util::camel_to(header);
        return header;
    }
    /** Convert header to name
     *
     * @param header column header
     * @return column name
     */
    inline std::string to_name(const model::table::column_header& header)
    {
        std::string name = header.id();
        util::replace(name, "%", "Percent");
        util::replace(name, ">=", "GreaterThan");
        util::replace(name, "(k/mm2)", "KPermm2");
        util::replace(name, " (k)", "K");
        util::camel_from(name);
        return name;
    }
    /** Populate the value offsets from the column headers
     *
     * @param offset destination offset array
     * @param headers column headers
     */
    inline void populate_column_offsets(table_fill_vector_t& offsets,
                                        const std::vector<model::table::column_header>& headers)
                                        throw(model::invalid_column_type)
    {
        const std::vector<std::string>& names(imaging_table_column_names());
        offsets.assign(model::table::ImagingColumnCount, std::vector<size_t>());
        for(size_t i=0;i<headers.size();++i)
        {
            std::vector<std::string>::const_iterator it = std::find(names.begin(), names.end(), to_name(headers[i].id()));
            if(it == names.end())
                INTEROP_THROW(model::invalid_column_type, "Column name not found: " << headers[i].id() << " - " << to_name(headers[i].id()));
            const size_t metric_column = static_cast<size_t>(std::distance(names.begin(), it));
            offsets[metric_column].push_back(i);
        }
    }
    /** Populate a vector with the table column headers
     *
     * @param headers destination vector
     * @param channels channel names to use in header
     * @param filled boolean vector indicating which columns to keep
     */
    inline void populate_column_headers(std::vector<model::table::column_header>& headers,
                                        const std::vector<std::string>& channels,
                                        const std::vector<bool>& filled)
                                        throw(model::invalid_column_type)
    {
        const std::vector<std::string>& names(imaging_table_column_names());
        headers.clear();
        headers.reserve(names.size()+channels.size()+constants::NUM_OF_BASES);
        if(filled.size() != names.size())
            INTEROP_THROW(model::invalid_column_type, "Filled vector does not match number of column names");
        for(size_t i=0;i<names.size();++i)
        {
            if(!filled[i]) continue;
            const std::string title = to_header(names[i]);
            switch(to_data_type(i))
            {
                case constants::IdType:
                case constants::StructType:
                case constants::ValueType:
                {
                    headers.push_back(title);
                    break;
                }
                case constants::BaseArray:
                {
                    for(size_t base = 0;base < constants::NUM_OF_BASES;++base)
                        headers.push_back(
                                model::table::column_header(title,
                                                            constants::to_string(static_cast<constants::dna_bases>(base))));

                    break;
                }
                case constants::ChannelArray:
                {
                    for(size_t channel = 0;channel < channels.size();++channel)
                        headers.push_back(model::table::column_header(title, channels[channel]));
                    break;
                }
                default:
                {
                    INTEROP_THROW( model::invalid_column_type, "Column index does not have a type");
                }
            }
        }
    }
    /** Populate a vector with the table column headers
     *
     * @param headers destination vector
     * @param channels channel names to use in header
     */
    inline void populate_column_headers(std::vector<model::table::column_header>& headers,
                                        const std::vector<std::string>& channels)
                                        throw(model::invalid_column_type)
    {
        const std::vector<bool> filled(imaging_table_column_names().size(), true);
        populate_column_headers(headers, channels, filled);
    }

    /** Populate the table using the iterator over a metric set
     *
     * @note this only support cycle metrics
     *
     * @param beg iterator to start of metric collection
     * @param end iterator to end of metric collection
     * @param q20_idx index of Q20 bin
     * @param q30_idx index of Q30 bin
     * @param naming_convention tile naming method
     * @param cycle_to_read map that takes a cycle and returns the read-number cycle-in-read pair
     * @param table table map a metric ID to a row in the table
     */
    template<typename I>
    void populate_imaging_table_by_cycle(I beg,
                                         I end,
                                         const size_t q20_idx,
                                         const size_t q30_idx,
                                         const constants::tile_naming_method naming_method,
                                         const read_cycle_vector_t& cycle_to_read,
                                         table_entry_map_t& table) throw(model::index_out_of_bounds_exception)
    {
        for(I cur=beg;cur != end;++cur)
        {
            table_entry_map_t::iterator it = table.find(cur->id());
            if(it == table.end())
            {
                if((cur->cycle()-1) >= cycle_to_read.size())
                    throw model::index_out_of_bounds_exception("Cycle exceeds total cycles from Reads in the RunInfo.xml");
                const summary::read_cycle& read = cycle_to_read[cur->cycle()-1];
                table[cur->id()] = model::table::table_entry(*cur,
                                                             read.number,
                                                             read.cycle_within_read,
                                                             q20_idx,
                                                             q30_idx,
                                                             naming_method);
            } // TODO populate tile here?
            else it->second.update(*cur);
        }
    }

    /** Populate the imaging table from all the metrics in run_metrics
     *
     * @ingroup table_logic
     * @param metrics run metrics
     * @param table collection of table rows
     * @param filled_columns collection boolean values indicating which columns are filled
     */
    template<class Table>
    inline void populate_imaging_table(const model::metrics::run_metrics& metrics,
                                       Table& table,
                                       std::vector< bool >& filled_columns) throw(model::index_out_of_bounds_exception)
    {
        table_entry_map_t entry_map;
        const constants::tile_naming_method naming_method = metrics.run_info().flowcell().naming_method();
        const size_t q20_idx = metric::index_for_q_value(metrics.get_set<model::metrics::q_metric>(), 20);
        const size_t q30_idx = metric::index_for_q_value(metrics.get_set<model::metrics::q_metric>(), 30);
        filled_columns.assign(model::table::ImagingColumnCount, false);

        read_cycle_vector_t cycle_to_read;
        summary::map_read_to_cycle_number(metrics.run_info().reads().begin(),
                                          metrics.run_info().reads().end(),
                                          cycle_to_read);
        // TODO replace with visitor that only selects cycle metrics
        populate_imaging_table_by_cycle(metrics.get_set<model::metrics::extraction_metric>().begin(),
                                        metrics.get_set<model::metrics::extraction_metric>().end(),
                                        q20_idx,
                                        q30_idx,
                                        naming_method,
                                        cycle_to_read,
                                        entry_map);
        model::table::table_entry::set_filled(metrics.get_set<model::metrics::extraction_metric>(), filled_columns);
        populate_imaging_table_by_cycle(metrics.get_set<model::metrics::error_metric>().begin(),
                                        metrics.get_set<model::metrics::error_metric>().end(),
                                        q20_idx,
                                        q30_idx,
                                        naming_method,
                                        cycle_to_read,
                                        entry_map);
        model::table::table_entry::set_filled(metrics.get_set<model::metrics::error_metric>(), filled_columns);
        populate_imaging_table_by_cycle(metrics.get_set<model::metrics::image_metric>().begin(),
                                        metrics.get_set<model::metrics::image_metric>().end(),
                                        q20_idx,
                                        q30_idx,
                                        naming_method,
                                        cycle_to_read,
                                        entry_map);
        model::table::table_entry::set_filled(metrics.get_set<model::metrics::image_metric>(), filled_columns);
        populate_imaging_table_by_cycle(metrics.get_set<model::metrics::corrected_intensity_metric>().begin(),
                                        metrics.get_set<model::metrics::corrected_intensity_metric>().end(),
                                        q20_idx,
                                        q30_idx,
                                        naming_method,
                                        cycle_to_read,
                                        entry_map);
        model::table::table_entry::set_filled(metrics.get_set<model::metrics::q_metric>(), filled_columns);
        populate_imaging_table_by_cycle(metrics.get_set<model::metrics::q_metric>().begin(),
                                        metrics.get_set<model::metrics::q_metric>().end(),
                                        q20_idx,
                                        q30_idx,
                                        naming_method,
                                        cycle_to_read,
                                        entry_map);
        model::table::table_entry::set_filled(metrics.get_set<model::metrics::corrected_intensity_metric>(),
                                              filled_columns);
        const model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics =
                metrics.get_set<model::metrics::tile_metric>();
        table.reserve(entry_map.size());
        for(table_entry_map_t::iterator b = entry_map.begin(), e=entry_map.end();b != e;++b)
        {
            if(tile_metrics.has_metric(b->second.Lane, b->second.Tile))
            {
                b->second.set_filled_metric(tile_metrics.get_metric(b->second.Lane, b->second.Tile), filled_columns);
                b->second.update(tile_metrics.get_metric(b->second.Lane, b->second.Tile));
            }
            table.push_back(b->second);
        }
        if(!table.empty())
        {
            model::table::table_entry::set_id_filled(filled_columns);
            filled_columns[model::table::SectionColumn] = table[0].Section > 0;
        }
    }

    /** Populate the imaging table from all the metrics in run_metrics
     *
     * @ingroup table_logic
     * @param metrics run metrics
     * @param table imaging table
     */
    inline void populate_imaging_table(const model::metrics::run_metrics& metrics,
                                       model::table::imaging_table& table)
                                        throw(model::index_out_of_bounds_exception)
    {
        std::vector< bool > filled_columns;
        populate_imaging_table(metrics, table, filled_columns);
        table.filled_columns(filled_columns);
    }
    /** Populate a vector with the table column headers
     *
     * @param channels channel names to use in header
     * @param table imaging table
     */
    inline void populate_column_headers(const std::vector<std::string>& channels,
                                        model::table::imaging_table& table)
                                        throw(model::invalid_column_type)
    {
        std::vector<model::table::column_header> headers;
        populate_column_headers(headers, channels, table.filled_columns());
        table.headers(headers);
    }

}}}}


namespace illumina { namespace interop { namespace model { namespace table {

    /** Read an imaging table from an input stream in the CSV format
     *
     * @param in input stream
     * @param table imaging table
     * @return input stream
     */
    std::istream &operator>>(std::istream &in, model::table::imaging_table &table)
    {
        io::table::read_csv_line(in, table.m_columns_header);
        if (!in.good()) return in;
        logic::table::table_fill_vector_t offsets;
        logic::table::populate_column_offsets(offsets, table.m_columns_header);

        std::vector<float> values;
        values.reserve(table.m_columns_header.size());
        model::table::table_entry row;
        while (in)
        {
            io::table::read_csv_line(in, values);
            if (values.empty()) continue;
            row.copy_from_vector(values, offsets);
            table.push_back(row);
        }
        return in;
    }
    /** Write the imaging table to the output stream in the CSV format
     *
     * @param out output stream
     * @param table imaging table
     * @return output stream
     */
    std::ostream &operator<<(std::ostream &out, const model::table::imaging_table &table)
    {
        std::vector<float> values;
        values.reserve(table.m_columns_header.size());
        if (!out.good()) return out;
        io::table::write_csv_line(out, table.m_columns_header);
        if (!out.good()) return out;
        for (model::table::imaging_table::const_iterator it = table.begin();
             it != table.end(); ++it)
        {
            it->copy_to_vector(values, table.filled_columns());
            io::table::write_csv_line(out, values);
            if (!out.good())return out;
        }
        return out;
    }
}}}}
