/** Register format layouts for image metrics
 *
 * Each version of the image metrics file has a layout defined below.
 *
 *  @file
 *  @date 8/20/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <vector>
#include <algorithm>
#include "interop/model/metrics/image_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"
#include "interop/io/format/text_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina { namespace interop { namespace io
{
#pragma pack(1)

    /** Image Metric Record Layout Version 1
     *
     * This class provides an interface to reading the image metric file:
     *  - InterOp/ImageMetrics.bin
     *  - InterOp/ImageMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: image_metric
     *      2. Version: 1
     */
    template<>
    struct generic_layout<image_metric, 1> : public default_layout<1, 1 /*Multi record */>
    {
        /** @page image_v1 Image Version 1
         *
         * This class provides an interface to reading the image metric file:
         *  - InterOp/ImageMetrics.bin
         *  - InterOp/ImageMetricsOut.bin
         *
         *  The file format for image metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (1)
         *          byte 1: record size (12)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number  (uint16)
         *          2 bytes: tile number  (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<image_metric, 1> (Class that parses this information)
         *
         *          2 bytes: channel number   (uint16)
         *          2 bytes: minimum contrast (uint16)
         *          2 bytes: maximum contrast (uint16)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Record type */
        typedef generic_layout<image_metric, 1> record_t;
        /** Channel type */
        typedef ::uint16_t channel_t;
        /** Contrast type */
        typedef ::uint16_t contrast_t;
        /** Channel */
        channel_t channel;
        /**  Minimum contrast */
        contrast_t min_contrast;
        /** Maximum contrast */
        contrast_t max_contrast;

        /** Read metric from the input stream
         *
         * @param stream input stream
         * @param metric destination metric
         * @return number of bytes read or total number of bytes written
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(std::istream &stream, Metric &metric, Header &, const bool)
        {
            record_t rec;
            std::streamsize count = stream_map<record_t>(stream, rec);
            if (stream.fail()) return count;
            metric.m_channel_count = image_metric::MAX_CHANNELS;
            INTEROP_ASSERTMSG(rec.channel < image_metric::MAX_CHANNELS,
                              metric.lane() << "_" << metric.tile() << " - " << rec.channel);
            metric.m_min_contrast[rec.channel] = rec.min_contrast;
            metric.m_max_contrast[rec.channel] = rec.max_contrast;
            return count;
        }

        /** Write metric to the output stream
         *
         * @param stream output stream
         * @param metric source metric
         * @return number of bytes read or total number of bytes written
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(std::ostream &stream, Metric &metric, Header &, const bool)
        {
            std::streamsize count = 0;
            metric_id_t metric_id;
            metric_id.set(metric);
            for (image_metric::ushort_t channel_index = 0; channel_index < image_metric::MAX_CHANNELS; ++channel_index)
            {
                if (channel_index > 0) write_binary(stream, metric_id);
                count += stream_map<channel_t>(stream, channel_index);
                count += stream_map<contrast_t>(stream, metric.m_min_contrast[channel_index]);
                count += stream_map<contrast_t>(stream, metric.m_max_contrast[channel_index]);
            }
            return count;
        }

        /** Throws an unimplemented error
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(const char *, const Metric &, const Header &, const bool)
        {
            INTEROP_THROW(std::runtime_error, "Function not implemented");
        }

        /** Compute the size of a single metric record
         *
         * @return record size
         */
        static record_size_t compute_size(const image_metric::header_type &)
        {
            return static_cast< record_size_t >(
                    sizeof(metric_id_t) + sizeof(record_t)
            );
        }

        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const image_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }
    };

    /** Image Metric Record Layout Version 2
     *
     * This class provides an interface to reading the image metric file:
     *  - InterOp/ImageMetrics.bin
     *  - InterOp/ImageMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: image_metric
     *      2. Version: 2
     */
    template<>
    struct generic_layout<image_metric, 2> : public default_layout<2>
    {
        /** @page image_v2 Image Version 2
         *
         * This class provides an interface to reading the image metric file:
         *  - InterOp/ImageMetrics.bin
         *  - InterOp/ImageMetricsOut.bin
         *
         *  The file format for image metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (2)
         *          byte 1: record size (6 + 2*channelCount + 2*channelCount)
         *
         *  @b Extended Header
         *
         *  illumina::interop::io::generic_layout<image_metric, 2>  (Class that parses this information)
         *
         *          byte 2: channel count (uint8)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<image_metric, 2> (Class that parses this information)
         *
         *          2*channelCount bytes: minimum contrast (uint16)
         *          2*channelCount bytes: maximum contrast (uint16)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Contrast type */
        typedef ::uint16_t contrast_t;
        /** Contrast type */
        typedef ::uint8_t channel_count_t;

        /** Map reading/writing a metric to a stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @param header metric header layout
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header &header, const bool)
        {
            std::streamsize count = 0;
            if (header.m_channel_count == 0)
                INTEROP_THROW(bad_format_exception, "Cannot write data where channel count is 0");
            copy_from(stream, metric.m_channel_count, header.m_channel_count);
            count += stream_map<contrast_t>(stream, metric.m_min_contrast, header.m_channel_count);
            count += stream_map<contrast_t>(stream, metric.m_max_contrast, header.m_channel_count);
            return count;
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const image_metric::header_type &header)
        {
            return static_cast<record_size_t>(sizeof(metric_id_t) + header.channel_count() * sizeof(contrast_t) * 2);
        }

        /** Map reading/writing a header to a stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param header source/destination header
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Header>
        static std::streamsize map_stream_for_header(Stream &stream, Header &header)
        {
            std::streamsize count = stream_map<channel_count_t>(stream, header.m_channel_count);
            if (stream.fail())return count;
            if (header.m_channel_count == 0)
                INTEROP_THROW(bad_format_exception, "Cannot write data where channel count is 0");
            return count;
        }

        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const image_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(channel_count_t) + sizeof(record_size_t) + sizeof(version_t));
        }
    };


#pragma pack() // DO NOT MOVE
    /** Image Metric CSV text format
     *
     * This class provide an interface for writing the image metrics to a CSV file:
     *
     *  - ImageMetrics.csv
     */
    template<>
    struct text_layout< image_metric, 1 >
    {
        /** Define a header type */
        typedef image_metric::header_type header_type;
        /** Write header to the output stream
         *
         * @param out output stream
         * @param header image metric header
         * @param channel_names list of channel names
         * @param sep column separator
         * @param eol row separator
         * @return number of column headers
         */
        static size_t write_header(std::ostream& out,
                                   const header_type& header,
                                   const std::vector<std::string>& channel_names,
                                   const char sep,
                                   const char eol)
        {
            if( static_cast<size_t>(header.channel_count()) != channel_names.size() )
                INTEROP_THROW(bad_format_exception, "Header and channel names count mismatch");
            const char* headers[] =
            {
                "Lane", "Tile", "Cycle"
            };
            out << "# Column Count: " << util::length_of(headers)+header.channel_count()*2 << eol;
            out << "# Channel Count: " << header.channel_count() << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            const std::string min_contrast = "MinContrast";
            for(size_t i=0;i<static_cast<size_t>(header.channel_count());++i)
                out << sep << min_contrast << "_" << channel_names[i];
            const std::string max_contrast = "MaxContrast";
            for(size_t i=0;i<static_cast<size_t>(header.channel_count());++i)
                out << sep << max_contrast << "_" << channel_names[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a image metric to the output stream
         *
         * @param out output stream
         * @param metric image metric
         * @param header image metric header
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const image_metric& metric,
                                   const header_type& header,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            if( header.channel_count() != metric.channel_count() )
                INTEROP_THROW(bad_format_exception, "Header and metric channel count mismatch");
            out << metric.lane() << sep << metric.tile() << sep << metric.cycle() << sep;
            for(size_t i=0;i<static_cast<size_t>(header.channel_count());i++)
                out << sep << metric.min_contrast(i);
            for(size_t i=0;i<static_cast<size_t>(header.channel_count());i++)
                out << sep << metric.max_contrast(i);
            out << eol;
            return 0;
        }
    };
}}}

INTEROP_FORCE_LINK_DEF(image_metric)

INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(image_metric, 1)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(image_metric, 2)


// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(image_metric, 1)

