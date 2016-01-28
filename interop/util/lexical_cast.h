/** Convert between arbitrary types
 *
 * Type conversion can be done in many ways in C++, but there is no simple portable method to
 * convert between std::string and int and back. This library provides a limited version of
 * the Boost.Lexical_cast.
 *
 *  @file
 *
 *  @date 8/9/15
 *  @version 1.0
 *  @opyright GNU Public License.
 */

#pragma once
#include <sstream>

namespace illumina {
    namespace interop {
        namespace util {
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
                static Destination cast(const std::string& str)
                {
                    Destination val;
                    std::istringstream iss(str);
                    iss >> val;
                    return val;
                }
            };
            /** Specialization that casts a string to an arbitrary type
             *
             * Note the arbitrary type must support the appropriate stream operator.
             */
            template<class Destination>
            struct lexical_cast_helper<const char*, Destination>
            {
                /** Cast a string to an arbitrary type
                 *
                 * @param str string
                 * @return arbitrary type
                 */
                static Destination cast(const char* str)
                {
                    Destination val;
                    std::istringstream iss(str);
                    iss >> val;
                    return val;
                }
            };
            /** Specialization that casts a string to an arbitrary type
             *
             * Note the arbitrary type must support the appropriate stream operator.
             */
            template<class Destination>
            struct lexical_cast_helper<char*, Destination>
            {
                /** Cast a string to an arbitrary type
                 *
                 * @param str string
                 * @return arbitrary type
                 */
                static Destination cast(const char* str)
                {
                    Destination val;
                    std::istringstream iss(str);
                    iss >> val;
                    return val;
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
                static std::string cast(const Source& source)
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
            struct lexical_cast_helper<const Source&, std::string> : lexical_cast_helper<Source, std::string>{};
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
                static const std::string& cast(const std::string& source)
                {
                    return source;
                }
            };
            /** Specialization that casts a string to a string
             */
            template<>
            struct lexical_cast_helper<const char*, std::string>
            {
                /** Cast a string to a string
                 *
                 * @param source string
                 * @return string
                 */
                static std::string cast(const char* source)
                {
                    return std::string(source);
                }
            };
            /** Specialization that casts a string to a string
             */
            template<>
            struct lexical_cast_helper<char*, std::string>
            {
                /** Cast a string to a string
                 *
                 * @param source string
                 * @return string
                 */
                static std::string cast(const char* source)
                {
                    return std::string(source);
                }
            };
            /** Specialization that casts a string to a string
             */
            template<>
            struct lexical_cast_helper<std::string, const char*>
            {
                /** Cast a string to a string
                 *
                 * @param source string
                 * @return string
                 */
                static const char* cast(const std::string& source)
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
            inline Destination lexical_cast(const Source& src)
            {
                return lexical_cast_helper<Source, Destination>::cast(src);
            }

        }
    }
}

