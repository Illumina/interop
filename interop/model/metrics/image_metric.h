/** Image metric
 *
 * Per tile per cycle per channel percentiles of pixel values used to autocontrast thumbnail images
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/ImageMetrics.bin
 *  - InterOp/ImageMetricsOut.bin
 *
 *  @file
 *
 *  @date 8/20/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <assert.h>
#include <cstring>
#include "interop/io/format/generic_layout.h"
#include "interop/io/layout/base_metric.h"
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"

namespace illumina {
    namespace interop {
        namespace model {
            namespace metrics {
                /** Header information for writing an image metric set
                 */
                class image_metric_header : public metric_base::base_cycle_metric::header_type
                {
                public:
                    enum{
                        /** Maximum number of channels supported **/
                        MAX_CHANNELS=4
                    };
                    /** Unsigned int16_t
                     */
                    typedef ::uint16_t ushort_t;
                public:
                    /** Constructor
                     *
                     * @param channelCount number of channels
                     */
                    image_metric_header(ushort_t channelCount) : m_channelCount(channelCount) {}
                    /** Number of channels
                     *
                     * @return number of channels
                     */
                    ushort_t channelCount()const{return m_channelCount;}
                    /** Generate a default header
                     *
                     * @return default header
                     */
                    static image_metric_header default_header()
                    {
                        return image_metric_header(MAX_CHANNELS);
                    }
                private:
                    ushort_t m_channelCount;
                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
                /** Image metric
                 *
                 * Per tile per cycle per channel percentiles of pixel values used to autocontrast thumbnail images
                 *
                 * @note Supported versions: 1 and 2
                 */
                class image_metric : public metric_base::base_cycle_metric
                {
                public:
                    enum{
                        /** Maximum number of channels supported **/
                        MAX_CHANNELS=image_metric_header::MAX_CHANNELS,
                        /** Unique type code for metric */
                        TYPE=constants::Image,
                        /** Latest version of the InterOp format */
                        LATEST_VERSION=2
                    };
                    /** Image metric header
                     */
                    typedef image_metric_header header_type;
                    /** Define a uint16_t array using an underlying vector
                     */
                    typedef std::vector<ushort_t> ushort_array_t;
                    /** Define a uint16_t pointer to a uint16_t array
                     */
                    typedef ::uint16_t* ushort_pointer_t;
                public:
                    /** Constructor
                     */
                    image_metric() :
                            metric_base::base_cycle_metric(0,0,0),
                            m_minContrast(MAX_CHANNELS, 0),
                            m_maxContrast(MAX_CHANNELS, 0),
                            m_channelCount(0){}
                    /** Constructor
                     *
                     * @note Version 2
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param channel_count number of channels
                     * @param minContrast minimum contrast for each channel
                     * @param maxContrast maximum contrast for each channel
                     */
                    image_metric(const uint_t lane,
                                      const uint_t tile,
                                      const uint_t cycle,
                                      const ushort_t channel_count,
                                      const ushort_array_t& minContrast,
                                      const ushort_array_t& maxContrast) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_minContrast(minContrast),
                            m_maxContrast(maxContrast),
                            m_channelCount(channel_count)
                    {
                    }
                    /** Constructor
                     *
                     * @note Version 2
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param channel_count number of channels
                     * @param minContrast minimum contrast for each channel
                     * @param maxContrast maximum contrast for each channel
                     */
                    image_metric(const uint_t lane,
                                 const uint_t tile,
                                 const uint_t cycle,
                                 const ushort_t channel_count,
                                 const ushort_pointer_t minContrast,
                                 const ushort_pointer_t maxContrast) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_minContrast(minContrast, minContrast+channel_count),
                            m_maxContrast(maxContrast, maxContrast+channel_count),
                            m_channelCount(channel_count)
                    {
                    }

                public:
                    /** @defgroup image_metric Image Metrics
                     *
                     * Per tile per cycle image contrast metrics
                     *
                     * @ref illumina::interop::model::metrics::image_metric "See full class description"
                     *
                     * @ingroup run_metrics
                     * @note All metrics in this class are supported by all versions
                     * @{
                     */
                    /** Minimum contrast intensity
                     *
                     * @return minimum contrast intensity
                     */
                    ushort_t minContrast(size_t channel)const
                    {
                        assert(channel < m_channelCount);
                        return m_minContrast[channel];
                    }
                    /** Maximum contrast intensity
                     *
                     * @return maximum contrast intensity
                     */
                    ushort_t maxContrast(size_t channel)const
                    {
                        assert(channel < m_channelCount);
                        return m_maxContrast[channel];
                    }
                    /** Minimum contrast intensity
                     *
                     * @return minimum contrast intensity
                     */
                    const ushort_array_t& minContrast()const
                    {
                        return m_minContrast;
                    }
                    /** Maximum contrast intensity
                     *
                     * @return maximum contrast intensity
                     */
                    const ushort_array_t& maxContrast()const
                    {
                        return m_maxContrast;
                    }
                    /** Number of channels
                     *
                     * @return number of channels
                     */
                    ushort_t channelCount()const
                    {
                        return m_channelCount;
                    }
                    /** @} */

                public:
                    /** Get the prefix of the InterOp filename
                     *
                     * @return prefix
                     */
                    static const char* prefix(){return "Image";}

                private:
                    ushort_array_t m_minContrast;
                    ushort_array_t m_maxContrast;
                    ushort_t m_channelCount;
                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
            }
        }
    }
}

