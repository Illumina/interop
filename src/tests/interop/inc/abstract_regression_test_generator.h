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
        typedef typename abstract_generator< Model >::base_type base_type;
    public:
        /** Constructor
         *
         * @param test_dir test subdir
         */
        abstract_regression_test_generator(const std::string& test_dir) : m_test_dir(test_dir)
        {
            regression_test_data::instance().add_subdir(test_dir);
        }
        /** Constructor
         *
         * @param run_folder run folder
         * @param test_dir test subdir
         */
        abstract_regression_test_generator(const std::string& run_folder, const std::string& test_dir) : m_run_folder(run_folder), m_test_dir(test_dir)
        {
        }
        /** Clone the concret implementation
         *
         * @param name run folder
         * @return copy of this object
         */
        base_type operator()(const std::string& name)const
        {
            m_run_folder=name;
            return clone();
        }
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         * @return true if the results should be tested (false if rebaselining)
         */
        bool generate(Model& expected, Model& actual)const
        {
            const regression_test_data& data = regression_test_data::instance();
            const std::string baseline_file = baseline();
            if(!data.rebaseline())
            {
                if(io::is_file_readable(baseline_file))
                {
                    read_expected(baseline_file, expected);
                    generate_actual(m_run_folder, actual);
                    return true;
                }
                else EXPECT_TRUE(false) << "Failed to find baseline: " << baseline_file;
            }
            else
            {
                std::cout << "[          ] Rebaseline: " << io::basename(m_run_folder) << std::endl;
                try
                {
                    generate_actual(m_run_folder, actual);
                    EXPECT_TRUE(write_actual(baseline_file, actual)) << "Failed to write baseline: " << baseline_file;
                }
                catch(const std::exception& ex)
                {
                    EXPECT_TRUE(false)<< "Failed to generate baseline: " << baseline_file << " " << ex.what();
                }
            }
            return false;
        }
        /** Get the full path of the baseline output file
         *
         * @return full path
         */
        std::string baseline()const
        {
            return io::combine(io::combine(regression_test_data::instance().baseline(), m_test_dir), io::basename(m_run_folder));
        }

    protected:
        /** Read the expected data from the baseline file into the model
         *
         * @param baseline_file baseline file
         * @param expected expected model data
         */
        virtual void read_expected(const std::string& baseline_file, Model& expected)const=0;
        /** Read the actual data from the run folder
         *
         * @param run_folder run folder
         * @param actual actual model data
         */
        virtual void generate_actual(const std::string& run_folder, Model& actual)const=0;
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
        virtual base_type clone()const=0;

    protected:
        /** Run Folder name */
        mutable std::string m_run_folder;
        /** Test output dir */
        std::string m_test_dir;
    };



}}}
