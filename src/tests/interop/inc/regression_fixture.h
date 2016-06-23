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


namespace illumina{ namespace interop { namespace unittest {



    /*
    class regression_test_data
    {
    private:
        regression_test_data()
        {
        }
        regression_test_data(const regression_test_data& data){}

    public:
        static regression_test_data& instance()
        {
            static regression_test_data regression_test_data_instance;
            return regression_test_data_instance;
        }

    public:
        void baseline(const std::string& filename)
        {
            m_baseline = filename;
        }
        const std::string& baseline()
        {
            return m_baseline;
        }
        void files(const std::vector< std::string >& filenames)
        {
            m_files = filenames;
        }
        const std::vector< std::string >& files()
        {
            return m_files;
        }

    private:
        std::string m_baseline;
        std::vector< std::string > m_files;
    };
    class regression_test_fixture
    {
    public:
        regression_test_fixture()
        {
        }

    public:

    };
*/
}}}
