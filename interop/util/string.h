/** String utilities
 *
 *  @file
 *  @date 5/16/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <cctype>
#include <algorithm>
#include <istream>
namespace illumina { namespace interop { namespace util
{

    namespace detail
    {
        /** Convert character to lower case
         *
         * @note workarond for MSVC  warning C4244: '=': conversion from 'int' to 'char', possible loss of data
         *
         * @param ch character
         * @return lowercase character
         */
        inline char tolower(const char ch)
        {
            return static_cast<char>(::tolower(ch));
        }
    }
    // replace, camel_to_space
    //
    /** Replace any first occurence of substring from with substring to
     *
     * @param str source/destination string
     * @param from search string
     * @param to replacement string
     * @return true if substring was found and replaced
     */
    inline bool replace(std::string& str, const std::string& from, const std::string& to)
    {
        const size_t start_pos = str.find(from);
        if(start_pos == std::string::npos) return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

    /** Split string based on upper case characters and separate with separator string
     *
     * E.g. "SignalToNoise" -> "Signal To Noise"
     *
     *
     * @param str source/destination string
     * @param sep seperator string
     */
    inline void camel_to(std::string& str, const std::string& sep=" ")
    {
        for(size_t i=1;i<str.length()-1;++i)
        {
            if(std::isupper(str[i]))
            {
                str.insert(i, sep);
                ++i;
            }
        }
    }
    /** Split string based on space characters and delineate with camel case
     *
     * E.g. "Signal To Noise" -> "SignalToNoise"
     *
     *
     * @param str source/destination string
     * @param sep separator string
     */
    inline void camel_from(std::string& str, const char sep=' ')
    {
        for(size_t i=1;i<str.length()-1;)
        {
            if(str[i] == sep)
            {
                str.erase(str.begin() + i);
                str[i] = static_cast<char>(::toupper(str[i]));
            }
            else ++i;
        }
    }
    /** Returns true iff only ASCII characters are used
     *
     * @param input string to check
     */
    inline bool contains_ASCII_only (const std::string& input) {
        for (size_t s = 0; s < input.size(); s++) {
            if (static_cast<unsigned char>(input[s]) > 127) {
                return false;
            }
        }
        return true;
    }
    inline std::string to_lower(const std::string &input) {
        std::string result(input);
        std::transform(input.begin(), input.end(), result.begin(), detail::tolower);
        return result;
    }

    inline bool ends_with(const std::string &input, const std::string &ending, const bool is_case_insensitive=false) {
        if (input.length() >= ending.length()) {
            if (is_case_insensitive) {
                std::string input_lower = illumina::interop::util::to_lower(input);
                std::string ending_lower = illumina::interop::util::to_lower(ending);
                return (input_lower.compare (input.length() - ending.length(), ending.length(), ending_lower) == 0);
            }
            return (input.compare (input.length() - ending.length(), ending.length(), ending) == 0);
        } else {
            return false;
        }
    }
    struct StringCaseInsensitiveComparator {
        bool operator()(const std::string &a, const std::string &b) const {
            return illumina::interop::util::to_lower(a) < illumina::interop::util::to_lower(b);
        }
    };

    //Function taken from https://gist.github.com/josephwb/df09e3a71679461fc104
    inline std::istream& cross_platform_getline(std::istream &is, std::string& line) {
        line.clear();
        std::istream::sentry se(is, true);
        std::streambuf* sb = is.rdbuf();
        for (;;) {
            int c = sb->sbumpc();
            switch (c) {
                case '\n':
                    return is;
                case '\r':
                    if (sb->sgetc() == '\n') {
                        sb->sbumpc();
                    }
                    return is;
                case EOF:
                    // Also handle the case when the last line has no line ending
                    if (line.empty()) {
                        is.setstate(std::ios::eofbit);
                    }
                    return is;
                default:
                    line += (char)c;
            }
        }
    }

}}}



