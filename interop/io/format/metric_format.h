/** Concrete metric layout formats for the abstract metric format interface.
 *
 *
 *  @file
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#ifdef _MSC_VER
#pragma warning(disable:4702) // MSVC warns that there is unreachable code
#endif


#include "interop/util/exception.h"
#include "interop/io/format/abstract_metric_format.h"
#include "interop/io/format/generic_layout.h"
#include "interop/io/format/stream_util.h"

namespace illumina { namespace interop { namespace io
{
    /** Shared functionality for reading and writing binary InterOp metrics
     *
     * This format assumes that the binary file as the following format:
     *   1. First byte is the version number
     *   2. Second byte is the record size
     *   3. Optional header
     *   4. A series of records (as defined by Layout)
     */
    template<class Metric, class Layout>
    struct metric_format : public abstract_metric_format<Metric>
    {
    private:
        typedef typename Metric::id_t id_t;
        typedef model::metric_base::metric_set<Metric> metric_set_t;
        typedef typename metric_set_t::offset_map_t offset_map_t;
    public:
        /** Define the metric type */
        typedef Metric metric_t;
        /** Define the metric header type */
        typedef typename Metric::header_type header_t;
        /** Define a layout ID type */
        typedef typename Layout::metric_id_t metric_id_t;
        /** Define the record size type */
        typedef typename Layout::record_size_t record_size_t;

        /** Write the header of a metric InterOp to the given output stream
         *
         * @param out output stream to write the binary InterOp file data
         * @param header header of a metric set
         */
        void write_metric_header(std::ostream &out, const header_t &header)
        {
            const ::uint8_t version = static_cast< ::uint8_t >(Layout::VERSION);
            write_binary(out, version);
            Layout::map_stream_record_size(out, Layout::compute_size(header));
            Layout::map_stream_for_header(out, header);
        }

        /** Write a metric record to the given output stream
         *
         * @param out output stream
         * @param metric a metric to write
         * @param header metric set header
         */
        void write_metric(std::ostream &out, const metric_t &metric, const header_t &header)
        {
            metric_id_t metric_id;
            metric_id.set(metric);
            write_binary(out, metric_id);
            Layout::map_stream(out, metric, header, false);
        }
        /** Read the header into a metric set
         *
         * @param in input stream
         * @param metric_set destination set of metrics
         * @return number of bytes read
         */
        size_t read_header(std::istream& in, model::metric_base::metric_set<Metric>& metric_set)
        {
            const size_t version_byte_size = 1;
            const std::streampos beg = in.tellg();
            read_header_impl(in, metric_set);
            return static_cast<size_t>(in.tellg()-beg)+version_byte_size;
        }

        /** Read all the metrics into a metric set
         *
         * @param in input stream
         * @param metric_set destination set of metrics
         * @param file_size size of the file
         */
        void read_metrics(std::istream& in, metric_set_t& metric_set, const size_t file_size)
        {
            const std::streamsize record_size = read_header_impl(in, metric_set);
            offset_map_t& metric_offset_map = metric_set.offset_map();
            metric_t metric(metric_set);
            if(file_size > 0 && !Layout::MULTI_RECORD)
            {
                const size_t record_count = static_cast<size_t>((file_size-header_size(metric_set))/record_size);
                metric_set.resize(metric_set.size()+record_count);
                std::vector<char> buffer(static_cast<size_t>(record_size));
                INTEROP_ASSERT(!buffer.empty());
                while (in)
                {
                    char *in_ptr = &buffer.front();
                    in.read(in_ptr, record_size);
                    const std::streamsize count = in.gcount();
                    try
                    {
                        if (!test_stream(in, metric_offset_map, count, record_size)) break;
                        read_record(in_ptr, metric_set, metric_offset_map, metric, record_size);
                    }
                    catch(const incomplete_file_exception& ex)
                    {
                        metric_set.trim(metric_offset_map.size());
                        throw ex;
                    }
                }
            }
            else
            {
                while (in)
                {
                    read_record(in, metric_set, metric_offset_map, metric, record_size);
                }
            }
            metric_set.trim(metric_offset_map.size());
        }
        /** Read a metric set from the given input stream
         *
         * @param in input stream containing binary InterOp file data
         * @param header metric set header
         * @return number of bytes in the record
         */
        std::streamsize read_header_impl(std::istream &in, header_t &header)
        {
            // TODO: optimize header reading with block read
            if (in.fail())
                INTEROP_THROW(incomplete_file_exception, "Insufficient header data read from the file"
                        << " for "
                        << Metric::prefix() <<  " "  << Metric::suffix()  <<  " v"  << Layout::VERSION);

            //if we're not actually reading the record size from the stream
            // (the stream position is the same before and after),
            // then don't compare the layout size against the record size
            const ::int64_t stream_position_pre_record_check = in.tellg();
            const std::streamsize record_size = Layout::map_stream_record_size(in,
                                                                               static_cast<record_size_t>(0));
            if(in.fail())
            {
                INTEROP_THROW(incomplete_file_exception, "Insufficient header data read from the file"
                        << " for "
                        << Metric::prefix() <<  " "  << Metric::suffix()  <<  " v"  << Layout::VERSION);
            }
            if(record_size==0)
            {
                INTEROP_THROW(bad_format_exception, "Record size cannot be 0");
            }
            const ::int64_t stream_position_post_record_check = in.tellg();
            Layout::map_stream_for_header(in, header);

            if (in.fail())
                INTEROP_THROW(incomplete_file_exception, "Insufficient extended header data read from the file");
            const std::streamsize layout_size = Layout::compute_size(header);
            if (stream_position_pre_record_check != stream_position_post_record_check && record_size != layout_size)
                INTEROP_THROW(bad_format_exception, "Record size does not match layout size, record size: " <<
                                           record_size << " != layout size: " <<
                                           layout_size << " for "  <<
                                           Metric::prefix() <<  " "  << Metric::suffix()  <<  " v"  <<
                                           Layout::VERSION);
            return layout_size;
        }

        /** Calculate the size of a record
         *
         * @param header metric set header
         * @return size of record in bytes
         */
        size_t record_size(const header_t &header) const
        {
            return static_cast<size_t>(Layout::compute_size(header));
        }

        /** Calculate the size of a record
         *
         * @param header metric set header
         * @return size of header in bytes
         */
        size_t header_size(const header_t &header) const
        {
            return static_cast<size_t>(Layout::compute_header_size(header));
        }

        /** Get the version of this metric format
         *
         * @return version number
         */
        ::int16_t version() const
        {
            return static_cast< ::int16_t >(Layout::VERSION);
        }
        /** Is the format a multi-record format
         *
         * @return true if multiple records make up a single metric
         */
        bool is_multi_record() const
        {
            return Layout::MULTI_RECORD > 0;
        }

    private:
        static bool test_stream(std::istream& in,
                         const offset_map_t& metric_offset_map,
                         const std::streamsize count,
                         const std::streamsize record_size)
        {
            if (in.fail())
            {
                if (count == 0 && !metric_offset_map.empty()) return false;
                INTEROP_THROW(incomplete_file_exception, "Insufficient data read from the file, got: " << count
                                                         << " != expected: " << record_size << " for "
                                                         << Metric::prefix() <<  " "  << Metric::suffix()  <<  " v"
                                                         << Layout::VERSION);
            }
            return true;
        }
        static bool test_stream(const char*, const offset_map_t&, const std::streamsize, const std::streamsize)
        {return true;}
        template<typename InputStream>
        static void read_record(InputStream& in,
                                model::metric_base::metric_set<Metric>& metric_set,
                                offset_map_t& metric_offset_map,
                                metric_t& metric,
                                const std::streamsize record_size)
        {
            metric_id_t id;
            const std::streamsize read_byte_count = read_binary_with_count (in, id);
            if(!test_stream(in, metric_offset_map, read_byte_count, record_size)) return;
            std::streamsize count=read_byte_count;
            if (Layout::is_valid(id))
                // TODO: Refactor tile metrics to move record type into layout id, then we can remove skip_metric,
                // simplifiy all this logic
            {
                metric.set_base(id);// TODO replace with static call
                if (metric_offset_map.find(metric.id()) == metric_offset_map.end())
                {
                    const size_t offset = metric_offset_map.size();
                    if(offset>= metric_set.size()) metric_set.resize(offset+1);
                    metric_set[offset].set_base(id);
                    count += Layout::map_stream(in, metric_set[offset], metric_set, true);
                    if(!test_stream(in, metric_offset_map, count, record_size)) return;
                    if(Layout::skip_metric(metric_set[offset]))//Avoid adding control lanes in tile metrics
                    {
                        metric_set.resize(offset);
                    }
                    else metric_offset_map[metric.id()] = offset;
                }
                else
                {
                    const size_t offset = metric_offset_map[metric.id()];
                    INTEROP_ASSERTMSG(metric_set[offset].lane() != 0, offset);
                    count += Layout::map_stream(in, metric_set[offset], metric_set, false);
                    INTEROP_ASSERT(metric_set[offset].id()>0);
                }
            }
            else
            {
                count += Layout::map_stream(in, metric, metric_set, true);
                //TODO: replace with skip function, simplify code, required for index metrics
            }
            if(!test_stream(in, metric_offset_map, count, record_size)) return;
            if (count != record_size)
            {
                INTEROP_THROW(bad_format_exception, "Record does not match expected size! for "
                                                     << Metric::prefix() <<  " "  << Metric::suffix()  <<  " v"
                                                     << Layout::VERSION << " count=" << count << " != "
                                                     << " record_size: " << record_size
                                                     << " n= " << metric_offset_map.size());
            }
        }
    };
}}}


