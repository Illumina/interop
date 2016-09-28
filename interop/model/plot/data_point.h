/** Data point in plot
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once


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

    private:
        x_type m_x;
        y_type m_y;
    };


}}}}

