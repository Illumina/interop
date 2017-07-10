/** Summary statistics compiled for sub derived class
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <cstddef>
#include <climits>
#include "interop/model/summary/metric_stat.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace summary
{

    /**  Summary statistics compiled by lane
     *
     * @note The reported standard deviation is always between tiles, not cycles. For metrics like error metrics,
     * the mean over all cycles is used for each tile, before calculating the standard deviation.
     *
     */
    class stat_summary
    {
    public:
        /** Collection of metric statistics type */
        typedef metric_stat metric_stat_t;
    public:
        /** Constructor
         *
         * param channel_count number of channels
         */
        stat_summary(const size_t /*channel_count*/) :
                m_percent_gt_q30(std::numeric_limits<float>::quiet_NaN()),
                m_yield_g(std::numeric_limits<float>::quiet_NaN()),
                m_projected_yield_g(0),
                m_reads(std::numeric_limits<float>::quiet_NaN()),
                m_reads_pf(std::numeric_limits<float>::quiet_NaN())

        {
        }

    public:
        /** @defgroup stat_summary Stat summary
         *
         * Summary of key metrics
         *
         * @ingroup read_summary
         * @ref illumina::interop::model::summary::lane_summary "See full class description"
         * @ref illumina::interop::model::summary::surface_summary "See full class description"
         * @{
         */
        /** Get the percent of bases greater than or equal to Q30 over all usable cycles
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return percent of bases greater than or equal to  Q30  over all usable cycles
         */
        float percent_gt_q30() const
        {
            return m_percent_gt_q30;
        }

        /** Get the yield of the run in gigabases over all usable cycles
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return yield of the run in gigabases  over all usable cycles
         */
        float yield_g() const
        {
            return m_yield_g;
        }

        /** Get the projected yield of the run in gigabases over all usable cycles
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return projected yield of the run in gigabases  over all usable cycles
         */
        float projected_yield_g() const
        {
            return m_projected_yield_g;
        }

        /** Get the total number of clusters in the lane
         *
         * @return total number of clusters in the lane
         */
        float reads() const
        {
            return m_reads;
        }

        /** Get the total number of clusters passing filter in the lane
         *
         * @return total number of clusters passing filter in the lane
         */
        float reads_pf() const
        {
            return m_reads_pf;
        }

    public:
        /** Get statistics summarizing the density of tiles in the lane (in clusters per mm2)
         *
         * @return statistics summarizing the density of tiles in the lane
         */
        const metric_stat_t &density() const
        {
            return m_density;
        }

        /** Get statistics summarizing the passing filter density of tiles in the lane (in clusters per mm2)
         *
         * @return statistics summarizing the passing filter density of tiles in the lane
         */
        const metric_stat_t &density_pf() const
        {
            return m_density_pf;
        }

        /** Get statistics summarizing the cluster count of tiles in the lane
         *
         * @return statistics summarizing the cluster count of tiles in the lane
         */
        const metric_stat_t &cluster_count() const
        {
            return m_cluster_count;
        }

        /** Get statistics summarizing the passing filter cluster count of tiles in the lane
         *
         * @return statistics summarizing the passing filter cluster count of tiles in the lane
         */
        const metric_stat_t &cluster_count_pf() const
        {
            return m_cluster_count_pf;
        }

        /** Get statistics summarizing the percent of clusters passing filter of tiles in the lane
         *
         * @return statistics summarizing the percent of clusters passing filter of tiles in the lane
         */
        const metric_stat_t &percent_pf() const
        {
            return m_percent_pf;
        }

        /** Get statistics summarizing the phasing of tiles in the lane
         *
         * @return statistics summarizing the phasing of tiles in the lane
         */
        const metric_stat_t &phasing() const
        {
            return m_phasing;
        }

        /** Get statistics summarizing the prephasing of tiles in the lane
         *
         * @return statistics summarizing the prephasing of tiles in the lane
         */
        const metric_stat_t &prephasing() const
        {
            return m_prephasing;
        }

        /** Get statistics summarizing the PhiX percent aligned of tiles in the lane
         *
         * @return statistics summarizing the PhiX percent aligned of tiles in the lane
         */
        const metric_stat_t &percent_aligned() const
        {
            return m_percent_aligned;
        }

        /** Get statistics summarizing the PhiX error rate of tiles in the lane over all usable cycles
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return statistics summarizing the PhiX error rate of tiles in the lane  over all usable cycles
         */
        const metric_stat_t &error_rate() const
        {
            return m_error_rate;
        }

        /** Get statistics summarizing the PhiX error rate over the first 35 usable cycles of tiles in the lane
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return statistics summarizing the PhiX error rate over the first 35 usable cycles of tiles in the lane
         */
        const metric_stat_t &error_rate_35() const
        {
            return m_error_rate_35;
        }

        /** Get statistics summarizing the PhiX error rate over the first 50 usable cycles of tiles in the lane
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return statistics summarizing the PhiX error rate over the first 50 usable cycles of tiles in the lane
         */
        const metric_stat_t &error_rate_50() const
        {
            return m_error_rate_50;
        }

        /** Get statistics summarizing the PhiX error rate over the first 75 usable cycles of tiles in the lane
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return statistics summarizing the PhiX error rate over the first 75 usable cycles of tiles in the lane
         */
        const metric_stat_t &error_rate_75() const
        {
            return m_error_rate_75;
        }

        /** Get statistics summarizing the PhiX error rate over the first 100 usable cycles  of tiles in the lane
         *
         * A "usable cycle" is a cycle that is fully corrected, so the last cycle of a read is excluded.
         *
         * @return statistics summarizing the PhiX error rate over the first 100 usable cycles  of tiles in the lane
         */
        const metric_stat_t &error_rate_100() const
        {
            return m_error_rate_100;
        }

        /** Get statistics summarizing the first cycle intensity of tiles in the lane
         *
         * @return statistics summarizing the first cycle intensity of tiles in the lane
         */
        const metric_stat_t &first_cycle_intensity() const
        {
            return m_first_cycle_intensity;
        }
        /** Get statistics summarizing the phasing slope
         *
         * @return statistics summarizing the slope of the best fit line of phasing weight vs cycle
         */
        const metric_stat_t &phasing_slope() const
        {
            return m_phasing_slope;
        }
        /** Get statistics summarizing the phasing offset
         *
         * @return statistics summarizing the offset of the best fit line of phasing weight vs cycle
         */
        const metric_stat_t &phasing_offset() const
        {
            return m_phasing_offset;
        }
        /** Get statistics summarizing the prephasing slope
         *
         * @return statistics summarizing the slope of the best fit line of prephasing weight vs cycle
         */
        const metric_stat_t &prephasing_slope() const
        {
            return m_prephasing_slope;
        }
        /** Get statistics summarizing the prephasing offset
         *
         * @return statistics summarizing the offset of the best fit line of prephasing weight vs cycle
         */
        const metric_stat_t &prephasing_offset() const
        {
            return m_prephasing_offset;
        }

        /** @} */

    public:
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
        /** Set the total number of clusters in the lane
         *
         * @param val total number of clusters in the lane
         */
        void reads(const float val)
        {
            m_reads = val;
        }
        /** Set the total number of clusters passing filter in the lane
         *
         * @param val total number of clusters passing filter in the lane
         */
        void reads_pf(const float val)
        {
            m_reads_pf = val;
        }
        /** Set statistics summarizing the density of tiles in the lane
         *
         * @param stat statistics summarizing the density of tiles in the lane
         */
        void density(const metric_stat_t& stat)
        {
            m_density = stat;
        }
        /** Set statistics summarizing the passing filter density of tiles in the lane
         *
         * @param stat statistics summarizing the passing filter density of tiles in the lane
         */
        void density_pf(const metric_stat_t& stat)
        {
            m_density_pf = stat;
        }
        /** Set statistics summarizing the cluster count of tiles in the lane
         *
         * @param stat statistics summarizing the cluster count of tiles in the lane
         */
        void cluster_count(const metric_stat_t& stat)
        {
            m_cluster_count = stat;
        }
        /** Set statistics summarizing the passing filter cluster count of tiles in the lane
         *
         * @param stat statistics summarizing the passing filter cluster count of tiles in the lane
         */
        void cluster_count_pf(const metric_stat_t& stat)
        {
            m_cluster_count_pf = stat;
        }
        /** Set statistics summarizing the percent of clusters passing filter of tiles in the lane
         *
         * @param stat statistics summarizing the percent of clusters passing filter of tiles in the lane
         */
        void percent_pf(const metric_stat_t& stat)
        {
            m_percent_pf = stat;
        }
        /** Set statistics summarizing the phasing of tiles in the lane
         *
         * @param stat statistics summarizing the phasing of tiles in the lane
         */
        void phasing(const metric_stat_t& stat)
        {
            m_phasing = stat;
        }
        /** Set statistics summarizing the prephasing of tiles in the lane
         *
         * @param stat statistics summarizing the prephasing of tiles in the lane
         */
        void prephasing(const metric_stat_t& stat)
        {
            m_prephasing = stat;
        }
        /** Set statistics summarizing the PhiX percent aligned of tiles in the lane
         *
         * @param stat statistics summarizing the PhiX percent aligned of tiles in the lane
         */
        void percent_aligned(const metric_stat_t& stat)
        {
            m_percent_aligned = stat;
        }
        /** Set statistics summarizing the PhiX error rate of tiles in the lane
         *
         * @param stat statistics summarizing the PhiX error rate of tiles in the lane
         */
        void error_rate(const metric_stat_t& stat)
        {
            m_error_rate = stat;
        }
        /** Set statistics summarizing the PhiX error rate over the first 35 cycles of tiles in the lane
         *
         * @param stat statistics summarizing the PhiX error rate over the first 35 cycles of tiles in the lane
         */
        void error_rate_35(const metric_stat_t& stat)
        {
            m_error_rate_35 = stat;
        }
        /** Set statistics summarizing the PhiX error rate over the first 50 cycles of tiles in the lane
         *
         * @param stat statistics summarizing the PhiX error rate over the first 50 cycles of tiles in the lane
         */
        void error_rate_50(const metric_stat_t& stat)
        {
            m_error_rate_50 = stat;
        }
        /** Set statistics summarizing the PhiX error rate over the first 75 cycles of tiles in the lane
         *
         * @param stat statistics summarizing the PhiX error rate over the first 75 cycles of tiles in the lane
         */
        void error_rate_75(const metric_stat_t& stat)
        {
            m_error_rate_75 = stat;
        }
        /** Set statistics summarizing the PhiX error rate over the first 100 usable cycles of tiles in the lane
         *
         * @param stat statistics summarizing the PhiX error rate over the first 100 usable cycles of tiles in the lane
         */
        void error_rate_100(const metric_stat_t& stat)
        {
            m_error_rate_100 = stat;
        }
        /** Set statistics summarizing the first cycle intensity of tiles in the lane
         *
         * @param stat statistics summarizing the first cycle intensity of tiles in the lane
         */
        void first_cycle_intensity(const metric_stat_t & stat)
        {
            m_first_cycle_intensity=stat;
        }
        /** Set statistics summarizing the phasing slope
         *
         * @param stat statistics summarizing the slope of the best fit line of phasing weight vs cycle
         */
        void phasing_slope(const metric_stat_t& stat)
        {
            m_phasing_slope = stat;
        }
        /** Set statistics summarizing the phasing offset
         *
         * @param stat statistics summarizing the offset of the best fit line of phasing weight vs cycle
         */
        void phasing_offset(const metric_stat_t& stat)
        {
            m_phasing_offset = stat;
        }
        /** Set statistics summarizing the prephasing slope
         *
         * @param stat statistics summarizing the slope of the best fit line of prephasing weight vs cycle
         */
        void prephasing_slope(const metric_stat_t& stat)
        {
            m_prephasing_slope = stat;
        }
        /** Set statistics summarizing the prephasing offset
         *
         * @param stat statistics summarizing the offset of the best fit line of prephasing weight vs cycle
         */
        void prephasing_offset(const metric_stat_t& stat)
        {
            m_prephasing_offset = stat;
        }

    public:
        /** Resize the underlying data
         */
        void resize_stat(const size_t)
        {
        }

    private:
        float m_percent_gt_q30;
        float m_yield_g;
        float m_projected_yield_g;
        float m_reads;
        float m_reads_pf;

    private:
        metric_stat_t m_density;
        metric_stat_t m_density_pf;
        metric_stat_t m_cluster_count;
        metric_stat_t m_cluster_count_pf;
        metric_stat_t m_percent_pf;
        metric_stat_t m_phasing;
        metric_stat_t m_prephasing;
        metric_stat_t m_percent_aligned;
        metric_stat_t m_error_rate;
        metric_stat_t m_error_rate_35;
        metric_stat_t m_error_rate_50;
        metric_stat_t m_error_rate_75;
        metric_stat_t m_error_rate_100;
        metric_stat_t m_first_cycle_intensity;
        metric_stat_t m_phasing_slope;
        metric_stat_t m_phasing_offset;
        metric_stat_t m_prephasing_slope;
        metric_stat_t m_prephasing_offset;
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

