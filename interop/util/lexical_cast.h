/** Convert between arbitrary types
 *
 * Type conversion can be done in many ways in C++, but there is no simple portable method to
 * convert between std::string and int and back. This library provides a limited version of
 * the Boost.Lexical_cast.
 *
 *  @file
 *  @date 8/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <sstream>
#include <iomanip>
#include <limits>
#include "interop/util/math.h"
#include "interop/util/type_traits.h"

namespace illumina { namespace interop { namespace util
{
    /** Cast between a string type and some arbitrary type
     *
     * Note the arbitrary type must support the appropriate stream operator.
     */
    template<class Source, class Destination>
    struct lexical_cast_helper;

    /** Specialization that casts a string to an arbitrary type
     *
     * Note the arbitrary type must support the appropriate stream operator.
     */
    template<class Destination>
    struct lexical_cast_helper<std::string, Destination>
    {
        /** Cast a string to an arbitrary type
         *
         * @param str string
         * @return arbitrary type
         */
        static Destination cast(const std::string &str)
        {
            if (is_nan(str, static_cast<Destination*>(0)))
            {
                return nan_value(static_cast<Destination*>(0));
            }
            std::istringstream iss(str);
            Destination val;
            iss >> val;
            return val;
        }

    private:
        static double nan_value(double*)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }
        static float nan_value(float*)
        {
            return std::numeric_limits<float>::quiet_NaN();
        }
        static Destination nan_value(void*)
        {
            return Destination();
        }
        static bool is_nan(const std::string &str, double*)
        {
            return is_string_nan(str);
        }
        static bool is_nan(const std::string &str, float*)
        {
            return is_string_nan(str);
        }
        static bool is_nan(const std::string&, void*)
        {
            return false;
        }
        static bool is_string_nan(const std::string &str)
        {
            const size_t n = str.length();
            if(n >= 3 && ::tolower(str[n-3]) =='n' && ::tolower(str[n-2]) == 'a' && ::tolower(str[n-1]) == 'n')
            {
                return true;
            }
            return false;
        }
    };

    /** Specialization that casts a string to an arbitrary type
     *
     * Note the arbitrary type must support the appropriate stream operator.
     */
    template<class Destination>
    struct lexical_cast_helper<const char *, Destination>
    {
        /** Cast a string to an arbitrary type
         *
         * @param str string
         * @return arbitrary type
         */
        static Destination cast(const char *str)
        {
            return lexical_cast_helper<std::string, Destination>::cast(str);
        }
    };

    /** Specialization that casts a string to an arbitrary type
     *
     * Note the arbitrary type must support the appropriate stream operator.
     */
    template<class Destination>
    struct lexical_cast_helper<char *, Destination>
    {
        /** Cast a string to an arbitrary type
         *
         * @param str string
         * @return arbitrary type
         */
        static Destination cast(const char *str)
        {
            return lexical_cast_helper<std::string, Destination>::cast(str);
        }
    };

    /** Specialization that casts an arbitrary type to a string
     *
     * Note the arbitrary type must support the appropriate stream operator.
     */
    template<class Source>
    struct lexical_cast_helper<Source, std::string>
    {
        /** Cast an arbitrary type to a string
         *
         * @param source arbitrary type
         * @return string
         */
        static std::string cast(const Source &source)
        {
            std::ostringstream oss;
            oss << source;
            return oss.str();
        }
    };

    /** Specialization that casts an arbitrary type to a string
     *
     * Note the arbitrary type must support the appropriate stream operator.
     */
    template<class Source>
    struct lexical_cast_helper<const Source &, std::string> : lexical_cast_helper<Source, std::string>
    {
    };

    /** Specialization that casts a string to a string
     */
    template<>
    struct lexical_cast_helper<std::string, std::string>
    {
        /** Cast a string to a string
         *
         * @param source string
         * @return string
         */
        static const std::string &cast(const std::string &source)
        {
            return source;
        }
    };

    /** Specialization that casts a string to a string
     */
    template<>
    struct lexical_cast_helper<const char *, std::string>
    {
        /** Cast a string to a string
         *
         * @param source string
         * @return string
         */
        static std::string cast(const char *source)
        {
            return std::string(source);
        }
    };

    /** Specialization that casts a string to a string
     */
    template<>
    struct lexical_cast_helper<char *, std::string>
    {
        /** Cast a string to a string
         *
         * @param source string
         * @return string
         */
        static std::string cast(const char *source)
        {
            return std::string(source);
        }
    };

    /** Specialization that casts a string to a string
     */
    template<>
    struct lexical_cast_helper<std::string, const char *>
    {
        /** Cast a string to a string
         *
         * @param source string
         * @return string
         */
        static const char *cast(const std::string &source)
        {
            return source.c_str();
        }
    };

    /** Cast between a string type and some arbitrary type
     *
     *  @note The arbitrary type should support the std::istream >> or std::ostream << operators
     *
     *  @param src source value of an arbitrary type
     *  @return value of arbitrary type
     */
    template<class Destination, class Source>
    inline Destination lexical_cast(const Source &src)
    {
        return lexical_cast_helper<Source, Destination>::cast(src);
    }

    /** Utility to format a floating point number as a string
     *
     * @param val floating point value
     * @param width width of the number
     * @param precision number of digits after decimal
     * @param fill fill character
     * @param fixed if true use fixed not default
     * @return string representation of float
     */
    inline std::string format(const float val, const int width, const int precision, const char fill = '0',
                              const bool fixed = true)
    {
        std::ostringstream oss;
        if (fixed) oss << std::fixed;
        if (width > -1) oss << std::setw(width);
        if (precision > -1) oss << std::setprecision(precision);
        if (fill != 0) oss << std::setfill(fill);
        if(std::isnan(val)) oss << std::numeric_limits<float>::quiet_NaN();
        else oss << val;
        return oss.str();
    }

}}}


