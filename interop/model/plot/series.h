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
        /** Collection of options for a plot */
        typedef std::vector<std::string> options_t;
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
               const std::string color="Blue",
               const series_types series_type=Candlestick) :
                m_title(title),
                m_color(color),
                m_series_type(series_type){}

    public:
        /** Get label of series for the legend
         *
         * @return series label
         */
        const std::string& title()const
        {
            return m_title;
        }
        /** Get color of the series
         *
         * @return color name or code of the series
         */
        const std::string& color()const
        {
            return m_color;
        }
        /** Get type of the series
         *
         * @return type of the series
         */
        series_types series_type()const
        {
            return m_series_type;
        }
        /** Collection of options
         *
         * @return collection of options
         */
        const options_t& options()const
        {
            return m_options;
        }
        /** Collection of options
         *
         * @param option string name of option
         */
        void add_option(const std::string& option)
        {
            m_options.push_back(option);
        }

        friend std::ostream& operator<<(std::ostream& out, const series& data)
        {
            out << data.m_title << ",";
            out << data.m_color << ",";
            out << data.m_series_type << ",";
            out << data.m_options.size() << ",";
            for(size_t i=0;i<data.m_options.size();++i)
                out << data.m_options[i] << ",";
            out << data.size() << ",";
            for(size_t i=0;i<data.size();++i)
            {
                out << data[i];
            }
            return out;
        }
        friend std::istream& operator>>(std::istream& in, series& data)
        {
            std::string tmp;
            std::getline(in, data.m_title, ',');
            std::getline(in, data.m_color, ',');
            std::getline(in, tmp, ',');
            int series_type = util::lexical_cast<int>(tmp);
            data.m_series_type = static_cast<series_types>(series_type);

            std::getline(in, tmp, ',');
            size_t num_options = util::lexical_cast<size_t>(tmp);
            data.m_options.resize(num_options);
            for(size_t i=0;i<data.m_options.size();++i)
            {
                std::getline(in, data.m_options[i], ',');
            }

            std::getline(in, tmp, ',');
            size_t num_points = util::lexical_cast<size_t>(tmp);
            data.resize(num_points);
            for(size_t i=0;i<data.size();++i)
            {
                in >> data[i];
            }
            return in;
        }

    private:
        std::string m_title;
        std::string m_color;
        series_types m_series_type;
        options_t m_options;
    };


}}}}

