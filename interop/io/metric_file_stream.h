/** Interface for reading/writing InterOps to binary files
 *
 * This also includes an interface for reading an InterOp from a binary byte buffer.
 *
 *  @file
 *
 *  @date 10/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/io/metric_stream.h"

namespace illumina {
namespace interop {
namespace io {
/** Generate a file name from a run directory and the metric type
 *
 * @param runDirectory file path to the run directory
 * @param useOut if true, append "Out" to the end of the filename
 * @return file path to the InterOp directory
 */
template<class MetricType>
std::string interop_filename(const std::string& runDirectory, bool useOut = true)
{
    return detail::interop_filename(runDirectory, MetricType::prefix(), useOut);
}
/** Generate a file name from a run directory and the metric type
 *
 * @param useOut if true, append "Out" to the end of the filename
 * @return file path to the InterOp directory
 */
template<class MetricType>
std::string interop_basename(bool useOut = true)
{
    return detail::interop_basename(MetricType::prefix(), useOut);
}


/** @defgroup file_io Reading/Writing Binary InterOp files
 *
 * These functions can be used to read or write a binary InterOp file.
 *
 * @{
 */
/** Write the metric to a binary string buffer
 *
 * @param metrics metric set
 * @return string containing binary data
 */

template<class MetricSet>
std::string write_interop_to_buffer(const MetricSet& metrics)
{
    std::ostringstream fout;
    write_metrics(fout, metrics, metrics.version());
    return fout.str();
}
/** Read the binary InterOp file into the given metric set
 *
 * @param buffer byte buffer
 * @param buffer_size length of the byte buffer
 * @param metrics metric set
 * @throw bad_format_exception
 * @throw incomplete_file_exception
 */
template<class MetricSet>
void read_interop_from_buffer(::uint8_t* buffer, size_t buffer_size, MetricSet& metrics) _INTEROP_FORMAT_THROWS
{
    detail::membuf sbuf(reinterpret_cast<char*>(buffer), reinterpret_cast<char*>(buffer) + buffer_size);
    std::istream in(&sbuf);
    read_metrics(in, metrics);
}
/** Read the binary InterOp file into the given metric set
 *
 * @snippet src/examples/example1.cpp Reading a binary InterOp file
 *
 * @param runDirectory file path to the run directory
 * @param metrics metric set
 * @param useOut use the copied version
 * @throw file_not_found_exception
 * @throw bad_format_exception
 * @throw incomplete_file_exception
 */
template<class MetricSet>
void read_interop(const std::string& runDirectory, MetricSet& metrics, const bool useOut=true) _INTEROP_FORMAT_THROWS
{
    std::string fileName = detail::interop_name(runDirectory, metrics.name(), useOut);
    std::ifstream fin(fileName.c_str(), std::ios::binary);
    if(!fin.good())
        throw file_not_found_exception("File not found: "+fileName);
    read_metrics(fin, metrics);
}
/** Write the metric set to a binary InterOp file
 *
 * @param runDirectory file path to the run directory
 * @param metrics metric set
 * @param useOut use the copied version
 * @param version version of format to write
 */
template<class MetricSet>
void write_interop(const std::string& runDirectory, const MetricSet& metrics, const bool useOut=true, const ::int16_t version=-1)
{
    std::string fileName = detail::interop_name(runDirectory, metrics.name(), useOut);
    std::ofstream fout(fileName.c_str(), std::ios::binary);
    if(!fout.good())
        throw file_not_found_exception("File not found: "+fileName);
    write_metrics(fout, metrics, version);
}
/** Write only the header to a binary InterOp file
 *
 * @param runDirectory file path to the run directory
 * @param version version of format to write
 * @param header header of the metric set
 * @param useOut use the copied version
 */
template<class MetricType>
void write_interop_header(const std::string& runDirectory, const ::int16_t version=-1, const typename MetricType::header_type& header = typename MetricType::header_type(), const bool useOut=true)
{
    std::string fileName = detail::interop_name(runDirectory, MetricType::prefix(), useOut);
    std::ofstream fout(fileName.c_str(), std::ios::binary);
    if(!fout.good())
        throw file_not_found_exception("File not found: "+fileName);
    write_metric_header(fout, header, version);
}
/** @} */

}
}
}

