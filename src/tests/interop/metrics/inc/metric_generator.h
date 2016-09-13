/** Classes that generated actual and expected metric sets for testing
 *
 *  @file
 *  @date 8/10/16.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/io/metric_file_stream.h"
#include "src/tests/interop/inc/regression_test_data.h"
#include "src/tests/interop/inc/abstract_regression_test_generator.h"

namespace illumina{ namespace interop { namespace unittest {

    /** Generate the actual metric set by reading in from hardcoded binary buffer
     *
     * The expected metric set is provided by the generator.
     */
    template<class Gen>
    class hardcoded_metric_generator : public abstract_generator< typename Gen::metric_set_t >
    {
        typedef typename Gen::metric_set_t metric_set_t;
        typedef typename abstract_generator<metric_set_t>::base_type parent_t;
    public:
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         */
        bool generate(metric_set_t& expected, metric_set_t& actual)const
        {
            actual.clear();
            Gen::create_expected(expected);
            std::vector< ::uint8_t > binary_data;
            Gen::create_binary_data(binary_data);
            try
            {
                io::read_interop_from_string(binary_data,
                                             actual);
            }
            catch (const std::exception &) { }
            return true;
        }
        /** Create a copy of this object
         *
         * @return pointer to copy
         */
        parent_t clone()const{return new hardcoded_metric_generator<Gen>;}

    };
    /** Generate the actual metric set writing out the expected and reading it back in again
     *
     * The expected metric set is provided by the generator.
     */
    template<class Gen>
    class write_read_metric_generator : public abstract_generator< typename Gen::metric_set_t >
    {
        typedef typename Gen::metric_set_t metric_set_t;
        typedef typename abstract_generator<metric_set_t>::base_type parent_t;
    public:
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         */
        bool generate(metric_set_t& expected, metric_set_t& actual)const
        {
            actual.clear();
            Gen::create_expected(expected);
            std::ostringstream fout;
            try
            {
                illumina::interop::io::write_metrics(fout, expected);
            }
            catch (const std::exception &) { }
            try
            {
                io::read_interop_from_string(fout.str(),
                                             actual);
            }
            catch (const std::exception &) { }
            return true;
        }
        /** Create a copy of this object
         *
         * @return pointer to copy
         */
        parent_t clone()const{return new write_read_metric_generator<Gen>;}
    };
    /** Generate the actual metric set writing out the expected and reading it back in again
     *
     * The expected metric set is provided by the generator.
     */
    template<class MetricSet>
    class regression_test_metric_generator : public abstract_regression_test_generator< MetricSet >
    {
        typedef abstract_regression_test_generator< MetricSet > parent_t;
        typedef typename parent_t::base_type base_type;
        typedef MetricSet metric_set_t;
    public:
        typedef typename abstract_regression_test_generator< MetricSet >::parent_type  parent_type;
    public:
        regression_test_metric_generator(const std::string& test_dir) : parent_t(test_dir)
        {
        }
        regression_test_metric_generator(const std::string& run_folder, const std::string& test_dir) : parent_t(run_folder, test_dir)
        {
        }

    protected:
        /** Read the expected data from the baseline file into the model
         *
         * @param baseline_file baseline file
         * @param expected expected model data
         */
        void read_expected(const std::string& baseline_file, metric_set_t& expected)const
        {
            expected.clear();
            try
            {
                illumina::interop::io::read_interop(baseline_file, expected);
            }
            catch(const io::incomplete_file_exception&){}
            catch(const io::file_not_found_exception&){}
        }
        /** Read the actual data from the run folder
         *
         * @param run_folder run folder
         * @param actual actual model data
         */
        void generate_actual(const std::string& run_folder,  metric_set_t& actual)const
        {
            actual.clear();
            try
            {
                illumina::interop::io::read_interop(run_folder, actual);
            }
            catch(const io::incomplete_file_exception&){}
        }
        /** Write the actual data to the run folder
         *
         * @param baseline_file baseline file
         * @param actual actual model data
         */
        bool write_actual(const std::string& baseline_file, const metric_set_t& actual)const
        {
            io::mkdir(parent_t::baseline());
            io::mkdir(io::combine(parent_t::baseline(), "InterOp"));
            return illumina::interop::io::write_interop(baseline_file, actual);
        }
        /** Create a copy of the current object
         *
         * @return pointer to new copy
         */
        base_type clone()const
        {
            return new regression_test_metric_generator<MetricSet>(parent_t::m_run_folder, parent_t::m_test_dir);
        }
    };

    /** Generate the actual binary data by writing out the expected metric set
     *
     * The expected binary data is provided by the generator.
     */
    template<class Gen>
    class write_metric_generator : public abstract_generator< std::string >
    {
        typedef typename Gen::metric_set_t metric_set_t;
        typedef typename abstract_generator<std::string>::base_type parent_t;
    public:
        /** Generate the expected and actual metric sets
         *
         * @param expected expected binary buffer
         * @param actual actual binary buffer
         */
        bool generate(std::string& expected, std::string& actual)const
        {
            Gen::create_binary_data(expected);
            metric_set_t metrics;
            Gen::create_expected(metrics);
            std::ostringstream fout;
            try
            {
                illumina::interop::io::write_metrics(fout, metrics);
            }
            catch (const std::exception &) { }
            actual = fout.str();
            return true;
        }
        /** Create a copy of this object
         *
         * @return pointer to copy
         */
        parent_t clone()const{return new write_metric_generator<Gen>;}
    };

}}}
