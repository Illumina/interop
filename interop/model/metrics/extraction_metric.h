/** Extraction metric
 *
 * The extraction metrics include the max intensity and the focus score for each color channel.
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/ExtractionMetrics.bin
 *  - InterOp/ExtractionMetricsOut.bin
 *
 *  @file
 *
 *  @date 8/19/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <ctime>
#include <cstring>
#include <algorithm>
#include "interop/util/time.h"
#include "interop/io/format/generic_layout.h"
#include "interop/io/layout/base_metric.h"
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"

namespace illumina {
    namespace interop {
        namespace model {
            namespace metrics {
                /** Extraction metric
                 *
                 * The extraction metrics include the max intensity and the focus score for each color channel.
                 *
                 * @note Supported versions: 2
                 */
                class extraction_metric : public metric_base::base_cycle_metric
                {
                public:
                    enum{
                        /** Maximum number of channels
                         */
                        MAX_CHANNELS=4,
                        /** Unique type code for metric */
                        TYPE=constants::Extraction,
                        /** Latest version of the InterOp format */
                        LATEST_VERSION=2
                    };
                    /** Define a uint16_t array using an underlying vector
                     */
                    typedef std::vector<ushort_t> ushort_array_t;
                    /** Define a float array using an underlying vector
                     */
                    typedef std::vector<float> float_array_t;
                    /** Define a uint16_t pointer to a uint16_t array
                     */
                    typedef ::uint16_t* ushort_pointer_t;
                    /** Define a float pointer to a float array
                     */
                    typedef float* float_pointer_t;

                public:
                    /** Constructor
                     */
                    extraction_metric() :
                            metric_base::base_cycle_metric(0,0,0),
                            m_date_time_csharp(0),
                            m_date_time(0),
                            m_max_intensity_values(MAX_CHANNELS, 0),
                            m_focus_scores(MAX_CHANNELS, 0)
                    {
                    }
                    /** Constructor
                     *
                     * @note Version 2
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param dateTime time extraction was completed
                     * @param intensities_p90 90th percentile of intensities for the given channel
                     * @param focusScores focus score for the given channel
                     */
                    extraction_metric(const uint_t lane,
                                      const uint_t tile,
                                      const uint_t cycle,
                                      const ulong_t dateTime,
                                      const ushort_array_t& intensities_p90,
                                      const float_array_t& focusScores) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_date_time_csharp(util::csharp_date_time::to_csharp(dateTime)),
                            m_date_time(dateTime),
                            m_max_intensity_values(intensities_p90),
                            m_focus_scores(focusScores)
                    {
                        INTEROP_ASSERT(intensities_p90.size() <= MAX_CHANNELS);
                        INTEROP_ASSERT(m_focus_scores.size() <= MAX_CHANNELS);
                        m_max_intensity_values.resize(MAX_CHANNELS, 0);
                        m_focus_scores.resize(MAX_CHANNELS, 0);
                    }
                    /** Constructor
                     *
                     * @note Version 2
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param dateTime time extraction was completed
                     * @param intensities_p90 90th percentile of intensities for the given channel
                     * @param focusScores focus score for the given channel
                     * @param channel_count number of channels
                     */
                    extraction_metric(const uint_t lane,
                                      const uint_t tile,
                                      const uint_t cycle,
                                      const ulong_t dateTime,
                                      const ushort_pointer_t intensities_p90,
                                      const float_pointer_t focusScores,
                                      const uint_t channel_count=MAX_CHANNELS) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_date_time_csharp(util::csharp_date_time::to_csharp(dateTime)),
                            m_date_time(dateTime),
                            m_max_intensity_values(intensities_p90, intensities_p90+channel_count),
                            m_focus_scores(focusScores, focusScores+channel_count)
                    {
                    }
                    /** Constructor
                     *
                     * @note Version 2
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param dateTime time extraction was completed
                     * @param intensities_p90 90th percentile of intensities for the given channel
                     * @param focusScores focus score for the given channel
                     */
                    extraction_metric(const uint_t lane,
                                      const uint_t tile,
                                      const uint_t cycle,
                                      const util::csharp_date_time dateTime,
                                      const ushort_array_t& intensities_p90,
                                      const float_array_t& focusScores) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_date_time_csharp(dateTime),
                            m_date_time(dateTime.to_unix()),
                            m_max_intensity_values(intensities_p90),
                            m_focus_scores(focusScores)
                    {
                        INTEROP_ASSERT(intensities_p90.size() <= MAX_CHANNELS);
                        INTEROP_ASSERT(m_focus_scores.size() <= MAX_CHANNELS);
                        m_max_intensity_values.resize(MAX_CHANNELS, 0);
                        m_focus_scores.resize(MAX_CHANNELS, 0);
                    }
                    /** Constructor
                     *
                     * @note Version 2
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param dateTime time extraction was completed
                     * @param intensities_p90 90th percentile of intensities for the given channel
                     * @param focusScores focus score for the given channel
                     * @param channel_count number of channels
                     */
                    extraction_metric(const uint_t lane,
                                      const uint_t tile,
                                      const uint_t cycle,
                                      const util::csharp_date_time dateTime,
                                      const ushort_pointer_t intensities_p90,
                                      const float_pointer_t focusScores,
                                      const uint_t channel_count=MAX_CHANNELS) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_date_time_csharp(dateTime),
                            m_date_time(dateTime.to_unix()),
                            m_max_intensity_values(intensities_p90, intensities_p90+channel_count),
                            m_focus_scores(focusScores, focusScores+channel_count)
                    {
                    }

                public:
                    /** @defgroup extraction_metric Extraction Metrics
                     *
                     * Per tile per cycle metrics related to extraction:
                     *  1. Per channel p90 intensity values
                     *  2. Per channel focus scores
                     *
                     * @ref illumina::interop::model::metrics::extraction_metric "See full class description"
                     *
                     * @ingroup run_metrics
                     * @note All metrics in this class are supported by all versions
                     * @{
                     */
                    /** Date time extraction completed
                     *
                     * @deprecated Will be removed in 1.1.x (use date_time instead)
                     * @return date time code
                     */
                    ulong_t dateTime()const
                    {
                        return m_date_time;
                    }
                    /** Date time extraction completed
                     *
                     * @return date time code
                     */
                    ulong_t date_time()const
                    {
                        return m_date_time;
                    }
                    /** Date time extraction completed
                     *
                     * @note C# DataTime.ToBinary int64 format
                     *
                     * @return date time code
                     */
                    const util::csharp_date_time& date_time_csharp()const
                    {
                        return m_date_time_csharp;
                    }
                    /** Median P90 (P99) intensity over all non-overlapping subregion bins
                     *
                     * @param channel channel index
                     * @return median 90th percentile of the intensities
                     */
                    ushort_t max_intensity(size_t channel)const
                    {
                       INTEROP_ASSERT(channel<MAX_CHANNELS);
                       INTEROP_ASSERT(m_max_intensity_values.size() == MAX_CHANNELS);
                        return m_max_intensity_values[channel];
                    }
                    /** Median Full Width Half Max (FWHM) Focus Metric
                     *
                     * @deprecated Will be removed in 1.1.x (use focus_score instead)
                     * @param channel channel index
                     * @return focus metric as measured by FWHM
                     */
                    float focusScore(size_t channel)const
                    {
                        INTEROP_ASSERT(channel <MAX_CHANNELS);
                        INTEROP_ASSERT(m_focus_scores.size()==MAX_CHANNELS);
                        return m_focus_scores[channel];
                    }
                    /** Median Full Width Half Max (FWHM) Focus Metric
                     *
                     * @param channel channel index
                     * @return focus metric as measured by FWHM
                     */
                    float focus_score(size_t channel)const
                    {
                        INTEROP_ASSERT(channel <MAX_CHANNELS);
                        INTEROP_ASSERT(m_focus_scores.size()==MAX_CHANNELS);
                        return m_focus_scores[channel];
                    }
                    /** Get an array of maximum intensity values
                     *
                     * These values are estimated to be a 9x percentile of intensities extracted from the image
                     *
                     * @return vector of intensity values
                     */
                    const ushort_array_t &max_intensity_values()const{return m_max_intensity_values;}
                    /** Get an array of focus scores
                     *
                     * These values are estimated using full width half max (FWHM).
                     *
                     * @return vector of focus scores
                     */
                    const float_array_t& focusScores()const{return m_focus_scores;}
                    /** Get the number of channels
                     *
                     * @return number of channels
                     */
                    size_t channel_count()const
                    {
                        return MAX_CHANNELS;
                    }
                    /** @} */

                public:
                    /** Get the prefix of the InterOp filename
                     *
                     * @return prefix
                     */
                    static const char* prefix(){return "Extraction";}

                private:
                    util::csharp_date_time m_date_time_csharp;
                    ulong_t m_date_time;
                    ushort_array_t m_max_intensity_values;
                    float_array_t m_focus_scores;
                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
            }
        }
    }
}

