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
#include <fstream>

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
            namespace detail {
#ifndef WIN32
                /** Helper functor to match path separator on Linux
                 */
                struct match_path_sep
                {
                    /** Test if given character is Linux path separator
                     *
                     * @param ch character to test
                     */
                    bool operator()(char ch)const
                    {
                        return ch == '/';
                    }
                };
#else
                /** Helper functor to match path separator on Windows
                 */
                struct match_path_sep
                {
                    /** Test if given character is Windows path separator
                     *
                     * @param ch character to test
                     */
                    bool operator()(char ch)const
                    {
                        return ch == '/' || ch == '\\';
                    }
                };
#endif
            }
            /** Get the file name from a file path
             *
             * @param source full file path
             * @return name of the file
             */
            inline std::string basename(std::string const& source)
            {
                if(source.empty()) return "";
                if( detail::match_path_sep()(source[source.length()-1] ) )
                {
                    return std::string(std::find_if(source.rbegin()+1, source.rend(), detail::match_path_sep()).base(), source.end()-1);
                }
                return std::string(std::find_if(source.rbegin(), source.rend(), detail::match_path_sep()).base(), source.end());
            }
            /** Get the directory name from a file path
             *
             * @param source full file path
             * @return name of the directory
             */
            inline std::string dirname(std::string source)
            {
                if (source.size() <= 1) //Make sure it's possible to check the last character.
                {
                    return source;
                }
                detail::match_path_sep is_sep;
                if (is_sep(*(source.rbegin() + 1))) //Remove trailing slash if it exists.
                {
                    source = source.substr(0, source.size()-1);
                    // source.pop_back(); // C++11
                }
                source.erase(std::find_if(source.rbegin(), source.rend(), is_sep).base(), source.end());
                return source;
            }
            /** Check if a file exists
             *
             * @param filename name of the file
             * @return true if the file exists and is readable
             */
            inline bool is_file_readable(const std::string& filename)
            {
                std::ifstream fin(filename.c_str());
                return fin.good();
            }
        }
    }
}

