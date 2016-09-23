/** Exceptions thrown by the model
 *
 *  @file
 *  @date 3/21/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <stdexcept>

#ifdef _MSC_VER
#pragma warning(disable:4290) // MSVC warns that it ignores the exception specification.
#endif

namespace illumina { namespace interop { namespace model
{

    /** @defgroup model_exceptions Model Exceptions
     *
     * Exceptions that can be thrown if a problem occurs while using the model
     *
     * @ingroup interop_exceptions
     * @{
     */
    /** Exception raised if an index goes out of bounds
     */
    struct index_out_of_bounds_exception : public std::out_of_range
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        index_out_of_bounds_exception(const std::string &mesg) : std::out_of_range(mesg)
        { }
    };

    /** Exception raised if the channel names are invalid
     */
    struct invalid_channel_exception : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_channel_exception(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** Exception raised if the read was not found
     */
    struct invalid_read_exception : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_read_exception(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** Exception raised if the metric type is invalid
     */
    struct invalid_metric_type : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_metric_type(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** Exception raised if the filter options are invalid
     */
    struct invalid_filter_option : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_filter_option(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** Exception raised if the tile naming convention is invalid
     */
    struct invalid_tile_naming_method : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_tile_naming_method(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** Exception raised if a function is given an invalid parameter
     */
    struct invalid_parameter : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_parameter(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** Exception raised if the column type is invalid
     */
    struct invalid_column_type : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_column_type(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** Exception raised if the RunInfo does not match the InterOps
     */
    struct invalid_run_info_exception : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        invalid_run_info_exception(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

    /** @} */
}}}


