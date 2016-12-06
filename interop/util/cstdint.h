/** Back port of integer types
 *
 * This is defines a set of fixed width integer types.
 *
 *  @file
 *
 *  @date 9/26/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#ifdef HAVE_STDINT_H
#include <stdint.h>
#elif HAVE_SYS_TYPES_H
#include <sys/types.h>
#else
#include "interop/util/pstdint.h"
#endif


#if 0
namespace illumina
{
namespace interop
{
namespace util
{


/** Create a fixed size integer of any size
 */
template<typename T, unsigned ::int16_t N>
struct fixed_integer
{
    enum{
        /** Size of integer in bytes */
        byte_count=N/8,
        /** Size of integer in bits */
        bit_count=N
    };
    /** Conversion to standard integer type
     *
     * @return integer type
     */
    operator T()const
    {
        return convert(data);
    }
    /** Convert binary data to integer
     *
     * @param pdata pointer to byte array
     * @return integer type
     */
    static T convert(char* pdata)
    {
        return pdata[byte_count-1] << (bit_count-8) | fixed_integer<T, bit_count-1>::convert(pdata);
    }
    /** Byte array */
    char data[byte_count];
};
/** Create a fixed size integer of any size
 *
 * Specialization for single byte.
 */
template<typename T>
struct fixed_integer<T, 1>
{
    enum{
        /** Size of integer in bytes */
        byte_count=1,
        /** Size of integer in bits */
        bit_count=8
    };
    /** Conversion to standard integer type
     *
     * @return integer type
     */
    operator T()const
    {
        return data;
    }
    /** Convert binary data to integer
     *
     * @param pdata pointer to byte array
     * @return integer type
     */
    static T convert(char* pdata)
    {
        return pdata[0];
    }
    /** Byte */
    char data;
};
/** Create a fixed size integer of any size
 *
 * Specialization for no data
 */
template<typename T>
struct fixed_integer<T, 0>{};


}
}
}

namespace std
{
    /** 16-bit unsigned integer */
    typedef unsigned int16_t uint16_t;
    /** 32-bit unsigned integer */
    typedef unsigned int uint32_t;
    /** 64-bit unsigned integer */
    typedef unsigned long long uint64_t;
}
#endif


