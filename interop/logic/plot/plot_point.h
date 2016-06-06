/** Plot a point in a series
 *
 *  @file
 *  @date 5/1/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <algorithm>
#include <iterator>
#include "interop/util/statistics.h"
#include "interop/model/plot/candle_stick_point.h"

namespace illumina { namespace interop { namespace logic { namespace plot {
    namespace detail
    {
        /** Test if v1 < v2 unless v2 > spread
         */
        class islesser_with_upper_limit
        {
        public:
            /** Constructor
             *
             * @param spread upper limit
             */
            islesser_with_upper_limit(const float spread) : m_spread(spread){}
            /** Test if v1 < v2 unless v2 > spread
             *
             * @param v1 value 1
             * @param v2 value 2
             * @return true if v1 < v2 and v2 < m_spread
             */
            bool operator()(const float v1, const float v2)const
            {
                if(v2 < m_spread) return v1 < v2;
                return v1 > v2;
            }
        private:
            float m_spread;
        };
        /** Test if v1 < v2 unless v1 < spread
         */
        struct islesser_with_lower_limit
        {
        public:
            /** Constructor
             *
             * @param spread lower limit
             */
            islesser_with_lower_limit(const float spread) : m_spread(spread){}
            /** Test if v1 < v2 unless v1 < spread
             *
             * @param v1 value 1
             * @param v2 value 2
             * @return true if v1 < v2 unless v1 < spread
             */
            bool operator()(const float v1, const float v2)const
            {
                if(v1 > m_spread) return v1 < v2;
                return v1 > v2;
            }
        private:
            float m_spread;
        };
    }

    /** Logic for creating a candle stick point
     *
     * @param point candle stick point
     * @param beg iterator to start of collection of values
     * @param end iterator to end of collection of values
     * @param x x-coordinate
     * @param outliers reusable memory for collecting outliers
     */
    template<typename I>
    void plot_candle_stick(model::plot::candle_stick_point& point, I beg, I end, const float x, std::vector<float>& outliers)
    {
        std::sort(beg, end);
        const float p25 = util::percentile_sorted<float>(beg, end, 25);
        const float p50 = util::percentile_sorted<float>(beg, end, 50);
        const float p75 = util::percentile_sorted<float>(beg, end, 75);

        // Really just some arbitrary criteria derived by empirical observation in the 70s.
        const float tukey_constant = 1.5f;
        const float iqr = p75-p25;
        const float lower = p25 - tukey_constant * iqr;
        const float upper = p75 + tukey_constant * iqr;
        if(outliers.capacity()>0)
        {
            util::outliers_lower(beg, end, lower, std::back_inserter(outliers));
            util::outliers_upper(beg, end, upper, std::back_inserter(outliers));
        }
        size_t count = static_cast<size_t>(std::distance(beg,end));
        const float max_val = *std::max_element(beg, end, detail::islesser_with_upper_limit(upper));
        const float min_val = *std::min_element(beg, end, detail::islesser_with_lower_limit(lower));
        point = model::plot::candle_stick_point(x, p25, p50, p75, min_val, max_val, count, outliers);
        outliers.clear();
    }


}}}}
