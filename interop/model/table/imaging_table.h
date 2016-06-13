/** Model for the imaging table
 *
 *  @file
 *  @date  5/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/error_metric.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/table/table_util.h"

/** Mapping of data for the imaging table
 *
 * Column 1:
 *  - Name of the field in the table_entry class
 *  - Converted to column header in imaging table
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
 *
 *  The metrics marked "dummy" are populated not in the update function, but in the constructor.
 *
 * @note This macro requires the macro INTEROP_TUPLE6 to be defined before use
 * @see illumina::interop::model::table::table_entry
 * @see illumina::interop::logic::table::imaging_table_column_names
 */
#define INTEROP_IMAGING_COLUMN_TYPES \
        INTEROP_TUPLE6(Lane,                  metric_base::base_metric,           lane,                       Void, UInt, IdType)\
        INTEROP_TUPLE6(Tile,                  metric_base::base_metric,           tile,                       Void, UInt, IdType)\
        INTEROP_TUPLE6(Cycle,                 metric_base::base_cycle_metric,     cycle/*dummy*/,             Void, UInt, IdType)\
        INTEROP_TUPLE6(Read,                  metric_base::base_read_metric,      read/*dummy*/,              Void, UInt, IdType)\
        INTEROP_TUPLE6(CycleWithinRead,       metric_base::base_read_metric,      id/*dummy*/,                Void, UInt, IdType)\
        INTEROP_TUPLE6(DensityKPermm2,        metrics::tile_metric,               cluster_density,            Void, Float, ValueType)\
        INTEROP_TUPLE6(ClusterCountPfK,       metrics::tile_metric,               cluster_density_pf,         Void, Float, ValueType)\
        INTEROP_TUPLE6(PercentPassFilter,     metrics::tile_metric,               percent_pf,                 Void, Float, ValueType)\
        INTEROP_TUPLE6(PercentAligned,        metrics::tile_metric,               percent_aligned_at,         Read, Float, ValueType)\
        INTEROP_TUPLE6(PercentPhasing,        metrics::tile_metric,               percent_phasing_at,         Read, Float, ValueType)\
        INTEROP_TUPLE6(PercentPrephasing,     metrics::tile_metric,               percent_prephasing_at,      Read, Float, ValueType)\
        INTEROP_TUPLE6(ErrorRate,             metrics::error_metric,              error_rate,                 Void, Float, ValueType)\
        INTEROP_TUPLE6(PercentGreaterThanQ20, metrics::q_metric,                  percent_over_qscore,        Q20,  Float, ValueType)\
        INTEROP_TUPLE6(PercentGreaterThanQ30, metrics::q_metric,                  percent_over_qscore,        Q30,  Float, ValueType)\
        INTEROP_TUPLE6(P90,                   metrics::extraction_metric,         max_intensity_values,       Void, UShort, ChannelArray)\
        INTEROP_TUPLE6(PercentNoCalls,        metrics::corrected_intensity_metric,percent_nocall,             Void, Float, ValueType)\
        INTEROP_TUPLE6(PercentBase,           metrics::corrected_intensity_metric,percent_bases,              Void, Float, BaseArray)\
        INTEROP_TUPLE6(Fwhm,                  metrics::extraction_metric,         focus_scores,               Void, Float, ChannelArray) \
        INTEROP_TUPLE6(Corrected,             metrics::corrected_intensity_metric,corrected_int_all_array,    Void, UShort, BaseArray)\
        INTEROP_TUPLE6(Called,                metrics::corrected_intensity_metric,corrected_int_called_array, Void, UShort, BaseArray)\
        INTEROP_TUPLE6(SignalToNoise,         metrics::corrected_intensity_metric,signal_to_noise,            Void, Float, ValueType)\
        INTEROP_TUPLE6(MinimumContrast,       metrics::image_metric,              min_contrast_array,         Void, UShort, ChannelArray)\
        INTEROP_TUPLE6(MaximumContrast,       metrics::image_metric,              max_contrast_array,         Void, UShort, ChannelArray)\
        INTEROP_TUPLE6(Time,                  metrics::extraction_metric,         date_time,                  Void, Float, ValueType)\
        INTEROP_TUPLE6(Surface,               metric_base::base_metric,           surface,                    NamingConvention, UInt, IdType)\
        INTEROP_TUPLE6(Swath,                 metric_base::base_metric,           swath,                      NamingConvention, UInt, IdType)\
        INTEROP_TUPLE6(Section,               metric_base::base_metric,           section,                    NamingConvention, UInt, IdType)\
        INTEROP_TUPLE6(TileNumber,            metric_base::base_metric,           number,                     NamingConvention, UInt, IdType)

namespace illumina { namespace interop { namespace model { namespace table {
#   define INTEROP_TUPLE6(Id, Ignored1, Ignored2, Ignored3, Ignored4, Ignored5) Id,
    /** Map column index to name */
    enum column_type{ INTEROP_IMAGING_COLUMN_TYPES ImagingColumnCount};
#   undef INTEROP_TUPLE6

    /** Column header for the imaging table
     */
    struct column_header
    {
        /** Constructor */
        column_header(){}
        /** Constructor
         *
         * @param title tile of the column
         */
        column_header(const std::string& title) : m_subtitle(title) {}
        /** Constructor
         *
         * @param title tile of the column
         * @param subtitle subtile of the column
         */
        column_header(const std::string& title, const std::string& subtitle) : m_title(title), m_subtitle(subtitle){}

    public:
        /** Get the group title
         *
         * @return group title
         */
        const std::string& title()const
        {
            return m_title;
        }
        /** Get the subtitle
         *
         * @return subtitle
         */
        const std::string& subtitle()const
        {
            return m_subtitle;
        }

    private:
        /** Title of a group of columns */
        std::string m_title;
        /** Title of the column */
        std::string  m_subtitle;
    };

    /** Single row for the imaging table
     */
    struct table_entry
    {
        /** Defines the type for the Id columns */
        typedef ::uint32_t UIntIdType;
        /** Defines the type for the metric value columns */
        typedef float FloatValueType;
        /** Defines the type for the ushort array columns */
        typedef metrics::extraction_metric::ushort_array_t ShortChannelArray;
        /** Defines the type for the float array columns */
        typedef metrics::extraction_metric::float_array_t FloatChannelArray;
        /** Defines the type for the ushort array columns */
        typedef metrics::extraction_metric::ushort_array_t UShortChannelArray;
        /** Defines the type for the float array columns */
        typedef metrics::corrected_intensity_metric::float_array_t FloatBaseArray;
        /** Defines the type for the float array columns */
        typedef metrics::corrected_intensity_metric::ushort_array_t UShortBaseArray;
        /** Shortens  tile_naming_method to naming_method */
        typedef constants::tile_naming_method naming_method;
        /** Defines unsigned int */
        typedef metrics::q_metric::uint_t uint_t;
        enum void_type {
            /** Place holder for functions that take no parameters */
            Void
        };
        /* Defines the initialization of the fields in this class.
         *
         * Example:
         * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
         * table_entry() : Lane(), Q20(), Q30(), NamingConvention(0) {}
         */
#       define INTEROP_TUPLE6(Id, Ignored1, Ignored2, Ignored3, Ignored4, Ignored5) Id(),
        /** Constructor
         */
        table_entry() : INTEROP_IMAGING_COLUMN_TYPES Q20(), Q30(), NamingConvention(constants::UnknownTileNamingMethod){}
        /** Constructor
         *
         * @param metric metric record
         * @param read read number
         * @param cycle_in_read cycle number within the read
         * @param q20_idx index of Q20 bin
         * @param q30_idx index of Q30 bin
         * @param naming_convention tile naming method
         */
        template<class Metric>
        table_entry(const Metric& metric,
                    const size_t read,
                    const size_t cycle_in_read,
                    const size_t q20_idx,
                    const size_t q30_idx,
                    const naming_method naming_convention) :
                INTEROP_IMAGING_COLUMN_TYPES
                Q20(static_cast<uint_t>(q20_idx)),
                Q30(static_cast<uint_t>(q30_idx)),
                NamingConvention(naming_convention)
        {
            Cycle = metric.cycle();
            Read = static_cast<UIntIdType>(read);
            CycleWithinRead = static_cast<UIntIdType>(cycle_in_read);
            update(metric);
        }
#       undef INTEROP_TUPLE6 // Reuse this for another conversion

        /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
         * Create a field for the table_entry class
         *
         * Example:
         * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
         * IdType Lane;
         *
         * @see INTEROP_IMAGING_COLUMN_TYPES
         */
#          define INTEROP_TUPLE6(Id, Ignored1, Ignored2, Ignored3, Type, Group) Type##Group Id;
            INTEROP_IMAGING_COLUMN_TYPES
#          undef INTEROP_TUPLE6 // Reuse this for another conversion
    private:
        // Additional parameters when getting the appropriate metric
        uint_t Q20; // Used as a parameter to percent_over_qscore to get PercentGreaterThanQ20
        uint_t Q30; // Used as a parameter to percent_over_qscore to get PercentGreaterThanQ30
        naming_method NamingConvention; // Used as a parameter to swath, section, surface, tile
        // Read (one of the column fields, is used to get percent_aligned, percent_phasing, percent_prephasing
    public:
        /** Update the table entry with the given metric
         *
         * @param metric source metric to populate the row of data
         */
        template<class Metric>
        void update(const Metric& metric)
        {
            /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
             * Add a method call to update each field with the given `metric`
             *
             * Example:
             * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
             *
             * update_laneVoid(metric, q20_idx, q30_idx, naming_convention);
             */
#           define INTEROP_TUPLE6(Ignore1, Ignore2, Method, Param, Ignore4, Ignore5) update_##Method##Param(metric);
            INTEROP_IMAGING_COLUMN_TYPES
#           undef INTEROP_TUPLE6 // Reuse this for another conversion
        }
        /** Update the boolean filled metric array for each column based on whether the metric set is not empty
         *
         * @param metrics source metric set to populate the row of data
         * @param filled boolean vector of flags indicating when the column is filled with data
         * @param force_true force the filled column to be set true
         */
        template<class MetricSet>
        static void set_filled(const MetricSet& metrics, std::vector<bool>& filled, const bool force_true=false)
        {
            /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
             * Add a method call to set a boolean flag to true if the column is occupied by values
             *
             * Example:
             * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
             *
             * set_filled_laneVoid(metric, filled, force);
             */
#           define INTEROP_TUPLE6(Ignore1, Ignore2, Method, Param, Ignore4, Ignore5) set_filled_##Method##Param(metrics, filled, force_true);
            INTEROP_IMAGING_COLUMN_TYPES
#           undef INTEROP_TUPLE6 // Reuse this for another conversion
        }
        /** Update the filled metric array for each column based on whether the value is valid
         *
         * @note While this will work for any metric, it is intended only for tile_metrics. Unlike other metrics,
         * tile_metrics may have partial entries.
         *
         * @param metric source metric to populate the row of data
         * @param filled boolean vector of flags indicating when the column is filled with data
         */
        template<class Metric>
        void set_filled_metric(const Metric& metric, std::vector<bool>& filled)const
        {
            /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
             * Add a method call to set a boolean flag to true if the column is occupied by valid values
             *
             * Example:
             * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
             *
             * set_filled_metric_laneVoid(metric, filled);
             */
#           define INTEROP_TUPLE6(Ignore1, Ignore2, Method, Param, Ignore4, Ignore5) set_filled_metric_##Method##Param(metric, filled);
            INTEROP_IMAGING_COLUMN_TYPES
#           undef INTEROP_TUPLE6 // Reuse this for another conversion
        }
        /** Set all the id columns as filled
         *
         * @note Unlike the case of base_metric, nothing derives from metric_set<metric_base::base_metric>, so this must be explicit
         * @param filled boolean vector of flags indicating when the column is filled with data
         */
        static void set_id_filled(std::vector<bool>& filled)
        {
            set_filled(metric_base::metric_set<metric_base::base_metric>(), filled, true);
            set_filled(metric_base::metric_set<metric_base::base_cycle_metric>(), filled, true);
            set_filled(metric_base::metric_set<metric_base::base_read_metric>(), filled, true);
        }

    private:
        /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
         * This macro creates two functions, one to populate a field/column with the data from the correspond metric
         * and an empty method to ignore a group
         *
         * Example:
         * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
         *
         * void update_laneVoid(const model::metric_base::base_metric& metric, const size_t Q20, const size_t Q30, const naming_method NamingConvention)
         * void update_laneVoid(const model::metric_base::empty_metric&)
         *
         * @note Param can be can field in this class, e.g. Read, or the function parameters Q20, Q30 or NamingConvention
         */
#       define INTEROP_TUPLE6(Id, Metric, Method, Param, Type, Kind) \
                void update_##Method##Param(const Metric& metric)\
                {\
                    Id = static_cast<Type##Kind>(call_adapter(metric, Param, &Metric::Method)); \
                    (void)Q20;(void)Q30;(void)NamingConvention;\
                }\
                void update_##Method##Param(const model::metric_base::empty_metric&){}
        INTEROP_IMAGING_COLUMN_TYPES
#       undef INTEROP_TUPLE6 // Reuse this for another conversion

        /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
         * This macro creates two functions, one to set the filled array to true if the metric set is not emtpy
         * and the other does nothing.
         *
         * Example:
         * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
         *
         * void set_filled_laneVoid(const metric_base::metric_set<Metric>& metrics, std::vector<bool>& filled, const bool force_true)
         * void set_filled_laneVoid(const model::metric_base::empty_header&, std::vector<bool>&, const bool)
         *
         */
#       define INTEROP_TUPLE6(Id, Metric, Method, Param, Ignored1, Ignored2) \
                static void set_filled_##Method##Param(const metric_base::metric_set<Metric>& metrics, std::vector<bool>& filled, const bool force_true)\
                {\
                    filled[table::Id] = (force_true) ? true : metrics.size()>0;\
                }\
                static void set_filled_##Method##Param(const model::metric_base::empty_header&, std::vector<bool>&, const bool){}
        INTEROP_IMAGING_COLUMN_TYPES
#       undef INTEROP_TUPLE6 // Reuse this for another conversion
        /* For every entry in INTEROP_IMAGING_COLUMN_TYPES
         * This macro creates two functions, one to mark a column as filled if there is at least one valid metric
         *
         * Example:
         * INTEROP_TUPLE6(Lane, metric_base::base_metric, lane, Void, UInt, IdType) ->
         *
         * void set_filled_metric_laneVoid(const Metric& metric, std::vector<bool>& filled)const
         * void set_filled_metric_laneVoid(const model::metric_base::empty_metric&, std::vector<bool>&)const
         */
#       define INTEROP_TUPLE6(Id, Metric, Method, Param, Ignored1, Ignored2) \
                void set_filled_metric_##Method##Param(const Metric& metric, std::vector<bool>& filled)const\
                {\
                    filled[table::Id] = is_valid(call_adapter(metric, Param, &Metric::Method));\
                }\
                void set_filled_metric_##Method##Param(const model::metric_base::empty_metric&, std::vector<bool>&)const{}
        INTEROP_IMAGING_COLUMN_TYPES
#       undef INTEROP_TUPLE6 // Reuse this for another conversion
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
        static bool is_valid(const T)
        {
            return true;
        }
        /** Test if a metric type is valid
         *
         * @param values vector of values
         * @return true if not empty
         */
        template<typename T>
        static bool is_valid(const std::vector<T>& values)
        {
            return !values.empty();
        }
    };

}}}}
