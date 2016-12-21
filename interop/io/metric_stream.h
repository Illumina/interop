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
#include "interop/io/format/text_format_factory.h"
#include "interop/io/paths.h"
#include "interop/util/filesystem.h"
#include "interop/util/assert.h"

#pragma once
namespace illumina { namespace interop { namespace io
{
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
        return paths::interop_filename<MetricType>(run_directory, use_out);
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
        return paths::interop_basename<MetricType>(use_out);
    }

    /** Copy list of version numbers to output iterator
     *
     * @param oit destination iterator for versions
     */
    template<class MetricType, typename OutputIterator>
    void copy_versions(OutputIterator oit)
    {
        typedef metric_format_factory<MetricType> factory_t;
        typedef typename factory_t::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_t::metric_formats();
        for(typename metric_format_map::const_iterator it = format_map.begin();it != format_map.end();++it, ++oit)
        {
            *oit = it->first;
        }
    }
    /** Read the header from the stream
     *
     * @param in input stream
     * @param metrics metric set
     * @return number of bytes read from the stream
     */
    template<class MetricSet>
    size_t read_header(std::istream &in, MetricSet &metrics)
    {
        typedef typename MetricSet::metric_type metric_t;
        typedef metric_format_factory<metric_t> factory_t;
        typedef typename factory_t::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_t::metric_formats();
        if (!in.good()) INTEROP_THROW(incomplete_file_exception, "Empty file found");
        const int version = in.get();
        if (version == -1) INTEROP_THROW(incomplete_file_exception, "Empty file found");
        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to parse " << paths::interop_basename<MetricSet>()
                                                                            << " with version: " << version << " of "
                                                                            << format_map.size() );
        INTEROP_ASSERT(format_map[version]);
        metrics.set_version(static_cast< ::int16_t>(version));
        return format_map[version]->read_header(in, metrics);
    }

    /** Read the binary InterOp file into the given metric set
     *
     * @param in input stream
     * @param metrics metric set
     * @param file_size number of bytes in the file
     * @param rebuild flag indicating whether to rebuild the lookup table
     */
    template<class MetricSet>
    void read_metrics(std::istream &in, MetricSet &metrics, const size_t file_size, const bool rebuild=true)
    {
        typedef typename MetricSet::metric_type metric_t;
        typedef metric_format_factory<metric_t> factory_t;
        typedef typename factory_t::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_t::metric_formats();
        if (!in.good()) INTEROP_THROW(incomplete_file_exception, "Empty file found");
        const int version = in.get();
        if (version == -1) INTEROP_THROW(incomplete_file_exception, "Empty file found");
        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to parse " << paths::interop_basename<MetricSet>()
                                                                            << " with version: " << version << " of "
                                                                            << format_map.size() );
        INTEROP_ASSERT(format_map[version]);
        metrics.set_version(static_cast< ::int16_t>(version));
        try
        {
            format_map[version]->read_metrics(in, metrics, file_size);
        }
        catch(const incomplete_file_exception& ex)
        {
            if(rebuild)metrics.rebuild_index();// TODO: Speed up with hash map if using c++11 compiler
            throw ex;
        }
        if(rebuild)metrics.rebuild_index();
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
        typedef metric_format_factory<MetricType> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();

        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                       version << " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        return format_map[version]->record_size(header);
    }

    /** Get the size of a single metric record
     *
     * @param header header for metric
     * @param version version of the format
     * @return size of a single metric record
     */
    template<class MetricType, class HeaderType>
    size_t size_of_record(const HeaderType &header,
                          const ::int16_t version = MetricType::LATEST_VERSION)
    {
        typedef metric_format_factory<MetricType> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();

        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                                version << " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        return format_map[version]->record_size(header);
    }
    /** Test whether the format support multi-records
     *
     * @param header header for metric
     * @param version version of the format
     * @return true if the format supports multiple records
     */
    template<class MetricSet>
    bool is_multi_record(const MetricSet &header,
                       ::int16_t version = -1)
    {
        typedef typename MetricSet::metric_type metric_t;
        typedef metric_format_factory<metric_t> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();
        if(version < 1) version = header.version();

        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                                                                               version <<  " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        return format_map[version]->is_multi_record();
    }


    /** Get the size of a metric file header
     *
     * @param header header for metric
     * @param version version of the format
     * @return size of metric file header
     */
    template<class MetricType, class Header>
    size_t size_of_header(const Header &header,
                          const ::int16_t version=MetricType::LATEST_VERSION)
    {
        typedef metric_format_factory<MetricType> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();
        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                                                                               version <<  " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        return format_map[version]->header_size(header);
    }

    /** Get the size of a metric file header
     *
     * @param header header for metric
     * @param version version of the format
     * @return size of metric file header
     */
    template<class MetricSet>
    size_t header_size(const MetricSet &header,
                       ::int16_t version = -1)
    {
        typedef typename MetricSet::metric_type metric_t;
        if(version < 1) version = header.version();
        return size_of_header<metric_t>(header, version);
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
        typedef metric_format_factory<MetricType> factory_type;
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
        typedef metric_format_factory<MetricType> factory_type;
        typedef typename factory_type::metric_format_map metric_format_map;
        metric_format_map &format_map = factory_type::metric_formats();
        if (format_map.find(version) == format_map.end())
            INTEROP_THROW(bad_format_exception, "No format found to write file with version: " <<
                                                version <<  " of " << format_map.size());

        INTEROP_ASSERT(format_map[version]);
        format_map[version]->write_metric_header(out, header);
    }

    /** Write a set of metrics to a binary Interop output stream
     *
     * @param out output stream
     * @param metrics set of metrics
     * @param version version of the InterOp to write (if less than 0, get from metric set)
     */
    template<class MetricSet>
    static void write_metrics(std::ostream &out, const MetricSet &metrics, ::int16_t version = -1)
    {
        typedef typename MetricSet::metric_type metric_type;
        typedef metric_format_factory<metric_type> factory_type;
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
        for (typename MetricSet::const_iterator it = metrics.begin();
             it != metrics.end(); it++)
            format_map[version]->write_metric(out, *it, metrics);
    }
    /** Write a set of metrics to a text output stream
     *
     * @param out output stream
     * @param metrics set of metrics
     * @param channel_names list of channel names
     * @param version version of the InterOp to write (if less than 0, get from metric set)
     * @param sep column separator
     * @param eol row separator
     * @param missing missing value indicator
     */
    template<class MetricSet>
    static void write_text(std::ostream &out,
                           const MetricSet &metrics,
                           const std::vector<std::string>& channel_names,
                           ::int16_t version = -1,
                           const char sep=',',
                           const char eol='\n',
                           const char missing='-')
    {
        typedef typename MetricSet::metric_type metric_type;
        typedef text_format_factory<metric_type> factory_type;
        typedef typename factory_type::abstract_text_format_t* abstract_text_format_pointer_t;

        factory_type &factory = factory_type::instance();
        abstract_text_format_pointer_t format = factory.find(version);
        if (format == 0)
            INTEROP_THROW(bad_format_exception,
                          "No format found to write file with version: "
                                  << version <<  " of " << factory.size()
                                  << " for " << metric_type::prefix() << "" << metric_type::suffix()
                                  << " with " << metrics.size() << " metrics");
        INTEROP_ASSERT(format);
        format->write_header(out, metrics, channel_names, sep, eol);
        for (typename MetricSet::const_iterator it = metrics.begin();
             it != metrics.end(); it++)
            format->write_metric(out, *it, metrics, sep, eol, missing);

    }
}}}


