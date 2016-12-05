/** Data point in plot
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include "interop/util/lexical_cast.h"

namespace illumina { namespace interop { namespace model { namespace plot {

    /** Single axis of a plot
     */
    class axis
    {
    public:
        /** Constructor */
        axis() : m_min(0), m_max(0){}
    public:
        /** Set the limits of the axis
         *
         * @param vmin minimum value
         * @param vmax maximum value
         */
        void set_range(const float vmin, const float vmax)
        {
            m_min = vmin;
            m_max = vmax;
        }
        /** Set the label of the axis
         *
         * @param label text label
         */
        void set_label(const std::string& label)
        {
            m_label = label;
        }
        /** Get the label of the axis
         *
         * @return text label
         */
        const std::string& label()const
        {
            return m_label;
        }
        /** Minimum value of the range
         *
         * @return minimum value of range
         */
        float min()const
        {
            return m_min;
        }
        /** Maximum value of the range
         *
         * @return maximum value of range
         */
        float max()const
        {
            return m_max;
        }
        /** Clear the axis
         */
        void clear()
        {
            m_min = 0.0f;
            m_max = 0.0f;
            m_label = "";
        }

        friend std::ostream& operator<<(std::ostream& out, const axis& data)
        {
            std::ios::fmtflags previous_state( out.flags() );
            out << std::setprecision(10) << data.m_min << "," << data.m_max << ",";
            out << data.m_label << ",";
            out.flags(previous_state);
            return out;
        }
        friend std::istream& operator>>(std::istream& in, axis& data)
        {
            std::string tmp;
            std::getline(in, tmp, ',');
            data.m_min = util::lexical_cast<float>(tmp);
            std::getline(in, tmp, ',');
            data.m_max = util::lexical_cast<float>(tmp);
            std::getline(in, data.m_label, ',');
            return in;
        }

    private:
        float m_min;
        float m_max;
        std::string m_label;
    };
    /** X & Y axes of a plot
     */
    class axes
    {
    public:
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
            m_x.set_range(vmin, vmax);
        }
        /** Set the limits of the Y-axis
         *
         * @param vmin minimum value
         * @param vmax maximum value
         */
        void set_yrange(const float vmin, const float vmax)
        {
            m_y.set_range(vmin, vmax);
        }
        /** Set the label of the X-axis
         *
         * @param label text label
         */
        void set_xlabel(const std::string& label)
        {
            m_x.set_label(label);
        }
        /** Set the label of the Y-axis
         *
         * @param label text label
         */
        void set_ylabel(const std::string& label)
        {
            m_y.set_label(label);
        }
        /** Get the label of the X-axis
         *
         * @return X-axis text label
         */
        const std::string& xlabel()const
        {
            return m_x.label();
        }
        /** Get the label of the Y-axis
         *
         * @return X-axis text label
         */
        const std::string& ylabel()const
        {
            return m_y.label();
        }
        /** Set the X-axis
         *
         * @param xaxes x axis
         */
        void set_xaxis(const axis& xaxes)
        {
            m_x = xaxes;
        }
        /** Set the Y-axis
         *
         * @param yaxes y axis
         */
        void set_yaxis(const axis& yaxes)
        {
            m_y = yaxes;
        }

    public:
        /** Get the X-axis
         *
         * @return axis
         */
        const axis& x()const
        {
            return m_x;
        }
        /** Get the Y-axis
         *
         * @return axis
         */
        const axis& y()const
        {
            return m_y;
        }
        /** Clear both axes
         */
        void clear()
        {
            m_x.clear();
            m_y.clear();
        }

        friend std::ostream& operator<<(std::ostream& out, const axes& data)
        {
            out << data.m_x;
            out << data.m_y;
            return out;
        }
        friend std::istream& operator>>(std::istream& in, axes& data)
        {
            in >> data.m_x;
            in >> data.m_y;
            return in;
        }

    private:
        axis m_x;
        axis m_y;
    };

}}}}

