/** Register format layouts for q-score metrics
 *
 * Each version of the q-score metrics file has a layout defined below.
 *
 *  @file
 *
 *  @date 8/21/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <vector>
#include <algorithm>
#include "interop/util/assert.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/io/format/metric_format_factory.h"

using namespace illumina::interop::model::metrics;

namespace illumina{ namespace interop{ namespace io {
#pragma pack(1)
            /** Q-score Metric Record Layout Version 4
             *
             * This class provides an interface to reading the registration metric file:
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
                 *          byte 0: version number
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
                 *  illumina::interop::io::generic_layout<q_metric, 4> (Class that parses this information)
                 *
                 *          200 bytes: q-score histogram (uint32_t*50)
                 */
                /** Metric ID type */
                typedef layout::base_cycle_metric metric_id_t;
                /** Map reading/writing to stream
                 *
                 * Reading and writing are symmetric operations, map it once
                 *
                 * @param stream input/output stream
                 * @param metric source/destination metric
                 * @return number of bytes read or total number of bytes written
                 */
                template<class Stream, class Metric, class Header>
                static std::streamsize map_stream(Stream& stream, Metric& metric, Header&, const bool)
                {
                    return stream_map< ::uint32_t >(stream, metric.m_qscoreHist, q_metric::MAX_Q_BINS);
                }
                /** Compute the layout size
                 *
                 * @return size of the record
                 */
                static record_size_t computeSize(const q_metric::header_type&)
                {
                    return static_cast<record_size_t>(sizeof(metric_id_t)+sizeof(::uint32_t)*q_metric::MAX_Q_BINS);
                }
            };
            /** Q-score Metric Record Layout Version 5
             *
             * This class provides an interface to reading the registration metric file:
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
                 *          byte 0: version number
                 *          byte 1: record size
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
                typedef layout::base_cycle_metric metric_id_t;

                /** Read metric from the input stream
                 *
                 * @param stream input stream
                 * @param metric destination metric
                 * @param header metric header
                 * @return number of bytes read or total number of bytes written
                 */
                template<class Metric, class Header>
                static std::streamsize map_stream(std::istream& stream, Metric& metric, Header& header, const bool)
                {
                    if(header.m_qscoreBins.size()>0)
                    {
                        ::uint32_t hist[q_metric::MAX_Q_BINS];
                        std::streamsize count = stream_map< ::uint32_t >(stream, hist, q_metric::MAX_Q_BINS);
                        map_resize(metric.m_qscoreHist, header.m_qscoreBins.size());
                        for(size_t i=0;i<header.m_qscoreBins.size();++i)
                        {
                            INTEROP_ASSERTMSG((header.m_qscoreBins[i].value() > 0), header.m_qscoreBins[i].value());
                            INTEROP_ASSERTMSG((header.m_qscoreBins[i].value() - 1) < q_metric::MAX_Q_BINS, header.m_qscoreBins[i].value()  - 1 << " < " << q_metric::MAX_Q_BINS);
                            INTEROP_ASSERTMSG(i< metric.m_qscoreHist.size(), metric.m_qscoreHist.size());
                            metric.m_qscoreHist[i] = hist[header.m_qscoreBins[i].value()-1];
                        }
                        return count;
                    }
                    return stream_map< ::uint32_t >(stream, metric.m_qscoreHist, q_metric::MAX_Q_BINS);
                }
                /** Write metric to the output stream
                 *
                 * @param stream output stream
                 * @param metric source metric
                 * @param header metric header
                 * @return number of bytes read or total number of bytes written
                 */
                template<class Metric, class Header>
                static std::streamsize map_stream(std::ostream& stream, Metric& metric, Header& header, const bool)
                {
                    if(header.m_qscoreBins.size()>0)
                    {
                        ::uint32_t hist[q_metric::MAX_Q_BINS];
                        std::fill(hist, hist+q_metric::MAX_Q_BINS, 0);
                        for(size_t i=0;i<header.m_qscoreBins.size();i++)
                        {
                            INTEROP_ASSERT(header.m_qscoreBins[i].value() > 0);
                            INTEROP_ASSERTMSG((header.m_qscoreBins[i].value() - 1) < q_metric::MAX_Q_BINS, header.m_qscoreBins[i].value()  - 1 << " < " << q_metric::MAX_Q_BINS);
                            hist[header.m_qscoreBins[i].value()-1] = metric.m_qscoreHist[i];
                        }
                        return stream_map< ::uint32_t >(stream, hist, q_metric::MAX_Q_BINS);
                    }
                    return stream_map< ::uint32_t >(stream, metric.m_qscoreHist, q_metric::MAX_Q_BINS);
                }
                /** Compute the layout size
                 *
                 * @return size of the record
                 */
                static record_size_t computeSize(const q_metric::header_type&)
                {
                    return static_cast<record_size_t>(sizeof(metric_id_t)+sizeof(::uint32_t)*q_metric::MAX_Q_BINS);
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
                    ::uint8_t has_bins = header.binCount()>0;
                    std::streamsize count = 0;
                    count += stream_map< ::uint8_t >(stream, has_bins);
                    if(stream.fail()) return count;
                    if(!has_bins) return count;
                    count+=stream_map< ::uint8_t >(stream, header.m_bin_count);
                    if(stream.fail()) return count;
                    const ::uint8_t bin_count = static_cast< ::uint8_t >(header.m_bin_count);
                    assert(bin_count>0);
                    ::uint8_t tmp[q_metric::MAX_Q_BINS];
                    map_resize(header.m_qscoreBins, bin_count);

                    copy_lower_write(header.m_qscoreBins, tmp);
                    count+=stream_map< ::uint8_t >(stream, tmp, bin_count);
                    copy_lower_read(header.m_qscoreBins, tmp);

                    copy_upper_write(header.m_qscoreBins, tmp);
                    count+=stream_map< ::uint8_t >(stream, tmp, bin_count);
                    copy_upper_read(header.m_qscoreBins, tmp);

                    copy_value_write(header.m_qscoreBins, tmp);
                    count+=stream_map< ::uint8_t >(stream, tmp, bin_count);
                    copy_value_read(header.m_qscoreBins, tmp);

                    return count;
                }
            private:
                template<size_t N>
                static void copy_lower_read(std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) bins[i].m_lower = tmp[i];
                }
                static void copy_lower_read(const std::vector<q_score_bin>&, ::uint8_t*) { }
                template<size_t N>
                static void copy_lower_write(const std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) tmp[i] = static_cast< ::uint8_t >(bins[i].m_lower);
                }
                static void copy_lower_write(std::vector<q_score_bin>&, ::uint8_t*) { }
            private:
                template<size_t N>
                static void copy_upper_read(std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) bins[i].m_upper = tmp[i];
                }
                static void copy_upper_read(const std::vector<q_score_bin>& , ::uint8_t*) { }
                template<size_t N>
                static void copy_upper_write(const std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) tmp[i] = static_cast< ::uint8_t >(bins[i].m_upper);
                }
                static void copy_upper_write(std::vector<q_score_bin>&, ::uint8_t*) { }
            private:
                template<size_t N>
                static void copy_value_read(std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) bins[i].m_value = tmp[i];
                }
                static void copy_value_read(const std::vector<q_score_bin>&, ::uint8_t*) { }
                template<size_t N>
                static void copy_value_write(const std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) tmp[i] = static_cast< ::uint8_t >(bins[i].m_value);
                }
                static void copy_value_write(std::vector<q_score_bin>&, ::uint8_t*) { }
            };
            /** Q-score Metric Record Layout Version 6
             *
             * This class provides an interface to reading the registration metric file:
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
                 *          byte 0: version number
                 *          byte 1: record size
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
                typedef layout::base_cycle_metric metric_id_t;

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
                static std::streamsize map_stream(Stream& stream, Metric& metric, Header& header, const bool)
                {
                    return stream_map< ::uint32_t >(stream, metric.m_qscoreHist, header.binCount());
                }
                /** Compute the layout size
                 *
                 * @return size of the record
                 */
                static record_size_t computeSize(const q_metric::header_type& header)
                {
                    return static_cast<record_size_t>(sizeof(metric_id_t)+sizeof(::uint32_t)*header.binCount());
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
                    ::uint8_t has_bins = header.binCount()>0;
                    std::streamsize count = 0;
                    count += stream_map< ::uint8_t >(stream, has_bins);
                    if(stream.fail()) return count;
                    if(!has_bins) return count;
                    count+=stream_map< ::uint8_t >(stream, header.m_bin_count);
                    if(stream.fail()) return count;
                    const ::uint8_t bin_count = static_cast< ::uint8_t >(header.m_bin_count);
                    assert(bin_count>0);
                    ::uint8_t tmp[q_metric::MAX_Q_BINS];
                    map_resize(header.m_qscoreBins, bin_count);

                    copy_lower_write(header.m_qscoreBins, tmp);
                    count+=stream_map< ::uint8_t >(stream, tmp, bin_count);
                    copy_lower_read(header.m_qscoreBins, tmp);

                    copy_upper_write(header.m_qscoreBins, tmp);
                    count+=stream_map< ::uint8_t >(stream, tmp, bin_count);
                    copy_upper_read(header.m_qscoreBins, tmp);

                    copy_value_write(header.m_qscoreBins, tmp);
                    count+=stream_map< ::uint8_t >(stream, tmp, bin_count);
                    copy_value_read(header.m_qscoreBins, tmp);

                    return count;
                }
            private:
                template<size_t N>
                static void copy_lower_read(std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) bins[i].m_lower = tmp[i];
                }
                static void copy_lower_read(const std::vector<q_score_bin>&, ::uint8_t*) { }
                template<size_t N>
                static void copy_lower_write(const std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) tmp[i] = static_cast< ::uint8_t >(bins[i].m_lower);
                }
                static void copy_lower_write(std::vector<q_score_bin>&, ::uint8_t*) { }
            private:
                template<size_t N>
                static void copy_upper_read(std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) bins[i].m_upper = tmp[i];
                }
                static void copy_upper_read(const std::vector<q_score_bin>&, ::uint8_t*) { }
                template<size_t N>
                static void copy_upper_write(const std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) tmp[i] = static_cast< ::uint8_t >(bins[i].m_upper);
                }
                static void copy_upper_write(std::vector<q_score_bin>&, ::uint8_t*) { }
            private:
                template<size_t N>
                static void copy_value_read(std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) bins[i].m_value = tmp[i];
                }
                static void copy_value_read(const std::vector<q_score_bin>&, ::uint8_t*) { }
                template<size_t N>
                static void copy_value_write(const std::vector<q_score_bin>& bins, ::uint8_t (&tmp)[N])
                {
                    assert(bins.size() <= N);
                    for(::uint8_t i=0;i<bins.size();++i) tmp[i] = static_cast< ::uint8_t >(bins[i].m_value);
                }
                static void copy_value_write(std::vector<q_score_bin>&, ::uint8_t*) { }
            };

#pragma pack()// DO NOT MOVE
        }}}

INTEROP_FORCE_LINK_DEF(q_metric)
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_metric, 4 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_metric, 5 )
INTEROP_REGISTER_METRIC_GENERIC_LAYOUT(q_metric, 6 )
