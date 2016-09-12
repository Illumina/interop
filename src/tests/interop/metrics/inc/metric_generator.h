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

namespace illumina{ namespace interop { namespace unittest {

    /** Generate the actual metric set by reading in from hardcoded binary buffer
     *
     * The expected metric set is provided by the generator.
     */
    template<class Gen>
    class hardcoded_metric_generator : public abstract_generator< typename Gen::metric_set_t >
    {
        typedef typename Gen::metric_set_t metric_set_t;
    public:
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         */
        bool generate(metric_set_t& expected, metric_set_t& actual)
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
    };
    /** Generate the actual metric set writing out the expected and reading it back in again
     *
     * The expected metric set is provided by the generator.
     */
    template<class Gen>
    class write_read_metric_generator : public abstract_generator< typename Gen::metric_set_t >
    {
        typedef typename Gen::metric_set_t metric_set_t;
    public:
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         */
        bool generate(metric_set_t& expected, metric_set_t& actual)
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
    };
    /** Generate the actual metric set writing out the expected and reading it back in again
     *
     * The expected metric set is provided by the generator.
     */
    template<class MetricSet>
    class regression_test_metric_generator : public abstract_generator< MetricSet >
    {
        typedef MetricSet metric_set_t;
    public:
        typedef abstract_generator< MetricSet >* parent_type;
    public:
        regression_test_metric_generator(const std::string& test_dir) : m_test_dir(test_dir)
        {
        }
        void operator()(const std::string& name)const
        {
            m_run_folder = name;
        }
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         */
        bool generate(metric_set_t& expected, metric_set_t& actual)
        {
            const regression_test_data& data = regression_test_data::instance();
            const std::string baseline_file = io::combine(io::combine(data.baseline(), m_test_dir), io::basename(m_run_folder));
            if(!data.rebaseline())
            {
                if(io::is_file_readable(baseline_file))
                {
                    expected.clear();
                    actual.clear();
                    illumina::interop::io::read_interop(baseline_file, expected);
                    illumina::interop::io::read_interop(m_run_folder, actual);
                    return true;
                }
                else EXPECT_TRUE(false) << "Failed to find baseline: " << baseline_file;
            }
            else
            {
                std::cout << "[          ] Rebaseline: " << io::basename(m_run_folder) << std::endl;
                actual.clear();
                illumina::interop::io::read_interop(m_run_folder, actual);
                try
                {
                    illumina::interop::io::write_interop(baseline_file, actual);
                }
                catch(const std::exception&)
                {
                    EXPECT_TRUE(false)<< "Failed to write baseline: " << baseline_file;
                }
            }
            return false;
        }
    private:
        mutable std::string m_run_folder;
        std::string m_test_dir;
    };

    /** Generate the actual binary data by writing out the expected metric set
     *
     * The expected binary data is provided by the generator.
     */
    template<class Gen>
    class write_metric_generator : public abstract_generator< std::string >
    {
        typedef typename Gen::metric_set_t metric_set_t;
    public:
        /** Generate the expected and actual metric sets
         *
         * @param expected expected binary buffer
         * @param actual actual binary buffer
         */
        bool generate(std::string& expected, std::string& actual)
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
    };

}}}
