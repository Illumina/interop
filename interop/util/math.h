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
    }
#endif

