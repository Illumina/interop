/** Fixture to reuse unit tests for regression testing
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
#include <gtest/gtest.h>
#include "persistent_parameter_generator.h"
#include "interop/util/filesystem.h"
#include "interop/model/run_metrics.h"


namespace illumina{ namespace interop { namespace unittest {


    /** Singleton containing global data for all tests
     */
    class regression_test_data
    {
    private:
        regression_test_data() : m_rebaseline(false)
        {
        }
        regression_test_data(const regression_test_data&){}

    public:
        /** Get instance of the singleton
         *
         * @return instance
         */
        static regression_test_data& instance()
        {
            static regression_test_data regression_test_data_instance;
            return regression_test_data_instance;
        }

    public:
        /** Set the baseline file path
         *
         * @param filename baseline file path
         */
        void baseline(const std::string& filename)
        {
            m_baseline = filename;
        }
        /** Get the baseline file path
         *
         * @return baseline file path
         */
        const std::string& baseline()const
        {
            return m_baseline;
        }
        /** Add a file to the file list
         *
         * @param filename file name
         */
        void add_file(const std::string& filename)
        {
            m_files.push_back(filename);
        }
        /** Get a vector of files to process
         *
         * @return file vector
         */
        const std::vector< std::string >& files()const
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
        bool rebaseline()const
        {
            return m_rebaseline;
        }

    private:
        std::string m_baseline;
        std::vector< std::string > m_files;
        bool m_rebaseline;

    };


    /** Convert an array to a vector
     *
     * Determines the length of the stack array automatically.
     *
     * @param vals array pointer
     * @return vector of values
     */
    template<typename T, size_t N>
    inline std::vector<T> to_vector(const T (&vals)[N])
    {
        return std::vector<T>(vals, vals + N);
    }

    /** Regression test fixture
     *
     * This parameter based fixture takes a set of run folders and populates the baseline for later testing.
     * Rebaseling the test is also supported through a command line argument.
     */
    template<class Fixture, class T>
    struct regression_test_fixture : public ::testing::TestWithParam< std::string >
    {
        /** Constructor */
        regression_test_fixture(const char* test_dir) : test(false)
        {
            run_folder = GetParam();
            const regression_test_data& data = regression_test_data::instance();
            const std::string baseline_file = io::combine(io::combine(data.baseline(), test_dir), io::basename(run_folder));

            model::metrics::run_metrics actual_metrics;
            EXPECT_NO_THROW(actual_metrics.read(run_folder)) << "Failed to find run folder: " << run_folder;
            actual.clear();
            Fixture::populate_actual(actual_metrics, actual);

            if(!data.rebaseline())
            {
                if(io::is_file_readable(baseline_file))
                {
                    expected.clear();
                    std::ifstream fin(baseline_file.c_str());
                    try
                    {
                        fin >> expected;
                    }
                    catch (const std::exception &)
                    { }
                    test = true;
                }
                else EXPECT_TRUE(false) << "Failed to find baseline: " << baseline_file;
            }
            else
            {
                std::cout << "[          ] Rebaseline: " << io::basename(run_folder) << std::endl;
                std::ofstream fout(baseline_file.c_str());
                try
                {
                    fout << actual;
                }
                catch(const std::exception&){}
                EXPECT_TRUE(fout.good()) << "Failed to write baseline: " << baseline_file;
            }
        }

        /** Run folder file path */
        std::string run_folder;
        /** Expected object to test */
        T expected;
        /** Actual object to test */
        T actual;
        /** True if in test mode, otherwise rebaseline mode */
        bool test;
    };

}}}
