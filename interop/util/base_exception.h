/** Base exception class from which all other InterOp exceptions are derived
 *
 *  @file
 *  @date 4/1/17
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include <stdexcept>

#ifdef _MSC_VER
#pragma warning(disable:4290) // MSVC warns that it ignores the exception specification.
#endif

namespace illumina { namespace interop { namespace util
{

    /** Base exception for all format errors
     */
    struct base_exception : public std::runtime_error
    {
        /** Constructor
         *
         *  @param mesg error message
         */
        base_exception(const std::string &mesg) : std::runtime_error(mesg)
        { }
    };

}}}

