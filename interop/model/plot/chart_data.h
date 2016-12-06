/** Encapsulates all data for a chart
 *
 *  @file
 *  @date 5/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <string>
#include "interop/model/plot/series.h"
#include "interop/model/plot/axes.h"

namespace illumina { namespace interop { namespace model { namespace plot
{

    /** Encapsulates all data for a chart
     */
    class chart_data
    {
    public:
        /** Set both axes
         *
         * @param xyaxes both axes
         */
        void set_axes(const axes &xyaxes)
        {
            m_axes = xyaxes;
        }

        /** Set the X-axis
         *
         * @param xaxes x axis
         */
        void set_xaxis(const axis &xaxes)
        {
            m_axes.set_xaxis(xaxes);
        }

        /** Set the Y-axis
         *
         * @param yaxes y axis
         */
        void set_yaxis(const axis &yaxes)
        {
            m_axes.set_yaxis(yaxes);
        }

        /** Set the label of the X-axis
         *
         * @param label text label
         */
        void set_xlabel(const std::string &label)
        {
            m_axes.set_xlabel(label);
        }

        /** Set the label of the Y-axis
         *
         * @param label text label
         */
        void set_ylabel(const std::string &label)
        {
            m_axes.set_ylabel(label);
        }

        /** Set the limits of the axis
         *
         * @param xmin minimum value
         * @param xmax maximum value
         * @param ymin minimum value
         * @param ymax maximum value
         */
        void set_range(const float xmin, const float xmax, const float ymin, const float ymax)
        {
            set_xrange(xmin, xmax);
            set_yrange(ymin, ymax);
        }

        /** Set the limits of the X-axis
         *
         * @param vmin minimum value
         * @param vmax maximum value
         */
        void set_xrange(const float vmin, const float vmax)
        {
            m_axes.set_xrange(vmin, vmax);
        }

        /** Set the limits of the Y-axis
         *
         * @param vmin minimum value
         * @param vmax maximum value
         */
        void set_yrange(const float vmin, const float vmax)
        {
            m_axes.set_yrange(vmin, vmax);
        }

        /** Get X-axis
         *
         * @return x-axis
         */
        const axis &x_axis() const
        {
            return m_axes.x();
        }

        /** Get Y-axis
         *
         * @return y-axis
         */
        const axis &y_axis() const
        {
            return m_axes.y();
        }

        /** Set the title of the plot
         *
         * @param title plot title
         */
        void set_title(const std::string &title)
        {
            m_title = title;
        }

        /** Get the title of the plto
         *
         * @return title
         */
        const std::string &title() const
        {
            return m_title;
        }

        /** Get the X/Y axes
         *
         * @return x/y axes
         */
        const axes &xyaxes() const
        {
            return m_axes;
        }
        /** Clear the title and axis
         */
        void clear()
        {
            m_axes.clear();
            m_title = "";
        }

        friend std::ostream& operator<<(std::ostream& out, const chart_data& data)
        {
            out << data.m_title << ",";
            out << data.m_axes;
            return out;
        }
        friend std::istream& operator>>(std::istream& in, chart_data& data)
        {
            std::getline(in, data.m_title, ',');
            in >> data.m_axes;
            return in;
        }

    private:
        axes m_axes;
        std::string m_title;
    };


}}}}

