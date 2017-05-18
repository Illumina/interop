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
#include "interop/constants/enums.h"
#include "interop/logic/utils/enums.h"

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


    /** Get the color for a specific line index
     *
     * @note From: http://stackoverflow.com/questions/1168260/algorithm-for-generating-unique-colors
     * @param index line index
     * @return color name
     */
    inline std::string color_name_for_index(const size_t index)
    {
        const char* more_colors[] =
        {
            "#01FFFE",
            "#FFA6FE",
            "#FFDB66",
            "#006401",
            "#010067",
            "#95003A",
            "#007DB5",
            "#FF00F6",
            "#FFEEE8",
            "#774D00",
            "#90FB92",
            "#0076FF",
            "#D5FF00",
            "#FF937E",
            "#6A826C",
            "#FF029D",
            "#FE8900",
            "#7A4782",
            "#7E2DD2",
            "#85A900",
            "#FF0056",
            "#A42400",
            "#00AE7E",
            "#683D3B",
            "#BDC6FF",
            "#263400",
            "#BDD393",
            "#00B917",
            "#9E008E",
            "#001544",
            "#C28C9F",
            "#FF74A3",
            "#01D0FF",
            "#004754",
            "#E56FFE",
            "#788231",
            "#0E4CA1",
            "#91D0CB",
            "#BE9970",
            "#968AE8",
            "#BB8800",
            "#43002C",
            "#DEFF74",
            "#00FFC6",
            "#FFE502",
            "#620E00",
            "#008F9C",
            "#98FF52",
            "#7544B1",
            "#B500FF",
            "#00FF78",
            "#FF6E41",
            "#005F39",
            "#6B6882",
            "#5FAD4E",
            "#A75740",
            "#A5FFD2",
            "#FFB167",
            "#009BFF",
            "#E85EBE"
        };
        const size_t color_offset = static_cast<size_t>(constants::PlotColorCount);
        if(index < color_offset)
        {
            INTEROP_ASSERT(index < constants::PlotColorCount);
            return constants::to_string(static_cast<constants::plot_colors>(index));
        }
        if(index >= (util::length_of(more_colors)+color_offset))
        {
            return constants::to_string(constants::UnknownColor);
        }
        return more_colors[index-color_offset];
    }
}}}}

