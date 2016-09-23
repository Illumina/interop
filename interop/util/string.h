/** String utilities
 *
 *  @file
 *  @date 5/16/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <cctype>

namespace illumina { namespace interop { namespace util
{

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

}}}



