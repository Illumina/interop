
/** C# Time Conversion
 *
 *  @file
 *  @date 1/14/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include "interop/util/time.h"

#include <cmath>
#include <iostream>
#include "interop/util/static_assert.h"

#ifdef HAVE_LONG64
#define INTEROP_UTIL_TICKS_MASK 0x3fffffffffffffffL
#define INTEROP_UTIL_TICKS_THRESHOLD 0x3fffff36d5964000L
#define INTEROP_UTIL_TICKS_OFFSET 0x4000000000000000L
#define INTEROP_UTIL_TICKS_NEG_OFFSET 0xc92a69c000L
#define INTEROP_UTIL_TICKS_ENCODE 9223372036854775808ul
#define INTEROP_UTIL_TICKS_1970 621355968000000000ul
#else
#define INTEROP_UTIL_TICKS_MASK 0x3fffffffffffffffLL
#define INTEROP_UTIL_TICKS_THRESHOLD 0x3fffff36d5964000LL
#define INTEROP_UTIL_TICKS_OFFSET 0x4000000000000000LL
#define INTEROP_UTIL_TICKS_NEG_OFFSET 0xc92a69c000LL
#define INTEROP_UTIL_TICKS_ENCODE 9223372036854775808ull
#define INTEROP_UTIL_TICKS_1970 621355968000000000ull
#endif

namespace illumina { namespace interop { namespace util
{

    /** Constructor */
    csharp_date_time::csharp_date_time(const ::uint64_t v ) : value(v) { }
    /** Convert to time_t unix format
     *
     * @return time_t unix format
     */
    ::uint64_t csharp_date_time::to_unix()const
    {
        return to_unix(value);
    }
    /** Convert to to seconds with fractions
     *
     * @return seconds with fractions
     */
    double csharp_date_time::to_seconds()const
    {
        return to_seconds(value);
    }

    /** Convert to time_t unix format
     *
     * http://stackoverflow.com/questions/4014652/how-do-datetime-tobinary-and-datetime-tofiletime-differ
     *
     * @param val C# DateTime.ToBinary format
     * @return time_t unix format
     */
    ::uint64_t csharp_date_time::to_unix(const ::uint64_t val)
    {
        static_assert(sizeof( ::uint64_t ) == 8, "Int64 has the wrong size");
        int64_t ticks = static_cast<int64_t>(val) & INTEROP_UTIL_TICKS_MASK;
        if (ticks > INTEROP_UTIL_TICKS_THRESHOLD)
            ticks -= INTEROP_UTIL_TICKS_OFFSET;
        // TODO: missing conversion to local time (use encoded kind)
        if (ticks < 0L)
        {
            ticks += INTEROP_UTIL_TICKS_NEG_OFFSET;
        }
        return static_cast< ::uint64_t >( (ticks - ticks_to_1970()) / ticks_per_second() );
    }
    /** Convert to seconds with fractions
     *
     * http://stackoverflow.com/questions/4014652/how-do-datetime-tobinary-and-datetime-tofiletime-differ
     *
     * @param val C# DateTime.ToBinary format
     * @return seconds with fractions
     */
    double csharp_date_time::to_seconds(const ::uint64_t val)
    {
        int64_t ticks = static_cast<int64_t>(val) & INTEROP_UTIL_TICKS_MASK;
        if (ticks > INTEROP_UTIL_TICKS_THRESHOLD)
            ticks -= INTEROP_UTIL_TICKS_OFFSET;
        // TODO: missing conversion to local time (use encoded kind)
        if (ticks < 0L)
        {
            ticks += INTEROP_UTIL_TICKS_NEG_OFFSET;
        }
        return (ticks - static_cast<double>(ticks_to_1970())) / static_cast<double>(ticks_per_second());
    }
    /** Convert to c# format
     *
     * @param uval time_t unix format
     * @return C# DateTime.ToBinary format
     */
    csharp_date_time csharp_date_time::to_csharp(const ::uint64_t uval)
    {
        int64_t val =  static_cast<int64_t>(uval);
        val *= ticks_per_second();
        val += ticks_to_1970();
        if(val < 0l) val += INTEROP_UTIL_TICKS_OFFSET;
        // TODO: missing conversion to local time (use encoded kind)
        return csharp_date_time(static_cast<uint64_t>(val | INTEROP_UTIL_TICKS_ENCODE));//-9223372036854775808
    }
    /** Implicit convertion to an unsigned 64-bit integer
     *
     * @todo Wrap  operator uint64_t in SWIG
     *
     * @return integer representation
     */
    ::uint64_t csharp_date_time::to_binary()const
    {
        return value;
    }
    /** Test if the object is equal to another of the same type
     *
     * @param other other object being compared
     * @return true if they are equal
     */
    bool csharp_date_time::operator==(const csharp_date_time& other)const
    {
        const int64_t val = static_cast<int64_t>(value-other.value);
        return ((val > 0) ? val : -val) < 5e14;
    }
    /** Write date type as integer to an output stream
     *
     * @param out output stream
     * @param date_time data time object
     * @return output stream
     */
    std::ostream& operator<<(std::ostream& out, const csharp_date_time& date_time )
    {
        // TODO: write out nice string representation
        //2015-02-25 20:04:00
        out << date_time.value;
        return out;
    }
    /** Read a date type from the input stream
     *
     * @param in input stream
     * @param date_time data time object
     * @return input stream
     */
    std::istream& operator>>(std::istream& in, csharp_date_time& date_time)
    {
        // TODO: read in nice string representation
        ::uint64_t val;
        in >> val;
        date_time = csharp_date_time(val);
        return in;
    }

    ::uint64_t csharp_date_time::ticks_per_second()
    {
        return 10000000;
    }
    ::uint64_t csharp_date_time::ticks_to_1970()
    {
        return INTEROP_UTIL_TICKS_1970;
    }
}}}

