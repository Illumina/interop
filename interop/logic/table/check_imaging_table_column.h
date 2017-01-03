/** Logic to check if a column should be populated
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
#include "interop/logic/utils/enums.h"
#include "interop/model/run_metrics.h"
#include "interop/model/model_exceptions.h"
#include "interop/model/table/imaging_column.h"
#include "interop/logic/table/table_util.h"

namespace illumina { namespace interop { namespace logic { namespace table
{
    /** Static class to hide methods created through macro expansion
     */
    class check_imaging_table_column
    {
        typedef model::metrics::q_metric::uint_t uint_t;
        enum VoidType
        {
            Void
        };
    public:
        /** Set all the id columns as filled
         *
         * @note Unlike the case of base_metric, nothing derives from metric_set<metric_base::base_metric>, so this must be explicit
         * @param filled boolean vector of flags indicating when the column is filled with data
         */
        static void set_id_filled(std::vector<bool> &filled)
        {
            set_filled_for_metric_set(model::metric_base::metric_set<model::metric_base::base_metric>(), filled, true);
            set_filled_for_metric_set(model::metric_base::metric_set<model::metric_base::base_cycle_metric>(), filled,
                                      true);
            set_filled_for_metric_set(model::metric_base::metric_set<model::metric_base::base_read_metric>(), filled,
                                      true);
        }

        /** Update the boolean filled metric array for each column based on whether the metric set is not empty
         *
         * @param metrics source metric set to populate the row of data
         * @param filled boolean vector of flags indicating when the column is filled with data
         * @param force_true force the filled column to be set true
         */
        template<class MetricSet>
        static void
        set_filled_for_metric_set(const MetricSet &metrics, std::vector<bool> &filled, const bool force_true = false)
        {
            /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
             * Add a method call to set a boolean flag to true if the column is occupied by values
             *
             * Example:
             * INTEROP_TUPLE7(Lane, metric_base::base_metric, lane, Void, UInt, IdType,0) ->
             *
             * set_filled_laneVoid(metrics, filled, force);
             */
#           define INTEROP_TUPLE7(Ignore1, Ignore2, Method, Param, Ignore4, Ignore5, Ignored6) set_filled_##Method##Param(metrics, filled, force_true);
            INTEROP_IMAGING_COLUMN_TYPES
#           undef INTEROP_TUPLE7 // Reuse this for another conversion
        }

        /** Update the filled metric array for each column based on whether the value is valid
         *
         * @note While this will work for any metric, it is intended only for tile_metrics. Unlike other metrics,
         * tile_metrics may have partial entries.
         *
         * @param metric source metric to populate the row of data
         * @param read read number
         * @param q20_idx index of the q20 value
         * @param q30_idx index of the q30 value
         * @param cluster_count_k cluster count in kilobases
         * @param naming_method tile naming method
         * @param filled boolean vector of flags indicating when the column is filled with data
         */
        template<class Metric>
        static void set_filled_for_metric(const Metric &metric,
                                          const size_t read,
                                          const size_t q20_idx,
                                          const size_t q30_idx,
                                          const float cluster_count_k,
                                          const constants::tile_naming_method naming_method,
                                          std::vector<bool> &filled)
        {
            /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
             * Add a method call to set a boolean flag to true if the column is occupied by valid values
             *
             * Example:
             * INTEROP_TUPLE7(Lane, metric_base::base_metric, lane, Void, UInt, IdType, 0) ->
             *
             * set_filled_metric_laneVoid(metric, read, q20_idx, q30_idx, naming_method, filled);
             */
#           define INTEROP_TUPLE7(Ignore1, Ignore2, Method, Param, Ignore4, Ignore5, Ignored6) \
                    set_filled_metric_##Method##Param(metric, read, static_cast<uint_t>(q20_idx), static_cast<uint_t>(q30_idx), cluster_count_k, naming_method, filled);
            INTEROP_IMAGING_COLUMN_TYPES
#           undef INTEROP_TUPLE7 // Reuse this for another conversion
        }

        /** Get the maximum number of digits to round
         *
         * @return maximum number of rounding digits
         */
        static uint32_t max_digits()
        {
#   define INTEROP_TUPLE7(Ignored0, Ignored1, Ignored2, Ignored3, Ignored4, Ignored5, Round) Round,
            uint32_t digits[] = { INTEROP_IMAGING_COLUMN_TYPES 0};
#   undef INTEROP_TUPLE7
            uint32_t max_digit = 0;
            for(size_t i=0;i<util::length_of(digits);++i)
                max_digit = std::max(max_digit, digits[i]);
            return max_digit;
        }

    private:
        /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
         * This macro creates two functions, one to set the filled array to true if the metric set is not empty
         * and the other does nothing.
         *
         * Example:
         * INTEROP_TUPLE7(Lane, metric_base::base_metric, lane, Void, UInt, IdType, 0) ->
         *
         * static void set_filled_laneVoid(const metric_base::metric_set<Metric>& metrics, std::vector<bool>& filled, const bool force_true)
         * static void set_filled_laneVoid(const model::metric_base::empty_header&, std::vector<bool>&, const bool)
         *
         */
#       define INTEROP_TUPLE7(Id, Metric, Method, Param, Ignored1, Ignored2, Ignored3) \
                static void set_filled_##Method##Param(const model::metric_base::metric_set<model:: Metric>& metrics, std::vector<bool>& filled, const bool force_true)\
                {\
                    filled[model::table::Id##Column] = (force_true) ? true : metrics.size()>0;\
                }\
                static void set_filled_##Method##Param(const model::metric_base::empty_header&, std::vector<bool>&, const bool){}
        INTEROP_IMAGING_COLUMN_TYPES
#       undef INTEROP_TUPLE7 // Reuse this for another conversion

    private:
        /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
         * This macro creates two functions, one to mark a column as filled if there is at least one valid metric
         *
         * Example:
         * INTEROP_TUPLE7(Lane, metric_base::base_metric, lane, Void, UInt, IdType, 0) ->
         *
         * static void set_filled_metric_laneVoid(const Metric& metric,
         *                                 const size_t Read,
         *                                 const uint_t Q20,
         *                                 const uint_t Q30,
         *                                 const constants::tile_naming_method NamingConvention,
         *                                 std::vector<bool>& filled)
         * static void set_filled_metric_laneVoid(const model::metric_base::empty_metric&
         *                                 const size_t,
         *                                 const uint_t,
         *                                 const uint_t,
         *                                 const constants::tile_naming_method,
         *                                 std::vector<bool>&)
         */
#       define INTEROP_TUPLE7(Id, Metric, Method, Param, Ignored1, Ignored2, Ignored3) \
                static void set_filled_metric_##Method##Param(const model::Metric& metric,\
                                                     const size_t Read,\
                                                     const uint_t Q20,\
                                                     const uint_t Q30,\
                                                     const float ClusterCountK,\
                                                     const constants::tile_naming_method NamingConvention,\
                                                     std::vector<bool>& filled)\
                {\
                    if(is_valid(call_adapter(metric, Param, &model:: Metric::Method))) filled[model::table::Id##Column] = true;\
                    (void)Q20;(void)Q30;(void)NamingConvention;(void)Read;(void)ClusterCountK;\
                }\
                static void set_filled_metric_##Method##Param(const model::metric_base::empty_metric&,\
                                                     const size_t,\
                                                     const uint_t,\
                                                     const uint_t,\
                                                     const float,\
                                                     const constants::tile_naming_method,\
                                                     std::vector<bool>&){}
        INTEROP_IMAGING_COLUMN_TYPES
#       undef INTEROP_TUPLE7 // Reuse this for another conversion

    private:
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

