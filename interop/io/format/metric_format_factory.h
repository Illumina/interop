/** Factory for generating metric formats
 *
 *  @file
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include "interop/util/map.h"
#include "interop/util/assert.h"
#include "interop/util/self_registration.h"
#include "interop/io/format/abstract_metric_format.h"
#include "interop/util/unique_ptr.h"
#include "interop/io/stream_exceptions.h"

/** Register a metric format with the factory
 *
 * @param Metric metric class
 * @param Version version number
 */
#define INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(Metric, Version) \
    illumina::interop::io::metric_format_factory< Metric >  \
    illumina_interop_io_##Type##Metric##Version(new illumina::interop::io::metric_format<Metric, illumina::interop::io::generic_layout<Metric, Version> >);

/** Register a metric format with the factory
 *
 * @param Metric metric class
 * @param Proxy actual metric parser to use
 * @param Version version number
 */
#define INTEROP_REGISTER_METRIC_ANOTHER_GENERIC_LAYOUT(Metric, Proxy, Version) \
    illumina::interop::io::metric_format_factory< Proxy >  \
    illumina_interop_io_##Type##Proxy##Version(new illumina::interop::io::metric_format<Proxy, illumina::interop::io::generic_layout<Metric, Version> >);


namespace illumina { namespace interop { namespace io
{
    /** Factory for generating metric formats
     *
     * This class defines static methods to register a metric format. The registered metric formats can
     * be accessed through the `metric_formats()` static function.
     *
     * @note this is not thread safe
     */
    template<class Metric>
    struct metric_format_factory
    {
        /** Define the metric type */
        typedef Metric metric_type;
        /** Define the abstract format type */
        typedef abstract_metric_format<metric_type> abstract_metric_format_t;
        /** Define the header type */
        typedef typename Metric::header_type header_type;
        /** Define a unique pointer to a metric format */
        typedef stdbp::unique_ptr<abstract_metric_format_t> metric_format_pointer;
        /** Define a map between format version and the format */
        typedef INTEROP_UNORDERED_MAP(int, metric_format_pointer) metric_format_map;

        /** Constructor
         *
         * This constructor is used to statically register a matric format in a source file.
         */
        metric_format_factory(abstract_metric_format_t *pformat)
        {
            INTEROP_ASSERT(pformat != 0);
            metric_formats()[pformat->version()] = metric_format_pointer(pformat);
        }

        /** Static initialization workaround for member variables
         *
         * @note this is not thread safe
         * @return map between version and format
         */
        static metric_format_map &metric_formats()
        {
            INTEROP_FORCE_LINK_USE(metric_type);
            static metric_format_map vec;
            return vec;
        }
    };

}}}


