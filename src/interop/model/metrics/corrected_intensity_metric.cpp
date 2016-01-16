/** Register format layouts for corrected intensity metrics
 *
 * Each version of the corrected intensity metrics file has a layout defined below.
 *
 *  @file
 *
 *  @date 8/6/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/io/format/metric_format_factory.h"


using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io {
#pragma pack(1)
            /** Corrected Intensity Metric Record Layout Version 2
             *
             * This class provides an interface to reading the corrected intensity metric file:
             *  - InterOp/CorrectedIntMetrics.bin
             *  - InterOp/CorrectedIntMetricsOut.bin
             *
             * The class takes two template arguments:
             *
             *      1. Metric Type: corrected_intensity_metric
             *      2. Version: 2
             */
            template<>
            struct generic_layout<corrected_intensity_metric,  2> : public default_layout<2>
            {
                /** @page corrected_v2 Corrected Intensity Version 2
                 *
                 *
                 * This class provides an interface to reading the corrected intensity metric file:
                 *  - InterOp/CorrectedIntMetrics.bin
                 *  - InterOp/CorrectedIntMetricsOut.bin
                 *
                 *  The file format for corrected intensity metrics is as follows:
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
                 *          2 bytes: lane number  (uint16)
                 *          2 bytes: tile number  (uint16)
                 *          2 bytes: cycle number (uint16)
                 *
                 *  illumina::interop::io::generic_layout<corrected_intensity_metric, 2> (Class that parses this information)
                 *
                 *          2 bytes: average cycle intensity (uint16)
                 *          2 bytes: average corrected intensity for channel A (uint16)
                 *          2 bytes: average corrected intensity for channel C (uint16)
                 *          2 bytes: average corrected intensity for channel G (uint16)
                 *          2 bytes: average corrected intensity for channel T (uint16)
                 *          2 bytes: average corrected int for called clusters for base A (uint16)
                 *          2 bytes: average corrected int for called clusters for base C (uint16)
                 *          2 bytes: average corrected int for called clusters for base G (uint16)
                 *          2 bytes: average corrected int for called clusters for base T (uint16)
                 *          4 bytes: number of base calls for No Call (uint32)
                 *          4 bytes: number of base calls for base A (uint32)
                 *          4 bytes: number of base calls for base C (uint32)
                 *          4 bytes: number of base calls for base G (uint32)
                 *          4 bytes: number of base calls for base T (uint32)
                 *          4 bytes: signal to noise ratio (float32)
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
                static std::streamsize map_stream(Stream& stream, Metric& metric, Header&, const bool)
                {
                    std::streamsize count = 0;
                    count += stream_map< ::uint16_t >(stream, metric.m_averageCycleIntensity);
                    count += stream_map< ::uint16_t >(stream, metric.m_correctedIntAll, constants::NUM_OF_BASES);
                    count += stream_map< ::uint16_t >(stream, metric.m_correctedIntCalled, constants::NUM_OF_BASES);
                    count += stream_map< ::uint32_t >(stream, metric.m_calledCounts, constants::NUM_OF_BASES_AND_NC);
                    count += stream_map< float >(stream, metric.m_signalToNoise);
                    return count;
                }
                /** Compute the layout size
                 *
                 * @return size of the record
                 */
                static record_size_t computeSize(const corrected_intensity_metric::header_type&)
                {
                    return static_cast<record_size_t>(
                            sizeof(metric_id_t)+
                                    sizeof(::uint16_t) +                                // m_averageCycleIntensity
                                    sizeof(::uint16_t)*constants::NUM_OF_BASES +        // m_correctedIntAll
                                    sizeof(::uint16_t)*constants::NUM_OF_BASES +        // m_correctedIntCalled
                                    sizeof(::uint32_t)*constants::NUM_OF_BASES_AND_NC + // m_calledCounts
                                    sizeof(float)                                       // m_signalToNoise
                    );
                }
            };
            /** Corrected Intensity Metric Record Layout Version 3
             *
             * This class provides an interface to reading the corrected intensity metric file:
             *  - InterOp/CorrectedIntMetrics.bin
             *  - InterOp/CorrectedIntMetricsOut.bin
             *
             * The class takes two template arguments:
             *
             *      1. Metric Type: corrected_intensity_metric
             *      2. Version: 3
             */
            template<>
            struct generic_layout<corrected_intensity_metric, 3> : public default_layout<3>
            {
                /** @page corrected_v3 Corrected Intensity Version 3
                 *
                 * This class provides an interface to reading the corrected intensity metric file:
                 *  - InterOp/CorrectedIntMetrics.bin
                 *  - InterOp/CorrectedIntMetricsOut.bin
                 *
                 *  The file format for corrected intensity metrics is as follows:
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
                 *  illumina::interop::io::generic_layout<corrected_intensity_metric, 3> (Class that parses this information)
                 *
                 *          2 bytes: average corrected int for called clusters for base A (uint16)
                 *          2 bytes: average corrected int for called clusters for base C (uint16)
                 *          2 bytes: average corrected int for called clusters for base G (uint16)
                 *          2 bytes: average corrected int for called clusters for base T (uint16)
                 *          4 bytes: number of base calls for No Call (uint32)
                 *          4 bytes: number of base calls for base A (uint32)
                 *          4 bytes: number of base calls for base C (uint32)
                 *          4 bytes: number of base calls for base G (uint32)
                 *          4 bytes: number of base calls for base T (uint32)
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
                static std::streamsize map_stream(Stream& stream, Metric& metric, Header&, const bool)
                {
                    std::streamsize count = 0;
                    // TODO: this does not need header layout, audit rest to see if that is true
                    count += stream_map< ::uint16_t >(stream, metric.m_correctedIntCalled, constants::NUM_OF_BASES);
                    count += stream_map< ::uint32_t >(stream, metric.m_calledCounts, constants::NUM_OF_BASES_AND_NC);
                    return count;
                }
                /** Compute the layout size
                 *
                 * @return size of the record
                 */
                static record_size_t computeSize(const corrected_intensity_metric::header_type&)
                {
                    return static_cast<record_size_t>(
                            sizeof(metric_id_t)+
                            sizeof(::uint16_t)*constants::NUM_OF_BASES +        // m_correctedIntCalled
                            sizeof(::uint32_t)*constants::NUM_OF_BASES_AND_NC   // m_calledCounts
                    );
                }
            };
#pragma pack()

        }
    }
}

INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(corrected_intensity_metric, 2 );
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(corrected_intensity_metric, 3 );
INTEROP_FORCE_LINK_DEF(corrected_intensity_metric);

