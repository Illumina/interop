/** Register format layouts for dynamic phasing metrics
 *
 * Each version of the dynamic phasing metrics file has a layout defined below.
 *
 *  @file
 *
 *  @date 8/19/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/model/metrics/dynamic_phasing_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/text_format.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io {
#pragma pack(1)

    /** Dynamic Phasing Metric Record Layout Version 1
     *
     * These metrics do not actually exist in file format, but are computed from EmpiricalPhasingMetricsOut.bin
     * These are dummy filler functions to allow us to use the base_read_metric framework
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: dynamic_phasing
     *      2. Version: 1
     */
    template<>
    struct generic_layout<dynamic_phasing_metric, 1> : public default_layout<1>
    {
        typedef layout::base_read_metric< ::uint16_t> metric_id_t;
        /** Map the stream
         *
         * @return 0
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream&, Metric&, Header&, const bool) INTEROP_THROW_SPEC((bad_format_exception))
        {
            INTEROP_THROW(bad_format_exception, "Dynamic phasing metric does not exist as a file");
        }
        /** Compute the layout size
         *
         * @return 0
         */
        static record_size_t compute_size(const dynamic_phasing_metric::header_type&)
        {
            INTEROP_THROW(bad_format_exception, "Dynamic phasing metric does not exist as a file");
        }
        /** Compute header size
         *
         * @return 0
         */
        static record_size_t compute_header_size(const dynamic_phasing_metric::header_type&)
        {
            INTEROP_THROW(bad_format_exception, "Dynamic phasing metric does not exist as a file");
        }

    };
#pragma pack()
    /** Dynamic phasing Metric CSV text format
     *
     * This class does nothing.
     */
    template<>
    struct text_layout< dynamic_phasing_metric, 1 >
    {
        /** Define a header type */
        typedef dynamic_phasing_metric::header_type header_type;
        /** Write header to the output stream
         *
         */
        static size_t write_header(std::ostream&,
                                   const header_type&,
                                   const std::vector<std::string>&,
                                   const char,
                                   const char)
        {
            return 0;
        }
        /** Write a error metric to the output stream
         *
         */
        static size_t write_metric(std::ostream&,
                                   const dynamic_phasing_metric&,
                                   const header_type&,
                                   const char,
                                   const char,
                                   const char)
        {
            return 0;
        }
    };
}}}

INTEROP_FORCE_LINK_DEF(dynamic_phasing_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(dynamic_phasing_metric, 1 )

// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(dynamic_phasing_metric, 1)
