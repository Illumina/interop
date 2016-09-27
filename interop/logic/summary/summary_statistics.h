/** Utilities for summarizing metrics
 *
 *  @file
 *  @date  3/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/model/model_exceptions.h"
#include "interop/util/statistics.h"
#include "interop/model/summary/run_summary.h"


namespace illumina { namespace interop { namespace logic { namespace summary
{
    /** Reserve space in a vector residing in another collection
     *
     * @param beg iterator to start of collection containing vectors
     * @param end iterator to end of collection containing vectors
     * @param n number of elements to reserve
     */
    template<typename I>
    void reserve(I beg, I end, const ptrdiff_t n)
    {
        for (; beg != end; ++beg) beg->reserve(n);
    }

    /** Collection of values organized by read, then lane per read
     */
    template<typename T>
    class summary_by_lane_read
    {
    public:
        /** Vector of values */
        typedef std::vector<T> vector_t; // TODO: Speed up with 2d vector?
        /** Vector of vector of values */
        typedef std::vector<vector_t> vector2d_t;
        /** Vector of vector of vector of values */
        typedef std::vector<vector2d_t> vector3d_t;
        /** Iterator to vector of values */
        typedef typename vector_t::const_iterator const_iterator;
        /** Iterator over read vector (vector of vector of vector of values) */
        typedef typename vector3d_t::iterator read_iterator;
        /** Constant reference to value */
        typedef typename vector_t::const_reference const_reference;


    public:
        /** Constructor
         *
         * @param run run summary
         * @param n maximum number of values expected
         * @param surface_count number of surfaces
         */
        summary_by_lane_read(const model::summary::run_summary &run, const ptrdiff_t n, const size_t surface_count=1) :
                m_summary_by_lane_read(run.size(), vector2d_t(run.lane_count()*std::max(static_cast<size_t>(1),surface_count))),
                m_lane_count(run.lane_count()),
                m_surface_count(std::max(static_cast<size_t>(1),surface_count))
        {
            for (read_iterator beg = m_summary_by_lane_read.begin(), end = m_summary_by_lane_read.end();
                 beg != end; ++beg)
                reserve(beg->begin(), beg->end(), n);
        }

    public:
        /** Access vector of values at given read and lane
         *
         * @param read read index (0-indexed)
         * @param lane lane index (0-indexed)
         * @param surface surface index (0-indexed)
         * @return vector of values
         */
        vector_t &operator()(const size_t read, const size_t lane, const size_t surface=0)
        {
            INTEROP_ASSERTMSG(read < m_summary_by_lane_read.size(), read << " < " << m_summary_by_lane_read.size());
            const size_t offset = lane*m_surface_count+surface;
            INTEROP_ASSERTMSG(offset < m_summary_by_lane_read[read].size(),
                              offset << " < " << m_summary_by_lane_read[read].size());
            return m_summary_by_lane_read[read][offset];
        }

        /** Clear the vector of values in each read/lane
         */
        void clear()
        {
            for (read_iterator beg = m_summary_by_lane_read.begin(), end = m_summary_by_lane_read.end();
                 beg != end; ++beg)
                clear(beg->begin(), beg->end());
        }

        /** Size of vector
         *
         * @return number of reads
         */
        size_t size() const
        {
            return m_summary_by_lane_read.size();
        }

        /** Size of read vector
         *
         * @return number of reads
         */
        size_t read_count() const
        {
            return m_summary_by_lane_read.size();
        }

        /** Number of lanes
         *
         * @return number of lanes
         */
        size_t lane_count() const
        {
            return m_lane_count;
        }
        /** Number of surfaces
         *
         * @return number of surfaces
         */
        size_t surface_count() const
        {
            return m_surface_count;
        }

    private:
        template<typename I>
        static void clear(I beg, I end)
        {
            for (; beg != end; ++beg) beg->clear();
        }

    private:
        vector3d_t m_summary_by_lane_read;
        size_t m_lane_count;
        size_t m_surface_count;
    };

    /** Calculate the mean, standard deviation (stddev) and median over a collection of values
     *
     * @param beg iterator to start of collection
     * @param end iterator to end of collection
     * @param stat object to store mean, stddev, and median
     * @param skip_median skip the median calculation
     */
    template<typename I, typename S>
    void summarize(I beg, I end, S &stat, const bool skip_median)
    {
        if (beg == end) return;
        stat.mean(util::mean<float>(beg, end));
        stat.stddev(std::sqrt(util::variance_with_mean<float>(beg, end, stat.mean())));
        if(!skip_median) stat.median(util::median_interpolated<float>(beg, end));
    }

    /** Calculate the mean, standard deviation (stddev) and median over a collection of values
     *
     * @param beg iterator to start of collection
     * @param end iterator to end of collection
     * @param stat object to store mean, stddev, and median
     * @param op unary/binary operator for getting a value in a complex object
     * @param comp comparison operator to compare a single value in a complex object
     * @param skip_median skip the median calculation
     */
    template<typename I, typename S, typename BinaryOp, typename Compare>
    void summarize(I beg, I end, S &stat, BinaryOp op, Compare comp, const bool skip_median)
    {
        if (beg == end) return;
        stat.mean(util::mean<float>(beg, end, op));
        stat.stddev(std::sqrt(util::variance_with_mean<float>(beg, end, stat.mean(), op)));
        if(!skip_median) stat.median(util::median_interpolated<float>(beg, end, comp, op));
    }

    /** Calculate the mean, standard deviation (stddev) and median over a collection of values, ignoring NaNs
     *
     * @param beg iterator to start of collection
     * @param end iterator to end of collection
     * @param stat object to store mean, stddev, and median
     * @param op unary/binary operator for getting a value in a complex object
     * @param comp comparison operator to compare a single value in a complex object
     * @param skip_median skip the median calculation
     * @return number of non-NaN elements
     */
    template<typename I, typename S, typename BinaryOp, typename Compare>
    size_t nan_summarize(I beg, I end, S &stat, BinaryOp op, Compare comp, const bool skip_median)
    {
        stat.clear();
        if (beg == end) return 0;
        end = util::remove_nan(beg, end, op);
        if (beg == end) return 0;
        stat.mean(util::mean<float>(beg, end, op));
        INTEROP_ASSERT(!std::isnan(stat.mean()));
        stat.stddev(std::sqrt(util::variance_with_mean<float>(beg, end, stat.mean(), op)));
        if(!skip_median) stat.median(util::median_interpolated<float>(beg, end, comp, op));
        return size_t(std::distance(beg, end));
    }

    /** Safe divide
     *
     * @param num numerator
     * @param div divisor
     * @return result of division
     */
    inline float divide(const float num, const float div)
    {
        static const float eps = 1e-9f;
        return (div < eps) ? 0 : num / div;
    }

    namespace op
    {
        /** Binary operator that sums an object that contains the total_cycles
         * member function.
         */
        template<typename UnaryOp>
        struct total_cycle_sum
        {
            /** Constructor
             *
             * @param op unary operator that returns an object that contains total_cycles
             */
            total_cycle_sum(const UnaryOp &op) : m_op(op)
            { }

            /** Sum total_cycles
             *
             * @param last current sum value
             * @param obj object that will contain total_cycles after the unary operator
             * @return current sum
             */
            template<class T>
            size_t operator()(const size_t last, const T &obj) const
            {
                return last + m_op(obj).total_cycles();
            }

        private:
            UnaryOp m_op;
        };

        /** Unary operators that take an object and return a read_info object
         *
         */
        struct default_get_read
        {
            /** Get a read_info from the read summary
             *
             * @param summary read summary
             * @return read_info object
             */
            model::run::read_info &operator()(model::summary::read_summary &summary) const
            {
                return summary.read();
            }

            /** Get a read_info from the read summary
             *
             * @param read read_info
             * @return read_info object
             */
            model::run::read_info &operator()(model::run::read_info &read) const
            {
                return read;
            }

            /** Get a read_info from the read summary
             *
             * @param summary read summary
             * @return constant read_info object
             */
            const model::run::read_info &operator()(const model::summary::read_summary &summary) const
            {
                return summary.read();
            }

            /** Get a read_info
             *
             * @param read read_info
             * @return constant read_info object
             */
            const model::run::read_info &operator()(const model::run::read_info &read) const
            {
                return read;
            }
        };
    }

}}}}

