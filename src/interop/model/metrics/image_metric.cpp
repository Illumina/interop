/** Register format layouts for image metrics
 *
 * Each version of the image metrics file has a layout defined below.
 *
 *  @file
 *
 *  @date 8/20/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <vector>
#include <algorithm>
#include "interop/model/metrics/image_metric.h"
#include "interop/io/format/metric_format_factory.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io {
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
            struct generic_layout<image_metric, 1> : public default_layout<1>
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
                 *          byte 0: version number
                 *          byte 1: record size
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
                typedef layout::base_cycle_metric metric_id_t;
                /** Record type */
                typedef generic_layout<image_metric, 1> record_t;
                /** Channel */
                ::uint16_t channel;
                /**  Minimum contrast */
                ::uint16_t minContrast;
                /** Maximum contrast */
                ::uint16_t maxContrast;
                /** Read metric from the input stream
                 *
                 * @param stream input stream
                 * @param metric destination metric
                 * @return number of bytes read or total number of bytes written
                 */
                template<class Metric, class Header>
                static std::streamsize map_stream(std::istream& stream, Metric& metric, Header&, const bool)
                {
                    record_t rec;
                    std::streamsize count = stream_map< record_t >(stream, rec);
                    if( stream.fail() ) return count;
                    metric.m_channelCount = image_metric::MAX_CHANNELS;
                    INTEROP_ASSERTMSG(rec.channel < image_metric::MAX_CHANNELS, metric.lane() << "_" << metric.tile()<<" - " << rec.channel);
                    metric.m_minContrast[rec.channel] = rec.minContrast;
                    metric.m_maxContrast[rec.channel] = rec.maxContrast;
                    return count;
                }
                /** Write metric to the output stream
                 *
                 * @param stream output stream
                 * @param metric source metric
                 * @return number of bytes read or total number of bytes written
                 */
                template<class Metric, class Header>
                static std::streamsize map_stream(std::ostream& stream, Metric& metric, Header&, const bool)
                {
                    std::streamsize count = 0;
                    metric_id_t metric_id;
                    metric_id.set(metric);
                    for(image_metric::ushort_t channelIndex=0;channelIndex < image_metric::MAX_CHANNELS;++channelIndex)
                    {
                        if(channelIndex > 0) write_binary(stream, metric_id);
                        count += stream_map< ::uint16_t >(stream, channelIndex);
                        count += stream_map< ::uint16_t >(stream, metric.m_minContrast[channelIndex]);
                        count += stream_map< ::uint16_t >(stream, metric.m_maxContrast[channelIndex]);
                    }
                    return count;
                }
                /** Compute the size of a single metric record
                 *
                 * @return record size
                 */
                static record_size_t computeSize(const image_metric::header_type&)
                {
                    return static_cast< record_size_t >(
                            sizeof(metric_id_t)+sizeof(record_t)
                    );
                }
                /** Compute header size
                 *
                 * @return header size
                 */
                static record_size_t computeHeaderSize(const image_metric::header_type&)
                {
                    return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(::uint8_t));
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
                 *          byte 0: version number
                 *          byte 1: record size
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
                typedef layout::base_cycle_metric metric_id_t;

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
                static std::streamsize map_stream(Stream& stream, Metric& metric, Header& header, const bool)
                {
                    std::streamsize count = 0;
                    copy_from(stream, metric.m_channelCount, header.m_channelCount);
                    count += stream_map< ::uint16_t >(stream, metric.m_minContrast, header.m_channelCount);
                    count += stream_map< ::uint16_t >(stream, metric.m_maxContrast, header.m_channelCount);
                    return count;
                }
                /** Compute the layout size
                 *
                 * @return size of the record
                 */
                static record_size_t computeSize(const image_metric::header_type& header)
                {
                    return static_cast<record_size_t>(sizeof(metric_id_t)+header.channelCount()*sizeof(::uint16_t)*2);
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
                static std::streamsize map_stream_for_header(Stream& stream, Header& header)
                {
                    return stream_map< ::uint8_t >(stream, header.m_channelCount);
                }
                /** Compute header size
                 *
                 * @return header size
                 */
                static record_size_t computeHeaderSize(const image_metric::header_type&)
                {
                    return static_cast<record_size_t>(sizeof(::uint8_t) + sizeof(record_size_t) + sizeof(::uint8_t));
                }
            };
#pragma pack() // DO NOT MOVE
        }}}

INTEROP_FORCE_LINK_DEF(image_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(image_metric, 1 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(image_metric, 2 )
