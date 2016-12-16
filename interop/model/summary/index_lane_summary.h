/** Index summary for a single lane
 *
 *  @file
 *  @date  5/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <algorithm>
#include "interop/util/cstdint.h"
#include "interop/util/exception.h"
#include "interop/io/format/generic_layout.h"
#include "interop/model/summary/index_count_summary.h"

namespace illumina { namespace interop { namespace model { namespace summary {

    /** Summary of metrics for index reads by lane
     */
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
        /** Define a read count type */
        typedef ::uint64_t read_count_t;

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
        index_lane_summary(const read_count_t total_reads=0,
                           const read_count_t total_pf_reads=0,
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
        /** Resize space for the number of indexes
         *
         * @param n number of indexes
         */
        void resize(const size_type n)
        {
            m_count_summaries.resize(n);
        }
        /** Reserve space for the number of indexes
         *
         * @param n number of indexes
         */
        void reserve(const size_type n)
        {
            m_count_summaries.reserve(n);
        }
        /** Add a new index count summary
         *
         * @todo determine why SWIG does not work with const_reference typedef
         * @param count_summary index count summary
         */
        void push_back(const index_count_summary& count_summary)
        {
            m_count_summaries.push_back(count_summary);
        }
        /** Get reference to lane summary at given index
         *
         * @param n index
         * @return reference to lane summary
         */
        index_count_summary& operator[](const size_type n) throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_count_summaries.size())
                INTEROP_THROW(index_out_of_bounds_exception, "Index sequence index exceeds index sequence count");
            return m_count_summaries[n];
        }
        /** Get constant reference to lane summary at given index
         *
         * @param n index
         * @return constant reference to lane summary
         */
        const index_count_summary& operator[](const size_t n)const throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_count_summaries.size())
                INTEROP_THROW(index_out_of_bounds_exception, "Index sequence  index exceeds index sequence count");
            return m_count_summaries[n];
        }
        /** Get reference to lane summary at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return reference to lane summary
         */
        index_count_summary& at(const size_t n) throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_count_summaries.size())
                INTEROP_THROW(index_out_of_bounds_exception, "Index sequence  index exceeds index sequence count");
            return m_count_summaries[n];
        }
        /** Get constant reference to lane summary at given index
         *
         * @deprecated Will be removed in next feature version (use operator[] instead for C++ Code)
         * @param n index
         * @return constant reference to lane summary
         */
        const_reference at(const size_type n)const throw( model::index_out_of_bounds_exception )
        {
            if(n >= m_count_summaries.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Index sequence  index exceeds index sequence count");
            return m_count_summaries[n];
        }
        /** Get number of summaries by read
         *
         * @return number of summaries by read
         */
        size_type size()const
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
        read_count_t total_reads()const
        {
            return m_total_reads;
        }
        /** Total PF reads
         *
         * @return total PF reads
         */
        read_count_t total_pf_reads()const
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
        /** Coefficient of variation of the mapped reads
         *
         * @return coefficient of variationof the mapped reads
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
        /** Sort the index summaries in each lane
         */
        void sort()
        {
            std::stable_sort(begin(), end());
        }

    public:
        /** Set the data for the lane summary
         *
         * @param total_mapped_reads number of reads mapped
         * @param pf_cluster_count_total number of PF clusters
         * @param cluster_count_total number of clusters
         * @param min_fraction_mapped minimum fraction of reads mapped
         * @param max_fraction_mapped maximum fraction of reads mapped
         * @param cv_fraction_mapped coefficient of variation of fraction of reads mapped
         */
        void set(const ::uint64_t total_mapped_reads,
                 const read_count_t pf_cluster_count_total,
                 const read_count_t cluster_count_total,
                 const float min_fraction_mapped,
                 const float max_fraction_mapped,
                 const float cv_fraction_mapped)
        {
            m_total_reads = cluster_count_total;
            m_total_pf_reads = pf_cluster_count_total;
            m_total_fraction_mapped_reads = (pf_cluster_count_total == 0) ? 0 :
                                            roundf(static_cast<float>(100.0*total_mapped_reads/pf_cluster_count_total)*10000)/10000;
            m_mapped_reads_cv = roundf(cv_fraction_mapped*10000)/10000;
            m_min_mapped_reads = (m_total_reads==0) ? 0 : roundf(min_fraction_mapped*10000)/10000;
            m_max_mapped_reads = (m_total_reads==0) ? 0 : roundf(max_fraction_mapped*10000)/10000;
        }
        /** Clear the lane info
         */
        void clear()
        {
            m_count_summaries.clear();
            m_total_reads = 0;
            m_total_pf_reads = 0;
            m_total_fraction_mapped_reads = 0;
            m_mapped_reads_cv = 0;
            m_min_mapped_reads = 0;
            m_max_mapped_reads = 0;
        }

    private:
        count_summary_vector_t m_count_summaries;

    private:
        read_count_t m_total_reads;
        read_count_t m_total_pf_reads;
        float m_total_fraction_mapped_reads;
        float m_mapped_reads_cv;
        float m_min_mapped_reads;
        float m_max_mapped_reads;

        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

