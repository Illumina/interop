/** Logic to populate the imaging table
 *
 *  @file
 *  @date  7/22/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "interop/util/exception.h"
#include "interop/util/constant_mapping.h"
#include "interop/constants/enums.h"
#include "interop/model/run_metrics.h"
#include "interop/model/model_exceptions.h"
#include "interop/logic/utils/enums.h"
#include "interop/logic/table/table_util.h"
#include "interop/logic/summary/map_cycle_to_read.h"
#include "interop/logic/table/create_imaging_table_columns.h"

namespace illumina { namespace interop { namespace logic { namespace table
{
    /** Populate a table with data from a single InterOp
     */
    class table_populator
    {
        enum VoidType
        {
            Void
        };
        typedef model::metrics::q_metric::uint_t uint_t;
    public:
        /** Populate the ID columns of the table
         *
         * @param metric metric with source data
         * @param read read number, cycle within read
         * @param q20_idx index of the q20 value
         * @param q30_idx index of the q30 value
         * @param cluster_count_k cluster count in kilobases
         * @param naming_method tile naming method enum
         * @param columns vector of table columns
         * @param data_it iterator to current row of table data
         * @param data_end iterator to end of table data
         */
        template<class Metric, typename OutputIterator>
        static void populate_id(const Metric &metric,
                                const summary::read_cycle &read,
                                const size_t q20_idx,
                                const size_t q30_idx,
                                const float cluster_count_k,
                                const constants::tile_naming_method naming_method,
                                const std::vector <size_t> &columns,
                                OutputIterator data_it, OutputIterator data_end)
        {
            populate(static_cast<const model::metric_base::base_metric &>(metric),
                     read.number,
                     q20_idx,
                     q30_idx,
                     cluster_count_k,
                     naming_method,
                     columns,
                     data_it,
                     data_end);
            populate(static_cast<const model::metric_base::base_cycle_metric &>(metric),
                     read.number,
                     q20_idx,
                     q30_idx,
                     cluster_count_k,
                     naming_method,
                     columns,
                     data_it,
                     data_end);
            assign(data_it[columns[model::table::ReadColumn]], read.number);
            assign(data_it[columns[model::table::CycleWithinReadColumn]], read.cycle_within_read);
            (void) data_end;
        }

        /** Populate the data columns of the table
         *
         * @param metric metric with source data
         * @param read read number, cycle within read
         * @param q20_idx index of the q20 value
         * @param q30_idx index of the q30 value
         * @param cluster_count_k cluster count in kilobases
         * @param naming_method tile naming method enum
         * @param columns vector of table columns
         * @param data_it iterator to current row of table data
         * @param data_end iterator to end of table data
         */
        template<class Metric, typename OutputIterator>
        static void populate(const Metric &metric,
                             const size_t read,
                             const size_t q20_idx,
                             const size_t q30_idx,
                             const float cluster_count_k,
                             const constants::tile_naming_method naming_method,
                             const std::vector <size_t> &columns,
                             OutputIterator data_it, OutputIterator data_end)
        {
            /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
             * Add a method call to update each field with the given `metric`
             *
             * Example:
             * INTEROP_TUPLE7(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
             *
             * update_laneVoid(metric, q20_idx, q30_idx, naming_convention);
             */
#           define INTEROP_TUPLE7(Ignore1, Ignore2, Method, Param, Ignore4, Ignore5, Ignored6) \
                    populate_##Method##Param(metric, read, static_cast<uint_t>(q20_idx), static_cast<uint_t>(q30_idx), cluster_count_k, naming_method, columns, data_it, data_end);
            INTEROP_IMAGING_COLUMN_TYPES
#           undef INTEROP_TUPLE7 // Reuse this for another conversion
        }

    private:
        /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
         * This macro creates two functions, one to populate a field/column with the data from the correspond metric
         * and an empty method to ignore a group
         *
         * Example:
         * INTEROP_TUPLE7(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
         *
         * void update_laneVoid(const model::metric_base::base_metric& metric, const size_t Q20, const size_t Q30, const naming_method NamingConvention)
         * void update_laneVoid(const model::metric_base::empty_metric&)
         *
         * @note Param can be can field in this class, e.g. Read, or the function parameters Q20, Q30 or NamingConvention
         */
#       define INTEROP_TUPLE7(Id, Metric, Method, Param, Type, Kind, Round) \
                template<typename OutputIterator>\
                static void populate_##Method##Param(const model:: Metric& metric,\
                                                     const size_t Read,\
                                                     const uint_t Q20,\
                                                     const uint_t Q30,\
                                                     const float ClusterCountK,\
                                                     const constants::tile_naming_method NamingConvention,\
                                                     const std::vector<size_t>& columns,\
                                                     OutputIterator data_it, OutputIterator data_end)\
                {\
                    INTEROP_ASSERT( model::table:: Id##Column < columns.size() ); \
                    const size_t index = columns[model::table:: Id##Column];\
                    if(!is_valid(index)) return; /*Missing column */ \
                    copy_to(data_it+index, data_end, call_adapter(metric, Param, &model:: Metric::Method), Round);\
                    (void)Q20;(void)Q30;(void)NamingConvention;(void)Read;(void)ClusterCountK;\
                }\
                template<class MetricType, typename OutputIterator>\
                static void populate_##Method##Param(const MetricType&,\
                                                     const size_t,\
                                                     const uint_t,\
                                                     const uint_t,\
                                                     const float,\
                                                     const constants::tile_naming_method,\
                                                     const std::vector<size_t>&,\
                                                     OutputIterator,OutputIterator){}
        INTEROP_IMAGING_COLUMN_TYPES
#       undef INTEROP_TUPLE7 // Reuse this for another conversion

    private:
        /** Assign a value to an iterator
         *
         * @param destination reference to iterator value
         * @param end iterator to end of destination collection
         * @param source  value to assign
         * @param num_digits number of digits after the decimal
         */
        template<typename T, typename U>
        static void assign(T &destination, const U source, const size_t num_digits = 0)
        {
            //INTEROP_ASSERTMSG(std::isnan(destination), destination);
            if(is_valid(source))
                destination = static_cast<T>(roundto(source, num_digits));
        }

    private:
        inline static float roundto(const float val, const size_t n)
        {
            if (n > 0)
            {
                double mult = 1.0;
                for (size_t i = 0; i < n; ++i) mult *= 10;
                return static_cast<float>(std::floor(val * mult + 0.5) / mult);
            }
            return val;
        }

        template<typename T>
        inline static T roundto(const T val, const size_t)
        { return val; }

        /** Assign a value to an iterator
         *
         * @param destination iterator to desination collection
         * @param end iterator to end of destination collection
         * @param source  value to assign
         * @param num_digits number of digits after the decimal
         */
        template<typename OutputIterator, typename U>
        static void copy_to(OutputIterator destination, OutputIterator end, const U source, const size_t num_digits)
        {
            INTEROP_ASSERT(destination < end);
            (void) end;
            assign(*destination, source, num_digits);
        }

        /** Assign a value to an iterator
         *
         * @param destination iterator to desination collection
         * @param end iterator to end of destination collection
         * @param source  value to assign
         * @param num_digits number of digits after the decimal
         */
        template<typename OutputIterator, typename U>
        static void
        copy_to(OutputIterator destination, OutputIterator end, const std::vector <U> &source, const size_t num_digits)
        {
            (void) end;
            for (typename std::vector<U>::const_iterator it = source.begin(); it != source.end(); ++it, ++destination)
            {
                INTEROP_ASSERT(destination < end);
                assign(*destination, *it, num_digits);
            }
        }

        /** Test if a metric type is valid
         *
         * @param val floating point value
         * @return true if not NaN
         */
        static bool is_valid(const float val)
        {
            return !std::isnan(val);
        }

        /** Test if a metric type is valid
         *
         * @param val integral value
         * @return true
         */
        template<typename T>
        static bool is_valid(const T val)
        {
            return val < std::numeric_limits<T>::max();
        }

        /** Test if a metric type is valid
         *
         * @param values vector of values
         * @return true if not empty
         */
        template<typename T>
        static bool is_valid(const std::vector <T> &values)
        {
            return !values.empty();
        }
    };
}}}}

