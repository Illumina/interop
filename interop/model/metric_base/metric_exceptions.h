/** Exceptions used by the metrics namespace
 *
 *
 *  @file
 *  @date 4/1/17
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include "interop/util/base_exception.h"

namespace illumina { namespace interop { namespace model
{
    /** @defgroup metric_exceptions Metric Exceptions
     *
     * Exceptions that can be thrown if a problem occurs while using the run model
     *
     * @ingroup interop_exceptions
     * @{
     */

    /** Exception raised if an index goes out of bounds
     */
    struct index_out_of_bounds_exception : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        index_out_of_bounds_exception(const std::string &mesg) : util::base_exception(mesg)
        { }
    };
    /** @} */

}}}

