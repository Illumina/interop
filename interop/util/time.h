/** C# Time Conversion
 *
 *  @file
 *  @date 1/14/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <iosfwd>
#include "interop/util/cstdint.h"

namespace illumina { namespace interop { namespace util
{
#pragma pack(1)
            /** Wrapper to ensure proper conversion of C# DateTime value
             *
             */
            struct csharp_date_time
            {
                /** Constructor */
                csharp_date_time(const ::uint64_t v = 0);
                /** Convert to time_t unix format
                 *
                 * @return time_t unix format
                 */
                ::uint64_t to_unix()const;
                /** Convert to to seconds with fractions
                 *
                 * @return seconds with fractions
                 */
                double to_seconds()const;

                /** Convert to time_t unix format
                 *
                 * http://stackoverflow.com/questions/4014652/how-do-datetime-tobinary-and-datetime-tofiletime-differ
                 *
                 * @param val C# DateTime.ToBinary format
                 * @return time_t unix format
                 */
                static ::uint64_t to_unix(const ::uint64_t val);
                /** Convert to seconds with fractions
                 *
                 * http://stackoverflow.com/questions/4014652/how-do-datetime-tobinary-and-datetime-tofiletime-differ
                 *
                 * @param val C# DateTime.ToBinary format
                 * @return seconds with fractions
                 */
                static double to_seconds(const ::uint64_t val);
                /** Convert to c# format
                 *
                 * @param uval time_t unix format
                 * @return C# DateTime.ToBinary format
                 */
                static csharp_date_time to_csharp(const ::uint64_t uval);

                /** Date time in csharp DateTime.ToBinary format */
                ::uint64_t value;

                /** Implicit convertion to an unsigned 64-bit integer
                 *
                 * @return integer representation
                 */
                operator uint64_t()const
                {
                    return value;
                }
                /** Implicit convertion to an unsigned 64-bit integer
                 *
                 * @todo Wrap  operator uint64_t in SWIG
                 *
                 * @return integer representation
                 */
                ::uint64_t to_binary()const;
                /** Test if the object is equal to another of the same type
                 *
                 * @param other other object being compared
                 * @return true if they are equal
                 */
                bool operator==(const csharp_date_time& other)const;
                /** Write date type as integer to an output stream
                 *
                 * @param out output stream
                 * @param date_time data time object
                 * @return output stream
                 */
                friend std::ostream& operator<<(std::ostream& out, const csharp_date_time& date_time );
                /** Read a date type from the input stream
                 *
                 * @param in input stream
                 * @param date_time data time object
                 * @return input stream
                 */
                friend std::istream& operator>>(std::istream& in, csharp_date_time& date_time);

            private:
                static ::uint64_t ticks_per_second();
                static ::uint64_t ticks_to_1970();
            };
#pragma pack()
}}}

