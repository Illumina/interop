/** Interface for reading/writing InterOps from/to a binary stream
 *
 *
 *  @file
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <string>
#include <fstream>
#include "interop/util/exception.h"
#include "interop/model/model_exceptions.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/util/filesystem.h"
#include "interop/util/assert.h"

#pragma once
namespace illumina { namespace interop { namespace io
{

    namespace detail
    {
        /** Append the InterOp directory to the run directory file path
         *
         * @param run_directory file path to the run directory
         * @return file path to the InterOp directory
         */
        inline std::string interop_directory_name(const std::string &run_directory)
        {
            return io::combine(run_directory, "InterOp");
        }

        /** Generate a file name from a run directory and the InterOp name
         *
         * @note The 'Out' suffix is appended when we read the file. We excluded the Out in certain conditions
         * when writing the file.
         *
         * @param prefix prefix name of the interop file
         * @param suffix suffix name of the interop file
         * @param use_out if true, append "Out" to the end of the filename
         * @return file path to the InterOp directory
         */
        inline std::string interop_basename(const std::string &prefix,
                                            const std::string &suffix,
                                            const bool use_out = true)
        {
            return prefix + "Metrics" + suffix + ((use_out) ? ("Out.bin") : (".bin"));
        }

        /** Generate a file name from a run directory and the InterOp name
         *
         * @note The 'Out' suffix is appended when we read the file. We excluded the Out in certain conditions
         * when writing the file.
         *
         * @param run_directory file path to the run directory
         * @param prefix prefix name of the interop file
         * @param suffix suffix name of the interop file
         * @param use_out if true, append "Out" to the end of the filename
         * @return file path to the InterOp directory
         */
        inline std::string interop_filename(const std::string &run_directory,
                                            const std::string &prefix,
                                            const std::string &suffix,
                                            const bool use_out = true)
        {
            if (io::basename(run_directory) == interop_basename(prefix, suffix, use_out))
                return run_directory;
            if (io::basename(run_directory) == "InterOp")
                return io::combine(run_directory, interop_basename(prefix, suffix, use_out));
            return io::combine(interop_directory_name(run_directory), interop_basename(prefix, suffix, use_out));
        }

        /** Memory buffer for a stream
         *
         * This class is used to turn a binary byte buffer into an input stream for reading.
         */
        struct membuf : std::streambuf
        {
            /** Constructor
             *
             * @param begin start iterator for a char buffer
             * @param end end iterator for a char buffer
             */
            membuf(char *begin, char *end)
            {
                this->setg(begin, begin, end);
            }
        };
    }

    /** Generate a file name from a run directory and the metric type
     *
     * @note The 'Out' suffix is appended when we read the file. We excluded the Out in certain conditions
     * when writing the file.
     *
     * @param run_directory file path to the run directory
     * @param use_out if true, append "Out" to the end of the filename
     * @return file path to the InterOp directory
     */
    template<class MetricType>
    std::string interop_filename(const std::string &run_directory, bool use_out = true)
    {
        return detail::interop_filename(run_directory, MetricType::prefix(), MetricType::suffix(), use_out);
    }

    /** Generate a file name from a run directory and the metric type
     *
     * @note The 'Out' suffix is appended when we read the file. We excluded the Out in certain conditions
     * when writing the file.
     *
     * @param use_out if true, append "Out" to the end of the filename
     * @return file path to the InterOp directory
     */
    template<class MetricType>
    std::string interop_basename(bool use_out = true)
    {
        return detail::interop_basename(MetricType::prefix(), MetricType::suffix(), use_out);
    }

    /** Read the binary InterOp file into the given metric set
     *
     * @param in input stream
     * @param metrics metric set
     */
    template<class MetricSet>
    void read_metrics(std::istream &in, MetricSet &metrics)
    {
        typedef typename MetricSet::metric_type metric_t;
        typedef typename metric_format_adapter<metric_t>::metric_t format_metric_t;
        typedef metric_format_factory<format_metric_t> factory_t;
        typedef typename factory_t::metric_format_map metric_format_map;
        typedef typename metric_t::id_t id_t;
        typedef std::map<id_t, size_t> offset_map_t;
        metric_format_map &format_map = factory_t::metric_formats();

        if (!in.good()) INTEROP_THROW(incomplete_file_exception, "Empty file found");
        const int version = in.get();
        if (version == -1) INTEROP_THROW(incomplete_file_exception, "Empty file found");
        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to parse " <<
                    interop_basename<MetricSet>() << " with version: " << version << " of " << format_map.size() );
        INTEROP_ASSERT(format_map[version]);
        metrics.set_version(static_cast< ::int16_t>(version));
        const std::streamsize record_size = format_map[version]->read_header(in, metrics);

        offset_map_t metric_offset_map;
        while (in)
        {
            metric_t metric;// Moving this out breaks index metrics - TODO: fix by adding clear to index metrics (or move) or swap
            const std::streamsize icount = format_map[version]->read_metric_id(in, metric);
            if (in.fail())
            {
                if (icount == 0 && !metric_offset_map.empty()) break;
                INTEROP_THROW(incomplete_file_exception, "Insufficient data read from the file, got: " << icount
                                                         << " != expected: " << record_size);
            }

            if (metric_offset_map.find(metric.id()) == metric_offset_map.end())
            {
                const std::streamsize count = format_map[version]->read_metric(in, metric, metrics, true) + icount;
                if (in.fail())
                {
                    if (count == 0 && !metric_offset_map.empty()) break;
                    INTEROP_THROW(incomplete_file_exception, "Insufficient data read from the file, got: " << count
                                                             << " != expected: " << record_size);
                }
                if (count != record_size)
                {
                    INTEROP_THROW(bad_format_exception, "Record does not match expected size: "
                            << count << " != " << record_size);
                }
                if (metric.lane() > 0 && (metric_t::CHECK_TILE_ID == 0 || metric.tile() > 0))
                {
                    metric_offset_map[metric.id()] = metrics.size();
                    metrics.insert(metric.id(), metric);
                    metrics.metric_updated_at(metric_offset_map[metric.id()]);
                }
            }
            else
            {
                const size_t offset = metric_offset_map[metric.id()];
                INTEROP_ASSERTMSG(metrics.at(offset).lane() != 0, offset);
                const std::streamsize count =
                        format_map[version]->read_metric(in, metrics.at(offset), metrics, false) + icount;
                if (in.fail())
                {
                    if (count == 0 && !metric_offset_map.empty()) break;
                    INTEROP_THROW(incomplete_file_exception, "Insufficient data read from the file, got: " << count
                                                             << " != expected: " << record_size);
                }
                if (count != record_size)
                {
                    INTEROP_THROW(bad_format_exception, "Record does not match expected size!");
                }
                metrics.metric_updated_at(offset);
            }
        }
    }

    /** Get the size of a single metric record
     *
     * @param header header for metric
     * @param version version of the format
     * @return size of a single metric record
     */
    template<class MetricType>
    size_t record_size(const typename MetricType::header_type &header,
                       const ::int16_t version = MetricType::LATEST_VERSION)
    {
        typedef typename metric_format_adapter<MetricType>::metric_t format_metric_t;
        typedef metric_format_factory<format_metric_t> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();

        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                       version << " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        return format_map[version]->record_size(header);
    }

    /** Get the size of a metric file header
     *
     * @param header header for metric
     * @param version version of the format
     * @return size of metric file header
     */
    template<class MetricType>
    size_t header_size(const typename MetricType::header_type &header,
                       const ::int16_t version = MetricType::LATEST_VERSION)
    {
        typedef typename metric_format_adapter<MetricType>::metric_t format_metric_t;
        typedef metric_format_factory<format_metric_t> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();

        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                       version <<  " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        return format_map[version]->header_size(header);
    }

    /** Write a metric to a binary InterOp file
     *
     * @param out output stream
     * @param metric metric
     * @param header header for metric
     * @param version version of the format
     */
    template<class MetricType>
    void write_metric(std::ostream &out,
                      const MetricType &metric,
                      const typename MetricType::header_type &header,
                      const ::int16_t version)
    {
        typedef typename metric_format_adapter<MetricType>::metric_t format_metric_t;
        typedef metric_format_factory<format_metric_t> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();

        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                                version <<  " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        format_map[version]->write_metric(out, metric, header);
    }

    /** Write a header describing the metric records
     *
     * @param out output stream
     * @param version version of format
     * @param header metric header
     */
    template<class MetricType>
    void write_metric_header(std::ostream &out,
                             const ::int16_t version,
                             const typename MetricType::header_type &header = typename MetricType::header_type())
    {
        typedef typename metric_format_adapter<MetricType>::metric_t format_metric_t;
        typedef metric_format_factory<format_metric_t> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();
        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                                version <<  " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        format_map[version]->write_metric_header(out, header);
    }

    /** Write a metric header to a binary Interop output stream
     *
     * @param out output stream
     * @param metrics set of metrics
     * @param version version of the InterOp to write (if less than 0, get from metric set)
     */
    template<class MetricSet>
    static void write_metrics(std::ostream &out, const MetricSet &metrics, ::int16_t version = -1)
    {
        typedef typename MetricSet::metric_type metric_type;
        typedef typename metric_format_adapter<metric_type>::metric_t format_metric_t;
        typedef metric_format_factory<format_metric_t> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();

        if (version < 0) version = metrics.version();
        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                                version <<  " of " << format_map.size()
                                                << " for " << metric_type::prefix() << "" << metric_type::suffix()
                                                << " with " << metrics.size() << " metrics");

        INTEROP_ASSERT(format_map[version]);
        format_map[version]->write_metric_header(out, metrics);
        for (typename MetricSet::metric_array_t::const_iterator it = metrics.metrics().begin();
             it != metrics.metrics().end(); it++)
            format_map[version]->write_metric(out, *it, metrics);
    }
}}}

