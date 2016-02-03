/** Interface for reading/writing InterOps from/to a binary stream
 *
 *
 *  @file
 *
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <string>
#include <fstream>
#include "interop/io/format/metric_format_factory.h"
#include "interop/util/filesystem.h"
#include "interop/util/assert.h"

#pragma once
namespace illumina {
    namespace interop {
        namespace io {

            namespace detail {
                /** Append the InterOp directory to the run directory file path
                 *
                 * @param runDirectory file path to the run directory
                 * @return file path to the InterOp directory
                 */
                inline std::string interop_directory_name(const std::string& runDirectory) {
                    return io::combine(runDirectory, "InterOp");
                }
                /** Generate a file name from a run directory and the InterOp name
                 *
                 * @param runDirectory file path to the run directory
                 * @param name name of the interop file
                 * @param useOut if true, append "Out" to the end of the filename
                 * @return file path to the InterOp directory
                 */
                inline std::string interop_name(const std::string& runDirectory, const std::string& name, bool useOut = true) {
                    return io::combine(interop_directory_name(runDirectory),
                                       name + "Metrics" + ((useOut) ? ("Out.bin") : (".bin")));
                }
                /** Memory buffer for a stream
                 *
                 * This class is used to turn a binary byte buffer into an input stream for reading.
                 */
                struct membuf : std::streambuf {
                    /** Constructor
                     *
                     * @param begin start iterator for a char buffer
                     * @param end end iterator for a char buffer
                     */
                    membuf(char *begin, char *end) {
                        this->setg(begin, begin, end);
                    }
                };
            }
            /** Read the binary InterOp file into the given metric set
             *
             * @param in input stream
             * @param metrics metric set
             */
            template<class MetricSet>
            void read_metrics(std::istream& in, MetricSet& metrics)
            {
                typedef typename MetricSet::metric_type metric_type;
                typedef metric_format_factory <metric_type> factory_type;
                typedef typename factory_type::metric_format_map metric_format_map;
                metric_format_map& format_map=factory_type::metric_formats();

                const int version = in.get();
                if (format_map.find(version) == format_map.end())
                    throw bad_format_exception("No format found to parse file with version: " +
                                               util::lexical_cast<std::string>(version) + " of " +
                                               util::lexical_cast<std::string>(format_map.size()));
                assert(format_map[version]);
                metrics.set_version(static_cast< ::int16_t>(version));
                const std::streamsize record_size = format_map[version]->read_header(in, metrics);

                std::map<typename metric_type::id_t, size_t> metric_offset_map;
                while(in)
                {
                    metric_type metric;// Moving this out breaks index metrics - TODO: fix by adding clear to index metrics (or move) or swap
                    const std::streamsize icount = format_map[version]->read_metric_id(in, metric);
                    if(in.fail())
                    {
                        if(icount==0 && !metric_offset_map.empty()) break;
                        throw incomplete_file_exception(std::string("Insufficient data read from the file, got: ")+
                                                        util::lexical_cast<std::string>(icount)+" != expected: "+
                                                        util::lexical_cast<std::string>(record_size));
                    }

                    if (metric_offset_map.find(metric.id()) == metric_offset_map.end())
                    {
                        const std::streamsize count = format_map[version]->read_metric(in, metric, metrics, true)+icount;
                        if(in.fail())
                        {
                            if(count==0 && !metric_offset_map.empty()) break;
                            throw incomplete_file_exception(std::string("Insufficient data read from the file, got: ")+
                                                            util::lexical_cast<std::string>(count)+" != expected: "+
                                                            util::lexical_cast<std::string>(record_size));
                        }
                        if(count != record_size)
                        {
                            throw bad_format_exception("Record does not match expected size!");
                        }
                        if(metric.lane() > 0 && metric.tile() > 0)
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
                        const std::streamsize count = format_map[version]->read_metric(in, metrics.at(offset), metrics, false)+icount;
                        if(in.fail())
                        {
                            if(count==0 && !metric_offset_map.empty()) break;
                            throw incomplete_file_exception(std::string("Insufficient data read from the file, got: ")+
                                                            util::lexical_cast<std::string>(count)+" != expected: "+
                                                            util::lexical_cast<std::string>(record_size));
                        }
                        if(count != record_size)
                        {
                            throw bad_format_exception("Record does not match expected size!");
                        }
                        metrics.metric_updated_at(offset);
                    }
                }
            }
            /** Write a metric to a binary InterOp file
             *
             * @param out output stream
             * @param metric metric
             * @param version version of the format
             */
            template<class MetricType>
            void write_metric(std::ostream& out, const MetricType& metric, const typename MetricType::header_type& header, const ::int16_t version)
            {
                typedef metric_format_factory <MetricType> factory_type;
                typedef typename factory_type::metric_format_map metric_format_map;
                metric_format_map& format_map=factory_type::metric_formats();

                if (format_map.find(version) == format_map.end())
                    throw bad_format_exception("No format found to write file with version: " +
                                               util::lexical_cast<std::string>(version) + " of " +
                                               util::lexical_cast<std::string>(format_map.size()));

                assert(format_map[version]);
                format_map[version]->write_metric(out, metric, header);
            }
            /** Write a header describing the metric records
             *
             * @param out output stream
             * @param version version of format
             * @param header metric header
             */
            template<class MetricType>
            void write_metric_header(std::ostream& out, const ::int16_t version, const typename MetricType::header_type& header=typename MetricType::header_type())
            {
                typedef metric_format_factory <MetricType> factory_type;
                typedef typename factory_type::metric_format_map metric_format_map;
                metric_format_map& format_map=factory_type::metric_formats();
                if (format_map.find(version) == format_map.end())
                    throw bad_format_exception("No format found to write file with version: " +
                                               util::lexical_cast<std::string>(version) + " of " +
                                               util::lexical_cast<std::string>(format_map.size()));

                assert(format_map[version]);
                format_map[version]->write_metric_header(out, header);
            }
            /** Write a metric header to a binary Interop output stream
             *
             * @param out output stream
             * @param metrics set of metrics
             * @param version version of the InterOp to write (if less than 0, get from metric set)
             */
            template<class MetricSet>
            static void write_metrics(std::ostream &out, const MetricSet &metrics, int version = -1)
            {
                typedef typename MetricSet::metric_type metric_type;
                typedef metric_format_factory <metric_type> factory_type;
                typedef typename factory_type::metric_format_map metric_format_map;
                metric_format_map& format_map=factory_type::metric_formats();

                if (version < 0) version = metrics.version();
                if (format_map.find(version) == format_map.end())
                    throw bad_format_exception("No format found to write file with version: " +
                                               util::lexical_cast<std::string>(version) + " of " +
                                               util::lexical_cast<std::string>(format_map.size()));

                assert(format_map[version]);
                format_map[version]->write_metric_header(out, metrics);
                for (typename MetricSet::metric_array_t::const_iterator it = metrics.metrics().begin();
                     it != metrics.metrics().end(); it++)
                    format_map[version]->write_metric(out, *it, metrics);
            }
        }
    }
}

