/** Description of a read
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <algorithm>
#include "interop/model/run/cycle_range.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace run
{

    /** Defines a range over cycles
     *
     */
    class read_info : public cycle_range
    {
    public:
        /** Define integral read number type */
        typedef size_t number_t;
    public:
        /** Constructor
         *
         * @param number read number identifier
         * @param first_cycle index of first cycle
         * @param last_cycle index of last cycle
         * @param is_index true if the read is an index read
         */
        read_info(const number_t number = 0,
                  const cycle_t first_cycle = 0,
                  const cycle_t last_cycle = 0,
                  const bool is_index = false) : cycle_range(first_cycle, last_cycle), m_number(number),
                                                 m_is_index(is_index)
        {
        }

    public:
        /** @defgroup read_info Read Information
         *
         * Information describing the read
         *
         * @ingroup run_info
         * @ref illumina::interop::model::run::read_info "See full class description"
         * @{
         */
        /** Get the number to identify the read
         *
         * @return  number to identify the read
         */
        number_t number() const
        {
            return m_number;
        }

        /** Check if read is index
         *
         * @return true if read is index
         */
        bool is_index() const
        {
            return m_is_index;
        }

        /** Get the total number of cycles in the read
         *
         * @return total number of cycles in read
         */
        cycle_t total_cycles() const
        {
            return (m_last_cycle >= m_first_cycle) ? m_last_cycle - m_first_cycle + 1 : 0;
        }

        /** Get the usable number of cycles in the read
         *
         * The last cycle is not fully corrected so it is not usable
         *
         * @return number of usable cycles in read
         */
        cycle_t useable_cycles() const
        {
            return (m_last_cycle >= m_first_cycle) ? m_last_cycle - m_first_cycle : 0;
        }
        /** @} */

    private:
        size_t m_number;
        bool m_is_index;

        friend class info;

        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };

}}}}

