/** Exceptions used by the plot namespace
 *
 *
 *  @file
 *  @date 4/2/17
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include "interop/util/base_exception.h"

namespace illumina { namespace interop { namespace model
{
    /** @defgroup plot_exceptions Plot Exceptions
     *
     * Exceptions that can be thrown if a problem occurs while using the run model
     *
     * @ingroup interop_exceptions
     * @{
     */


    /** Exception raised if the filter options are invalid
     */
    struct invalid_filter_option : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_filter_option(const std::string &mesg) : util::base_exception(mesg)
        { }
    };
    /** @} */
}}}


