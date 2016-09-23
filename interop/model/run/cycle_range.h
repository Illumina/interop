/** Cycle range
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <limits>
#include "interop/util/assert.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace run
{

    /** Defines a range over cycles
     *
     */
    class cycle_range
    {
    public:
        /** Define integral cycle type */
        typedef size_t cycle_t;
    public:
        /** Constructor
         *
         * @param first_cycle index of first cycle
         * @param last_cycle index of last cycle
         */
        cycle_range(const cycle_t first_cycle = std::numeric_limits<cycle_t>::max(), const cycle_t last_cycle = 0) :
                m_first_cycle(first_cycle),
                m_last_cycle(last_cycle)
        {
        }

    public:
        /** @defgroup cycle_range Cycle range
         *
         * Information describing the cycle rnage
         *
         * @ingroup read_info
         * @ref illumina::interop::model::run::cycle_range "See full class description"
         * @{
         */
        /** Get the index of the first cycle
         *
         * @return index of first cycle
         */
        cycle_t first_cycle() const
        {
            if (empty()) return 0;
            return m_first_cycle;
        }

        /** Get the index of the last cycle
         *
         * @return index of last cycle
         */
        cycle_t last_cycle() const
        {
            return m_last_cycle;
        }

        /** Set the index of the first cycle
         *
         * @param val index of first cycle
         */
        void first_cycle(const cycle_t val)
        {
            m_first_cycle = val;
        }

        /** Set the index of the last cycle
         *
         * @param val index of last cycle
         */
        void last_cycle(const cycle_t val)
        {
            m_last_cycle = val;
        }
        /** Test if the cycle range is empty
         *
         * @return true if first cycle is sentinel
         */
        bool empty()const
        {
            return m_first_cycle == std::numeric_limits<cycle_t>::max();
        }
        /** @} */

    public:
        /** Update the cycle state
         *
         * If this is less than the first cycle, save as the first cycle.
         * If this is greater than the last cycle, save as the last cycle.
         *
         * @param cycle current cycle
         */
        void update(const cycle_t cycle)
        {
            if (cycle > m_last_cycle) m_last_cycle = cycle;
            if (cycle < m_first_cycle) m_first_cycle = cycle;
        }

        /** Update the cycle state
         *
         * @param rng cycle state
         */
        void update(const cycle_range &rng)
        {
            if (rng.m_last_cycle > m_last_cycle) m_last_cycle = rng.m_last_cycle;
            if (rng.m_first_cycle < m_first_cycle) m_first_cycle = rng.m_first_cycle;
        }

        /** Subtract a cycle range from the first cycle of the read safely.
         *
         * @note This function checks if the first_cycle_of_read is in later than the last cycle in the range, if so
         * the cycle range is returned as is (no subtraction)
         *
         * @param lhs cycle range
         * @param first_cycle_of_read first cycle of read (0-indexed)
         * @return new cycle range
         */
        friend cycle_range operator-(const cycle_range &lhs, const cycle_t &first_cycle_of_read)
        {
            if (lhs.m_last_cycle < first_cycle_of_read) return lhs;
            return cycle_range(lhs.m_first_cycle - first_cycle_of_read, lhs.m_last_cycle - first_cycle_of_read);
        }

    protected:
        /** First cycle index */
        cycle_t m_first_cycle;
        /** Last cycle index */
        cycle_t m_last_cycle;

        friend class info;
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

