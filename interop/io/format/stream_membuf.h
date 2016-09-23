/** Stream membuf class
 *
 *  @file
 *  @date 9/3/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <istream>
#include <cstddef>
#include <vector>
#include "interop/util/exception.h"
#include "interop/io/stream_exceptions.h"
#include "interop/util/lexical_cast.h"
#include "interop/util/cstdint.h"

namespace illumina { namespace interop { namespace io
{
    namespace detail
    {
        /** Memory buffer for a stream
         *
         * This class is used to turn a binary byte buffer into an input stream for reading.
         */
        struct membuf : std::streambuf
        {
            /** Constructor
             *
             * @param begin start iterator for a char buffer
             * @param end end iterator for a char buffer
             */
            membuf(char *begin, char *end)
            {
                this->setg(begin, begin, end);
            }
        };
    }
}}}

