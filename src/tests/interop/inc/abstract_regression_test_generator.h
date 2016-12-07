/** Classes that generated run metrics and some other model for testing
 *
 *  @file
 *  @date 8/10/16.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/io/metric_file_stream.h"
#include "src/tests/interop/inc/regression_test_data.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"

namespace illumina{ namespace interop { namespace unittest
{
    /** Generate the actual metric set writing out the expected and reading it back in again
     *
     * The expected metric set is provided by the generator.
     */
    template<class Model>
    class abstract_regression_test_generator : public abstract_generator< Model >
    {
    public:
        /** Define the abstract base type */
        typedef typename abstract_generator< Model >::parent_type base_t;
    public:
        /** Constructor
         *
         * @param test_dir test subdir
         * @param test_modifier flag to modify a test
         */
        abstract_regression_test_generator(const std::string& test_dir, const int test_modifier=0) :
                abstract_generator< Model >(test_modifier), m_test_dir(test_dir)
        {
            regression_test_data::instance().add_subdir(test_dir);
        }
        /** Constructor
         *
         * @param run_folder run folder
         * @param test_dir test subdir
         * @param test_modifier flag to modify a test
         */
        abstract_regression_test_generator(const std::string& run_folder,
                                           const std::string& test_dir,
                                           const int test_modifier=0) :
                abstract_generator< Model >(test_modifier), m_run_folder(run_folder), m_test_dir(test_dir)
        {
        }
        /** Clone the concrete implementation
         *
         * @param name run folder
         * @return copy of this object
         */
        base_t operator()(const std::string& name)const
        {
            m_run_folder=name;
            return clone(name);
        }
        /** Clone the concrete implementation
         *
         * @param name run folder
         * @return copy of this object
         */
        virtual base_t clone(const std::string& name)const=0;
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         * @param skip_test flag to set if the test should be skipped
         * @return success if the results should be tested
         */
        ::testing::AssertionResult generate(Model& expected, Model& actual, bool* skip_test)const
        {
            const regression_test_data& data = regression_test_data::instance();
            const std::string baseline_file = baseline();
            ::testing::Message msg;
            if(!data.rebaseline())
            {
                const bool expected_found = read_expected(baseline_file, expected);
                const bool actual_generated = generate_actual(m_run_folder, actual);
                if(expected_found == actual_generated)
                {
                    *skip_test = !expected_found;
                    return ::testing::AssertionSuccess();
                }
                if(!expected_found)
                    return ::testing::AssertionFailure()  << "Failed to find baseline: " << baseline_file;
                return ::testing::AssertionFailure()  << "Failed to generate data for baseline: " << baseline_file;
            }
            else
            {
                try
                {
                    if(generate_actual(m_run_folder, actual))
                    {
                        msg << "[          ] Rebaseline: " << *this;
                        if(!write_actual(baseline_file, actual))
                            return ::testing::AssertionFailure() << "Failed to write baseline: " << baseline_file;
                    }
                    else
                    {
                        msg << "[          ] Skipped: " << *this;
                    }
                    *skip_test = true;
                }
                catch(const std::exception& ex)
                {
                    std::cerr << "Failed to generate baseline: " << baseline_file << " " << ex.what() << std::endl;
                    std::exit(1);
                    //return ::testing::AssertionFailure()  << "Failed to generate baseline: " << baseline_file << " " << ex.what();
                }
            }
            return ::testing::AssertionFailure(msg);
        }
        /** Get the full path of the baseline output file
         *
         * @return full path
         */
        virtual std::string baseline()const
        {
            return io::combine(io::combine(io::combine(regression_test_data::instance().baseline(), io::basename(m_run_folder)), m_test_dir), "baseline");
        }

    protected:
        /** Read the expected data from the baseline file into the model
         *
         * @param baseline_file baseline file
         * @param expected expected model data
         * @return true if the file was found, and the read completed without failure
         */
        virtual bool read_expected(const std::string& baseline_file, Model& expected)const=0;
        /** Read the actual data from the run folder
         *
         * @param run_folder run folder
         * @param actual actual model data
         * @return true if data was generated
         */
        virtual bool generate_actual(const std::string& run_folder, Model& actual)const=0;
        /** Write the actual data to the run folder
         *
         * @param baseline_file baseline file
         * @param actual actual model data
         */
        virtual bool write_actual(const std::string& baseline_file, const Model& actual)const=0;
        /** Create a copy of this object
         *
         * @return pointer to copy
         */
        virtual base_t clone()const=0;

    protected:
        /** Run Folder name */
        mutable std::string m_run_folder;
        /** Test output dir */
        std::string m_test_dir;
    };



}}}

