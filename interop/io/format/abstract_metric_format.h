/** Metric format interface for the metric format factory
 *
 *  @file
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <iosfwd>
#include "interop/util/cstdint.h"
#include "interop/model/metric_base/metric_set.h"

namespace illumina { namespace interop { namespace io
{
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
        typedef Metric metric_t;
        /** Define the metric header type */
        typedef typename Metric::header_type header_t;
        /** ID type */
        typedef typename metric_t::id_t id_t;

        /** Destructor
         */
        virtual ~abstract_metric_format()
        { }

        /** Calculate the size of the file header
         *
         * @param header metric set header
         * @return size of header in bytes
         */
        virtual size_t header_size(const header_t &header) const = 0;

        /** Calculate the size of a record
         *
         * @param header metric set header
         * @return size of record in bytes
         */
        virtual size_t record_size(const header_t &header) const = 0;
        /** Read all the metrics into a metric set
         *
         * @param in input stream
         * @param metric_set destination set of metrics
         * @param file_size number of bytes in the file
         */
        virtual void read_metrics(std::istream& in,
                                  model::metric_base::metric_set<Metric>& metric_set,
                                  const size_t file_size)=0;
        /** Read only the header of a metric set
         *
         * @param in input stream
         * @param metric_set destination set of metrics
         * @return number of bytes read
         */
        virtual size_t read_header(std::istream& in, model::metric_base::metric_set<Metric>& metric_set)=0;

        /** Write a metric record to the given output stream
         *
         * @param out output stream to write the binary InterOp file data
         * @param metric interop metric data to write
         * @param header interop metric header data to write
         */
        virtual void write_metric(std::ostream &out, const metric_t &metric, const header_t &header) = 0;

        /** Write the header for a set of metric records to the given output stream
         *
         * @param out output stream to write the binary InterOp file data
         * @param header header of a metric set
         */
        virtual void write_metric_header(std::ostream &out, const header_t &header) = 0;

        /** Version of the binary InterOp file format
         *
         * @return version number
         */
        virtual ::int16_t version() const = 0;
        /** Is the format a multi-record format
         *
         * @return true if multiple records make up a single metric
         */
        virtual bool is_multi_record() const = 0;
    };
}}}


