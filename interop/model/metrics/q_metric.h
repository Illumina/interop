/** Q-score metric
 *
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/QMetrics.bin
 *  - InterOp/QMetricsOut.bin
 *
 *  @file
 *
 *  @date 8/21/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <cstring>
#include <numeric>
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/io/format/generic_layout.h"

namespace illumina {
    namespace interop {
        namespace model {
            namespace metrics {
                /** Bin in the q-score histogram
                 */
                class q_score_bin
                {
                public:
                    /** Integer type of the bin */
                    typedef ::uint16_t bin_type;
                public:
                    /** Constructor
                     *
                     * @param lower lower end of the bin range
                     * @param upper upper end of the bin range
                     * @param value value of the bin
                     */
                    q_score_bin(const bin_type lower=0, const bin_type upper=0, const bin_type value=0) :
                            m_lower(lower),
                            m_upper(upper),
                            m_value(value){}
                public:
                    /** Assign type val to point2d
                     *
                     * @param val type that defines m_x and m_y
                     * @return point2d
                     */
                    template<class T>
                    q_score_bin& operator=(const T& val)
                    {
                        m_lower = val.m_lower;
                        m_upper = val.m_upper;
                        m_value = val.m_value;
                        return *this;
                    }

                public:
                    /** @defgroup q_score_bin Quality Bin
                     *
                     * Bin information for a q-score
                     *
                     * @ref illumina::interop::model::metrics::q_score_bin "See full class description"
                     * @ingroup q_score_header
                     * @{
                     */
                    /** Lower end of the bin
                     *
                     * @return lower end of the bin
                     */
                    bin_type lower()const{return m_lower;}
                    /** Upper end of the bin
                     *
                     * @return upper end of the bin
                     */
                    bin_type upper()const{return m_upper;}
                    /** Value of the bin
                     *
                     * @return value of the bin
                     */
                    bin_type value()const{return m_value;}
                    /** @} */

                private:
                    bin_type m_lower;
                    bin_type m_upper;
                    bin_type m_value;
                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
                /** Header information for writing an image metric set
                 */
                class q_score_header : public metric_base::base_cycle_metric::header_type
                {
                public:
                    enum{
                        /** Maximum number of q-score bins */
                        MAX_Q_BINS=50
                    };
                    /** Vector of q-scores type */
                    typedef std::vector<q_score_bin> qscore_bin_vector_type;
                    /** Q-score bin type */
                    typedef q_score_bin bin_t;
                public:
                    /** Constructor
                     */
                    q_score_header() : m_bin_count(0) {}
                    /** Constructor
                     *
                     * @param bins q-score bin vector
                     */
                    q_score_header(const qscore_bin_vector_type& bins) :
                            m_qscoreBins(bins), m_bin_count(bins.size()){}

                    /** @defgroup q_score_header Quality Metric Header
                     *
                     * Collection of q-score bins
                     *
                     * @ref illumina::interop::model::metrics::q_score_header "See full class description"
                     * @ingroup q_metric
                     * @{
                     */
                    /** Get a q-score bin
                     *
                     * @return q-score bin
                     */
                    const q_score_bin& binAt(const size_t n)const
                    {
                       INTEROP_ASSERT(n < binCount());
                        return m_qscoreBins[n];
                    }
                    /** Get the q-score bins
                     *
                     * @return vector of q-score bins
                     */
                    const qscore_bin_vector_type& bins()const
                    {
                        return m_qscoreBins;
                    }
                    /** Get the q-score bins
                     *
                     * @return vector of q-score bins
                     */
                    qscore_bin_vector_type& bins()
                    {
                        return m_qscoreBins;
                    }
                    /** Get the number of bins in header
                     *
                     * @return number of bins in header
                     */
                    size_t binCount()const{return m_qscoreBins.size();}
                    /** Get the number of bins for each metric
                     *
                     * If the number of header bins is 0, then this returns 50.
                     *
                     * @return number of bins for each metric
                     */
                    q_score_bin::bin_type max_q_value()const
                    {
                        return m_bin_count == static_cast<size_t>(MAX_Q_BINS) ||
                                       m_bin_count == 0 ? static_cast<q_score_bin::bin_type>(MAX_Q_BINS) : m_qscoreBins.back().upper();
                    }
                    /** Get the index for the given q-value
                     *
                     * @note Never call this function directly, use interop::logic::metric::index_for_q_value
                     * @param qval q-value
                     * @return index;
                     */
                    size_t index_for_q_value(const size_t qval)const
                    {
                        if(m_qscoreBins.size() == 0) return qval;
                        size_t index=0;
                        while(index < m_qscoreBins.size() && binAt(index).value() < qval) index++;
                        return index+1;
                    }
                    /** @} */
                    /** Generate a default header
                     *
                     * @return default header
                     */
                    static q_score_header default_header()
                    {
                        return q_score_header();
                    }
                    /** Get the index for the given q-value
                     *
                     * @param qval q-value
                     * @return index;
                     */
                    size_t index_for_q_value(size_t qval)const
                    {
                        if(m_qscoreBins.size() == 0) return qval;
                        size_t index=0;
                        while(binAt(index).value() < qval && index < m_qscoreBins.size()) index++;
                        return index+1;
                    }
                    /** Generate a default header
                     *
                     * @return default header
                     */
                    static q_score_header default_header()
                    {
                        return q_score_header();
                    }

                protected:
                    /** Q-score bins */
                    qscore_bin_vector_type m_qscoreBins;
                private:
                    size_t m_bin_count;
                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
                /** Q-score metric
                 *
                 * @note Supported versions: 4, 5 and 6
                 */
                class q_metric : public metric_base::base_cycle_metric
                {
                public:
                    enum{
                        /** Unique type code for metric */
                        TYPE=constants::Q,
                        /** Latest version of the InterOp format */
                        LATEST_VERSION=6
                    };
                public:
                    enum{
                        /** Maximum number of q-score bins */
                        MAX_Q_BINS=q_score_header::MAX_Q_BINS
                    };
                    /** Q-score metric header
                     */
                    typedef q_score_header header_type;
                    /** Vector of q-scores type */
                    typedef header_type::qscore_bin_vector_type qscore_bin_vector_type;
                    /** Defines a vector of unsigned ints
                     */
                    typedef std::vector<uint_t> uint_vector;
                    /** Define a uint pointer to a uint array
                     */
                    typedef ::uint32_t* uint_pointer_t;
                public:
                    /** Constructor
                     */
                    q_metric() :
                            metric_base::base_cycle_metric(0,0,0) {}
                    /** Constructor
                     *
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param qscoreHist q-score histogram
                     */
                    q_metric(const uint_t lane,
                             const uint_t tile,
                             const uint_t cycle,
                             const uint_vector& qscoreHist) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_qscoreHist(qscoreHist),
                            m_qscoreHist_cumulative(qscoreHist.size(), 0)
                    {
                    }
                    /** Constructor
                     *
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param qscoreHist q-score histogram
                     * @param count number of elements in the histogram
                     */
                    q_metric(const uint_t lane,
                             const uint_t tile,
                             const uint_t cycle,
                             const uint_pointer_t qscoreHist,
                             const uint_t count) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_qscoreHist(qscoreHist, qscoreHist+count),
                            m_qscoreHist_cumulative(count, 0)
                    {
                    }

                public:
                    /** @defgroup q_metric Quality Metrics
                     *
                     * Per tile per cycle quality metrics
                     *
                     * @ref illumina::interop::model::metrics::q_metric "See full class description"
                     *
                     * @note All metrics in this class are supported by all versions
                     * @ingroup run_metrics
                     * @{
                     */
                    /** Q-score value of the histogram
                     *
                     * @return q-score value of the histogram
                     */
                    uint_t qscoreHist(const size_t n)const
                    {
                       INTEROP_ASSERT(n<m_qscoreHist.size());
                        return m_qscoreHist[n];
                    }
                    /** Q-score histogram
                     *
                     * @return q-scorehistogram
                     */
                    const uint_vector& qscoreHist()const
                    {
                        return m_qscoreHist;
                    }
                    /** Number of bins in the q-score histogram
                     *
                     * @return number of bins in the q-score histogram
                     */
                    size_t size()const
                    {
                        return m_qscoreHist.size();
                    }
                    /** Sum the q-score histogram
                     *
                     * @return q-score histogram sum
                     */
                    uint_t sum_qscore()const
                    {
                        return std::accumulate(m_qscoreHist.begin(), m_qscoreHist.end(), 0);
                    }
                    /** Sum the cumulative q-score histogram
                     *
                     * @return cumulative q-score histogram sum
                     */
                    uint_t sum_qscore_cumulative()const
                    {
                        return std::accumulate(m_qscoreHist_cumulative.begin(), m_qscoreHist_cumulative.end(), 0);
                    }
                    /** Number of clusters over the given q-score
                     *
                     * This calculates over the local histogram. This function either requires the bins from the header
                     * or the index of the q-value for the first parameter. Note that the header is apart of the
                     * metric set (q_metrics).
                     *
                     * @snippet src/examples/example_q_metric.cpp Calculating Total >= Q30
                     *
                     * @sa q_score_header::bins()
                     * @param qscore percentage of clusters over the given q-score value
                     * @param bins q-score histogram bins
                     * @return total of clusters over the given q-score
                     */
                    uint_t total_over_qscore(const uint_t qscore, const qscore_bin_vector_type& bins=qscore_bin_vector_type())const
                    {
                        uint_t totalCount=0;
                        if(bins.size() == 0)
                        {
                           INTEROP_ASSERT(qscore > 0);
                            totalCount = std::accumulate(m_qscoreHist.begin()+qscore-1, m_qscoreHist.end(), 0);
                        }
                        else
                        {
                            for(size_t i=0;i<bins.size();i++)
                            {
                                if( bins[i].value() > qscore)
                                    totalCount += m_qscoreHist[i];
                            }
                        }
                        return totalCount;
                    }
                    /** Number of clusters over the given q-score
                     *
                     * This calculates over the cumlative histogram. This function either requires the bins from the
                     * header or the index of the q-value for the first parameter. Note that the header is apart of the
                     * metric set (q_metrics).
                     *
                     * @sa q_score_header::bins()
                     * @param qscore percentage of clusters over the given q-score value
                     * @param bins q-score histogram bins
                     * @return total of clusters over the given q-score
                     */
                    uint_t total_over_qscore_cumulative(const uint_t qscore,
                                                        const qscore_bin_vector_type &bins = qscore_bin_vector_type())const
                    {
                       INTEROP_ASSERT(m_qscoreHist_cumulative.size() > 0);
                        uint_t totalCount=0;
                        if(bins.size() == 0)
                        {
                           INTEROP_ASSERT(qscore > 0);
                            totalCount = std::accumulate(m_qscoreHist_cumulative.begin() + qscore - 1, m_qscoreHist_cumulative.end(), 0);
                        }
                        else
                        {
                            for(size_t i=0;i<bins.size();i++)
                            {
                                if( bins[i].value() > qscore)
                                    totalCount += m_qscoreHist_cumulative[i];
                            }
                        }
                        return totalCount;
                    }
                    /** Percent of clusters over the given q-score
                     *
                     * This calculates over the local histogram. This function either requires the bins from the header
                     * or the index of the q-value for the first parameter. Note that the header is apart of the
                     * metric set (q_metrics).
                     *
                     * @snippet src/examples/example_q_metric.cpp Calculating Percent >= Q30
                     *
                     * @sa q_score_header::bins()
                     * @param qscore percentage of clusters over the given q-score value
                     * @param bins q-score histogram bins
                     * @return percent of cluster over the given q-score
                     */
                    float percent_over_qscore(const uint_t qscore,
                                              const qscore_bin_vector_type& bins=qscore_bin_vector_type())const
                    {
                        const float total = static_cast<float>(sum_qscore());
                        if(total == 0.0f) return std::numeric_limits<float>::quiet_NaN();
                        uint_t totalCount=total_over_qscore(qscore, bins);
                        return 100 * totalCount / total;
                    }
                    /** Percent of clusters over the given q-score
                     *
                     * This calculates over the cumlative histogram. This function either requires the bins from the
                     * header or the index of the q-value for the first parameter. Note that the header is apart of the
                     * metric set (q_metrics).
                     *
                     * @sa q_score_header::bins()
                     * @param qscore percentage of clusters over the given q-score value
                     * @param bins q-score histogram bins
                     * @return percent of cluster over the given q-score
                     */
                    float percent_over_qscore_cumulative(const uint_t qscore,
                                                         const qscore_bin_vector_type &bins =
                                                         qscore_bin_vector_type())const
                    {
                       INTEROP_ASSERT(m_qscoreHist_cumulative.size() > 0);
                        const float total = static_cast<float>(sum_qscore_cumulative());
                        if(total == 0.0f) return std::numeric_limits<float>::quiet_NaN();
                        uint_t totalCount= total_over_qscore_cumulative(qscore, bins);
                        return 100 * totalCount / total;
                    }
                    /** Get the median q-score
                     *
                     * If the median cannot be found, return the maximum integer. This function either requires the bins
                     * from the  header or the index of the q-value for the first parameter. Note that the header is
                     * apart of the metric set (q_metrics).
                     *
                     * @sa q_score_header::bins()
                     * @return median q-score
                     */
                    uint_t median(const qscore_bin_vector_type& bins=qscore_bin_vector_type())const
                    {
                        uint_t total = sum_qscore();
                        uint_t position = total % 2 == 0 ? total / 2 + 1 : (total + 1) / 2;
                        uint_t i=0;
                        uint_t sum = 0;
                        for(;i<m_qscoreHist.size();i++)
                        {
                            sum += m_qscoreHist[i];
                            if(sum >= position)break;
                        }
                        if(bins.size()==0) return i+1;
                        if(i<bins.size()) return bins[i].value();
                        return std::numeric_limits<uint_t>::max();
                    }
                    /** Check if the cumulative histogram has not been populated
                     *
                     * @return true if cumulative histogram has not been populated
                     */
                    bool is_cumulative_empty()const
                    {
                        return m_qscoreHist_cumulative.empty();
                    }
                    /** @} */
                    /** Accumulate q-score histogram from last cycle
                     *
                     * This helper function also allocates memory for the cumulative q-score histogram.
                     *
                     * @param metric last cycle q-metric
                     */
                    void accumulate(const q_metric& metric)
                    {
                        uint_vector::const_iterator beg = metric.m_qscoreHist_cumulative.begin(),
                                end = metric.m_qscoreHist_cumulative.end();
                        m_qscoreHist_cumulative = m_qscoreHist;
                        if(&metric != this) {
                            for (uint_vector::iterator cur = m_qscoreHist_cumulative.begin(); beg != end; ++beg, ++cur)
                                *cur += *beg;
                        }
                    }

                public:
                    /** Get the prefix of the InterOp filename
                     *
                     * @return prefix
                     */
                    static const char* prefix(){return "Q";}

                private:
                    uint_vector m_qscoreHist;
                    uint_vector m_qscoreHist_cumulative;

                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
            }
        }
    }
}

