/** Regression test data singleton
 *
 *
 *  @file
 *  @date 6/23/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include <vector>

namespace illumina{ namespace interop { namespace unittest
{
    /** Singleton containing global data for all tests
     */
    class regression_test_data
    {
    private:
        regression_test_data() : m_rebaseline(false)
        {
        }

        regression_test_data(const regression_test_data &)
        {}

    public:
        /** Get instance of the singleton
         *
         * @return instance
         */
        static regression_test_data &instance()
        {
            static regression_test_data regression_test_data_instance;
            return regression_test_data_instance;
        }

    public:
        /** Set the baseline file path
         *
         * @param filename baseline file path
         */
        void baseline(const std::string &filename)
        {
            m_baseline = filename;
        }

        /** Get the baseline file path
         *
         * @return baseline file path
         */
        const std::string &baseline() const
        {
            return m_baseline;
        }

        /** Add a file to the file list
         *
         * @param filename file name
         */
        void add_file(const std::string &filename)
        {
            m_files.push_back(filename);
        }

        /** Get a vector of files to process
         *
         * @return file vector
         */
        const std::vector <std::string> &files() const
        {
            return m_files;
        }

        /** Set the rebaseline flag
         *
         * If true, create a new baseline
         *
         * @param rebase rebaseline flag
         */
        void rebaseline(const bool rebase)
        {
            m_rebaseline = rebase;
        }

        /** Test if new baseline should be created
         *
         * @return rebaseline flag state
         */
        bool rebaseline() const
        {
            return m_rebaseline;
        }

    private:
        std::string m_baseline;
        std::vector <std::string> m_files;
        bool m_rebaseline;
    };
}}}
