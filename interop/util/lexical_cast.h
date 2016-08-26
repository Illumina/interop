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
            Destination val;
            if (handle_special_float(str, val)) return val;
            std::istringstream iss(str);
            iss >> val;
            return val;
        }

    private:
        static bool handle_special_float(const std::string &str, float &val)
        {
            if (str.length() == 3 && ::tolower(str[0]) =='n' && ::tolower(str[1]) == 'a' && ::tolower(str[2]) == 'n')
            {
                val = std::numeric_limits<float>::quiet_NaN();
                return true;
            }
            if (str.length() == 4 && str[0] =='-' && ::tolower(str[1]) =='n' && ::tolower(str[2]) == 'a' &&
                    ::tolower(str[3]) == 'n')
            {
                val = -std::numeric_limits<float>::quiet_NaN();
                return true;
            }
            return false;
        }

        static bool handle_special_float(const std::string &str, double &val)
        {
            if (str.length() == 3 && ::tolower(str[0]) =='n' && ::tolower(str[1]) == 'a' && ::tolower(str[2]) == 'n')
            {
                val = std::numeric_limits<double>::quiet_NaN();
                return true;
            }
            if (str.length() == 4 && str[0] =='-' && ::tolower(str[1]) =='n' && ::tolower(str[2]) == 'a' &&
                ::tolower(str[3]) == 'n')
            {
                val = -std::numeric_limits<double>::quiet_NaN();
                return true;
            }
            return false;
        }

        template<typename T>
        static bool handle_special_float(const std::string &, T &)
        {
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
        oss << val;
        return oss.str();
    }

}}}

