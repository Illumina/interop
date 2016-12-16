/** Collection of data points
 *
 *  @file
 *  @date 5/2/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include "interop/util/exception.h"
#include "interop/util/assert.h"
#include "interop/model/model_exceptions.h"

namespace illumina { namespace interop { namespace model { namespace plot
{

    /** Collection of data points
     */
    template<class Point>
    class data_point_collection
    {
        typedef std::vector<Point> point_vector_t;
    public:
        /** Constant reference to data point */
        typedef typename point_vector_t::const_reference const_reference;
        /** Reference to data point */
        typedef typename point_vector_t::reference reference;
        /** Size type */
        typedef typename point_vector_t::size_type size_type;
        /** Constant iterator */
        typedef typename point_vector_t::const_iterator const_iterator;

    public:
        /** Add a point to the collection
         *
         * @param point data point
         */
        void push_back(const Point &point)
        {
            m_points.push_back(point);
        }

        /** Get point at index
         *
         * @param index index of point
         * @return data point
         */
        const Point & operator[](const size_t index) const throw(model::index_out_of_bounds_exception)
        {
            if(index >= m_points.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Index out of bounds");
            return m_points[index];
        }

        /** Get point at index
         *
         * @param index index of point
         * @return data point
         */
        Point & operator[](const size_t index) throw(model::index_out_of_bounds_exception)
        {
            if(index >= m_points.size())
                INTEROP_THROW( model::index_out_of_bounds_exception, "Index out of bounds");
            return m_points[index];
        }

        /** Get point at index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param index index of point
         * @return data point
         */
        const Point &at(const size_t index) const throw(model::index_out_of_bounds_exception)
        {
            if(index >= m_points.size())
                INTEROP_THROW(model::index_out_of_bounds_exception, "Index out of bounds");
            return m_points[index];
        }

        /** Number of points in collection
         *
         * @return number of points in the collection
         */
        size_t size() const
        {
            return m_points.size();
        }

        /** Assign value to collection
         *
         * @param n number of times to assign
         * @param val value
         */
        void assign(const size_t n, const Point &val)
        {
            m_points.assign(n, val);
        }

        /** Resize collection to given size
         *
         * @param n given size
         */
        void resize(const size_t n)
        {
            m_points.resize(n);
        }

        /** Get iterator to start of collection
         *
         * @return iterator to start of collection
         */
        const_iterator begin() const
        {
            return m_points.begin();
        }

        /** Get iterator to end of collection
         *
         * @return iterator to end of collection
         */
        const_iterator end() const
        {
            return m_points.end();
        }

    private:
        point_vector_t m_points;
    };


}}}}

