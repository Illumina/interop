/** Encapsulates all the data for a single plot
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/exception.h"
#include "interop/model/plot/series.h"
#include "interop/model/plot/axes.h"
#include "interop/model/plot/chart_data.h"

namespace illumina { namespace interop { namespace model { namespace plot {

    /** Encapsulates all data for a single plot
     */
    template<class Point>
    class plot_data : public chart_data
    {
        typedef series<Point> series_t;
        typedef std::vector<series_t> series_collection_t;
    public:
        /** Constant reference to data point */
        typedef typename series_collection_t::const_reference const_reference;
        /** Reference to data point */
        typedef typename series_collection_t::reference reference;
        /** Size type */
        typedef typename series_collection_t::size_type size_type;
        /** Iterator */
        typedef typename series_collection_t::iterator iterator;
        /** Constant iterator */
        typedef typename series_collection_t::const_iterator const_iterator;

    public:
        /** Clear all series
         */
        void clear()
        {
            m_series.clear();
            chart_data::clear();
        }
        /** Resize collection to given size
         *
         * @param n given size
         */
        void resize(const size_t n)
        {
            m_series.resize(n);
        }
        /** Assign value to collection
         *
         * @param n given size
         * @param val value to assign
         */
        void assign(const size_t n, const series<Point>& val)
        {
            m_series.assign(n, val);
        }
        /** Push a value to the back of the collection
         *
         * @param val value to add
         */
        void push_back(const series<Point>& val)
        {
            m_series.push_back(val);
        }
        /** Get point at index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param index index of point
         * @return data point
         */
        const series<Point>& at(const size_t index)const throw(model::index_out_of_bounds_exception)
        {
            if(index >= m_series.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Row index out of bounds");
            return m_series[index];
        }
        /** Get point at index
         *
         * @param index index of point
         * @return data point
         */
        series<Point>& operator[](const size_t index) throw(model::index_out_of_bounds_exception)
        {
            if(index >= m_series.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Row index out of bounds");
            return m_series[index];
        }
        /** Get point at index
         *
         * @param index index of point
         * @return data point
         */
        const series<Point>& operator[](const size_t index)const throw(model::index_out_of_bounds_exception)
        {
            if(index >= m_series.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Row index out of bounds");
            return m_series[index];
        }
        /** Number of points in collection
         *
         * @return number of points in the collection
         */
        size_t size()const
        {
            return m_series.size();
        }
        /** Check if object has points
         *
         * @return true if plot has points
         */
        bool empty()const
        {
            return size()==0;
        }

    public:
        /** Get iterator to start of collection
         *
         * @return iterator to start of collection
         */
        const_iterator begin()const
        {
            return m_series.begin();
        }
        /** Get iterator to end of collection
         *
         * @return iterator to end of collection
         */
        const_iterator end()const
        {
            return m_series.end();
        }
        friend std::ostream& operator<<(std::ostream& out, const plot_data& data)
        {
            out << static_cast<const chart_data&>(data);
            out << data.m_series.size() << ",";
            for(size_t i=0;i<data.m_series.size();++i)
                out << data.m_series[i];
            return out;
        }
        friend std::istream& operator>>(std::istream& in, plot_data& data)
        {
            std::string tmp;
            in >> static_cast<chart_data&>(data);
            std::getline(in, tmp, ',');
            size_t n = util::lexical_cast<size_t>(tmp);
            data.m_series.resize(n);
            for(size_t i=0;i<data.m_series.size();++i)
                in >> data.m_series[i];
            return in;
        }

    private:
        series_collection_t m_series;
    };


}}}}

