/** Register format layouts for corrected intensity metrics
 *
 * Each version of the corrected intensity metrics file has a layout defined below.
 *
 *  @file
 *  @date 8/6/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"


using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io
{
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
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (2)
         *          byte 1: record size (48)
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
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Intensity type */
        typedef ::uint16_t intensity_t;
        /** Count type */
        typedef ::uint32_t count_t;
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
            count += stream_map< intensity_t >(stream, metric.m_average_cycle_intensity);
            count += stream_map< intensity_t >(stream, metric.m_corrected_int_all, constants::NUM_OF_BASES);
            count += stream_map< intensity_t >(stream, metric.m_corrected_int_called, constants::NUM_OF_BASES);
            count += stream_map< count_t >(stream, metric.m_called_counts, constants::NUM_OF_BASES_AND_NC);
            count += stream_map< float >(stream, metric.m_signal_to_noise);
            return count;
        }
        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const corrected_intensity_metric::header_type&)
        {
            return static_cast<record_size_t>(
                    sizeof(metric_id_t)+
                            sizeof(intensity_t) +                                // m_average_cycle_intensity
                            sizeof(intensity_t)*constants::NUM_OF_BASES +        // m_corrected_int_all
                            sizeof(intensity_t)*constants::NUM_OF_BASES +        // m_corrected_int_called
                            sizeof(count_t)*constants::NUM_OF_BASES_AND_NC + // m_called_counts
                            sizeof(float)                                       // m_signal_to_noise
            );
        }
        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const corrected_intensity_metric::header_type&)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
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
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (3)
         *          byte 1: record size (34)
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
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Intensity type */
        typedef ::uint16_t intensity_t;
        /** Count type */
        typedef ::uint32_t count_t;
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
            count += stream_map< intensity_t >(stream, metric.m_corrected_int_called, constants::NUM_OF_BASES);
            count += stream_map< count_t >(stream, metric.m_called_counts, constants::NUM_OF_BASES_AND_NC);
            return count;
        }
        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const corrected_intensity_metric::header_type&)
        {
            return static_cast<record_size_t>(
                    sizeof(metric_id_t)+
                    sizeof(intensity_t)*constants::NUM_OF_BASES +    // m_corrected_int_called
                    sizeof(count_t)*constants::NUM_OF_BASES_AND_NC   // m_called_counts
            );
        }
        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const corrected_intensity_metric::header_type&)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }
    };

#pragma pack()
}}}

INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(corrected_intensity_metric, 2 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(corrected_intensity_metric, 3 )
INTEROP_FORCE_LINK_DEF(corrected_intensity_metric)


