/** Index summary for a single lane
 *
 *  @file
 *  @date  5/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/summary/index_count_summary.h"

namespace illumina { namespace interop { namespace model { namespace summary {

    class index_lane_summary
    {
    public:
        /** Lane summary vector type */
        typedef std::vector<index_count_summary> count_summary_vector_t;
    public:
        /** Reference to lane summary */
        typedef count_summary_vector_t::reference reference;
        /** Constant reference to lane summary */
        typedef count_summary_vector_t::const_reference const_reference;
        /** Random access iterator to vector of lane summary */
        typedef count_summary_vector_t::iterator iterator;
        /** Constant random access iterator to vector of lane summary */
        typedef count_summary_vector_t::const_iterator const_iterator;
        /** Unsigned integral type (usually size_t) */
        typedef count_summary_vector_t::size_type size_type;
    public:
        /** Constructor
         *
         * @param total_reads total clusters
         * @param total_pf_reads total PF clusters
         * @param total_fraction_mapped_reads total fraction of reads mapped
         * @param mapped_reads_cv standard deviation of the mapped reads
         * @param min_mapped_reads minimum of the mapped reads
         * @param max_mapped_reads maximum of the mapped reads
         */
        index_lane_summary(const size_t total_reads=0,
                           const size_t total_pf_reads=0,
                           const float total_fraction_mapped_reads=0,
                           const float mapped_reads_cv=0,
                           const float min_mapped_reads=0,
                           const float max_mapped_reads=0) : m_total_reads(total_reads),
                                                             m_total_pf_reads(total_pf_reads),
                                                             m_total_fraction_mapped_reads(total_fraction_mapped_reads),
                                                             m_mapped_reads_cv(mapped_reads_cv),
                                                             m_min_mapped_reads(min_mapped_reads),
                                                             m_max_mapped_reads(max_mapped_reads)
        {
        }

    public:
        /** Get reference to lane summary at given index
         *
         * @param n index
         * @return reference to lane summary
         */
        reference operator[](const size_type n) throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_count_summaries.size()) throw index_out_of_bounds_exception("Read index exceeds read count");
            return m_count_summaries[n];
        }
        /** Get constant reference to lane summary at given index
         *
         * @param n index
         * @return constant reference to lane summary
         */
        const_reference operator[](const size_type n)const throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_count_summaries.size()) throw index_out_of_bounds_exception("Read index exceeds read count");
            return m_count_summaries[n];
        }
        /** Get reference to lane summary at given index
         *
         * @param n index
         * @return reference to lane summary
         */
        reference at(const size_type n) throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_count_summaries.size()) throw index_out_of_bounds_exception("Read index exceeds read count");
            return m_count_summaries[n];
        }
        /** Get constant reference to lane summary at given index
         *
         * @param n index
         * @return constant reference to lane summary
         */
        const_reference at(const size_type n)const throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_count_summaries.size()) throw index_out_of_bounds_exception("Read index exceeds read count");
            return m_count_summaries[n];
        }
        /** Get number of summaries by read
         *
         * @return number of summaries by read
         */
        size_t size()const
        {
            return m_count_summaries.size();
        }
        /** Get random access iterator to start of summaries by read
         *
         * @return random access iterator
         */
        iterator begin()
        {
            return m_count_summaries.begin();
        }
        /** Get random access iterator to end of summaries by read
         *
         * @return random access iterator
         */
        iterator end()
        {
            return m_count_summaries.end();
        }
        /** Get constant random access iterator to start of summaries by read
         *
         * @return constant random access iterator
         */
        const_iterator begin()const
        {
            return m_count_summaries.begin();
        }
        /** Get constant random access iterator to end of summaries by read
         *
         * @return constant random access iterator
         */
        const_iterator end()const
        {
            return m_count_summaries.end();
        }

    public:
        /** Total reads
         *
         * @return total reads
         */
        size_t total_reads()const
        {
            return m_total_reads;
        }
        /** Total PF reads
         *
         * @return total PF reads
         */
        size_t total_pf_reads()const
        {
            return m_total_pf_reads;
        }
        /** Total fraction of mapped reads
         *
         * @return total fraction of mapped reads
         */
        float total_fraction_mapped_reads()const
        {
            return m_total_fraction_mapped_reads;
        }
        /** Standard deviation of the mapped reads
         *
         * @return standard deviation of the mapped reads
         */
        float mapped_reads_cv()const
        {
            return m_mapped_reads_cv;
        }
        /** Minimum of the mapped reads
         *
         * @return minimum of the mapped reads
         */
        float min_mapped_reads()const
        {
            return m_min_mapped_reads;
        }
        /** Maximum of the mapped reads
         *
         * @return maximum of the mapped reads
         */
        float max_mapped_reads()const
        {
            return m_max_mapped_reads;
        }

    private:
        count_summary_vector_t m_count_summaries;

    private:
        size_t m_total_reads;
        size_t m_total_pf_reads;
        float m_total_fraction_mapped_reads;
        float m_mapped_reads_cv;
        float m_min_mapped_reads;
        float m_max_mapped_reads;
    };

}}}}
