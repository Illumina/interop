/** Summary of RTA states by cycle
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/model/run/cycle_range.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace summary
{

    /** Defines a range over cycles
     */
    class cycle_state_summary
    {
    public:
        /** Constructor
         *
         */
        cycle_state_summary()
        {
        }

    public:
        /** @defgroup cycle_state_summary Cycle state summary
         *
         * Current cycle state for the run
         *
         * @ingroup run_summary
         * @ref illumina::interop::model::summary::cycle_state_summary "See full class description"
         * @{
         */
        /** Get the extracted cycle range
         *
         * @return  extracted cycle range
         */
        const run::cycle_range &extracted_cycle_range() const
        {
            return m_extracted_cycle_range;
        }

        /** Get the base called cycle range
         *
         * @return base called cycle range
         */
        const run::cycle_range &called_cycle_range() const
        {
            return m_called_cycle_range;
        }

        /** Get the q-scored cycle range
         *
         * @return q-scored cycle range
         */
        const run::cycle_range &qscored_cycle_range() const
        {
            return m_qscored_cycle_range;
        }

        /** Get the aligned phix error rate cycle range
         *
         * @return aligned phix error rate cycle range
         */
        const run::cycle_range &error_cycle_range() const
        {
            return m_error_cycle_range;
        }
        /** Test if the cycle state is empty
         *
         * @return true if all cycle ranges are empty
         */
        bool empty()const
        {
            return m_extracted_cycle_range.empty() &&
                    m_called_cycle_range.empty() &&
                    m_qscored_cycle_range.empty() &&
                    m_error_cycle_range.empty();
        }
        /** @} */

    public:
        /** Set the extracted cycle range
         *
         * @param val  extracted cycle range
         */
        void extracted_cycle_range(const run::cycle_range &val)
        {
            m_extracted_cycle_range = val;
        }

        /** Set the base called cycle range
         *
         * @param val  base called cycle range
         */
        void called_cycle_range(const run::cycle_range &val)
        {
            m_called_cycle_range = val;
        }

        /** Set the q-scored cycle range
         *
         * @param val q-scored cycle range
         */
        void qscored_cycle_range(const run::cycle_range &val)
        {
            m_qscored_cycle_range = val;
        }

        /** Set the aligned phix error rate cycle range
         *
         * @param val aligned phix error rate cycle range
         */
        void error_cycle_range(const run::cycle_range &val)
        {
            m_error_cycle_range = val;
        }

    private:
        run::cycle_range m_extracted_cycle_range;
        run::cycle_range m_called_cycle_range;
        run::cycle_range m_qscored_cycle_range;
        run::cycle_range m_error_cycle_range;

        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

