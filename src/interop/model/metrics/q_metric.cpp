/** Register format layouts for q-score metrics
 *
 * Each version of the q-score metrics file has a layout defined below.
 *
 *  @file
 *  @date 8/21/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <vector>
#include <algorithm>
#include "interop/util/assert.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/model/metrics/q_by_lane_metric.h"
#include "interop/io/format/text_format_factory.h"
#include "interop/io/format/default_layout.h"
#include "interop/io/format/metric_format.h"
#include "interop/io/format/text_format.h"

using namespace illumina::interop::model::metrics;

namespace illumina { namespace interop { namespace io
{

#pragma pack(1)

    /** Q-score Metric Record Layout Version 4
     *
     * This class provides an interface to reading the q-metric file:
     *  - InterOp/QMetrics.bin
     *  - InterOp/QMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_metric
     *      2. Version: 4
     */
    template<>
    struct generic_layout<q_metric, 4> : public default_layout<4>
    {
        /** @page q_v4 Q-Metrics Version 4
         *
         * This class provides an interface to reading the q-metric file:
         *  - InterOp/QMetrics.bin
         *  - InterOp/QMetricsOut.bin
         *
         *  The file format for q-metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (4)
         *          byte 1: record size (206)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<q_metric, 4> (Class that parses this information)
         *
         *          200 bytes: q-score histogram (uint32_t*50)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Histogram count type */
        typedef ::uint32_t count_t;

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
            const std::streamsize count = stream_map<count_t>(stream, metric.m_qscore_hist, q_metric::MAX_Q_BINS);
            resize_accumulated(stream, metric);
            return count;
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const q_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(metric_id_t) + sizeof(count_t) * q_metric::MAX_Q_BINS);
        }

        /** Compute header size
         *
         * @return header size
         */
        static record_size_t compute_header_size(const q_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t));
        }

    private:
        static void resize_accumulated(const char*, q_metric& metric)
        {
            metric.m_qscore_hist_cumulative.resize(metric.m_qscore_hist.size(), 0);
        }
        static void resize_accumulated(std::istream &, q_metric &metric)
        {
            metric.m_qscore_hist_cumulative.resize(metric.m_qscore_hist.size(), 0);
        }

        static void resize_accumulated(std::ostream &, const q_metric &)
        { }
    };

    /** Q-score Metric Record Layout Version 5
     *
     * This class provides an interface to reading the q-metric file:
     *  - InterOp/QMetrics.bin
     *  - InterOp/QMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_metric
     *      2. Version: 5
     */
    template<>
    struct generic_layout<q_metric, 5> : public default_layout<5>
    {
        /** @page q_v5 Q-Metrics Version 5
         *
         * This class provides an interface to reading the q-metric file:
         *  - InterOp/QMetrics.bin
         *  - InterOp/QMetricsOut.bin
         *
         *  The file format for q-metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (5)
         *          byte 1: record size (206)
         *
         *  @b Extended Header
         *
         *  illumina::interop::io::generic_layout<q_metric, 5> (Class that parses this information)
         *
         *          byte 2:                         flag indicating whether is has bins (bool)
         *
         *     If byte 2 is true, then the following information is also in the header:
         *
         *          byte 3:                         number of bins (uint8)
         *          byte 4-binCount:                array of low ends for each bin (uint8)
         *          byte 4+binCount-4+binCount*2:   array of high ends for each bin (uint8)
         *          byte 4+2*binCount-4+binCount*3: array of values for each  (uint8)
         *
         *  @b n-Records
         *
         *  illumina::interop::io::layout::base_cycle_metric (Class that parses this information)
         *
         *          2 bytes: lane number (uint16)
         *          2 bytes: tile number (uint16)
         *          2 bytes: cycle number (uint16)
         *
         *  illumina::interop::io::generic_layout<q_metric, 5> (Class that parses this information)
         *
         *          200 bytes: q-score histogram (uint32_t*50)
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Histogram count type */
        typedef ::uint32_t count_t;
        /** Bool type */
        typedef ::uint8_t bool_t;
        /** Bin count type */
        typedef ::uint8_t bin_count_t;
        /** Bin type */
        typedef ::uint8_t bin_t;

        /** Read metric from the input stream
         *
         * @param stream input stream
         * @param metric destination metric
         * @param header metric header
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header &header, const bool)
        {
            if (header.m_qscore_bins.size() > 0)
            {
                count_t hist[q_metric::MAX_Q_BINS];
                std::streamsize count = stream_map<count_t>(stream, hist, q_metric::MAX_Q_BINS);
                map_resize(metric.m_qscore_hist, header.m_qscore_bins.size());
                for (size_t i = 0; i < header.m_qscore_bins.size(); ++i)
                {
                    INTEROP_ASSERTMSG((header.m_qscore_bins[i].value() > 0), header.m_qscore_bins[i].value());
                    INTEROP_ASSERTMSG((header.m_qscore_bins[i].value() - 1) < q_metric::MAX_Q_BINS,
                                      header.m_qscore_bins[i].value() - 1 << " < " << q_metric::MAX_Q_BINS);
                    INTEROP_ASSERTMSG(i < metric.m_qscore_hist.size(), metric.m_qscore_hist.size());
                    metric.m_qscore_hist[i] = hist[header.m_qscore_bins[i].value() - 1];
                }
                resize_accumulated(stream, metric);
                return count;
            }
            const std::streamsize count = stream_map<count_t>(stream, metric.m_qscore_hist, q_metric::MAX_Q_BINS);
            resize_accumulated(stream, metric);
            return count;
        }

        /** Write metric to the output stream
         *
         * @param stream output stream
         * @param metric source metric
         * @param header metric header
         * @return number of bytes read or total number of bytes written
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(std::ostream &stream, Metric &metric, Header &header, const bool)
        {
            if (header.m_qscore_bins.size() > 0)
            {
                count_t hist[q_metric::MAX_Q_BINS];
                std::fill(hist, hist + q_metric::MAX_Q_BINS, 0);
                for (size_t i = 0; i < header.m_qscore_bins.size(); i++)
                {
                    INTEROP_ASSERT(header.m_qscore_bins[i].value() > 0);
                    INTEROP_ASSERTMSG((header.m_qscore_bins[i].value() - 1) < q_metric::MAX_Q_BINS,
                                      header.m_qscore_bins[i].value() - 1 << " < " << q_metric::MAX_Q_BINS);
                    hist[header.m_qscore_bins[i].value() - 1] = metric.m_qscore_hist[i];
                }
                return stream_map<count_t>(stream, hist, q_metric::MAX_Q_BINS);
            }
            return stream_map<count_t>(stream, metric.m_qscore_hist, q_metric::MAX_Q_BINS);
        }
        /** Throws an unimplemented error
         */
        template<class Metric, class Header>
        static std::streamsize map_stream(const char*, const Metric &, const Header &, const bool)
        {
            INTEROP_THROW(std::runtime_error, "Function not implemented");
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const q_metric::header_type &)
        {
            return static_cast<record_size_t>(sizeof(metric_id_t) + sizeof(count_t) * q_metric::MAX_Q_BINS);
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
        static std::streamsize map_stream_for_header(Stream &stream, Header &header)
        {
            bool_t has_bins = header.bin_count() > 0;
            std::streamsize count = 0;
            count += stream_map<bool_t>(stream, has_bins);
            if (stream.fail()) return count;
            if (!has_bins) return count;
            bin_count_t bin_count = static_cast<bin_count_t>(header.bin_count());
            count += stream_map<bin_count_t>(stream, bin_count);
            if (stream.fail()) return count;
            if(bin_count==0)
                INTEROP_THROW(bad_format_exception, "Zero bins is not supported");
            INTEROP_ASSERT(bin_count > 0);
            bin_t tmp[q_metric::MAX_Q_BINS];
            map_resize(header.m_qscore_bins, bin_count);

            copy_lower_write(header.m_qscore_bins, tmp);
            count += stream_map<bin_t>(stream, tmp, bin_count);
            copy_lower_read(header.m_qscore_bins, tmp);

            copy_upper_write(header.m_qscore_bins, tmp);
            count += stream_map<bin_t>(stream, tmp, bin_count);
            copy_upper_read(header.m_qscore_bins, tmp);

            copy_value_write(header.m_qscore_bins, tmp);
            count += stream_map<bin_t>(stream, tmp, bin_count);
            copy_value_read(header.m_qscore_bins, tmp);

            return count;
        }

        /** Compute header size
         *
         * @param header q-metric header
         * @return header size
         */
        static record_size_t compute_header_size(const q_metric::header_type &header)
        {
            if (header.bin_count() == 0)
                return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t) + sizeof(bool_t));
            return static_cast<record_size_t>(sizeof(record_size_t) +
                                              sizeof(version_t) + // version
                                              sizeof(bool_t) + // has bins
                                              sizeof(bin_count_t) + // number of bins
                                              header.bin_count() * 3 * sizeof(bin_t));
        }

    private:
        template<size_t N>
        static void copy_lower_read(std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) bins[i].m_lower = tmp[i];
        }

        static void copy_lower_read(const std::vector<q_score_bin> &, bin_t *)
        { }

        template<size_t N>
        static void copy_lower_write(const std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) tmp[i] = static_cast< bin_t >(bins[i].m_lower);
        }

        static void copy_lower_write(std::vector<q_score_bin> &, bin_t *)
        { }

    private:
        template<size_t N>
        static void copy_upper_read(std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) bins[i].m_upper = tmp[i];
        }

        static void copy_upper_read(const std::vector<q_score_bin> &, bin_t *)
        { }

        template<size_t N>
        static void copy_upper_write(const std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) tmp[i] = static_cast< bin_t >(bins[i].m_upper);
        }

        static void copy_upper_write(std::vector<q_score_bin> &, bin_t *)
        { }

    private:
        template<size_t N>
        static void copy_value_read(std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) bins[i].m_value = tmp[i];
        }

        static void copy_value_read(const std::vector<q_score_bin> &, bin_t *)
        { }

        template<size_t N>
        static void copy_value_write(const std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) tmp[i] = static_cast< bin_t >(bins[i].m_value);
        }

        static void copy_value_write(std::vector<q_score_bin> &, bin_t *)
        { }

    private:
        static void resize_accumulated(const char*, q_metric& metric)
        {
            metric.m_qscore_hist_cumulative.resize(metric.m_qscore_hist.size(), 0);
        }
        static void resize_accumulated(std::istream &, q_metric &metric)
        {
            metric.m_qscore_hist_cumulative.resize(metric.m_qscore_hist.size(), 0);
        }

        static void resize_accumulated(std::ostream &, const q_metric &)
        { }
    };

    /** Q-score Metric Record Layout Version 6
     *
     * This class provides an interface to reading the q-metric file:
     *  - InterOp/QMetrics.bin
     *  - InterOp/QMetricsOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_metric
     *      2. Version: 6
     */
    template<>
    struct generic_layout<q_metric, 6> : public default_layout<6>
    {
        /** @page q_v6 Q-Metrics Version 6
         *
         * This class provides an interface to reading the q-metric file:
         *  - InterOp/QMetrics.bin
         *  - InterOp/QMetricsOut.bin
         *
         *  The file format for q-metrics is as follows:
         *
         *  @b Header
         *
         *  illumina::interop::io::read_metrics (Function that parses this information)
         *
         *          byte 0: version number (6)
         *          byte 1: record size (6 + 4*binCount if flag is true otherwise 206)
         *
         *  @b Extended Header
         *
         *  illumina::interop::io::generic_layout<q_metric, 6> (Class that parses this information)
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
         *  illumina::interop::io::generic_layout<q_metric, 6> (Class that parses this information)
         *
         *          4*binCount bytes: q-score histogram (uint32_t*binCount)
         *
         *  Note, if the header has no bins, then binCount is 50 for the records
         */
        /** Metric ID type */
        typedef layout::base_cycle_metric< ::uint16_t > metric_id_t;
        /** Histogram count type */
        typedef ::uint32_t count_t;
        /** Bool type */
        typedef ::uint8_t bool_t;
        /** Bin count type */
        typedef ::uint8_t bin_count_t;
        /** Bin type */
        typedef ::uint8_t bin_t;

        /** Map reading/writing to stream
         *
         * Reading and writing are symmetric operations, map it once
         *
         * @param stream input/output stream
         * @param metric source/destination metric
         * @param header metric header layout
         * @return number of bytes read or total number of bytes written
         */
        template<class Stream, class Metric, class Header>
        static std::streamsize map_stream(Stream &stream, Metric &metric, Header &header, const bool)
        {
            const size_t bin_count = (header.bin_count() == 0) ? static_cast<size_t>(q_metric::MAX_Q_BINS) : header.bin_count();
            const std::streamsize count = stream_map<count_t>(stream, metric.m_qscore_hist, bin_count);
            resize_accumulated(stream, metric);
            return count;
        }

        /** Compute the layout size
         *
         * @return size of the record
         */
        static record_size_t compute_size(const q_metric::header_type &header)
        {
            const size_t bin_count = header.bin_count() == 0 ? static_cast<size_t>(q_metric::MAX_Q_BINS) : header.bin_count();
            return static_cast<record_size_t>(sizeof(metric_id_t) + sizeof(count_t) * bin_count);
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
        static std::streamsize map_stream_for_header(Stream &stream, Header &header)
        {
            bool_t has_bins = header.bin_count() > 0;
            std::streamsize count = 0;
            count += stream_map<bool_t>(stream, has_bins);
            if (stream.fail()) return count;
            if (!has_bins) return count;
            bin_count_t bin_count = static_cast<bin_count_t>(header.bin_count());
            count += stream_map<bin_count_t>(stream, bin_count);
            if (stream.fail()) return count;
            if(bin_count==0)
                INTEROP_THROW(bad_format_exception, "Zero bins is not supported");
            INTEROP_ASSERT(bin_count > 0);
            bin_t tmp[q_metric::MAX_Q_BINS];
            map_resize(header.m_qscore_bins, bin_count);

            copy_lower_write(header.m_qscore_bins, tmp);
            count += stream_map<bin_t>(stream, tmp, bin_count);
            copy_lower_read(header.m_qscore_bins, tmp);

            copy_upper_write(header.m_qscore_bins, tmp);
            count += stream_map<bin_t>(stream, tmp, bin_count);
            copy_upper_read(header.m_qscore_bins, tmp);

            copy_value_write(header.m_qscore_bins, tmp);
            count += stream_map<bin_t>(stream, tmp, bin_count);
            copy_value_read(header.m_qscore_bins, tmp);

            return count;
        }

        /** Compute header size
         *
         * @param header q-metric header
         * @return header size
         */
        static record_size_t compute_header_size(const q_metric::header_type &header)
        {
            if (header.bin_count() == 0)
                return static_cast<record_size_t>(sizeof(record_size_t) + sizeof(version_t) + sizeof(bool_t));
            return static_cast<record_size_t>(sizeof(record_size_t) +
                                              sizeof(version_t) + // version
                                              sizeof(bool_t) + // has bins
                                              sizeof(bin_count_t) + // number of bins
                                              header.bin_count() * 3 * sizeof(bin_t));
        }

    private:
        template<size_t N>
        static void copy_lower_read(std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) bins[i].m_lower = tmp[i];
        }

        static void copy_lower_read(const std::vector<q_score_bin> &, bin_t *)
        { }

        template<size_t N>
        static void copy_lower_write(const std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) tmp[i] = static_cast< bin_t >(bins[i].m_lower);
        }

        static void copy_lower_write(std::vector<q_score_bin> &, bin_t *)
        { }

    private:
        template<size_t N>
        static void copy_upper_read(std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) bins[i].m_upper = tmp[i];
        }

        static void copy_upper_read(const std::vector<q_score_bin> &, bin_t *)
        { }

        template<size_t N>
        static void copy_upper_write(const std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) tmp[i] = static_cast< bin_t >(bins[i].m_upper);
        }

        static void copy_upper_write(std::vector<q_score_bin> &, bin_t *)
        { }

    private:
        template<size_t N>
        static void copy_value_read(std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) bins[i].m_value = tmp[i];
        }

        static void copy_value_read(const std::vector<q_score_bin> &, bin_t *)
        { }

        template<size_t N>
        static void copy_value_write(const std::vector<q_score_bin> &bins, bin_t (&tmp)[N])
        {
            INTEROP_ASSERT(bins.size() <= N);
            for (size_t i = 0; i < bins.size(); ++i) tmp[i] = static_cast< bin_t >(bins[i].m_value);
        }

        static void copy_value_write(std::vector<q_score_bin> &, bin_t *)
        { }

    private:
        static void resize_accumulated(const char*, q_metric& metric)
        {
            metric.m_qscore_hist_cumulative.resize(metric.m_qscore_hist.size(), 0);
        }
        static void resize_accumulated(std::istream &, q_metric &metric)
        {
            metric.m_qscore_hist_cumulative.resize(metric.m_qscore_hist.size(), 0);
        }

        static void resize_accumulated(std::ostream &, const q_metric &)
        { }
    };

    /** Packed layout of a q-score bin
     *
     */
    struct q_score_bin_layout
    {
        /** Bin type */
        typedef ::uint8_t bin_t;
        /** lower bound of bin */
        bin_t m_lower;
        /** upper bound of bin */
        bin_t m_upper;
        /** value of bin */
        bin_t m_value;

        /** Copy constructor
         *
         * @param bin q-score bin
         */
        q_score_bin_layout(const q_score_bin &bin = q_score_bin())
        {
            m_lower = static_cast< bin_t >(bin.lower());
            m_upper = static_cast< bin_t >(bin.upper());
            m_value = static_cast< bin_t >(bin.value());
        }

        /** Assign a point2d to this layout
         *
         * @param bin q-score bin
         * @return this layout
         */
        q_score_bin_layout &operator=(const q_score_bin &bin)
        {
            m_lower = static_cast< bin_t >(bin.lower());
            m_upper = static_cast< bin_t >(bin.upper());
            m_value = static_cast< bin_t >(bin.value());
            return *this;
        }

    };

    /** Q-score by lane Metric Record Layout Version 4
     *
     * This class provides an interface to reading the q_by_lane_metric file:
     *  - InterOp/QMetricsByLane.bin
     *  - InterOp/QMetricsByLaneOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_by_lane_metric
     *      2. Version: 4
     */
    template<>
    struct generic_layout<q_by_lane_metric, 4> : public generic_layout<q_metric, 4>
    {
        /** Skip inserting this metric into the metric set
         *
         * This function was originally added to skip control records in tile metrics.
         *
         * @param id metric to check
         * @return true, if the metric id is 0
         */
        template<class LayoutId>
        static bool is_valid(const LayoutId& id)
        {
            return id.lane > 0;
        }
    };
    /** Q-score by lane Metric Record Layout Version 5
     *
     * This class provides an interface to reading the q_by_lane_metric file:
     *  - InterOp/QMetricsByLane.bin
     *  - InterOp/QMetricsByLaneOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_by_lane_metric
     *      2. Version: 5
     */
    template<>
    struct generic_layout<q_by_lane_metric, 5> : public generic_layout<q_metric, 5>
    {
        /** Skip inserting this metric into the metric set
         *
         * This function was originally added to skip control records in tile metrics.
         *
         * @param id metric to check
         * @return true, if the metric id is 0
         */
        template<class LayoutId>
        static bool is_valid(const LayoutId& id)
        {
            return id.lane > 0;
        }
    };
    /** Q-score by lane Metric Record Layout Version 6
     *
     * This class provides an interface to reading the q_by_lane_metric file:
     *  - InterOp/QMetricsByLane.bin
     *  - InterOp/QMetricsByLaneOut.bin
     *
     * The class takes two template arguments:
     *
     *      1. Metric Type: q_by_lane_metric
     *      2. Version: 6
     */
    template<>
    struct generic_layout<q_by_lane_metric, 6> : public generic_layout<q_metric, 6>
    {
        /** Skip inserting this metric into the metric set
         *
         * This function was originally added to skip control records in tile metrics.
         *
         * @param id metric to check
         * @return true, if the metric id is 0
         */
        template<class LayoutId>
        static bool is_valid(const LayoutId& id)
        {
            return id.lane > 0;
        }
    };

#pragma pack()// DO NOT MOVE
    /** Tile Metric CSV text format
     *
     * This class provide an interface for writing the tile metrics to a CSV file:
     *
     *  - TileMetrics.csv
     */
    template<>
    struct text_layout< q_metric, 1 >
    {
        /** Define a header type */
        typedef q_metric::header_type header_type;
        /** Write header to the output stream
         *
         * @param out output stream
         * @param header q-metric header
         * @param sep column separator
         * @param eol row separator
         * @return number of column headers
         */
        static size_t write_header(std::ostream& out,
                                   const header_type& header,
                                   const std::vector<std::string>&,
                                   const char sep,
                                   const char eol)
        {
            const char* headers[] =
            {
                "Lane", "Tile", "Cycle"
            };
            out << "# Bin Count: " << header.q_val_count() << eol;
            if(header.bin_count() > 0)
            {
                const char* bin_headers[] =
                {
                    "Lower", "Value", "Upper"
                };
                out << "# Column Count: " << util::length_of(bin_headers) << eol;
                out << bin_headers[0];
                for(size_t i=1;i<util::length_of(bin_headers);++i)
                    out << sep << bin_headers[i];
                out << eol;
                for(size_t i=0;i<header.bin_count();++i)
                {
                    out << header.bin_at(i).lower() << sep;
                    out << header.bin_at(i).value() << sep;
                    out << header.bin_at(i).upper() << eol;
                }
            }

            out << "# Column Count: " << util::length_of(headers)+header.q_val_count() << eol;

            out << headers[0];
            for(size_t i=1;i<util::length_of(headers);++i)
                out << sep << headers[i];
            for(size_t i=0;i<header.q_val_count();++i)
                out << sep << "Bin_" << (i+1);
            out << eol;
            return util::length_of(headers)+header.q_val_count();
        }
        /** Write a tile metric to the output stream
         *
         * @param out output stream
         * @param metric q-metric metric
         * @param header q-metric header
         * @param sep column separator
         * @param eol row separator
         * @return number of columns written
         */
        static size_t write_metric(std::ostream& out,
                                   const q_metric& metric,
                                   const header_type& header,
                                   const char sep,
                                   const char eol,
                                   const char)
        {
            if( static_cast<size_t>(header.q_val_count()) != metric.size() )
                INTEROP_THROW(bad_format_exception, "Header and metric bin count mismatch: "
                        << header.q_val_count() << " != " << metric.size());
            out << metric.lane() << sep << metric.tile() << sep << metric.cycle();
            for(size_t i=0;i<header.q_val_count();++i)
            {
                out << sep << metric.qscore_hist()[i];
            }
            out << eol;
            return 0;
        }
    };
    /** Tile Metric CSV text format
     *
     * This class provide an interface for writing the tile metrics to a CSV file:
     *
     *  - TileMetrics.csv
     */
    template<>
    struct text_layout< q_by_lane_metric, 1 > : public text_layout< q_metric, 1 >
    {
    };
}}}

INTEROP_FORCE_LINK_DEF(q_metric)

INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_metric, 4)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_metric, 5)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_metric, 6)

INTEROP_FORCE_LINK_DEF(q_by_lane_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_by_lane_metric, 4)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_by_lane_metric, 5)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_by_lane_metric, 6)

// Text formats
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(q_metric, 1)
INTEROP_REGISTER_METRIC_TEXT_LAYOUT(q_by_lane_metric, 1)
