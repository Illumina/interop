/** Register format layouts for extraction metrics
 *
 * Each version of the extraction metrics file has a layout defined below.
 *
 *  @file
 *
 *  @date 8/20/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <limits>
#include "interop/model/metrics/extraction_metric.h"
#include "interop/io/format/metric_format_factory.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io {
#pragma pack(1)
            /** Extraction Metric Record Layout Version 1
             *
             * This class provides an interface to reading the extraction metric file:
             *  - InterOp/ExtractionMetrics.bin
             *  - InterOp/ExtractionMetricsOut.bin
             *
             * The class takes two template arguments:
             *
             *      1. Metric Type: extraction_metric
             *      2. Version: 2
             */
            template<>
            struct generic_layout<extraction_metric, 2> : public default_layout<2>
            {
                /** @page extraction_v2 Extraction Version 2
                 *
                 * This class provides an interface to reading the extraction metric file:
                 *  - InterOp/ExtractionMetrics.bin
                 *  - InterOp/ExtractionMetricsOut.bin
                 *
                 *  The file format for extraction metrics is as follows:
                 *
                 *  @b Header
                 *
                 *  illumina::interop::io::metric_format_stream (Class that parses this information)
                 *
                 *          byte 0: version number
                 *          byte 1: record size
                 *
                 *  @b n-Records
                 *
                 *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
                 *
                 *          2 bytes: lane number (uint16)
                 *          2 bytes: tile number (uint16)
                 *          2 bytes: cycle number (uint16)
                 *
                 *  illumina::interop::io::generic_layout<extraction_metric, 2> (Class that parses this information)
                 *
                 *          4 bytes: focus for channel A (float32)
                 *          4 bytes: focus for channel C (float32)
                 *          4 bytes: focus for channel G (float32)
                 *          4 bytes: focus for channel T (float32)
                 *          2 bytes: max intensity for channel A (uint16)
                 *          2 bytes: max intensity for channel C (uint16)
                 *          2 bytes: max intensity for channel G (uint16)
                 *          2 bytes: max intensity for channel T (uint16)
                 *          8 bytes: date time stamp (uint64)
                 */
                /** Metric ID type */
                typedef layout::base_cycle_metric metric_id_t;
                /** Map reading/writing to stream
                 *
                 * Reading and writing are symmetric operations, map it once
                 *
                 * @param stream input/output stream
                 * @param metric source/destination metric
                 * @return number of bytes read or total number of bytes written
                 */
                template<class Stream, class Metric, class Header>
                static std::streamsize map_stream(Stream& stream, Metric& metric, Header&)
                {
                    std::streamsize count = 0;
                    count += stream_map< float >(stream, metric.m_focusScores, extraction_metric::MAX_CHANNELS);
                    count += stream_map< ::uint16_t >(stream, metric.m_max_intensity_values, extraction_metric::MAX_CHANNELS);
                    count += stream_map< ::uint64_t >(stream, metric.m_dateTime);
                    return count;
                }
                /** Compute the layout size
                 *
                 * @return size of the record
                 */
                static record_size_t computeSize(const extraction_metric::header_type&)
                {
                    return static_cast<record_size_t>(
                            sizeof(metric_id_t)+
                            sizeof(float)*extraction_metric::MAX_CHANNELS +     // m_focusScores
                            sizeof(::uint16_t)*extraction_metric::MAX_CHANNELS+ // m_max_intensity_values
                            sizeof(::uint64_t)                                  // m_dateTime
                    );
                }
            private:
                //http://en.cppreference.com/w/c/chrono/time_t
                //https://msdn.microsoft.com/en-us/library/z2xf7zzk(v=vs.110).aspx
                inline static ::uint64_t ticks_per_second()
                {
                    return 10000000;
                }
                inline static ::uint64_t ticks_to_1970()
                {
                    return 621355968000000000;
                }
                static ::uint64_t convert_to_csharp_datetime(const ::uint64_t time)
                {
                    return time * ticks_per_second() + ticks_to_1970();
                }
                static ::uint64_t& convert_to_csharp_datetime(::uint64_t& time){return time;}
                static void convert_from_csharp_datetime(const ::uint64_t){}
                static void convert_from_csharp_datetime(::uint64_t& time)
                {
                    time -= ticks_to_1970();
                    time /= ticks_per_second();
                }
            };


#pragma pack()
        }}}

INTEROP_FORCE_LINK_DEF(extraction_metric);
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(extraction_metric, 2 );
