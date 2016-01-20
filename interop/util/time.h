
/** C# Time Conversion
 *
 *  @file
 *  @date 1/14/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include "interop/util/cstdint.h"
#include "interop/util/static_assert.h"

#if defined(_WIN32)
#define INTEROP_UTIL_TICKS_MASK 0x3fffffffffffffffLL
#define INTEROP_UTIL_TICKS_THRESHOLD 0x3fffff36d5964000LL
#define INTEROP_UTIL_TICKS_OFFSET 0x4000000000000000LL
#define INTEROP_UTIL_TICKS_NEG_OFFSET 0xc92a69c000LL
#define INTEROP_UTIL_TICKS_ENCODE 9223372036854775808ull
#else
#define INTEROP_UTIL_TICKS_MASK 0x3fffffffffffffffL
#define INTEROP_UTIL_TICKS_THRESHOLD 0x3fffff36d5964000L
#define INTEROP_UTIL_TICKS_OFFSET 0x4000000000000000L
#define INTEROP_UTIL_TICKS_NEG_OFFSET 0xc92a69c000L
#define INTEROP_UTIL_TICKS_ENCODE 9223372036854775808ul
#endif

namespace illumina {
namespace interop {
namespace util {
#pragma pack(1)
            /** Wrapper to ensure proper conversion of C# DateTime value
             *
             */
            struct csharp_date_time {
                /** Constructor */
                csharp_date_time(uint64_t v = 0) : value(v) { }
                /** Convert to time_t unix format
                 *
                 * @return time_t unix format
                 */
                uint64_t to_unix()const
                {
                    return to_unix(value);
                }
                /** Convert to to seconds with fractions
                 *
                 * @return seconds with fractions
                 */
                double to_seconds()const
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
                static uint64_t to_unix(uint64_t val)
                {
                    static_assert(sizeof(uint64_t) == 8, "Int64 has the wrong size");
                    int64_t ticks = static_cast<int64_t>(val) & INTEROP_UTIL_TICKS_MASK;
                    if (ticks > INTEROP_UTIL_TICKS_THRESHOLD)
                        ticks -= INTEROP_UTIL_TICKS_OFFSET;
                    // TODO: missing conversion to local time (use encoded kind)
                    if (ticks < 0L)
                    {
                        ticks += INTEROP_UTIL_TICKS_NEG_OFFSET;
                    }
                    return static_cast<uint64_t>( (ticks - ticks_to_1970()) / ticks_per_second() );
                }
                /** Convert to seconds with fractions
                 *
                 * http://stackoverflow.com/questions/4014652/how-do-datetime-tobinary-and-datetime-tofiletime-differ
                 *
                 * @param val C# DateTime.ToBinary format
                 * @return seconds with fractions
                 */
                static double to_seconds(uint64_t val)
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
                static csharp_date_time to_csharp(uint64_t uval)
                {
                    int64_t val =  static_cast<int64_t>(uval);
                    val *= ticks_per_second();
                    val += ticks_to_1970();
                    if(val < 0l) val += INTEROP_UTIL_TICKS_OFFSET;
                    // TODO: missing conversion to local time (use encoded kind)
                    return csharp_date_time(static_cast<uint64_t>(val | INTEROP_UTIL_TICKS_ENCODE));//-9223372036854775808
                }

                /** Date time in csharp DateTime.ToBinary format */
                uint64_t value;

            private:
                inline static ::uint64_t ticks_per_second()
                {
                    return 10000000;
                }
                inline static ::uint64_t ticks_to_1970()
                {
                    return 621355968000000000;
                }
            };
#pragma pack()
}
}
}