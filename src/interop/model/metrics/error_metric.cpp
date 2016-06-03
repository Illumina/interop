/** Register format layouts for error metrics
 *
 * Each version of the error metrics file has a layout defined below.
 *
 *  @file
 *
 *  @date 8/19/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/model/metrics/error_metric.h"
#include "interop/io/format/metric_format_factory.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io {
#pragma pack(1)

            /** Error Metric Record Layout Version 3
             *
             * This class provides an interface to reading the error metric file:
             *  - InterOp/ErrorMetrics.bin
             *  - InterOp/ErrorMetricsOut.bin
             *
             * The class takes two template arguments:
             *
             *      1. Metric Type: error_metric
             *      2. Version: 3
             */
            template<>
            struct generic_layout<error_metric, 3> : public default_layout<3>
            {
                /** @page error_v3 Error Version 3
                 *
                 * This class provides an interface to reading the error metric file:
                 *  - InterOp/ErrorMetrics.bin
                 *  - InterOp/ErrorMetricsOut.bin
                 *
                 *  The file format for error metrics is as follows:
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
                 *  illumina::interop::io::generic_layout<error_metric, 3> (Class that parses this information)
                 *
                 *          4 bytes: error rate (float32)
                 *          4 bytes: number of perfect reads (uint32)
                 *          4 bytes: number of reads with 1 error (uint32)
                 *          4 bytes: number of reads with 2 error (uint32)
                 *          4 bytes: number of reads with 3 error (uint32)
                 *          4 bytes: number of reads with 4 error (uint32)
                 */
                /** Metric ID type */
                typedef layout::base_cycle_metric metric_id_t;
                /** Error type */
                typedef float error_t;
                /** Error type */
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
                    count += stream_map< error_t >(stream, metric.m_error_rate);
                    count += stream_map< count_t >(stream, metric.m_mismatch_cluster_count, error_metric::MAX_MISMATCH);
                    return count;
                }
                /** Compute the layout size
                 *
                 * @return size of the record
                 */
                static record_size_t compute_size(const error_metric::header_type&)
                {
                    return static_cast<record_size_t>(sizeof(metric_id_t)+
                            sizeof(error_t)+                                  // m_error_rate
                            sizeof(count_t)*error_metric::MAX_MISMATCH   // m_mismatch_cluster_count
                    );
                }
                /** Compute header size
                 *
                 * @return header size
                 */
                static record_size_t compute_header_size(const error_metric::header_type&)
                {
                    return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
                }

            };


#pragma pack()
        }}}

INTEROP_FORCE_LINK_DEF(error_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(error_metric, 3 )
