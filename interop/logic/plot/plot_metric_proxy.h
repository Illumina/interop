/** Logic to select metric from data for plotting
 *
 * Supports: Flowcell, ByCycle and ByLane
 *
 *  @file
 *  @date  4/24/17
 *  @version 1.0
 *  @copyright Illumina Use Only
 */
#pragma once
#include "interop/util/statistics.h"
#include "interop/model/plot/filter_options.h"
#include "interop/model/run_metrics.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{

    /** Proxy using reflection-like macros to select plotting data from run_metrics
     *
     */
    class plot_metric_proxy
    {
        typedef model::metric_base::base_metric::uint_t id_t;
        typedef model::metric_base::metric_set<model::metrics::tile_metric> tile_metric_set;
    public:
        /** Switch statement that selects the proper metric set and function from the individual metric
         *
         * @tparam Plot functor type
         * @param metrics run metrics
         * @param options filter options
         * @param type metric type
         * @param plot plotting functor
         */
        template<typename Plot>
        static void select(const model::metrics::run_metrics& metrics,
                            const model::plot::filter_options& options,
                            const constants::metric_type type,
                           Plot& plot)
        {
            switch(type)
            {
#       define INTEROP_TUPLE7(Enum, Unused1, Unused2, Unused3, Metric, Method, Param) \
                case constants:: Enum:\
                    select_metric_##Method##Param(metrics.get< model::metrics:: Metric >(),\
                                                  options, options.channel(),\
                                                  options.dna_base(),\
                                                  options.read(), metrics.get<tile_metric_set>(), plot);\
                    break;//
#       define INTEROP_TUPLE4(Unused1, Unused2, Unused3, Unused4)
#       define INTEROP_TUPLE1(Unused1)
                INTEROP_ENUM_METRIC_TYPES
#       undef INTEROP_TUPLE4 // Reuse this for another conversion
#       undef INTEROP_TUPLE7 // Reuse this for another conversion
#       undef INTEROP_TUPLE1
                default:
                    INTEROP_THROW(model::invalid_metric_type, "Invalid metric group: " << constants::to_string(type));
            };
        }
        /** Check if the metric is present
         *
         * @param metrics run metrics
         * @param type metric type
         * @return true if the metric is present
         */
        static bool is_present(const model::metrics::run_metrics& metrics,
                               const constants::metric_type type)
        {
            switch(type)
            {
#       define INTEROP_TUPLE7(Enum, Unused1, Unused2, Unused3, Metric, Method, Param) \
                case constants:: Enum:\
                    return is_metric_present_##Method##Param(metrics.get< model::metrics:: Metric >(), 0u, \
                                                  constants::A, 1u,\
                                                  metrics.get<tile_metric_set>());//
#       define INTEROP_TUPLE4(Unused1, Unused2, Unused3, Unused4)
#       define INTEROP_TUPLE1(Unused1)
                INTEROP_ENUM_METRIC_TYPES
#       undef INTEROP_TUPLE4 // Reuse this for another conversion
#       undef INTEROP_TUPLE7 // Reuse this for another conversion
#       undef INTEROP_TUPLE1
                default:
                    return false;
            };
        }

    private:
#       define INTEROP_TUPLE7(Enum, Unused1, Unused2, Unused3, Metric, Method, Param) \
                template<typename Plot>\
                static void select_metric_##Method##Param(const model::metric_base::metric_set< model::metrics:: Metric >& metrics,\
                                                          const model::plot::filter_options& options,\
                                                          const size_t channel, \
                                                          const constants::dna_bases base,\
                                                          const id_t read,\
                                                          const tile_metric_set& tile_metrics,\
                                                          Plot& plot)\
                {\
                    const util::op::dummy_arg Void;\
                    (void)channel;(void)base;(void)read;(void)tile_metrics;(void)Void;\
                    plot(metrics, options, util::op::const_member_function(Param, &model::metrics:: Metric::Method));\
                }
#       define INTEROP_TUPLE4(Unused1, Unused2, Unused3, Unused4)
#       define INTEROP_TUPLE1(Unused1)
        INTEROP_ENUM_METRIC_TYPES
#       undef INTEROP_TUPLE7 // Reuse this for another conversion
#       undef INTEROP_TUPLE4 // Reuse this for another conversion
#       undef INTEROP_TUPLE1

    private:
#       define INTEROP_TUPLE7(Enum, Unused1, Unused2, Unused3, Metric, Method, Param) \
                static bool is_metric_present_##Method##Param(const model::metric_base::metric_set< model::metrics:: Metric >& metrics,\
                                                          const size_t channel, \
                                                          const constants::dna_bases base,\
                                                          const id_t read,\
                                                          const tile_metric_set& tile_metrics)\
                {\
                    const util::op::dummy_arg Void;\
                    (void)channel;(void)base;(void)read;(void)tile_metrics;(void)Void;\
                    return is_metric_present_in(metrics, util::op::const_member_function(Param, &model::metrics:: Metric::Method));\
                }
#       define INTEROP_TUPLE4(Unused1, Unused2, Unused3, Unused4)
#       define INTEROP_TUPLE1(Unused1)
        INTEROP_ENUM_METRIC_TYPES
#       undef INTEROP_TUPLE7 // Reuse this for another conversion
#       undef INTEROP_TUPLE4 // Reuse this for another conversion
#       undef INTEROP_TUPLE1
        template<class MetricSet, typename Func>
        static bool is_metric_present_in(const MetricSet& metricset, Func proxy)
        {
            for(typename MetricSet::const_iterator it = metricset.begin(); it != metricset.end();++it)
            {
                if(is_valid(proxy(*it))) return true;
            }
            return false;
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
    };

}}}}
