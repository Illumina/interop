/** Register format layouts for extraction metrics
 *
 * Each version of the extraction metrics file has a layout defined below.
 *
 *  @file
 *  @date 8/20/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <limits>
#include "interop/util/math.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/io/format/metric_format_factory.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"
#include "interop/io/format/text_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina { namespace interop { namespace io
{
#pragma pack(1)

    /** Extraction Metric Record Layout Version 2
     *
     * This class provides an interface to reading the extraction metric file:
     *  - InterOp/ExtractionMetrics.bin
     *  - InterOp/ExtractionMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: extraction_metric
     *      2. Version: 2
     */
    template<>
    struct generic_layout<extraction_metric, 2> : public default_layout<2>
    {
        /** @page extraction_v2 Extraction Version 2
         *
         * This class provides an interface to reading the extraction metric file:
         *  - InterOp/ExtractionMetrics.bin
         *  - InterOp/ExtractionMetricsOut.bin
         *
         *  The file format for extraction metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (2)
         *          byte 1: record size (38)
         *
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<extraction_metric, 2> (Class that parses this information)
         *
         *          4 bytes: focus for channel A (float32)
         *          4 bytes: focus for channel C (float32)
         *          4 bytes: focus for channel G (float32)
         *          4 bytes: focus for channel T (float32)
         *          2 bytes: max intensity for channel A (uint16)
         *          2 bytes: max intensity for channel C (uint16)
         *          2 bytes: max intensity for channel G (uint16)
         *          2 bytes: max intensity for channel T (uint16)
         *          8 bytes: date time stamp (uint64)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Intensity type */
        typedef float focus_t;
        /** Intensity type */
        typedef ::uint16_t intensity_t;
        /** Time type */
        typedef ::uint64_t datetime_t;

        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header &, const bool)
        {
            std::streamsize count = 0;
            count += stream_map<focus_t>(stream, metric.m_focus_scores, extraction_metric::MAX_CHANNELS);
            if (stream)
                set_nan_to_zero(stream, metric.m_focus_scores);// TODO: Remove and rebaseline regression tests
            else return count;
            count += stream_map<intensity_t>(stream, metric.m_max_intensity_values, extraction_metric::MAX_CHANNELS);
            count += stream_map<datetime_t>(stream, metric.m_date_time_csharp.value);
            convert_datetime(stream, metric);
            return count;
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const extraction_metric::header_type &)
        {
            return static_cast<record_size_t>(
                    sizeof(metric_id_t) +
                    sizeof(focus_t) * extraction_metric::MAX_CHANNELS +     // m_focus_scores
                    sizeof(intensity_t) * extraction_metric::MAX_CHANNELS + // m_max_intensity_values
                    sizeof(datetime_t)                                  // m_dateTime
            );
        }

        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const extraction_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }

    private:
        static void convert_datetime(std::ostream &, const extraction_metric &)
        {
            // We do not support writing this date/time in C# format from C++
        }
        static void convert_datetime(const char*, extraction_metric& metric)
        {
            metric.m_date_time = metric.m_date_time_csharp.to_unix();
        }

        static void convert_datetime(std::istream &in, extraction_metric &metric)
        {
            if (in.fail()) return;
            metric.m_date_time = metric.m_date_time_csharp.to_unix();
        }
        static void set_nan_to_zero(const char*, std::vector<float>& vals)
        {
            for(size_t i=0;i<vals.size();++i)
                if(std::isnan(vals[i])) vals[i] = 0;
        }

        static void set_nan_to_zero(std::ostream &, const std::vector<float> &) // TODO: Remove and rebaseline
        {

        }
        static void set_nan_to_zero(std::istream&, std::vector<float>& vals)
        {
            for(size_t i=0;i<vals.size();++i)
                if(std::isnan(vals[i])) vals[i] = 0;
        }
    };


#pragma pack()
    /** Extraction Metric CSV text format
     *
     * This class provide an interface for writing the extraction metrics to a CSV file:
     *
     *  - ExtractionMetrics.csv
     */
    template<>
    struct text_layout< extraction_metric, 1 >
    {
        /** Define a header type */
        typedef extraction_metric::header_type header_type;
        /** Write header to the output stream
         *
         * @param out output stream
         * @param header extraction metric header
         * @param channel_names list of channel names
         * @param sep column separator
         * @param eol row separator
         * @return number of column headers
         */
        static size_t write_header(std::ostream& out,
                                   const header_type& header,
                                   const std::vector<std::string>& channel_names,
                                   const char sep,
                                   const char eol)
        {
            if( static_cast<size_t>(header.channel_count()) != channel_names.size() )
                INTEROP_THROW(bad_format_exception, "Header and channel names count mismatch");
            const char* headers[] =
            {
                "Lane", "Tile", "Cycle", "TimeStamp"
            };
            out << "# Column Count: " << util::length_of(headers)+header.channel_count()*2 << eol;
            out << "# Channel Count: " << header.channel_count() << eol;
            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            const std::string max_intensity = "MaxIntensity";
            for(size_t i=0;i<static_cast<size_t>(header.channel_count());++i)
                out << sep << max_intensity << "_" << channel_names[i];
            const std::string focus = "Focus";
            for(size_t i=0;i<static_cast<size_t>(header.channel_count());++i)
                out << sep << focus << "_" << channel_names[i];
            out << eol;
            return util::length_of(headers);
        }
        /** Write a extraction metric to the output stream
         *
         * @param out output stream
         * @param metric extraction metric
         * @param header extraction metric header
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const extraction_metric& metric,
                                   const header_type& header,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            if( header.channel_count() != metric.channel_count() )
                INTEROP_THROW(bad_format_exception, "Header and metric channel count mismatch");
            out << metric.lane() << sep << metric.tile() << sep << metric.cycle() << sep;
            out << metric.date_time(); // TODO: Format date/time
            for(size_t i=0;i<static_cast<size_t>(header.channel_count());i++)
                out << sep << metric.max_intensity(i);
            for(size_t i=0;i<static_cast<size_t>(header.channel_count());i++)
                out << sep << metric.focus_score(i);
            out << eol;
            return 0;
        }
    };
}}}

INTEROP_FORCE_LINK_DEF(extraction_metric)

INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(extraction_metric, 2)


// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(extraction_metric, 1)

