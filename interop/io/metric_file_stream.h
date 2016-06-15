/** Interface for reading/writing InterOps to binary files
 *
 * This also includes an interface for reading an InterOp from a binary byte buffer.
 *
 *  @file
 *  @date 10/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/exception.h"
#include "interop/io/metric_stream.h"

namespace illumina { namespace interop { namespace io {

    /** @defgroup file_io Reading/Writing Binary InterOp files
     *
     * These functions can be used to read or write a binary InterOp file.
     *
     * @{
     */
    /** Compute the number of bytes to write
     *
     * @param metrics metric set
     * @return number of bytes required
     */
    template<class MetricSet>
    size_t compute_buffer_size(const MetricSet& metrics)
    {
        typedef typename MetricSet::metric_type metric_t;
        return header_size<metric_t>(metrics) + record_size<metric_t>(metrics) * metrics.size();
    }
    /** Write the metric to a binary byte buffer
     *
     * @param metrics metric set
     * @return number of bytes written
     */
    template<class MetricSet>
    size_t write_interop_to_buffer(const MetricSet& metrics, ::uint8_t* buffer, size_t buffer_size)
                        throw(std::invalid_argument, bad_format_exception, incomplete_file_exception)
    {
        std::ostringstream fout;
        write_metrics(fout, metrics, metrics.version());
        std::string str = fout.str();
        if(buffer_size < str.length()) throw std::invalid_argument("Buffer size too small");
        size_t i=0;
        for(;i<str.length();++i)
            buffer[i] = static_cast< ::uint8_t >(str[i]);
        return i;
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
    void read_interop_from_buffer(::uint8_t* buffer, size_t buffer_size, MetricSet& metrics)  throw
                                                                            (interop::io::file_not_found_exception,
                                                                            interop::io::bad_format_exception,
                                                                            interop::io::incomplete_file_exception,
                                                                            model::index_out_of_bounds_exception)
    {
        detail::membuf sbuf(reinterpret_cast<char*>(buffer), reinterpret_cast<char*>(buffer) + buffer_size);
        std::istream in(&sbuf);
        read_metrics(in, metrics);
    }
    /** Read the binary InterOp file into the given metric set
     *
     * @snippet src/examples/example1.cpp Reading a binary InterOp file
     *
     * @note The 'Out' suffix (parameter: use_out) is appended when we read the file. We excluded the Out in certain
     * conditions when writing the file.
     *
     * @param run_directory file path to the run directory
     * @param metrics metric set
     * @param use_out use the copied version
     * @throw file_not_found_exception
     * @throw bad_format_exception
     * @throw incomplete_file_exception
     */
    template<class MetricSet>
    void read_interop(const std::string& run_directory, MetricSet& metrics, const bool use_out=true)   throw
                                                                        (interop::io::file_not_found_exception,
                                                                        interop::io::bad_format_exception,
                                                                        interop::io::incomplete_file_exception,
                                                                        model::index_out_of_bounds_exception)
    {
        const std::string fileName = interop_filename<MetricSet>(run_directory, use_out);
        std::ifstream fin(fileName.c_str(), std::ios::binary);
        if(!fin.good()) INTEROP_THROW(file_not_found_exception, "File not found: " << fileName);
        read_metrics(fin, metrics);
    }
    /** Write the metric set to a binary InterOp file
     *
     * @note The 'Out' suffix (parameter: use_out) is appended when we read the file. We excluded the Out in certain
     * conditions when writing the file.
     *
     * @param run_directory file path to the run directory
     * @param metrics metric set
     * @param use_out use the copied version
     * @param version version of format to write
     */
    template<class MetricSet>
    void write_interop(const std::string& run_directory,
                       const MetricSet& metrics,
                       const bool use_out=true,
                       const ::int16_t version=-1) throw(file_not_found_exception, bad_format_exception, incomplete_file_exception)
    {
        const std::string fileName = interop_filename<MetricSet>(run_directory, use_out);
        std::ofstream fout(fileName.c_str(), std::ios::binary);
        if(!fout.good())INTEROP_THROW(file_not_found_exception, "File not found: " << fileName);
        write_metrics(fout, metrics, version);
    }
    /** Write only the header to a binary InterOp file
     *
     * @note The 'Out' suffix (parameter: use_out) is appended when we read the file. We excluded the Out in certain
     * conditions when writing the file.
     *
     * @param run_directory file path to the run directory
     * @param version version of format to write
     * @param header header of the metric set
     * @param use_out use the copied version
     */
    template<class MetricType>
    void write_interop_header(const std::string& run_directory,
                              const ::int16_t version=-1,
                              const typename MetricType::header_type& header = typename MetricType::header_type(),
                              const bool use_out=true) throw(file_not_found_exception,
                                                                bad_format_exception, incomplete_file_exception)
    {
        const std::string fileName = interop_filename<MetricType>(run_directory, use_out);
        std::ofstream fout(fileName.c_str(), std::ios::binary);
        if(!fout.good())INTEROP_THROW(file_not_found_exception, "File not found: " << fileName);
        write_metric_header(fout, header, version);
    }
    /** @} */

}}}

