/** Index summary for entire flowcell
 *
 *  @file
 *  @date  5/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "interop/util/exception.h"
#include "interop/util/assert.h"
#include "interop/model/model_exceptions.h"
#include "interop/io/format/generic_layout.h"
#include "interop/model/summary/index_lane_summary.h"

namespace illumina { namespace interop { namespace model { namespace summary
{

    /** Summary of metrics for index reads for the entire flowcell
     */
    class index_flowcell_summary
    {
    public:
        /** Lane summary vector type */
        typedef std::vector<index_lane_summary> lane_summary_vector_t;
    public:
        /** Reference to lane summary */
        typedef lane_summary_vector_t::reference reference;
        /** Constant reference to lane summary */
        typedef lane_summary_vector_t::const_reference const_reference;
        /** Random access iterator to vector of lane summary */
        typedef lane_summary_vector_t::iterator iterator;
        /** Constant random access iterator to vector of lane summary */
        typedef lane_summary_vector_t::const_iterator const_iterator;
        /** Unsigned integral type (usually size_t) */
        typedef lane_summary_vector_t::size_type size_type;
    public:
        /** Constructor
         *
         * @param n number of lanes
         */
        index_flowcell_summary(const size_t n=0) : m_lane_summaries(n)
        {
        }

    public:
        /** Allocate space for the number of lanes
         *
         * @param n number of lanes
         */
        void resize(const size_type n)
        {
            m_lane_summaries.resize(n);
        }
        /** Get reference to lane summary at given index
         *
         * @param n index
         * @return reference to lane summary
         */
        index_lane_summary& operator[](const size_t n) throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_lane_summaries.size())
                INTEROP_THROW(index_out_of_bounds_exception, "Lane index exceeds lane count");
            return m_lane_summaries[n];
        }
        /** Get constant reference to lane summary at given index
         *
         * @param n index
         * @return constant reference to lane summary
         */
        const index_lane_summary& operator[](const size_t n)const throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_lane_summaries.size())
                INTEROP_THROW(index_out_of_bounds_exception, "Lane index exceeds lane count");
            return m_lane_summaries[n];
        }
        /** Get reference to lane summary at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return reference to lane summary
         */
        index_lane_summary& at(const size_t n) throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_lane_summaries.size())
                INTEROP_THROW(index_out_of_bounds_exception, "Lane index exceeds lane count");
            return m_lane_summaries[n];
        }
        /** Get constant reference to lane summary at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return constant reference to lane summary
         */
        const_reference at(const size_type n)const throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_lane_summaries.size())
                INTEROP_THROW(index_out_of_bounds_exception, "Lane index exceeds lane count");
            return m_lane_summaries[n];
        }
        /** Get number of summaries by read
         *
         * @return number of summaries by read
         */
        size_t size()const
        {
            return m_lane_summaries.size();
        }
        /** Get random access iterator to start of summaries by read
         *
         * @return random access iterator
         */
        iterator begin()
        {
            return m_lane_summaries.begin();
        }
        /** Get random access iterator to end of summaries by read
         *
         * @return random access iterator
         */
        iterator end()
        {
            return m_lane_summaries.end();
        }
        /** Get constant random access iterator to start of summaries by read
         *
         * @return constant random access iterator
         */
        const_iterator begin()const
        {
            return m_lane_summaries.begin();
        }
        /** Get constant random access iterator to end of summaries by read
         *
         * @return constant random access iterator
         */
        const_iterator end()const
        {
            return m_lane_summaries.end();
        }
        /** Sort the index summaries in each lane
         */
        void sort()
        {
            for(iterator b = begin(), e=end();b != e;++b)
                b->sort();
        }
        /** Clear the lane info
         */
        void clear()
        {
            m_lane_summaries.clear();
        }

        friend std::ostream& operator<<(std::ostream& out, const index_flowcell_summary& summary);
        friend std::istream& operator>>(std::istream& in, index_flowcell_summary& summary);

    private:
        lane_summary_vector_t m_lane_summaries;


        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

