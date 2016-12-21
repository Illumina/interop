/** Summary statistics compiled by lane
 *
 *  @file
 *  @date  3/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include "interop/util/exception.h"
#include "interop/util/assert.h"
#include "interop/model/summary/lane_summary.h"
#include "interop/model/summary/metric_summary.h"
#include "interop/model/run/read_info.h"
#include "interop/model/model_exceptions.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace summary
{

    /**  Summary statistics compiled by lane
     *
     */
    class read_summary
    {
        /** Lane vector type */
        typedef std::vector<lane_summary> lane_summary_vector_t;
    public:
        /** Reference to lane_summary */
        typedef lane_summary_vector_t::reference reference;
        /** Constant reference to lane_summary */
        typedef lane_summary_vector_t::const_reference const_reference;
        /** Random access iterator to vector of lane_summary */
        typedef lane_summary_vector_t::iterator iterator;
        /** Constant random access iterator to vector of lane_summary */
        typedef lane_summary_vector_t::const_iterator const_iterator;
        /** Unsigned integral type (usually size_t) */
        typedef lane_summary_vector_t::size_type size_type;
    public:
        /** Constructor
         *
         * @param read read information
         * @param channel_count number of channels
         */
        read_summary(const run::read_info &read = run::read_info(), const size_t channel_count=0) :
                m_read(read), m_metric_summary(channel_count)
        {
        }

    public:
        /** Get reference to lane_summary at given index
         *
         * @param n index
         * @return reference to lane_summary
         */
        lane_summary & operator[](const size_type n) throw(model::index_out_of_bounds_exception)
        {
            if (n >= m_summary_by_lane.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Lane index exceeds lane count");
            return m_summary_by_lane[n];
        }

        /** Get reference to lane_summary at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return reference to lane_summary
         */
        lane_summary &at(const size_type n) throw(model::index_out_of_bounds_exception)
        {
            if (n >= m_summary_by_lane.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Lane index exceeds lane count");
            return m_summary_by_lane[n];
        }

        /** Get random access iterator to start of summaries by lane
         *
         * @return random access iterator
         */
        iterator begin()
        {
            return m_summary_by_lane.begin();
        }

        /** Get random access iterator to end of summaries by lane
         *
         * @return random access iterator
         */
        iterator end()
        {
            return m_summary_by_lane.end();
        }

        /** Resize the summary by lane vector
         *
         * @param n new size of summary by lane vector
         */
        void resize(const size_type n)
        {
            m_summary_by_lane.resize(n);
        }

        /** Get read information
         *
         * @return read information
         */
        run::read_info &read()
        {
            return m_read;
        }

        /** Get summary metrics
         *
         * @return summary metrics
         */
        metric_summary &summary()
        {
            return m_metric_summary;
        }

        /** Set the summary
         *
         * @param summary metric summary
         */
        void summary(const metric_summary &summary)
        {
            m_metric_summary = summary;
        }

    public:
        /** @defgroup read_summary Read summary
         *
         * Summary of key metrics in each read
         *
         * @ingroup run_summary
         * @ref illumina::interop::model::summary::read_summary "See full class description"
         * @{
         */
        /** Get constant reference to lane_summary at given index
         *
         * @param n index
         * @return constant reference to lane_summary
         */
        const lane_summary& operator[](const size_t n) const throw(model::index_out_of_bounds_exception)
        {
            if (n >= m_summary_by_lane.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Lane index exceeds lane count");
            return m_summary_by_lane[n];
        }

        /** Get constant reference to lane_summary at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return constant reference to lane_summary
         */
        const_reference at(const size_type n) const throw(model::index_out_of_bounds_exception)
        {
            if (n >= m_summary_by_lane.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Lane index exceeds lane count");
            return m_summary_by_lane[n];
        }

        /** Get number of summaries by lane
         *
         * @return number of summaries by lane
         */
        size_t size() const
        {
            return m_summary_by_lane.size();
        }

        /** Get number of lanes
         *
         * @return number of lanes
         */
        size_t lane_count() const
        {
            return m_summary_by_lane.size();
        }

        /** Get constant random access iterator to start of summaries by lane
         *
         * @return constant random access iterator
         */
        const_iterator begin() const
        {
            return m_summary_by_lane.begin();
        }

        /** Get constant random access iterator to end of summaries by lane
         *
         * @return constant random access iterator
         */
        const_iterator end() const
        {
            return m_summary_by_lane.end();
        }

    public:
        /** Get read information
         *
         * @return read information
         */
        const run::read_info &read() const
        {
            return m_read;
        }

        /** Get summary metrics
         *
         * @return summary metrics
         */
        const metric_summary &summary() const
        {
            return m_metric_summary;
        }
        /** @} */

    private:
        /** Read information */
        run::read_info m_read;
        metric_summary m_metric_summary;

    private:
        lane_summary_vector_t m_summary_by_lane;
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

