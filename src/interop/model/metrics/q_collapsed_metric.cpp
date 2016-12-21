/** Register format layouts for collapsed q-score metrics
 *
 * Each version of the collapsed q-score metrics file has a layout defined below.
 *
 *  @file
 *  @date 5/18/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <vector>
#include <algorithm>
#include "interop/util/assert.h"
#include "interop/model/metrics/q_collapsed_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"
#include "interop/io/format/text_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io {

    /** Collapsed Q-score Metric Record Layout Version 2
     *
     * This class provides an interface to reading the collapsed q-metric file:
     *  - InterOp/QMetrics2030.bin
     *  - InterOp/QMetrics2030Out.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_collapsed_metric
     *      2. Version: 2
     */
    template<>
    struct generic_layout<q_collapsed_metric, 2> : public default_layout<2>
    {
        /** @page q_collapsed_v2 Collapsed Q-Metrics Version 2
         *
         * This class provides an interface to reading the collapsed q-metric file:
         *  - InterOp/QMetrics2030.bin
         *  - InterOp/QMetrics2030Out.bin
         *
         *  The file format for collapsed q-metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (2)
         *          byte 1: record size (can take two different values)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<q_collapsed_metric, 2> (Class that parses this information)
         *
         *          4 bytes: Q20 count (uint32)
         *          4 bytes: Q30 count (uint32)
         *          4 bytes: Total count (uint32)
         *          4 bytes: Median score (uint32)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Histogram count type */
        typedef ::uint32_t count_t;
        /** Histogram count type */
        typedef float median_t;
        enum{
            /** Define the size of the record without the ID */
            RECORD_SIZE = sizeof(count_t)*3,
            /** Define the size of the record with the ID */
            TOTAL_RECORD_SIZE = RECORD_SIZE+sizeof(metric_id_t),
            /** Alternative record size*/
            ALT_RECORD_SIZE=TOTAL_RECORD_SIZE+sizeof(median_t)};

        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @param header metric set header
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header& header, const bool)
        {
            std::streamsize count = 0;
            count += stream_map< count_t >(stream, metric.m_q20);
            count += stream_map< count_t >(stream, metric.m_q30);
            count += stream_map< count_t >(stream, metric.m_total);
            if(header.m_record_size == ALT_RECORD_SIZE || is_output(stream))
            {
                std::streamsize extra = stream_map<median_t>(stream, metric.m_median_qscore);
                test_incomplete(stream, extra);
                count += extra;
            }
            else
            {
                if(count != TOTAL_RECORD_SIZE )
                    INTEROP_THROW( incomplete_file_exception, "Insufficient data read from the file, got: "<<
                            count << " != expected: " << TOTAL_RECORD_SIZE );
                return ALT_RECORD_SIZE;
            }
            return count;
        }

        /** Compute the layout size
         *
         * @note The size of the record is not fixed for this format, the median q-score may more may not be there
         *
         * @return 1
         */
        static record_size_t compute_size(const q_collapsed_metric::header_type&)
        {
            return static_cast<record_size_t>(ALT_RECORD_SIZE);
        }

        /** Compute header size
         *
         * @return header size
         */
        static size_t compute_header_size(const q_collapsed_metric::header_type &)
        {
            return static_cast<size_t>(sizeof(record_size_t) + sizeof(version_t));
        }
        /** Map reading/writing a header to a stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param header metric set header
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Header>
        static std::streamsize map_stream_for_header(Stream& stream, Header& header)
        {
            record_size_t record_size = ALT_RECORD_SIZE;
            std::streamsize count = 0;
            count += stream_map< record_size_t >(stream, record_size);
            if(stream.fail())
                INTEROP_THROW(incomplete_file_exception, "Insufficient extended header data read from the file");
            if( (record_size != static_cast<record_size_t>(TOTAL_RECORD_SIZE )) &&
                    (record_size != static_cast<record_size_t>(ALT_RECORD_SIZE)) )
            {
                INTEROP_THROW( bad_format_exception, "QMetric2030 requires a record size of 3 or 4 uint32 values (" <<
                                           (TOTAL_RECORD_SIZE) << ", " <<
                                           (ALT_RECORD_SIZE) <<
                                           ") not " <<
                                           (int(record_size)));
            }
            set_record_size(stream, header, record_size);
            return count;
        }
        /** Does not read/write record size, this is done in `map_stream_for_header`
         *
         * @return size of the record
         */
        template<class Stream>
        static record_size_t map_stream_record_size(Stream&, record_size_t)
        {
            return static_cast<record_size_t>(ALT_RECORD_SIZE);
        }
    private:
        static bool is_output(const char*){return false;}
        static bool is_output(std::istream&){return false;}
        static bool is_output(std::ostream&){return true;}
        static void test_incomplete(const char*, const std::streamsize extra)
        {
            if(static_cast<size_t>(extra) != sizeof(median_t) )
                INTEROP_THROW( incomplete_file_exception, "Insufficient data read from the file, got: " <<
                                                          extra << " != expected: " <<
                                                          sizeof(median_t) );
        }
        static void test_incomplete(std::istream&, const std::streamsize extra)
        {
            if(static_cast<size_t>(extra) != sizeof(median_t) )
                INTEROP_THROW( incomplete_file_exception, "Insufficient data read from the file, got: " <<
                                                          extra << " != expected: " <<
                                                          sizeof(median_t) );
        }
        static void test_incomplete(std::ostream&, const std::streamsize ){}
        template<class Header>
        static void set_record_size(std::istream&, Header& header, const record_size_t record_size)
        {
            header.m_record_size = record_size;
        }
        template<class Header>
        static void set_record_size(const char*, Header& header, const record_size_t record_size)
        {
            header.m_record_size = record_size;
        }
        template<class Header>
        static void set_record_size(std::ostream&, Header&, const record_size_t){}
    };

    /** Collapsed Q-score Metric Record Layout Version 3
     *
     * This class provides an interface to reading the collapsed q-metric file:
     *  - InterOp/QMetrics2030.bin
     *  - InterOp/QMetrics2030Out.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_collapsed_metric
     *      2. Version: 3
     */
    template<>
    struct generic_layout<q_collapsed_metric, 3> : public generic_layout<q_collapsed_metric, 2>
    {
        /** @page q_collapsed_v3 Collapsed Q-Metrics Version 3
         *
         * This class provides an interface to reading the collapsed q-metric file:
         *  - InterOp/QMetrics2030.bin
         *  - InterOp/QMetrics2030Out.bin
         *
         *  The file format for collapsed q-metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (3)
         *          byte 1: record size
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<q_collapsed_metric, 3> (Class that parses this information)
         *
         *          4 bytes: Q20 count (uint32)
         *          4 bytes: Q30 count (uint32)
         *          4 bytes: Total count (uint32)
         *          4 bytes: Median score (uint32)
         */
        enum{/**Version of the format */VERSION=3};
    };


    /** Collapsed Q-score Metric Record Layout Version 4
     *
     * This class provides an interface to reading the collapsed q-metric file:
     *  - InterOp/QMetrics2030.bin
     *  - InterOp/QMetrics2030Out.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_collapsed_metric
     *      2. Version: 4
     */
    template<>
    struct generic_layout<q_collapsed_metric, 4> : public generic_layout<q_collapsed_metric, 2>
    {
        /** @page q_collapsed_v4 Collapsed Q-Metrics Version 4
         *
         * This class provides an interface to reading the collapsed q-metric file:
         *  - InterOp/QMetrics2030.bin
         *  - InterOp/QMetrics2030Out.bin
         *
         *  The file format for collapsed q-metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (4)
         *          byte 1: record size
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<q_collapsed_metric, 4> (Class that parses this information)
         *
         *          4 bytes: Q20 count (uint32)
         *          4 bytes: Q30 count (uint32)
         *          4 bytes: Total count (uint32)
         *          4 bytes: Median score (uint32)
         */
        enum{/**Version of the format */VERSION=4};
    };


    /** Collapsed Q-score Metric Record Layout Version 5
     *
     * This class provides an interface to reading the collapsed q-metric file:
     *  - InterOp/QMetrics2030.bin
     *  - InterOp/QMetrics2030Out.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_collapsed_metric
     *      2. Version: 5
     */
    template<>
    struct generic_layout<q_collapsed_metric, 5> : public default_layout<5, 1 /*Tmp hack */>
    {
        /** @page q_collapsed_v5 Collapsed Q-Metrics Version 5
         *
         * This class provides an interface to reading the collapsed q-metric file:
         *  - InterOp/QMetrics2030.bin
         *  - InterOp/QMetrics2030Out.bin
         *
         *  The file format for collapsed q-metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (5)
         *          byte 1: record size
         *
         *  @b Extended Header
         *
         *  illumina::interop::io::generic_layout<q_collapsed_metric, 5> (Class that parses this information)
         *
         *          byte 2:                         flag indicating whether is has bins (bool)
         *
         *     If byte 2 is true, then the following information is also in the header:
         *
         *          byte 3:                         number of bins (uint8)
         *          byte 4-binCount:                array of low ends for each bin (uint8)
         *          byte 4+binCount-4+binCount*2:   array of high ends for each bin (uint8)
         *          byte 4+2*binCount-4+binCount*3: array of values for each bin (uint8)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<q_collapsed_metric, 5> (Class that parses this information)
         *
         *          4 bytes: Q20 count (uint32)
         *          4 bytes: Q30 count (uint32)
         *          4 bytes: Total count (uint32)
         *          4 bytes: Median score (uint32)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Histogram count type */
        typedef ::uint32_t count_t;
        /** Histogram count type */
        typedef float median_t;
        /** Bool type */
        typedef ::uint8_t bool_t;
        /** Bin count type */
        typedef ::uint8_t bin_count_t;
        /** Bin type */
        typedef ::uint8_t bin_t;
        enum{
            /** Define the size of the record without the ID */
            RECORD_SIZE = sizeof(count_t)*3,
            /** Define the size of the record with the ID */
            TOTAL_RECORD_SIZE = RECORD_SIZE+sizeof(metric_id_t),
            /** Alternative record size*/
            ALT_RECORD_SIZE=TOTAL_RECORD_SIZE+sizeof(median_t)
            };

        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @param header metric set header
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header & header, const bool)
        {
            std::streamsize count = 0;
            count += stream_map<count_t>(stream, metric.m_q20);
            count += stream_map<count_t>(stream, metric.m_q30);
            count += stream_map<count_t>(stream, metric.m_total);
            if (header.m_record_size == ALT_RECORD_SIZE || is_output(stream))
            {
                const std::streamsize extra = stream_map<median_t>(stream, metric.m_median_qscore);
                test_incomplete(stream, extra);
                count += extra;
            }
            else
            {
                if (count != TOTAL_RECORD_SIZE)
                    INTEROP_THROW(incomplete_file_exception, "Insufficient data read from the file, got: " <<
                                                             count << " != expected: " << TOTAL_RECORD_SIZE);
                return ALT_RECORD_SIZE;
            }
            return count;
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const q_collapsed_metric::header_type &)
        {
            return static_cast<record_size_t>(ALT_RECORD_SIZE);
        }
        /** Map reading/writing a header to a stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param header source/destination header
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Header>
        static std::streamsize map_stream_for_header(Stream& stream, Header& header)
        {
            record_size_t record_size = ALT_RECORD_SIZE;
            std::streamsize count = 0;
            count += stream_map< record_size_t >(stream, record_size);
            if(stream.fail())
                INTEROP_THROW( incomplete_file_exception, "Insufficient extended header data read from the file");
            if( record_size != static_cast<record_size_t>(TOTAL_RECORD_SIZE) &&
                    record_size != static_cast<record_size_t>(ALT_RECORD_SIZE))
                INTEROP_THROW( bad_format_exception, "QMetric2030 requires a record size of 3 or 4 uint32 values ("<<
                                                   TOTAL_RECORD_SIZE <<  ", " <<
                                                   ALT_RECORD_SIZE <<
                                                   ") not " << int(record_size));

            set_record_size(stream, header, record_size);
            bool_t has_bins = false; // Never write the header
            count += stream_map< bool_t >(stream, has_bins);
            if(stream.fail()) return count;
            if(!has_bins) return count;
            bin_count_t temp_bin_count=0;
            count+=stream_map< bin_count_t >(stream, temp_bin_count);
            if(stream.fail()) return count;
            const bin_count_t bin_count = static_cast< bin_count_t >(temp_bin_count);
            INTEROP_ASSERTMSG(bin_count>0, VERSION);
            bin_t tmp[q_metric::MAX_Q_BINS];
            count+=stream_map< bin_t >(stream, tmp, bin_count*3);
            map_resize(header.m_qscore_bins, bin_count);

            return count;
        }
        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const q_collapsed_metric::header_type& header)
        {
            if (header.bin_count() == 0)
                return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t) + sizeof(bool_t));
            return static_cast<record_size_t>(sizeof(record_size_t) +
                                              sizeof(version_t) + // version
                                              sizeof(bool_t) + // has bins
                                              sizeof(bin_count_t) + // number of bins
                                              header.bin_count() * 3 * sizeof(bin_t));
        }
        /** Does not read/write record size, this is done in `map_stream_for_header`
         *
         * @return size of the record
         */
        template<class Stream>
        static record_size_t map_stream_record_size(Stream&, record_size_t)
        {
            return static_cast<record_size_t>(ALT_RECORD_SIZE);
        }
    private:
        static bool is_output(const char*){return false;}
        static bool is_output(std::istream&){return false;}
        static bool is_output(std::ostream&){return true;}
        static void test_incomplete(const char*, const std::streamsize extra)
        {
            if(static_cast<size_t>(extra) != sizeof(median_t) )
                INTEROP_THROW( incomplete_file_exception, "Insufficient data read from the file, got: " <<
                                                          extra << " != expected: " <<
                                                          sizeof(median_t) );
        }
        static void test_incomplete(std::istream&, const std::streamsize extra)
        {
            if(static_cast<size_t>(extra) != sizeof(median_t) )
                INTEROP_THROW( incomplete_file_exception, "Insufficient data read from the file, got: " <<
                                                extra << " != expected: " <<
                                                sizeof(median_t) );
        }
        static void test_incomplete(std::ostream&, const std::streamsize ){}
        template<class Header>
        static void set_record_size(const char*, Header& header, const record_size_t record_size)
        {
            header.m_record_size = record_size;
        }
        template<class Header>
        static void set_record_size(std::istream&, Header& header, const record_size_t record_size)
        {
            header.m_record_size = record_size;
        }
        template<class Header>
        static void set_record_size(std::ostream&, Header&, const record_size_t){}
    };

    /** Collapsed Q-score Metric Record Layout Version 6
     *
     * This class provides an interface to reading the collapsed q-metric file:
     *  - InterOp/QMetrics2030.bin
     *  - InterOp/QMetrics2030Out.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_collapsed_metric
     *      2. Version: 6
     */
    template<>
    struct generic_layout<q_collapsed_metric, 6> : public generic_layout<q_collapsed_metric, 5>
    {
        /** @page q_collapsed_v6 Collapsed Q-Metrics Version 6
         *
         * This class provides an interface to reading the collapsed q-metric file:
         *  - InterOp/QMetrics2030.bin
         *  - InterOp/QMetrics2030Out.bin
         *
         *  The file format for collapsed q-metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (6)
         *          byte 1: record size (22)
         *
         *  @b Extended Header
         *
         *  illumina::interop::io::generic_layout<q_collapsed_metric, 6> (Class that parses this information)
         *
         *          byte 2:                         flag indicating whether is has bins (bool)
         *
         *     If byte 2 is true, then the following information is also in the header:
         *
         *          byte 3:                         number of bins (uint8)
         *          byte 4-binCount:                array of low ends for each bin (uint8)
         *          byte 4+binCount-4+binCount*2:   array of high ends for each bin (uint8)
         *          byte 4+2*binCount-4+binCount*3: array of values for each bin (uint8)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<q_collapsed_metric, 6> (Class that parses this information)
         *
         *          4 bytes: Q20 count (uint32)
         *          4 bytes: Q30 count (uint32)
         *          4 bytes: Total count (uint32)
         *          4 bytes: Median score (uint32)
         */
        enum{/**Version of the format */VERSION=6};
    };
    /** Tile Metric CSV text format
     *
     * This class provide an interface for writing the tile metrics to a CSV file:
     *
     *  - TileMetrics.csv
     */
    template<>
    struct text_layout< q_collapsed_metric, 1 >
    {
        /** Define a header type */
        typedef q_collapsed_metric::header_type header_type;
        /** Write header to the output stream
         *
         * @param out output stream
         * @param sep column separator
         * @param eol row separator
         * @return number of column headers
         */
        static size_t write_header(std::ostream& out,
                                   const header_type&,
                                   const std::vector<std::string>&,
                                   const char sep,
                                   const char eol)
        {
            const char* headers[] =
            {
                "Lane", "Tile", "Cycle",
                "Q20", "Q30", "Total", "MedianQScore"
            };
            out << "# Column Count: " << util::length_of(headers) << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a tile metric to the output stream
         *
         * @param out output stream
         * @param metric tile metric
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const q_collapsed_metric& metric,
                                   const header_type&,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            out << metric.lane() << sep << metric.tile() << sep << metric.cycle() << sep;
            out << metric.q20() << sep << metric.q30() << sep << metric.total() << sep << metric.median_qscore() << eol;
            return 0;
        }
    };
}}}
INTEROP_FORCE_LINK_DEF(q_collapsed_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_collapsed_metric, 2 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_collapsed_metric, 3 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_collapsed_metric, 4 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_collapsed_metric, 5 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_collapsed_metric, 6 )


// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(q_collapsed_metric, 1)

