/** Summary statistics compiled by lane
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <cstddef>
#include <climits>
#include "interop/model/model_exceptions.h"
#include "interop/model/summary/cycle_state_summary.h"
#include "interop/model/summary/stat_summary.h"
#include "interop/model/summary/surface_summary.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace summary
{

    /**  Summary statistics compiled by lane
     *
     * @note The reported standard deviation is always between tiles, not cycles. For metrics like error metrics,
     * the mean over all cycles is used for each tile, before calculating the standard deviation.
     *
     */
    class lane_summary : public stat_summary
    {
        /** Lane vector type */
        typedef std::vector<surface_summary> surface_summary_vector_t;
    public:
        /** Collection of metric statistics type */
        typedef metric_stat metric_stat_t;
    public:
        /** Reference to lane_summary */
        typedef surface_summary_vector_t::reference reference;
        /** Constant reference to lane_summary */
        typedef surface_summary_vector_t::const_reference const_reference;
        /** Random access iterator to vector of lane_summary */
        typedef surface_summary_vector_t::iterator iterator;
        /** Constant random access iterator to vector of lane_summary */
        typedef surface_summary_vector_t::const_iterator const_iterator;
        /** Unsigned integral type (usually size_t) */
        typedef surface_summary_vector_t::size_type size_type;
    public:
        /** Constructor
         *
         * @param lane lane number
         * @param channel_count channel count
         */
        lane_summary(const size_t lane = 0, const size_t channel_count=0) :
                stat_summary(channel_count),
                m_lane(lane),
                m_tile_count(0)
        {
        }

    public:
        /** Get reference to lane_summary at given index
         *
         * @param n index
         * @return reference to lane_summary
         */
        surface_summary & operator[](const size_t n) throw(model::index_out_of_bounds_exception)
        {
            if (n >= m_summary_by_surface.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Surface index exceeds surface count");
            return m_summary_by_surface[n];
        }

        /** Get reference to lane_summary at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return reference to lane_summary
         */
        surface_summary &at(const size_t n) throw(model::index_out_of_bounds_exception)
        {
            if (n >= m_summary_by_surface.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Surface index exceeds surface count");
            return m_summary_by_surface[n];
        }

        /** Get random access iterator to start of summaries by lane
         *
         * @return random access iterator
         */
        iterator begin()
        {
            return m_summary_by_surface.begin();
        }

        /** Get random access iterator to end of summaries by lane
         *
         * @return random access iterator
         */
        iterator end()
        {
            return m_summary_by_surface.end();
        }

        /** Get random access iterator to start of summaries by lane
         *
         * @return random access iterator
         */
        const_iterator begin()const
        {
            return m_summary_by_surface.begin();
        }

        /** Get random access iterator to end of summaries by lane
         *
         * @return random access iterator
         */
        const_iterator end()const
        {
            return m_summary_by_surface.end();
        }

        /** Resize the summary by lane vector
         *
         * @param n new size of summary by lane vector
         */
        void resize(const size_type n)
        {
            m_summary_by_surface.resize(n);
        }
        /** Set lane number
         *
         * @param val lane number
         */
        void lane(const size_t val)
        {
            m_lane = val;
        }
        /** Set number of tiles in the lane
         *
         * @param val number of tiles in the lane
         */
        void tile_count(const size_t val)
        {
            m_tile_count = val;
        }
        /** Get statistics summarizing the cycle of each RTA state of tiles in the lane
         *
         * @return statistics summarizing the cycle of each RTA state of tiles in the lane
         */
        cycle_state_summary &cycle_state()
        {
            return m_cycle_state;
        }

    public:
        /** @defgroup lane_summary Read/Lane summary
         *
         * Summary of key metrics in each read/lane
         *
         * @ingroup read_summary
         * @ref illumina::interop::model::summary::lane_summary "See full class description"
         * @ref illumina::interop::model::summary::stat_summary "See full class description"
         * @{
         */
        /** Get constant reference to lane_summary at given index
         *
         * @param n index
         * @return constant reference to lane_summary
         */
        const surface_summary& operator[](const size_t n) const throw(model::index_out_of_bounds_exception)
        {
            if (n >= m_summary_by_surface.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Surface index exceeds surface count");
            return m_summary_by_surface[n];
        }

        /** Get constant reference to lane_summary at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return constant reference to lane_summary
         */
        const_reference at(const size_type n) const throw(model::index_out_of_bounds_exception)
        {
            if (n >= m_summary_by_surface.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Surface index exceeds surface count");
            return m_summary_by_surface[n];
        }
        /** Get lane number
         *
         * @return lane number
         */
        size_t lane() const
        {
            return m_lane;
        }
        /** Get number of tiles in the lane
         *
         * @return number of tiles in the lane
         */
        size_t tile_count() const
        {
            return m_tile_count;
        }
        /** Get statistics summarizing the cycle of each RTA state of tiles in the lane
         *
         * @return statistics summarizing the cycle of each RTA state of tiles in the lane
         */
        const cycle_state_summary &cycle_state() const
        {
            return m_cycle_state;
        }
        /** Get number of summaries by surface
         *
         * @return number of summaries by surface
         */
        size_t size() const
        {
            return m_summary_by_surface.size();
        }
        /** @} */

    private:
        size_t m_lane;
        size_t m_tile_count;
        cycle_state_summary m_cycle_state;
        surface_summary_vector_t m_summary_by_surface;
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

