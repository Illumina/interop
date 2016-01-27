/** Factory for generating metric formats
 *
 *  @file
 *
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <assert.h>
#include <map>
#include <vector>
#include "abstract_metric_format.h"
#include "interop/util/unique_ptr.h"
#include "interop/util/lexical_cast.h"
#include "interop/io/stream_exceptions.h"
#include "metric_format.h"

/** Register a metric format with the factory
 *
 * @param Metric metric class
 * @param Version version number
 */
#define INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(Metric, Version) \
    illumina::interop::io::metric_format_factory< Metric >  \
    illumina_interop_io_##Type##Metric##Version(new illumina::interop::io::metric_format<Metric, illumina::interop::io::generic_layout<Metric, Version> >);

/** Ensure that static libraries are properly linked
 *  This must be used in a function that will definitely be linked.
 *
 *  Tested for Microsoft Visual C++, GCC and CLang
 */
#define INTEROP_FORCE_LINK_USE(X) void force_link_metric_format(X*);  force_link_metric_format(0);
/** Ensure that static libraries are properly linked
 *  This must be used in a file that may not be linked.
 *
 *  Tested for Microsoft Visual C++, GCC and CLang
 */
#define INTEROP_FORCE_LINK_DEF(X) namespace illumina{namespace interop{namespace io{ void force_link_metric_format(X*){} }}} \
    void force_link_metric_format(X*){} // For Microsoft Visual C++

namespace illumina {
    namespace interop {
        namespace io {
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
                /** Define the header type */
                typedef typename Metric::header_type header_type;
                /** Define a unique pointer to a metric format
                 */
                typedef stdbp::unique_ptr< abstract_metric_format<Metric> > metric_format_pointer;
                /** Define a map between format version and the format
                 */
                typedef std::map<int, metric_format_pointer> metric_format_map;
                /** Constructor
                 *
                 * This constructor is used to statically register a matric format in a source file.
                 */
                metric_format_factory(abstract_metric_format<Metric>* pformat)
                {
                    assert(pformat!=0);
                    metric_formats()[pformat->version()]=metric_format_pointer(pformat);
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

        }
    }
}

