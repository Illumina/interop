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
                m_percent_aligned(0), // TODO: Update logic to allow this to be NaN!
                m_first_cycle_intensity(std::numeric_limits<float>::quiet_NaN()),
                m_percent_gt_q30(std::numeric_limits<float>::quiet_NaN()),
                m_yield_g(0), // TODO: Update logic to allow this to be NaN!
                m_projected_yield_g(0) // TODO: Update logic to allow this to be NaN!

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
        /** Get the error rate of PHIX for the run
         *
         * @return error rate of PHIX for run
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
        /** Get the percent of bases greater than Q30
         *
         * @return percent of bases greater than Q30
         */
        float percent_gt_q30()const
        {
            return m_percent_gt_q30;
        }
        /** Get the yield of the run in gigabases
         *
         * @return yield of the run in gigabases
         */
        float yield_g()const
        {
            return m_yield_g;
        }

        /** Get the projected yield of teh run in gigabases
         *
         * @return projected yield of teh run in gigabases
         */
        float projected_yield_g()const
        {
            return m_projected_yield_g;
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
        /** Set the percent of bases greater than Q30
         *
         * @param val percent of bases greater than Q30
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
        /** Set the projected yield of teh run in gigabases
         *
         * @param val projected yield of teh run in gigabases
         */
        void projected_yield_g(const float val)
        {
            m_projected_yield_g = val;
        }

        /** Resize the underlying data
         */
        void resize(const size_t)
        {
        }

    private:
        float m_error_rate;
        float m_percent_aligned;
        float m_first_cycle_intensity;
        float m_percent_gt_q30;
        float m_yield_g;
        float m_projected_yield_g;
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };



}}}}

