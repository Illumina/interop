/** Register metric stream layouts
 *
 * The header provides exceptions for specific errors that may be raised while reading a binary InterOp file.
 *
 *  @file
 *
 *  @date 8/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <string>
#include <stdexcept>

#ifdef _MSC_VER
#pragma warning(disable:4290) // MSVC warns that it ignores the exception specification.
#endif

#define _INTEROP_FORMAT_THROWS throw(   interop::io::file_not_found_exception, \
                                        interop::io::bad_format_exception, \
                                        interop::io::incomplete_file_exception)

namespace illumina {
    namespace interop {
        namespace io {
            /** Base exception for all format errors
             */
            struct format_exception : public std::runtime_error {
                /** Constructor
                 *
                 *  @param mesg error message
                 */
                format_exception(const std::string &mesg) : std::runtime_error(mesg) { }
            };
            /** @defgroup read_exceptions Read Exceptions
             *
             * Exceptions that will be thrown if a problem occurs while reading a binary InterOp file.
             *
             * @{
             */
            /** Exception raised if the InterOp file is not found in the file system
             */
            struct file_not_found_exception : public std::runtime_error {
                /** Constructor
                 *
                 *  @param mesg error message
                 */
                file_not_found_exception(const std::string &mesg) : std::runtime_error(mesg) {}
            };
            /** Exception raised if the InterOp file does not have the correct format
             *
             * This is only raised if the format is bad.
             */
            struct bad_format_exception : public format_exception {
                /** Constructor
                 *
                 *  @param mesg error message
                 */
                bad_format_exception(const std::string &mesg) : format_exception(mesg) { }
            };
            /** Exception raised if the InterOp file record is incomplete
             *
             * This is only raised if the record is read from the stream incomplete
             */
            struct incomplete_file_exception : public std::runtime_error {
                /** Constructor
                 *
                 *  @param mesg error message
                 */
                incomplete_file_exception(const std::string &mesg) : std::runtime_error(mesg) { }
            };
            /** @} */
        }
    }
}