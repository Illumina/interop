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

    /** @defgroup run_exceptions Run Exceptions
     *
     * Exceptions that can be thrown if a problem occurs while using the run model
     *
     * @ingroup interop_exceptions
     * @{
     */

    /** Exception raised if the read was not found
     */
    struct invalid_read_exception : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_read_exception(const std::string &mesg) : util::base_exception(mesg)
        { }
    };

    /** Exception raised if the tile naming convention is invalid
     */
    struct invalid_tile_naming_method : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_tile_naming_method(const std::string &mesg) : util::base_exception(mesg)
        { }
    };

    /** Exception raised if the RunInfo does not match the InterOps
     */
    struct invalid_run_info_exception : public util::base_exception
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_run_info_exception(const std::string &mesg) : util::base_exception(mesg)
        { }
    };
    /** @} */

}}}

