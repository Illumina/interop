/** Plot a point in a series
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <limits>
#include "interop/model/plot/plot_data.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{

    /** Logic for auto scaling both axes of plot data
     *
     * @param data model for plot data
     * @param zero_min if true, set minimum to 0
     * @param scale_y_max factor to scale maximum Y value
     */
    template<typename Point>
    void auto_scale(model::plot::plot_data <Point> &data, const bool zero_min = true, const float scale_y_max = 1.1f)
    {
        typedef typename model::plot::plot_data<Point>::const_iterator const_series_iterator;
        typedef typename model::plot::series<Point>::const_iterator const_point_iterator;
        float ymin = std::numeric_limits<float>::max();
        float xmin = std::numeric_limits<float>::max();
        float ymax = -std::numeric_limits<float>::max();
        float xmax = -std::numeric_limits<float>::max();
        for (const_series_iterator cur_series = data.begin(), series_end = data.end();
             cur_series != series_end; ++cur_series)
        {
            for (const_point_iterator cur_pt = cur_series->begin(), end_pt = cur_series->end();
                 cur_pt != end_pt; ++cur_pt)
            {
                ymax = std::max(ymax, cur_pt->max_value());
                xmax = std::max(xmax, cur_pt->x());
                if (zero_min) continue;
                ymin = std::min(ymin, cur_pt->min_value());
                xmin = std::min(xmin, cur_pt->x());
            }
        }
        if (ymin == std::numeric_limits<float>::max()) ymin = 0;
        if (xmin == std::numeric_limits<float>::max()) xmin = 0;
        ymax = (ymax == -std::numeric_limits<float>::max()) ? 0 : scale_y_max * ymax + 0.0001f;
        if (xmax == -std::numeric_limits<float>::max()) xmax = 0;
        data.set_range(xmin, xmax, ymin, ymax);
    }

    /** Logic for auto scaling the x-axis of plot data
     *
     * @param data model for plot data
     * @param zero_min if true, set minimum to 0
     */
    template<typename Point>
    void auto_scale_x(model::plot::plot_data <Point> &data, const bool zero_min = true)
    {
        typedef typename model::plot::plot_data<Point>::const_iterator const_series_iterator;
        typedef typename model::plot::series<Point>::const_iterator const_point_iterator;
        float xmin = std::numeric_limits<float>::max();
        float xmax = -std::numeric_limits<float>::max();
        for (const_series_iterator cur_series = data.begin(), series_end = data.end();
             cur_series != series_end; ++cur_series)
        {
            for (const_point_iterator cur_pt = cur_series->begin(), end_pt = cur_series->end();
                 cur_pt != end_pt; ++cur_pt)
            {
                xmax = std::max(xmax, cur_pt->x());
                if (zero_min) continue;
                xmin = std::min(xmin, cur_pt->x());
            }
        }
        if (xmin == std::numeric_limits<float>::max()) xmin = 0;
        if (xmax == -std::numeric_limits<float>::max()) xmax = 0;
        data.set_xrange(xmin, xmax);
    }

    /** Logic for auto scaling the y-axis of plot data
     *
     * @param data model for plot data
     * @param zero_min if true, set minimum to 0
     * @param scale_y_max factor to scale maximum Y value
     */
    template<typename Point>
    void auto_scale_y(model::plot::plot_data <Point> &data, const bool zero_min = true, const float scale_y_max = 1.1f)
    {
        typedef typename model::plot::plot_data<Point>::const_iterator const_series_iterator;
        typedef typename model::plot::series<Point>::const_iterator const_point_iterator;
        float ymin = std::numeric_limits<float>::max();
        float ymax = -std::numeric_limits<float>::max();
        for (const_series_iterator cur_series = data.begin(), series_end = data.end();
             cur_series != series_end; ++cur_series)
        {
            for (const_point_iterator cur_pt = cur_series->begin(), end_pt = cur_series->end();
                 cur_pt != end_pt; ++cur_pt)
            {
                ymax = std::max(ymax, cur_pt->max_value());
                if (zero_min) continue;
                ymin = std::min(ymin, cur_pt->min_value());
            }
        }
        if (ymin == std::numeric_limits<float>::max()) ymin = 0;
        ymax = (ymax == -std::numeric_limits<float>::max()) ? 0 : scale_y_max * ymax + 0.0001f;
        data.set_yrange(ymin, ymax);
    }
}}}}

