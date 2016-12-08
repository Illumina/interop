/** Index summary counts
 *
 *  @file
 *  @date  5/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include "interop/util/math.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace summary {

    /** Summary of index counts
     */
    class index_count_summary
    {
    public:
        /** Constructor
         *
         * @param id index unique identifier (based on a counter)
         * @param index1 index sequence 1
         * @param index2 index sequence 2
         * @param sample_id sample id
         * @param project_name project name
         * @param count number of indices
         * @param fraction_mapped fraction of reads mapped
         */
        index_count_summary(const size_t id=0,
                            const std::string& index1="",
                            const std::string& index2="",
                            const std::string& sample_id="",
                            const std::string& project_name="",
                            const ::uint64_t count=0,
                            const float fraction_mapped=0.0f) : m_id(id),
                                                                  m_index1(index1),
                                                                  m_index2(index2),
                                                                  m_fraction_mapped(fraction_mapped),
                                                                  m_cluster_count(count),
                                                                  m_sample_id(sample_id),
                                                                  m_project_name(project_name)
        {}

    public:
        /** Get the index unique identifier (based on a counter)
         *
         * @return index identifier
         */
        size_t id()const
        {
            return m_id;
        }
        /** Set the index unique identifier (based on a counter)
         *
         * @param n index identifier
         */
        void id(const size_t n)
        {
            m_id = n;
        }
        /** Index sequence 1
         *
         * @return index sequence
         */
        const std::string& index1()const
        {
            return m_index1;
        }
        /** Index sequence 2
         *
         * @return index sequence
         */
        const std::string& index2()const
        {
            return m_index2;
        }
        /** Fraction of reads mapped
         *
         * @return fraction of reads mapped
         */
        float fraction_mapped()const
        {
            return m_fraction_mapped;
        }
        /** Number of clusters that have this index sequence
         *
         * @return cluster count
         */
        ::uint64_t cluster_count()const
        {
            return m_cluster_count;
        }
        /** Sample id
         *
         * @return sample id
         */
        const std::string& sample_id()const
        {
            return m_sample_id;
        }
        /** Project name
         *
         * @return project name
         */
        const std::string& project_name()const
        {
            return m_project_name;
        }

    public:
        /** Update the count
         *
         * @param cluster_count count to add
         * @return reference to this object
         */
        index_count_summary& operator+=(const ::uint64_t cluster_count)
        {
            m_cluster_count += cluster_count;
            return *this;
        }
        /** Update the fraction mapped from the total PF cluster count
         *
         * @param total_pf_cluster_count total PF cluster count
         */
        void update_fraction_mapped(const double total_pf_cluster_count)
        {
            if(total_pf_cluster_count != 0.0f)
                m_fraction_mapped = roundf(static_cast<float>(m_cluster_count / total_pf_cluster_count * 100.0)*10000)/10000;
        }
        /** Compare two index count summaries by their ids
         *
         * @param rhs index_count_summary on the right hand side (rhs) of the < symbol
         * @return true if this is less than rhs
         */
        bool operator<(const index_count_summary& rhs)const
        {
            return id() < rhs.id();
        }

    private:
        size_t m_id;
        std::string m_index1;
        std::string m_index2;
        float m_fraction_mapped;
        ::uint64_t m_cluster_count;
        std::string m_sample_id;
        std::string m_project_name;


        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

