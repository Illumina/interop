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
            /** Extraction Metric Record Layout Version 2
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
                 *  illumina::interop::io::read_metrics (Function that parses this information)
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
                /** Intensity type */
                typedef float focus_t;
                /** Intensity type */
                typedef ::uint16_t intensity_t;
                /** Time type */
                typedef ::uint64_t datetime_t;
                /** Map reading/writing to stream
                 *
                 * Reading and writing are symmetric operations, map it once
                 *
                 * @param stream input/output stream
                 * @param metric source/destination metric
                 * @return number of bytes read or total number of bytes written
                 */
                template<class Stream, class Metric, class Header>
                static std::streamsize map_stream(Stream& stream, Metric& metric, Header&, const bool)
                {
                    std::streamsize count = 0;
                    count += stream_map< focus_t >(stream, metric.m_focus_scores, extraction_metric::MAX_CHANNELS);
                    count += stream_map< intensity_t >(stream, metric.m_max_intensity_values, extraction_metric::MAX_CHANNELS);
                    count += stream_map< datetime_t >(stream, metric.m_date_time_csharp.value);
                    convert_datetime(stream, metric);
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
                            sizeof(focus_t)*extraction_metric::MAX_CHANNELS +     // m_focus_scores
                            sizeof(intensity_t)*extraction_metric::MAX_CHANNELS+ // m_max_intensity_values
                            sizeof(datetime_t)                                  // m_dateTime
                    );
                }
                /** Compute header size
                 *
                 * @return header size
                 */
                static record_size_t computeHeaderSize(const extraction_metric::header_type&)
                {
                    return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
                }
            private:
                static void convert_datetime(std::ostream&, const extraction_metric&)
                {
                }
                static void convert_datetime(std::istream& in, extraction_metric& metric)
                {
                    if(in.fail()) return;
                    metric.m_date_time = metric.m_date_time_csharp.to_unix();
                }
            };


#pragma pack()
        }}}

INTEROP_FORCE_LINK_DEF(extraction_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(extraction_metric, 2 )
