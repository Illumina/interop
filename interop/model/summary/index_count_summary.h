/** Index summary counts
 *
 *  @file
 *  @date  5/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>

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
         * @param fraction_mapped fraction of reads mapped
         * @param count number of indices
         * @param sample_id sample id
         * @param project_name project name
         */
        index_count_summary(const size_t id=0,
                            const std::string& index1="",
                            const std::string index2="",
                            const float fraction_mapped=0.0f,
                            const size_t count=0,
                            const std::string& sample_id="",
                            const std::string& project_name="") : m_id(id),
                                                                  m_index1(index1),
                                                                  m_index2(index2),
                                                                  m_fraction_mapped(fraction_mapped),
                                                                  m_count(count),
                                                                  m_sample_id(sample_id),
                                                                  m_project_name(project_name)
        {}

    public:
        /* Get the index unique identifier (based on a counter)
         *
         * @return index identifier
         */
        size_t id()const
        {
            return m_id;
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
        size_t count()const
        {
            return m_count;
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

    private:
        size_t m_id;
        std::string m_index1;
        std::string m_index2;
        float m_fraction_mapped;
        size_t m_count;
        std::string m_sample_id;
        std::string m_project_name;
    };

}}}}
