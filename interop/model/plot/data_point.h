/** Data point in plot
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/exception.h"
#include "interop/util/lexical_cast.h"
#include "interop/io/table/csv_format.h"


namespace illumina { namespace interop { namespace model { namespace plot {

    /** Data point in plot
     *
     * Defines the x and y coordinates
     */
    template<typename X, typename Y>
    class data_point
    {
    public:
        /** Type of x-coordinate */
        typedef X x_type;
        /** Type of y-coordinate */
        typedef Y y_type;

    public:
        /** Constructor
         *
         * @param x x-coordinate
         * @param y y-coordinate
         */
        data_point(const x_type x=0, const y_type y=0) : m_x(x), m_y(y){}

    public:
        /** Get the x-coordinate of the point
         *
         * @return x-coordinate
         */
        X x()const
        {
            return m_x;
        }
        /** Get the y-coordinate of the point
         *
         * @return y-coordinate
         */
        Y y()const
        {
            return m_y;
        }
        /** Get maximum y-value
         *
         * @return maximum y-value
         */
        X max_value()const
        {
            return m_y;
        }
        /** Get minimum y-value
         *
         * @return minimum y-value
         */
        Y min_value()const
        {
            return m_y;
        }

    public:
        /** Add a coordinate to the point
         *
         * @param x x-coordinate
         * @param y y-coordinate
         */
        void add(const x_type x, const y_type y)
        {
            m_x += x;
            m_y += y;
        }
        /** Add a coordinate to the point
         *
         * @param x x-coordinate
         * @param y y-coordinate
         */
        void set(const x_type x, const y_type y)
        {
            m_y = y;
            m_x = x;
        }
        friend std::ostream& operator<<(std::ostream& out, const data_point& data)
        {
            out << std::setprecision(10) << io::table::handle_nan(data.m_x) << ",";
            out << std::setprecision(10) << io::table::handle_nan(data.m_y) << ",";
            //out << "\n";
            return out;
        }
        friend std::istream& operator>>(std::istream& in, data_point& data)
        {
            std::string tmp;
            std::getline(in, tmp, ',');
            data.m_x = util::lexical_cast<x_type>(tmp);
            std::getline(in, tmp, ',');
            data.m_y = util::lexical_cast<y_type>(tmp);
            return in;
        }

    private:
        x_type m_x;
        y_type m_y;
    };


}}}}

