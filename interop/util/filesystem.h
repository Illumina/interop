/** Filesystem utility functions
 *
 * This header provides facilities to manipulate files, directories and the paths that identify them.
 *
 *  @file
 *
 *  @date 8/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#ifdef WIN32
/** Platform dependent path separator */
#define INTEROP_OS_SEP '\\'
#else
/** Platform dependent path separator */
#define INTEROP_OS_SEP '/'
#endif

#include <string>
#include <algorithm>

#pragma once
namespace illumina {
    namespace interop {
        namespace io {
            /** Combine two directories or a directory and a filename into a file path
             *
             * This function provides a platform independent way to generate a file path. It currently supports most
             * operating systems include Mac OSX, Windows and Linux/Unix.
             *
             * @param path string representing a file path
             * @param name string representing a file or directory name to append to the end of the path
             * @return proper os-dependent file path
             */
            inline std::string combine(const std::string& path, const std::string& name) {
                if (path != "" && path[path.size() - 1] != INTEROP_OS_SEP && name != "" && name[0] != INTEROP_OS_SEP) {
                    return path + INTEROP_OS_SEP + name;
                }
                return path + name;
            }
            inline std::string dirname(std::string source)
            {
                if (source.size() <= 1) //Make sure it's possible to check the last character.
                {
                    return source;
                }
                if (*(source.rbegin() + 1) == '/') //Remove trailing slash if it exists.
                {
                    source = source.substr(0, source.size()-1);
                    // source.pop_back(); // C++11
                }
                source.erase(std::find(source.rbegin(), source.rend(), '/').base(), source.end());
                return source;
            }
            namespace detail {
#ifdef WIN32
            struct match_path_sep
            {
                bool operator()(char ch)const
                {
                    return ch == '/';
                }
            };
#else
            struct match_path_sep
            {
                bool operator()(char ch)const
                {
                    return ch == '/' || ch == '\\';
                }
            };
#endif
            }
            inline std::string basename(std::string const& source)
            {
                return std::string(std::find_if(source.rbegin(), source.rend(), detail::match_path_sep()).base(), source.end());
            }
        }
    }
}

