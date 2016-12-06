/** Filesystem utility functions
 *
 * This header provides facilities to manipulate files, directories and the paths that identify them.
 *
 *  @file
 *  @date 8/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <string>
#include "interop/util/cstdint.h"

namespace illumina { namespace interop { namespace io
{
    /** Combine two directories or a directory and a filename into a file path
     *
     * This function provides a platform independent way to generate a file path. It currently supports most
     * operating systems include Mac OSX, Windows and Linux/Unix.
     *
     * @param path string representing a file path
     * @param name string representing a file or directory name to append to the end of the path
     * @return proper os-dependent file path
     */
    std::string combine(const std::string& path, const std::string& name);
    /** Combine two directories or a directory and a filename into a file path
     *
     * This function provides a platform independent way to generate a file path. It currently supports most
     * operating systems include Mac OSX, Windows and Linux/Unix.
     *
     * @param path1 string representing a file path 1
     * @param path2 string representing a file path 2
     * @param path3 string representing a file path 3
     * @return proper os-dependent file path
     */
    std::string combine(const std::string& path1, const std::string& path2, const std::string& path3);
    /** Get the file name from a file path
     *
     * @param source full file path
     * @return name of the file
     */
    std::string basename(std::string const& source);
    /** Get the directory name from a file path
     *
     * @param source full file path
     * @return name of the directory
     */
    std::string dirname(std::string source);
    /** Check if a file exists
     *
     * @param filename name of the file
     * @return true if the file exists and is readable
     */
    bool is_file_readable(const std::string& filename);
    /** Create a directory
     *
     * @param path path to new directory
     * @param mode permissions on directory
     * @return true if directory was created
     */
    bool mkdir(const std::string& path, const int mode=0733);

    /** Get the size of a file
     *
     * This should be more efficient than opening a file and seeking the end.
     *
     * @param path path to the target file
     * @return size of the file or -1 if the operation failed
     */
    ::int64_t file_size(const std::string& path);
}}}


