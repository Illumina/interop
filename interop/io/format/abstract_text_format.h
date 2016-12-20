/** Metric format interface for the text format factory
 *
 *  @file
 *  @date 12/19/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <iosfwd>
#include "interop/util/cstdint.h"

namespace illumina { namespace interop { namespace io
{
    /** Abstract class that provides an interface for the text format of a metric
     *
     * The template argument for this class corresponds to a specific type
     * of metric set.
     */
    template<class Metric>
    struct abstract_text_format
    {
        /** Define the metric type */
        typedef Metric metric_t;
        /** Define the metric header type */
        typedef typename Metric::header_type header_t;
        /** ID type */
        typedef typename metric_t::id_t id_t;

        /** Destructor
         */
        virtual ~abstract_text_format() {}
        /** Write the header for a set of metric records to the given output stream
         *
         * @param out output stream to write the binary InterOp file data
         * @param header header of a metric set
         * @param channel_names list of channel names
         * @param sep column seperator
         * @param eol row separator
         * @return number of column headers
         */
        virtual size_t write_header(std::ostream &out,
                                    const header_t &header,
                                    const std::vector<std::string>& channel_names,
                                    const char sep,
                                    const char eol) = 0;
        /** Write a metric record to the given output stream
         *
         * @param out output stream to write the binary InterOp file data
         * @param metric interop metric data to write
         * @param header interop metric header data to write
         * @param sep column seperator
         * @param eol row separator
         * @param missing missing value indicator
         * @return number of columns written
         */
        virtual size_t write_metric(std::ostream &out,
                                    const metric_t &metric,
                                    const header_t &header,
                                    const char sep,
                                    const char eol,
                                    const char missing) = 0;

        /** Get the version of this metric format
         *
         * @return version number
         */
        virtual ::int16_t version() const=0;
    };
}}}

