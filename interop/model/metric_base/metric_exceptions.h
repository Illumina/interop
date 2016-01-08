/** Exceptions thrown by a metric or a metric set
 *
 *  @file
 *
 *  @date 10/26/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <stdexcept>

#define _INTEROP_METRIC_THROWS throw(   interop::model::metric_base::index_out_of_bounds_exception)

namespace illumina {
namespace interop {
namespace model {
namespace metric_base {

/** Exception raised if an index goes out of bounds
 */
struct index_out_of_bounds_exception : public std::runtime_error {
    /** Constructor
     *
     *  @param mesg error message
     */
    index_out_of_bounds_exception(const std::string &mesg) : std::runtime_error(mesg) { }
};


}
}
}
}

