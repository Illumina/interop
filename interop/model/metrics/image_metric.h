/** Image metric
 *
 * Per tile per cycle per channel percentiles of pixel values used to autocontrast thumbnail images
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/ImageMetrics.bin
 *  - InterOp/ImageMetricsOut.bin
 *
 *  @file
 *  @date 8/20/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <cstring>
#include "interop/util/exception.h"
#include "interop/io/format/generic_layout.h"
#include "interop/io/layout/base_metric.h"
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** Header information for writing an image metric set
     */
    class image_metric_header : public metric_base::base_cycle_metric::header_type
    {
    public:
        enum
        {
            /** Maximum number of channels supported **/
            MAX_CHANNELS = 4
        };
        /** Unsigned int16_t
         */
        typedef ::uint16_t ushort_t;
    public:
        /** Constructor
         *
         * @param channel_count number of channels
         */
        image_metric_header(ushort_t channel_count) : m_channel_count(channel_count)
        { }

        /** Number of channels
         *
         * @deprecated Use channel_count() instead
         * @return number of channels
         */
        ushort_t channelCount() const
        { return m_channel_count; }
        /** Number of channels
         *
         * @return number of channels
         */
        ushort_t channel_count() const
        { return m_channel_count; }

        /** Generate a default header
         *
         * @return default header
         */
        static image_metric_header default_header()
        {
            return image_metric_header(MAX_CHANNELS);
        }
        /** Clear the data
         */
        void clear()
        {
            m_channel_count=MAX_CHANNELS;
            metric_base::base_cycle_metric::header_type::clear();
        }

    private:
        ushort_t m_channel_count;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
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
        enum
        {
            /** Maximum number of channels supported **/
            MAX_CHANNELS = image_metric_header::MAX_CHANNELS,
            /** Unique type code for metric */
            TYPE = constants::Image,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 2
        };
        /** Image metric header
         */
        typedef image_metric_header header_type;
        /** Define a uint16_t array using an underlying vector
         */
        typedef std::vector<ushort_t> ushort_array_t;
        /** Define a uint16_t pointer to a uint16_t array
         */
        typedef ::uint16_t *ushort_pointer_t;
    public:
        /** Constructor
         */
        image_metric() :
                metric_base::base_cycle_metric(0, 0, 0),
                m_min_contrast(MAX_CHANNELS, 0),
                m_max_contrast(MAX_CHANNELS, 0),
                m_channel_count(0)
        { }
        /** Constructor
         *
         * @param header metric set header
         */
        image_metric(const header_type& header) :
                metric_base::base_cycle_metric(0, 0, 0),
                m_min_contrast(header.channel_count(), 0),
                m_max_contrast(header.channel_count(), 0),
                m_channel_count(0)
        { }

        /** Constructor
         *
         * @note Version 2
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param channel_count number of channels
         * @param min_contrast minimum contrast for each channel
         * @param max_contrast maximum contrast for each channel
         */
        image_metric(const uint_t lane,
                     const uint_t tile,
                     const uint_t cycle,
                     const ushort_t channel_count,
                     const ushort_array_t &min_contrast,
                     const ushort_array_t &max_contrast) :
                metric_base::base_cycle_metric(lane, tile, cycle),
                m_min_contrast(min_contrast),
                m_max_contrast(max_contrast),
                m_channel_count(channel_count)
        {
        }

        /** Constructor
         *
         * @note Version 2
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param channel_count number of channels
         * @param min_contrast minimum contrast for each channel
         * @param max_contrast maximum contrast for each channel
         */
        image_metric(const uint_t lane,
                     const uint_t tile,
                     const uint_t cycle,
                     const ushort_t channel_count,
                     const ushort_pointer_t min_contrast,
                     const ushort_pointer_t max_contrast) :
                metric_base::base_cycle_metric(lane, tile, cycle),
                m_min_contrast(min_contrast, min_contrast + channel_count),
                m_max_contrast(max_contrast, max_contrast + channel_count),
                m_channel_count(channel_count)
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
        ushort_t min_contrast(const size_t channel) const throw(index_out_of_bounds_exception)
        {
            if(channel >= m_min_contrast.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Channel out of bounds");
            return m_min_contrast[channel];
        }

        /** Maximum contrast intensity
         *
         * @return maximum contrast intensity
         */
        ushort_t max_contrast(const size_t channel) const throw(index_out_of_bounds_exception)
        {
            if(channel >= m_max_contrast.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Channel out of bounds");
            return m_max_contrast[channel];
        }

        /** Minimum contrast intensity
         *
         * @return minimum contrast intensity
         */
        const ushort_array_t &min_contrast_array() const
        {
            return m_min_contrast;
        }

        /** Maximum contrast intensity
         *
         * @return maximum contrast intensity
         */
        const ushort_array_t &max_contrast_array() const
        {
            return m_max_contrast;
        }

        /** Number of channels
         *
         * @return number of channels
         */
        ushort_t channel_count() const
        {
            return m_channel_count;
        }
        /** @} */
        /** Minimum contrast intensity
         *
         * @deprecated Will be removed in 1.1.x (use min_contrast instead)
         * @return minimum contrast intensity
         */
        ushort_t minContrast(size_t channel) const
        {
            INTEROP_ASSERT(channel < m_channel_count);
            return m_min_contrast[channel];
        }

        /** Maximum contrast intensity
         *
         * @deprecated Will be removed in 1.1.x (use max_contrast instead)
         * @return maximum contrast intensity
         */
        ushort_t maxContrast(size_t channel) const
        {
            INTEROP_ASSERT(channel < m_channel_count);
            return m_max_contrast[channel];
        }

        /** Minimum contrast intensity
         *
         * @deprecated Will be removed in 1.1.x (use min_contrast_array instead)
         * @return minimum contrast intensity
         */
        const ushort_array_t &minContrast() const
        {
            return m_min_contrast;
        }

        /** Maximum contrast intensity
         *
         * @deprecated Will be removed in 1.1.x (use max_contrast_array instead)
         * @return maximum contrast intensity
         */
        const ushort_array_t &maxContrast() const
        {
            return m_max_contrast;
        }

        /** Number of channels
         *
         * @deprecated Will be removed in 1.1.x (use channel_count instead)
         * @return number of channels
         */
        ushort_t channelCount() const
        {
            return m_channel_count;
        }

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return prefix
         */
        static const char *prefix()
        { return "Image"; }

    private:
        ushort_array_t m_min_contrast;
        ushort_array_t m_max_contrast;
        ushort_t m_channel_count;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}


