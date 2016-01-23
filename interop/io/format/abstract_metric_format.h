/** Metric format interface for the metric format factory
 *
 *  @file
 *
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <istream>
#include <ostream>
#include <assert.h>
#include "interop/util/cstdint.h"

namespace illumina {
    namespace interop {
        namespace io {
            /** Abstract class that provides an interface for the format of a metric
             *
             * The template argument for this class corresponds to a specific type
             * of metric set.
             *
             */
            template<class Metric>
            struct abstract_metric_format
            {
                /** Define the metric type */
                typedef Metric metric_type;
                /** Define the metric header type */
                typedef typename Metric::header_type header_t;
                /** ID type */
                typedef typename metric_type::id_t id_t;
                /** Destructor
                 */
                virtual ~abstract_metric_format(){}
                /** Read a metric set from the given input stream
                 *
                 * @param in input stream containing binary InterOp file data
                 * @param header metric set header
                 * @return number of bytes in the record
                 */
                virtual std::streamsize read_header(std::istream& in, header_t& header)=0;
                /** Read a metric id from the given input stream
                 *
                 * @param in input stream containing binary InterOp file data
                 * @param metric metric
                 * @return number of bytes in the record
                 */
                virtual std::streamsize read_metric_id(std::istream& in, metric_type& metric)=0;
                /** Read a metric from the given input stream
                 *
                 * @param in input stream containing binary InterOp file data
                 * @param metric metric
                 * @param header metric set header
                 * @param is_new true if the metric is new to the set
                 * @return number of bytes in the record
                 */
                virtual std::streamsize read_metric(std::istream& in, metric_type& metric, const header_t& header, const bool is_new)=0;
                /** Write the metric to the given output stream
                 *
                 * @param out output stream to write the binary InterOp file data
                 * @param metric interop metric data to write
                 * @param header interop metric header data to write
                 */
                virtual void write_metric(std::ostream& out, const metric_type &metric, const header_t& header)=0;
                /** Write the metric header to the given output stream
                 *
                 * @param out output stream to write the binary InterOp file data
                 * @param header header of a metric set
                 */
                virtual void write_metric_header(std::ostream& out, const header_t& header)=0;
                /** Version of the binary InterOp file format
                 *
                 * @return version number
                 */
                virtual ::int16_t version()const=0;
            };
        }
    }
}

