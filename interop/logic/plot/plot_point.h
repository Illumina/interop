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
        const float eps = 1e-7f;
        const float NaN = std::numeric_limits<float>::quiet_NaN();
        INTEROP_ASSERT(beg != end);
        //std::sort(beg, end);
        std::stable_sort(beg, end);
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

        I upper_it = std::lower_bound(beg, end, upper);// Not less
        I lower_it = std::lower_bound(beg, end, lower-(eps*lower));



        const float max_val = (upper_it != beg) ?
                              ((upper_it == end || *upper_it > upper) ? *(upper_it-1) : *upper_it)
                                                : // TODO: should be > not >=
                              ((upper_it != end) ? *upper_it : NaN);

        const float min_val = (lower_it != end) ? *lower_it : NaN;
        point = model::plot::candle_stick_point(x, p25, p50, p75, min_val, max_val, count, outliers);
        outliers.clear();
    }


}}}}

