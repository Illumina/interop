/** Exceptions thrown by the model
 *
 *  @file
 *  @date 3/21/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/util/base_exception.h"
#include "interop/model/metric_base/metric_exceptions.h"

namespace illumina { namespace interop { namespace model
{

    /** @defgroup model_exceptions Model Exceptions
     *
     * Exceptions that can be thrown if a problem occurs while using the model
     *
     * @ingroup interop_exceptions
     * @{
     */

    /** Exception raised if the channel names are invalid
     */
    struct invalid_channel_exception : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_channel_exception(const std::string &mesg) : util::base_exception(mesg)
        { }
    };

    /** Exception raised if the metric type is invalid
     */
    struct invalid_metric_type : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_metric_type(const std::string &mesg) : util::base_exception(mesg)
        { }
    };

    /** Exception raised if a function is given an invalid parameter
     */
    struct invalid_parameter : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_parameter(const std::string &mesg) : util::base_exception(mesg)
        { }
    };

    /** @} */
}}}


