/** Keep a running average of a metric
 *
 *  @file
 *  @date 1/4/17
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/math.h"

namespace illumina { namespace interop { namespace model { namespace summary
{
    /** Keep a running average of a metric */
    class metric_average // TODO: Use this everywhere to clean up summary logic!
    {
    public:
        /** Constructor */
        metric_average() : m_sum(0), m_count(0){}

    public:
        /** Add another value to the average
         *
         * @param val source value for average
         * @return reference to this object
         */
        metric_average& operator+=(const float val)
        {
            if(!std::isnan(val))
            {
                m_sum += val;
                m_count += 1;
            }
            return *this;
        }
        /** Compute current average
         *
         * @return current average
         */
        operator float()const
        {
            return average();
        }
        /** Compute current average
         *
         * @return current average
         */
        float average()const
        {
            if(m_count == 0) return std::numeric_limits<float>::quiet_NaN();
            return m_sum / static_cast<float>(m_count);
        }

    private:
        float m_sum;
        size_t m_count;
    };


}}}}

