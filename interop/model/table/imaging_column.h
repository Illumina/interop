/** Model for the imaging table column
 *
 *  @file
 *  @date  7/20/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include <iosfwd>
#include <vector>
#include "interop/util/assert.h"
#include "interop/util/length_of.h"
#include "interop/util/string.h"
#include "interop/constants/enums.h"
#include "interop/logic/utils/enums.h"


/** Mapping of data for the imaging table
 *
 * Column 1:
 *  - Name of the field in the table_entry class
 *  - Converted to column header in imaging table
 *  - Suffixed with 'Column' when used as an enum, e.g. Lane -> LaneColumn
 * Column 2:
 *  - metric that holds the column data
 * Column 3:
 *  - method in metric that holds the column data
 * Column 4:
 *  - Parameter taken by metric method
 *      e.g. percent_aligned requires a read number, which can be taken from table_entry
 *      e.g. percent_over_qscore needs to know the index of the bin to use
 *      e.g. surface needs to know the tile naming convention to properly parse the name
 * Column 5:
 *  - Data type for the field in table_entry
 * Column 6:
 *  - Flag for properties for this field
 * Column 7:
 *  - Number of decimal places to round to
 *
 *  The metrics marked "dummy" are populated not in the update function, but in the constructor.
 *
 * @note This macro requires the macro INTEROP_TUPLE7 to be defined before use
 * @see illumina::interop::model::table::table_entry
 * @see illumina::interop::logic::table::imaging_table_column_names
 */
#define INTEROP_IMAGING_COLUMN_TYPES \
        INTEROP_TUPLE7(Lane,                  metric_base::base_metric,           lane,                       Void, UInt, IdType, 0)\
        INTEROP_TUPLE7(Tile,                  metric_base::base_metric,           tile,                       Void, UInt, IdType, 0)\
        INTEROP_TUPLE7(Cycle,                 metric_base::base_cycle_metric,     cycle/*dummy*/,             Void, UInt, IdType, 0)\
        INTEROP_TUPLE7(Read,                  metric_base::base_read_metric,      read/*dummy*/,              Void, UInt, IdType, 0)\
        INTEROP_TUPLE7(CycleWithinRead,       metric_base::base_read_metric,      id/*dummy*/,                Void, UInt, IdType, 0)\
        INTEROP_TUPLE7(DensityKPermm2,        metrics::tile_metric,               cluster_density_k,          Void, Float, ValueType, 1)\
        INTEROP_TUPLE7(DensityPfKPermm2,      metrics::tile_metric,               cluster_density_pf_k,       Void, Float, ValueType, 1)\
        INTEROP_TUPLE7(ClusterCountK,         metrics::tile_metric,               cluster_count_k,            Void, Float, ValueType, 1)\
        INTEROP_TUPLE7(ClusterCountPfK,       metrics::tile_metric,               cluster_count_pf_k,         Void, Float, ValueType, 1)\
        INTEROP_TUPLE7(PercentPassFilter,     metrics::tile_metric,               percent_pf,                 Void, Float, ValueType, 1)\
        INTEROP_TUPLE7(PercentAligned,        metrics::tile_metric,               percent_aligned_at,         Read, Float, ValueType, 1)\
        INTEROP_TUPLE7(PercentPhasing,        metrics::tile_metric,               percent_phasing_at,         Read, Float, ValueType, 3)\
        INTEROP_TUPLE7(PercentPrephasing,     metrics::tile_metric,               percent_prephasing_at,      Read, Float, ValueType, 3)\
        INTEROP_TUPLE7(ErrorRate,             metrics::error_metric,              error_rate,                 Void, Float, ValueType, 3)\
        INTEROP_TUPLE7(PercentGreaterThanQ20, metrics::q_metric,                  percent_over_qscore,        Q20,  Float, ValueType, 2)\
        INTEROP_TUPLE7(PercentGreaterThanQ30, metrics::q_metric,                  percent_over_qscore,        Q30,  Float, ValueType, 2)\
        INTEROP_TUPLE7(P90,                   metrics::extraction_metric,         max_intensity_values,       Void, UShort, ChannelArray, 0)\
        INTEROP_TUPLE7(PercentNoCalls,        metrics::corrected_intensity_metric,percent_nocall,             Void, Float, ValueType, 1)\
        INTEROP_TUPLE7(PercentBase,           metrics::corrected_intensity_metric,percent_bases,              Void, Float, BaseArray, 1)\
        INTEROP_TUPLE7(Fwhm,                  metrics::extraction_metric,         focus_scores,               Void, Float, ChannelArray, 2) \
        INTEROP_TUPLE7(Corrected,             metrics::corrected_intensity_metric,corrected_int_all_array,    Void, UShort, BaseArray, 0)\
        INTEROP_TUPLE7(Called,                metrics::corrected_intensity_metric,corrected_int_called_array, Void, UShort, BaseArray, 0)\
        INTEROP_TUPLE7(SignalToNoise,         metrics::corrected_intensity_metric,signal_to_noise,            Void, Float, ValueType, 2)\
        INTEROP_TUPLE7(MinimumContrast,       metrics::image_metric,              min_contrast_array,         Void, UShort, ChannelArray, 0)\
        INTEROP_TUPLE7(MaximumContrast,       metrics::image_metric,              max_contrast_array,         Void, UShort, ChannelArray, 0)\
        INTEROP_TUPLE7(Surface,               metric_base::base_metric,           surface,                    NamingConvention, UInt, IdType, 0)\
        INTEROP_TUPLE7(Swath,                 metric_base::base_metric,           swath,                      NamingConvention, UInt, IdType, 0)\
        INTEROP_TUPLE7(Section,               metric_base::base_metric,           section,                    NamingConvention, UInt, IdType, 0)\
        INTEROP_TUPLE7(TileNumber,            metric_base::base_metric,           number,                     NamingConvention, UInt, IdType, 0)
        //INTEROP_TUPLE7(Time,                  metrics::extraction_metric,         date_time_csharp,           Void, DateTime, StructType, 0)

namespace illumina { namespace interop { namespace model { namespace table
{
#   define INTEROP_TUPLE7(Id, Ignored1, Ignored2, Ignored3, Ignored4, Ignored5, Ignored6) Id##Column,
    /** Map column index to name. Note, the enum has a 'Column' suffix added to the name above*/
    enum column_id{ INTEROP_IMAGING_COLUMN_TYPES ImagingColumnCount, UnknownColumnId=INTEROP_UNKNOWN};
#   undef INTEROP_TUPLE7

    /** Information describing a single column or a group of columns
     */
    class imaging_column
    {
    public:
        /** Define a vector of strings */
        typedef std::vector<std::string> string_vector;
    public:
        /** Constructor */
        imaging_column() : m_id(UnknownColumnId), m_offset(0) {}
        /** Constructor
         *
         * @param index column id
         * @param offset offset of the data into the data array
         */
        imaging_column(const size_t index, const size_t offset) :
                m_id(static_cast<column_id>(index)),
                m_name(to_header(m_id)),
                m_offset(offset)
        {
            INTEROP_ASSERT(index < ImagingColumnCount);
        }
        /** Constructor
         *
         * @param index column id
         * @param offset offset of the data into the data array
         * @param sub_columns sub column header names
         */
        imaging_column(const size_t index, const size_t offset, const string_vector& sub_columns) :
                m_id(static_cast<column_id>(index)),
                m_name(to_header(m_id)),
                m_offset(offset),
                m_subcolumn_names(sub_columns)
        {
            INTEROP_ASSERT(index < ImagingColumnCount);
        }

    public:
        /** Get the column id enum
         *
         * @return column id enum
         */
        column_id id()const
        {
            return m_id;
        }
        /** Get the name of the column
         *
         * @return name of the column
         */
        const std::string& name()const
        {
            return m_name;
        }
        /** Check if the column has sub column headers
         *
         * @return true if there are sub column headers
         */
        bool has_children()const
        {
            return !m_subcolumn_names.empty();
        }
        /** Get the offset into the data array
         *
         * @return offset into the data array
         */
        size_t offset()const
        {
            return m_offset;
        }
        /** Get a vector of subcolumn headers
         *
         * @return vector of subcolumn headers
         */
        const string_vector& subcolumns()const
        {
            return m_subcolumn_names;
        }
        /** Get the full name of the column and sub column
         *
         * @param sub_index index of subcolumn
         * @return name of the column
         */
        std::string full_name(const size_t sub_index)const throw(model::index_out_of_bounds_exception)
        {
            if (has_children())
            {
                if(sub_index >= m_subcolumn_names.size())
                    INTEROP_THROW(model::index_out_of_bounds_exception, "Sub column index out of bounds");
                return m_name + "_" + m_subcolumn_names[sub_index];
            }
            return m_name;
        }

    public:
        /** Update the offset of the column
         *
         * @param off new offset into data array
         */
        void offset(const size_t off)
        {
            m_offset = off;
        }
        /** Update the offset of the column
         *
         * @param val new id
         */
        void id(const column_id val)
        {
            m_id = val;
        }
        /** Update the offset of the column
         *
         * @param header header description for new id
         */
        void parse_header_for_id(const std::string& header)
        {
            id(constants::parse<column_id>(to_name(header)));
        }

    public:
        /** Get total number of subcolumns
         *
         * @return total number of subcolumns
         */
        size_t size()const
        {
            return (has_children() ? m_subcolumn_names.size() : 1);
        }
        /** Get total number of columns in the data up to this column
         *
         * @return total number of columns in the data up to this column
         */
        size_t column_count()const
        {
            return m_offset + size();
        }

    public:
        /** Convert a column id enum to the string representation
         *
         * @param id column id enum
         * @return header representation
         */
        static std::string to_header(const column_id id)
        {
            return to_header(constants::to_string(id));
        }
        /** Convert name to header
         *
         * @param name column name
         * @return header representation
         */
        static std::string to_header(const std::string& name)
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
         * @param header column header name
         * @return column name
         */
        static std::string to_name(const std::string& header)
        {
            std::string name = header;
            util::replace(name, "%", "Percent");
            util::replace(name, ">=", "GreaterThan");
            util::replace(name, "(k/mm2)", "KPermm2");
            util::replace(name, " (k)", "K");
            util::camel_from(name);
            return name;
        }
        /** Convert header to name
         *
         * @param header column header
         * @return column name
         */
        static std::string to_name(const model::table::imaging_column& header)
        {
            return to_name(header.name());
        }

    private:
        friend std::istream& operator>>(std::istream& in, imaging_column& column);
        friend std::ostream& operator<<(std::ostream& out, const imaging_column& column);

    private:
        column_id m_id;
        std::string m_name;
        size_t m_offset;
        string_vector m_subcolumn_names;
    };


}}}}

