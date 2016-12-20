/** Concrete text layout formats for the abstract text format interface.
 *
 *
 *  @file
 *  @date 12/19/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#ifdef _MSC_VER
#pragma warning(disable:4702) // MSVC warns that there is unreachable code
#endif


#include "interop/util/exception.h"
#include "interop/io/format/abstract_text_format.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace io
{
    /** Shared functionality for writing text InterOp metrics
     */
    template<class Metric, class Layout>
    struct text_format;
    /** Shared functionality for writing text InterOp metrics
     *
     * Specialization for text_layout
     */
    template<class Metric, int Version>
    struct text_format<Metric, text_layout<Metric, Version> >  : public abstract_text_format<Metric>
    {
    private:
        typedef text_layout<Metric, Version> layout_t;
        typedef typename Metric::id_t id_t;
    public:
        /** Define the metric type */
        typedef Metric metric_t;
        /** Define the metric header type */
        typedef typename Metric::header_type header_t;

        /** Write the header of text format to the given output stream
         *
         * @param out output stream to write the binary InterOp file data
         * @param header header of a metric set
         * @param channel_names list of channel names
         * @param sep column seperator
         * @param eol row separator
         * @return number of column headers
         */
        size_t write_header(std::ostream &out,
                            const header_t &header,
                            const std::vector<std::string>& channel_names,
                            const char sep,
                            const char eol)
        {
            out << "# " << Metric::prefix() << Metric::suffix() << sep;
            out << Version << eol;
            return layout_t::write_header(out, header, channel_names, sep, eol);
        }

        /** Write a text record to the given output stream
         *
         * @param out output stream
         * @param metric a metric to write
         * @param header metric set header
         * @param sep column seperator
         * @param eol row separator
         * @param missing missing value indicator
         * @return number of columns written
         */
        size_t write_metric(std::ostream &out,
                            const metric_t &metric,
                            const header_t &header,
                            const char sep,
                            const char eol,
                            const char missing)
        {
            return layout_t::write_metric(out, metric, header, sep, eol, missing);
        }

        /** Get the version of this metric format
         *
         * @return version number
         */
        ::int16_t version() const
        {
            return static_cast< ::int16_t >(Version);
        }
    };
}}}


