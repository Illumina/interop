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

namespace illumina { namespace interop { namespace model { namespace plot {

    /** Encapsulates all data for a single plot
     */
    template<class Point>
    class plot_data
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

    public:
        /** Set both axes
         *
         * @param xyaxes both axes
         */
        void set_axes(const axes& xyaxes)
        {
            m_axes = xyaxes;
        }
        /** Set the X-axis
         *
         * @param xaxes x axis
         */
        void set_xaxis(const axis& xaxes)
        {
            m_axes.set_xaxis(xaxes);
        }
        /** Set the Y-axis
         *
         * @param yaxes y axis
         */
        void set_yaxis(const axis& yaxes)
        {
            m_axes.set_yaxis(yaxes);
        }
        /** Set the label of the X-axis
         *
         * @param label text label
         */
        void set_xlabel(const std::string& label)
        {
            m_axes.set_xlabel(label);
        }
        /** Set the label of the Y-axis
         *
         * @param label text label
         */
        void set_ylabel(const std::string& label)
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
        const axis& x_axis()
        {
            return m_axes.x();
        }
        /** Get Y-axis
         *
         * @return y-axis
         */
        const axis& y_axis()
        {
            return m_axes.y();
        }
        /** Set the title of the plot
         *
         * @param title plot title
         */
        void set_title(const std::string& title)
        {
            m_title = title;
        }
        /** Get the title of the plto
         *
         * @return title
         */
        const std::string& title()const
        {
            return m_title;
        }

    private:
        series_collection_t m_series;
        axes m_axes;
        std::string m_title;
    };


}}}}
