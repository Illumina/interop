/** Exceptions used by the table namespace
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
    /** @defgroup table_exceptions Table Exceptions
     *
     * Exceptions that can be thrown if a problem occurs while using the run model
     *
     * @ingroup interop_exceptions
     * @{
     */


    /** Exception raised if the column type is invalid
     */
    struct invalid_column_type : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_column_type(const std::string &mesg) : util::base_exception(mesg)
        { }
    };
    /** @} */
}}}


