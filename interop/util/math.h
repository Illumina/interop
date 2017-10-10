/** Back port of C++11 math functions
 *
 * @todo include this everywhere isnan is used
 *
 *  @file
 *  @date 4/20/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <limits>

#include <cmath>
#if defined(HAVE_NO_STD_ISNAN)
#   if defined(HAVE_ISNAN)
#       include <math.h>
    #elif defined(HAVE___ISNAN)
#       include <math.h>
    #elif defined(HAVE_FLOAT_H_ISNAN)
#       include <float.h>
    #endif
    namespace std
    {
        /** Test if a floating point number is not a number (NaN)
         *
         * @param val floating point number
         * @return true if floating point is NaN
         */
        template<typename T>
        bool isnan(const T val)
        {
#   if defined(HAVE_ISNAN)
            return ::isnan(val);
#   elif defined(HAVE___ISNAN)
            return __isnan(val);
#   elif defined(HAVE_FLOAT_H_ISNAN)
            return _isnan(val);
#   else
            return val != val;
#endif
        }
        /** Test if a floating point number is infinity (INF)
         *
         * @param val floating point number
         * @return true if floating point is infinity
         */
        template<typename T>
        bool isinf(const T val)
        {
#   if defined(HAVE_ISNAN)
            return ::isinf(val);
#   elif defined(HAVE___ISNAN)
            return __isinf(val);
#   elif defined(HAVE_FLOAT_H_ISNAN)
            return _isinf(val);
#   else
            return !isnan(val) && isnan(val-val);
#endif
        }
    }
#endif

namespace illumina { namespace interop { namespace util
{
    /** Cast a number to a float, return NaN if it matches the sentinel
     *
     * The default sentinel is the maximum value for the type.
     *
     * @tparam T
     * @param val number to cast
     * @param sentinel NaN sentinel, default max value
     * @return
     */
    template<typename T>
    float float_cast(const T val, const T sentinel = std::numeric_limits<T>::max())
    {
        if (val == sentinel)
            return std::numeric_limits<float>::quiet_NaN();
        return static_cast<float>(val);
    }
}}}
