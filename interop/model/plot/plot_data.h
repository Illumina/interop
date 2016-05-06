/** Encapsulates all the data for a single plot
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
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
        }
        /** Resize collection to given size
         *
         * @param n given size
         */
        void resize(const size_type n)
        {
            m_series.resize(n);
        }
        /** Assign value to collection
         *
         * @param n given size
         * @param val value to assign
         */
        void assign(const size_type n, const series<Point>& val)
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
         * @param index index of point
         * @return data point
         */
        const series<Point>& at(const size_type index)const
        {
            return m_series.at(index);
        }
        /** Get point at index
         *
         * @param index index of point
         * @return data point
         */
        const_reference operator[](const size_type index)const
        {
            INTEROP_ASSERT(index < m_series.size());
            return m_series[index];
        }
        /** Get point at index
         *
         * @param index index of point
         * @return data point
         */
        reference operator[](const size_type index)
        {
            INTEROP_ASSERT(index < m_series.size());
            return m_series[index];
        }
        /** Number of points in collection
         *
         * @return number of points in the collection
         */
        size_type size()const
        {
            return m_series.size();
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

    private:
        series_collection_t m_series;
    };


}}}}
