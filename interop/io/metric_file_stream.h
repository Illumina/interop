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
#include "interop/util/filesystem.h"
#include "interop/io/format/stream_membuf.h"
#include "interop/io/metric_stream.h"

namespace illumina { namespace interop { namespace io
{

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
    size_t compute_buffer_size(const MetricSet& metrics) throw(invalid_argument, bad_format_exception)
    {
        typedef typename MetricSet::metric_type metric_t;
        if(is_multi_record(metrics))
            throw invalid_argument("Format does not currently support computing the buffer size");
        return header_size(metrics, metrics.version()) +
                size_of_record<metric_t>(metrics, metrics.version()) * metrics.size();
    }
    /** Write the metric to a binary byte buffer
     *
     * @param metrics metric set
     * @param buffer destination binary buffer
     * @param buffer_size maximum size of binary buffer
     * @return number of bytes written
     */
    template<class MetricSet>
    size_t write_interop_to_buffer(const MetricSet& metrics, ::uint8_t* buffer, size_t buffer_size)
                        throw(invalid_argument, bad_format_exception, incomplete_file_exception)
    {
        std::ostringstream fout;
        write_metrics(fout, metrics, metrics.version());
        std::string str = fout.str();
        if(buffer_size < str.length())
            INTEROP_THROW(invalid_argument, "Buffer size too small");
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
        read_metrics(in, metrics, buffer_size);
    }
    /** Read the binary InterOp file into the given metric set
     *
     * @param buffer string holding a byte buffer
     * @param metrics metric set
     * @param rebuild whether to rebuild the id map
     * @throw bad_format_exception
     * @throw incomplete_file_exception
     * @throw model::index_out_of_bounds_exception
     */
    template<class MetricSet>
    void read_interop_from_string(const std::string& buffer, MetricSet& metrics, const bool rebuild=true)  throw
    (interop::io::bad_format_exception,
    interop::io::incomplete_file_exception,
    model::index_out_of_bounds_exception)
    {
        std::istringstream in(buffer);
        read_metrics(in, metrics, buffer.length(), rebuild);
    }
    /** Read the binary InterOp file into the given metric set
     *
     * @param buffer string holding a byte buffer
     * @param metrics metric set
     * @throw bad_format_exception
     * @throw incomplete_file_exception
     * @throw model::index_out_of_bounds_exception
     */
    template<class MetricSet>
    size_t read_header_from_string(const std::string& buffer, MetricSet& metrics)  throw
    (interop::io::bad_format_exception,
    interop::io::incomplete_file_exception,
    model::index_out_of_bounds_exception)
    {
        std::istringstream in(buffer);
        return read_header(in, metrics);
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
                                                                        (file_not_found_exception,
                                                                        bad_format_exception,
                                                                        incomplete_file_exception,
                                                                        model::index_out_of_bounds_exception)
    {
        std::string file_name = interop_filename<MetricSet>(run_directory, use_out);
        std::ifstream fin(file_name.c_str(), std::ios::binary);
        if(!fin.good())
        {
            file_name = interop_filename<MetricSet>(run_directory, !use_out);
            fin.open(file_name.c_str(), std::ios::binary);
        }
        if(!fin.good()) INTEROP_THROW(file_not_found_exception, "File not found: " << file_name);
        read_metrics(fin, metrics, static_cast<size_t>(file_size(file_name)));
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
     * @return true if write is successful
     */
    template<class MetricSet>
    bool write_interop(const std::string& run_directory,
                       const MetricSet& metrics,
                       const bool use_out=true,
                       const ::int16_t version=-1)
    throw(file_not_found_exception,
    bad_format_exception,
    incomplete_file_exception)
    {
        if(metrics.empty() || metrics.version() == 0 )return true;
        const std::string file_name = interop_filename<MetricSet>(run_directory, use_out);
        std::ofstream fout(file_name.c_str(), std::ios::binary);
        if(!fout.good())INTEROP_THROW(file_not_found_exception, "File not found: " << file_name);
        write_metrics(fout, metrics, version);
        return fout.good();
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
                              const bool use_out=true)
    throw(file_not_found_exception,
    bad_format_exception,
    incomplete_file_exception)
    {
        const std::string file_name = interop_filename<MetricType>(run_directory, use_out);
        std::ofstream fout(file_name.c_str(), std::ios::binary);
        if(!fout.good())INTEROP_THROW(file_not_found_exception, "File not found: " << file_name);
        write_metric_header(fout, header, version);
    }
    /** Check for the existence of the binary InterOp file into the given metric set
     *
     * @note The 'Out' suffix (parameter: use_out) is appended when we read the file. We excluded the Out in certain
     * conditions when writing the file.
     *
     * @param run_directory file path to the run directory
     * @param use_out use the copied version
     */
    template<class MetricSet>
    bool interop_exists(const std::string& run_directory, MetricSet&, const bool use_out=true)
    throw(file_not_found_exception,
    bad_format_exception,
    incomplete_file_exception,
    model::index_out_of_bounds_exception)
    {
        const std::string file_name = interop_filename<MetricSet>(run_directory, use_out);
        std::ifstream fin(file_name.c_str(), std::ios::binary);
        if(!fin.good()) return false;
        return true;
    }
    /** List all possible InterOp file names
     *
     * @note The first filename is the legacy name
     *
     * @param files destination list of files
     * @param run_directory file path to the run directory
     * @param use_out use the copied version
     */
    template<class MetricSet>
    void list_interop_filenames(std::vector<std::string>& files,
                                const std::string& run_directory,
                                const bool use_out=true)
    {
        files.resize(1);
        files[0] = interop_filename<MetricSet>(run_directory, use_out);
    }
    /** @} */

}}}


