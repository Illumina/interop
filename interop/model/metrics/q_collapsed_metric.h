/** Collapsed Q-score metric: Q20, Q30
 *
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/QMetrics2030.bin
 *  - InterOp/QMetrics2030Out.bin
 *
 *  @file
 *  @date 5/17/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/constants/enums.h"
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/io/format/generic_layout.h"
#include "interop/model/metrics/q_metric.h"

namespace illumina { namespace interop { namespace model { namespace metrics {

    /** Header for the collapsed Q-metric
     */
    class q_collapsed_header : public q_score_header
    {
    public:
        /** Constructor
         */
        q_collapsed_header() : m_record_size(0) {}
        /** Constructor
         *
         * @param bins q-score bin vector
         */
        q_collapsed_header(const qscore_bin_vector_type& bins) : q_score_header(bins), m_record_size(0){}
        /** Generate a default header
         *
         * @return default header
         */
        static q_collapsed_header default_header()
        {
            return q_collapsed_header();
        }
        /** Clear the data
         */
        void clear()
        {
            m_record_size=0;
            q_score_header::clear();
        }
    private:
        ::uint32_t m_record_size; // Hack to support this format
    private:
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };
    /** Pre-calculated Q20 and Q30 counts for each tile/cycle
     */
    class q_collapsed_metric : public metric_base::base_cycle_metric
    {
    public:
        enum{
            /** Unique type code for metric */
            TYPE=constants::QCollapsed,
            /** Latest version of the InterOp format */
            LATEST_VERSION=6
        };
        /** Define a unsigned long type */
        typedef ::uint64_t ulong_t;
        /** Q-score metric header */
        typedef q_collapsed_header header_type;
    public:
        /** Constructor
         */
        q_collapsed_metric() :
                metric_base::base_cycle_metric(0,0,0),
                m_q20(0),
                m_q30(0),
                m_total(0),
                m_median_qscore(0),
                m_cumulative_q20(0),
                m_cumulative_q30(0),
                m_cumulative_total(0) {}
        /** Constructor
         */
        q_collapsed_metric(const header_type&) :
                metric_base::base_cycle_metric(0,0,0),
                m_q20(0),
                m_q30(0),
                m_total(0),
                m_median_qscore(0),
                m_cumulative_q20(0),
                m_cumulative_q30(0),
                m_cumulative_total(0) {}
        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param q20 Q20 cluster count
         * @param q30 Q30 cluster count
         * @param total total clusters
         * @param median_qscore median q-score
         */
        q_collapsed_metric(const uint_t lane,
                           const uint_t tile,
                           const uint_t cycle,
                           const uint_t q20,
                           const uint_t q30,
                           const uint_t total,
                           const uint_t median_qscore) :
                metric_base::base_cycle_metric(lane, tile, cycle),
                m_q20(q20),
                m_q30(q30),
                m_total(total),
                m_median_qscore(median_qscore),
                m_cumulative_q20(0),
                m_cumulative_q30(0),
                m_cumulative_total(0)

        {
        }

    public:
        /** @defgroup q_collapsed_metric Collapsed Q metrics
         *
         * Summary of q-metrics for each cycle
         *
         * @ref illumina::interop::model::metrics::q_collapsed_metric "See full class description"
         *
         * @ingroup run_metrics
         * @{
         */
        /** Number of Q20 clusters
         *
         * @return number of q20 clusters
         */
        uint_t q20()const
        {
            return m_q20;
        }
        /** Number of Q30 clusters
         *
         * @return number of q30 clusters
         */
        uint_t q30()const
        {
            return m_q30;
        }
        /** Total number of q-scored clusters
         *
         * @return total number of q-scored clusters
         */
        uint_t total()const
        {
            return m_total;
        }
        /** Median q-score for tile
         *
         * @return median q-score
         */
        uint_t median_qscore()const
        {
            return m_median_qscore;
        }
        /** Cumulative number of Q20 clusters over all cycles up to the current cycle
         *
         * @return number of q20 clusters cumulative over cycles
         */
        ulong_t cumulative_q20()const
        {
            return m_cumulative_q20;
        }
        /** Cumulative number of Q30 clusters over all cycles up to the current cycle
         *
         * @return number of q30 clusters  cumulative over cycles
         */
        ulong_t cumulative_q30()const
        {
            return m_cumulative_q30;
        }
        /** Cumulative total clusters over all cycles up to the current cycle
         *
         * @return total clusters cumulative over cycles
         */
        ulong_t cumulative_total()const
        {
            return m_cumulative_total;
        }
        /** Percent >= Q20
         *
         * @return % >= Q20
         */
        float percent_over_q20()const
        {
            return 100.0f * m_q20 / m_total;
        }
        /** Percent >= Q30
         *
         * @return % >= Q30
         */
        float percent_over_q30()const
        {
            return 100.0f * m_q30 / m_total;
        }
        /** Percent >= Q20 accumulated up to the current cycle
         *
         * @return % >= Q20 accumulated up to the current cycle
         */
        float cumulative_percent_over_q20()const
        {
            return 100.0f * m_cumulative_q20 / m_cumulative_total;
        }
        /** Percent >= Q30 accumulated up to the current cycle
         *
         * @return % >= Q30 accumulated up to the current cycle
         */
        float cumulative_percent_over_q30()const
        {
            return 100.0f * m_cumulative_q30 / m_cumulative_total;
        }
        /** @} */
        /** Accumulate q-score histogram from last cycle
         *
         * This helper function also allocates memory for the cumulative q-score histogram.
         *
         * @param metric last cycle q-metric
         */
        void accumulate(const q_collapsed_metric& metric)
        {
            m_cumulative_q20 = m_q20;
            m_cumulative_q30 = m_q30;
            m_cumulative_total = m_total;
            if(&metric != this) {
                m_cumulative_q20 += metric.m_cumulative_q20;
                m_cumulative_q30 += metric.m_cumulative_q30;
                m_cumulative_total += metric.m_cumulative_total;
            }
        }

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return prefix
         */
        static const char* prefix(){return "Q";}
        /** Get the suffix of the InterOp filename
         *
         * @return suffix
         */
        static const char* suffix(){return "2030";}

    private:
        uint_t m_q20;
        uint_t m_q30;
        uint_t m_total;
        uint_t m_median_qscore;
        ulong_t m_cumulative_q20;
        ulong_t m_cumulative_q30;
        ulong_t m_cumulative_total;

    private:
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };


}}}}

