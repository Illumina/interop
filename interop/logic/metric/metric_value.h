/** Logic to get a value from a metric
 *
 *  @file
 *  @date  4/29/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include <set>
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/model_exceptions.h"
#include "interop/constants/enums.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/metrics/corrected_intensity_metric.h"
#include "../../constants/enums.h"


namespace illumina { namespace interop { namespace logic { namespace metric {

    /** This template class retrieves a value from a metric object based on the value of the
     * provided metric_type enum and possibly an index or other information.
     *
     * This template class must be specialized for each metric type.
     */
    template<class M>
    class metric_value;

    /** Specialization for model::metrics::extraction_metric
     *
     * Supports enums: Intensity and FWHM
     */
    template<>
    class metric_value<model::metrics::extraction_metric>
    {
    public:
        /** Constructor
         *
         * @param _channel specific channel to select
         */
        metric_value(const size_t _channel) : channel(_channel){}
        /** Get the metric value corresponding to the metric_type enum value
         *
         * @param metric extraction metric
         * @param type metric type
         * @return metric value
         */
        float operator()(const model::metrics::extraction_metric& metric, const constants::metric_type type)const
        {
            switch(type)
            {
                case constants::Intensity:
                    return metric.max_intensity(channel);
                case constants::FWHM:
                    return metric.focus_score(channel);
                default:
                    throw model::invalid_metric_type("Unknown metric type");
            }
        }
    private:
        const size_t channel;
    };


    /** Specialization for model::metrics::q_metric
     *
     * Supports enums: PercentQ20, PercentQ30, AccumPercentQ20, AccumPercentQ30, and QScore
     */
    template<>
    class metric_value<model::metrics::q_metric>
    {
        typedef model::metrics::q_metric::uint_t uint_t;
    public:
        /** Constructor
         *
         * @param _index_for_qvalue Q20 or Q30 index
         * @param _bins bins for Q-value histogram
         */
        metric_value(const size_t _index_for_qvalue, const model::metrics::q_metric::qscore_bin_vector_type& _bins) :
                index_for_qvalue(static_cast<uint_t>(_index_for_qvalue)), bins(_bins){}
        /** Get the metric value corresponding to the metric_type enum value
         *
         * @param metric q-metric
         * @param type metric type
         * @return metric value
         */
        float operator()(const model::metrics::q_metric& metric, const constants::metric_type type)const
        {
            switch(type)
            {
                case constants::PercentQ20:
                case constants::PercentQ30:
                    return metric.percent_over_qscore(index_for_qvalue);
                case constants::AccumPercentQ20:
                case constants::AccumPercentQ30:
                    return metric.percent_over_qscore_cumulative(index_for_qvalue);
                case constants::QScore:
                    return static_cast<float>(metric.median(bins));
                default:
                    throw model::invalid_metric_type("Unknown metric type");
            }
        }
    private:
        const uint_t index_for_qvalue;
        const model::metrics::q_metric::qscore_bin_vector_type& bins;
    };

    /** Specialization for model::metrics::error_metric
     *
     * Supports enums: ErrorRate
     */
    template<>
    class metric_value<model::metrics::error_metric>
    {
    public:
        /** Get the metric value corresponding to the metric_type enum value
         *
         * @param metric error metric
         * @param type metric type
         * @return metric value
         */
        float operator()(const model::metrics::error_metric& metric, const constants::metric_type type)const
        {
            switch(type)
            {
                case constants::ErrorRate:
                    return metric.error_rate();
                default:
                    throw model::invalid_metric_type("Unknown metric type");
            }
        }
    };

    /** Specialization for model::metrics::corrected_intensity_metric
     *
     * Supports enums: PercentBase, CorrectedIntensity, CalledIntensity, and SignalToNoise
     */
    template<>
    class metric_value<model::metrics::corrected_intensity_metric>
    {
    public:
        /** Constructor
         *
         * @param _base specific base to select
         */
        metric_value(const constants::dna_bases _base) : base(_base){}
        /** Get the metric value corresponding to the metric_type enum value
         *
         * @param metric error metric
         * @param type metric type
         * @return metric value
         */
        float operator()(const model::metrics::corrected_intensity_metric& metric, const constants::metric_type type)const
        {
            switch(type)
            {
                case constants::PercentBase:
                    return metric.percent_base(base);
                case constants::CorrectedIntensity:
                    return metric.corrected_int_all(base);
                case constants::CalledIntensity:
                    return metric.corrected_int_called(base);
                case constants::SignalToNoise:
                    return metric.signal_to_noise();
                default:
                    throw model::invalid_metric_type("Unknown metric type");
            }
        }
    private:
        const constants::dna_bases base;
    };


    /** Specialization for model::metrics::tile_metric
     *
     * Supports enums: DensityPF, Density, ClusterCount, ClusterCountPF, PercentAligned, PercentPhasing,
     * and PercentPrephasing
     */
    template<>
    class metric_value<model::metrics::tile_metric>
    {
    public:
        /** Constructor
         *
         * @param _read specific read to select
         */
        metric_value(const size_t _read) : read(_read){}
        /** Get the metric value corresponding to the metric_type enum value
         *
         * @param metric error metric
         * @param type metric type
         * @return metric value
         */
        float operator()(const model::metrics::tile_metric& metric, const constants::metric_type type)const
        {
            const float density_scale = 1000;
            const float count_scale = 1000000;
            switch(type)
            {
                case constants::DensityPF:
                {
                    return metric.cluster_density_pf() / density_scale;
                }
                case constants::Density:
                {
                    return  metric.cluster_density() / density_scale;
                }
                case constants::ClusterCount:
                {
                    return metric.cluster_count() / count_scale;
                }
                case constants::ClusterCountPF:
                {
                    return metric.cluster_count_pf() / count_scale;
                }
                case constants::PercentAligned:
                {
                    for(size_t i=0;i<metric.read_metrics().size();++i)
                    {
                        if (read == metric.read_metrics()[i].read())
                        {
                            return metric.read_metrics()[i].percent_aligned();
                        }
                    }
                    return std::numeric_limits<float>::quiet_NaN();
                }
                case constants::PercentPhasing:
                {
                    for(size_t i=0;i<metric.read_metrics().size();++i)
                    {
                        if (read == metric.read_metrics()[i].read())
                        {
                            return metric.read_metrics()[i].percent_phasing();
                        }
                    }
                    return std::numeric_limits<float>::quiet_NaN();
                }
                case constants::PercentPrephasing:
                {
                    for(size_t i=0;i<metric.read_metrics().size();++i)
                    {
                        if (read == metric.read_metrics()[i].read())
                        {
                            return metric.read_metrics()[i].percent_prephasing();
                        }
                    }
                    return std::numeric_limits<float>::quiet_NaN();
                }
                default:
                    throw model::invalid_metric_type("Unknown metric type");
            }
        }
    private:
        const size_t read;
    };

}}}}

