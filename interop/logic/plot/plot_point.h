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
        std::sort(beg, end);
        const float p25 = util::percentile_sorted<float>(beg, end, 25);
        const float p50 = util::percentile_sorted<float>(beg, end, 50);
        const float p75 = util::percentile_sorted<float>(beg, end, 75);
        const float lower = util::lower_spread(beg, end, p25, p75);
        const float upper = util::upper_spread(beg, end, p25, p75);
        util::outliers_lower(beg, end, lower, std::back_inserter(outliers));
        util::outliers_upper(beg, end, upper, std::back_inserter(outliers));
        point = model::plot::candle_stick_point(x, p25, p50, p75, lower, upper, outliers);
        outliers.clear();
    }


}}}}
