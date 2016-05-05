/** Series of data points
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/plot/data_point_collection.h"

namespace illumina { namespace interop { namespace model { namespace plot {

    /** Series of data points and meta data describing points
     */
    template<class Point>
    class series : public data_point_collection<Point>
    {
    public:
        /** Defines a collection of points */
        typedef data_point_collection<Point> point_collection_t;
    public:
        /** Constant iterator */
        typedef typename point_collection_t::const_iterator const_iterator;
        /** Constant reference to data point */
        typedef typename point_collection_t::const_reference const_reference;
        /** Reference to data point */
        typedef typename point_collection_t::reference reference;
        /** Size type */
        typedef typename point_collection_t::size_type size_type;

    public:
        /** Type of plot to display */
        enum series_types
        {
            /** Candle stick points */
            Candlestick,
            /** Line points */
            Line,
            /** Bar points */
            Bar
        };

    public:
        /** Constructor
         *
         * @param title label of series in legend
         * @param color color for points
         * @param series_type type of the series
         */
        series(const std::string& title="",
               const std::string color="Black",
               const series_types series_type=Candlestick) :
                m_title(title),
                m_color(color),
                m_series_type(series_type){}

    public:
        /** Get label of series for the legend
         *
         * @return series label
         */
        const std::string& title()
        {
            return m_title;
        }
        /** Get color of the series
         *
         * @return color name or code of the series
         */
        const std::string& color()
        {
            return m_color;
        }
        /** Get type of the series
         *
         * @return type of the series
         */
        series_types series_type()
        {
            return m_series_type;
        }

    private:
        std::string m_title;
        std::string m_color;
        series_types m_series_type;
    };


}}}}
