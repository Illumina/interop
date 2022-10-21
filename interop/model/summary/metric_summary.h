/** Summary statistics by read, total and non-indexed
 *
 *  @file
 *  @date  3/14/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace summary {

    /**  Summary statistics by read, total and non-indexed
     *
     * @todo: use this is lane_summary too
     */
    class metric_summary
    {
    public:
        /** Constructor
         */
        metric_summary(const size_t /*channel_count*/) :
                m_error_rate(std::numeric_limits<float>::quiet_NaN()),
                m_percent_aligned(std::numeric_limits<float>::quiet_NaN()),
                m_first_cycle_intensity(std::numeric_limits<float>::quiet_NaN()),
                m_percent_gt_q30(std::numeric_limits<float>::quiet_NaN()),
                m_yield_g(std::numeric_limits<float>::quiet_NaN()),
                m_projected_yield_g(0),
                m_percent_occupied(std::numeric_limits<float>::quiet_NaN()),
                m_percent_occupancy_proxy(std::numeric_limits<float>::quiet_NaN())

        {}
    public:
        /** @defgroup metric_summary Metric summary
         *
         * Summary of key metrics
         *
         * @ingroup run_summary
         * @ref illumina::interop::model::summary::metric_summary "See full class description"
         * @{
         */
        /** Get the error rate of PHIX for the run over all "usable cycles".
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return error rate of PHIX for run over all "usable cycles".
         */
        float error_rate()const
        {
            return m_error_rate;
        }
        /** Get the percent aligned PHIX for the run
         *
         * @return percent aligned PHIX
         */
        float percent_aligned()const
        {
            return m_percent_aligned;
        }
        /** Get the first cycle intensity
         *
         * @return first cycle intensity
         */
        float first_cycle_intensity()const
        {
            return m_first_cycle_intensity;
        }
        /** Get the percent of bases greater than or equal to Q30 over all "usable cycles"
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return percent of bases greater than or equal to Q30 over all "usable cycles"
         */
        float percent_gt_q30()const
        {
            return m_percent_gt_q30;
        }
        /** Get the yield of the run in gigabases over all "usable cycles"
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return yield of the run in gigabases over all "usable cycles"
         */
        float yield_g()const
        {
            return m_yield_g;
        }

        /** Get the projected yield of the run in gigabases over all "usable cycles"
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return projected yield of the run in gigabases over all "usable cycles"
         */
        float projected_yield_g()const
        {
            return m_projected_yield_g;
        }
        /** Get the percent occupied
         *
         * @return percent occupied
         */
        float percent_occupied()const
        {
            return m_percent_occupied;
        }
        /** Get the percent occupancy proxy
         *
         * Also known as % loading concentration
         *
         * @return percent occupancy proxy
         */
        float percent_occupancy_proxy()const
        {
            return m_percent_occupancy_proxy;
        }
        /** Get the cluster count
         *
         * @return cluster count
         */
        double cluster_count()const
        {
            return m_cluster_count_raw;
        }

        /** Get the cluster count PF
         *
         * @return cluster count PF
         */
        double cluster_count_pf()const
        {
            return m_cluster_count_pf;
        }
        /** Get the reads PF value for run
         *
         * @return reads PF val for run
         */
        uint64_t reads()const
        {
            return m_reads_raw;
        }
        /** Get the reads PF value for run
         *
         * @return reads PF val for run
         */
        uint64_t reads_pf()const
        {
            return m_reads_pf;
        }
        /** @} */
        /** Set the first cycle intensity
         *
         * @param val first cycle intensity
         */
        void first_cycle_intensity(const float val)
        {
            m_first_cycle_intensity = val;
        }
        /** Set the error rate of PHIX for the run
         *
         * @param val error rate of PHIX for run
         */
        void error_rate(const float val)
        {
            m_error_rate = val;
        }
        /** Set the percent aligned PHIX for the run
         *
         *  @param val percent aligned PHIX
         */
        void percent_aligned(const float val)
        {
            m_percent_aligned = val;
        }
        /** Set the percent of bases greater than or equal to Q30
         *
         * @param val percent of bases greater than or equal to Q30
         */
        void percent_gt_q30(const float val)
        {
            m_percent_gt_q30 = val;
        }

        /** Set the yield of the run in gigabases
         *
         * @param val yield of the run in gigabases
         */
        void yield_g(const float val)
        {
            m_yield_g = val;
        }
        /** Set the projected yield of the run in gigabases
         *
         * @param val projected yield of the run in gigabases
         */
        void projected_yield_g(const float val)
        {
            m_projected_yield_g = val;
        }
        /** Set the percent occupied
         *
         * @param val percent occupied
         */
        void percent_occupied(const float val)
        {
            m_percent_occupied = val;
        }
        /** Set the percent occupancy proxy
         *
         * Also known as % loading concentration
         *
         * @param val percent occupancy proxy
         */
        void percent_occupancy_proxy(const float val)
        {
            m_percent_occupancy_proxy = val;
        }

        /** Resize the underlying data
         */
        void resize(const size_t)
        {
        }

        /** Set the cluster count of the run
         *
         * @param val cluster count of the run.
         */
        void cluster_count(const double val)
        {
            m_cluster_count_raw = val;
        }

        /** Set the cluster count PF of the run
         *
         * @param val cluster count PF of the run.
         */
        void cluster_count_pf(const double val)
        {
            m_cluster_count_pf = val;
        }

        /** Set the total reads for the run
         *
         * @param val total reads for run
         */
        void reads(const uint64_t val)
        {
            m_reads_raw = val;
        }

        /** Set the reads PF for the run
         *
         * @param val reads PF for run
         */
        void reads_pf(const uint64_t val)
        {
            m_reads_pf = val;
        }

    private:
        float m_error_rate;
        float m_percent_aligned;
        float m_first_cycle_intensity;
        float m_percent_gt_q30;
        float m_yield_g;
        float m_projected_yield_g;
        float m_percent_occupied;
        float m_percent_occupancy_proxy;
        uint64_t m_reads_raw;
        uint64_t m_reads_pf;
        double m_cluster_count_raw;
        double m_cluster_count_pf;
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };



}}}}

