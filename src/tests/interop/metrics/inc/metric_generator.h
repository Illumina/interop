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
#include "src/tests/interop/metrics/inc/format_registry.h"

namespace illumina{ namespace interop { namespace unittest
{
    template<class Gen>
    class hardcoded_metric_generator;
    /** Hardcoded metric registry */
    typedef registry_factory<hardcoded_metric_generator> hardcoded_metric_registry_t;
    template<class Gen>
    class write_read_metric_generator;
    /** Write-read metric registry */
    typedef registry_factory<write_read_metric_generator> write_read_metric_registry_t;
    /** Generate the actual metric set by reading in from hardcoded binary buffer
     *
     * The expected metric set is provided by the generator.
     */
    template<class Gen>
    class hardcoded_metric_generator : public abstract_generator< typename Gen::metric_set_t >
    {
        typedef typename Gen::metric_set_t metric_set_t;
        typedef typename Gen::metric_t metric_t;
        typedef typename abstract_generator<metric_set_t>::parent_type parent_t;
    public:
        /** Constructor */
        hardcoded_metric_generator()
        {
            hardcoded_metric_registry_t::instance()(metric_t(), Gen::VERSION);
        }
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         */
        ::testing::AssertionResult generate(metric_set_t& expected, metric_set_t& actual, bool*)const
        {
            actual.clear();
            Gen::create_expected(expected);
            std::string binary_data;//std::vector< ::uint8_t > binary_data;
            Gen::create_binary_data(binary_data);
            io::read_interop_from_string(binary_data, actual);
            return ::testing::AssertionSuccess();
        }
        /** Create a copy of this object
         *
         * @return pointer to copy
         */
        parent_t clone()const{return new hardcoded_metric_generator<Gen>;}
        /** Write generator info to output stream
         *
         * @param out output stream
         */
        void write(std::ostream& out)const
        {
            out << "hardcoded_metric_generator<" << Gen::name() << ">";
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
        typedef typename Gen::metric_t metric_t;
        typedef typename abstract_generator<metric_set_t>::parent_type parent_t;
    public:
        /** Constructor */
        write_read_metric_generator() : abstract_generator< typename Gen::metric_set_t >(1)
        {
            write_read_metric_registry_t::instance()(metric_t(), Gen::VERSION);
        }
        /** Generate the expected and actual metric sets
         *
         * @param expected expected metric set
         * @param actual actual metric set
         */
        ::testing::AssertionResult generate(metric_set_t& expected, metric_set_t& actual, bool*)const
        {
            actual.clear();
            Gen::create_expected(expected);
            std::ostringstream fout;
            illumina::interop::io::write_metrics(fout, expected);
            //print_actual(std::cout, fout.str());
            io::read_interop_from_string(fout.str(), actual);
            return ::testing::AssertionSuccess();
        }
        /** Create a copy of this object
         *
         * @return pointer to copy
         */
        parent_t clone()const{return new write_read_metric_generator<Gen>;}
        /** Write generator info to output stream
         *
         * @param out output stream
         */
        void write(std::ostream& out)const
        {
            out << "write_read_metric_generator<" << Gen::name() << ">";
        }

    private:
        static void print_actual(std::ostream& out, const std::string& data)
        {
            for(size_t i=0;i<data.size();++i)
            {
                out << "," << int(data[i]);
                if( i%20 == 0) out << std::endl;
            }
            out << std::endl;
            out << std::endl;
            out << std::endl;
        }
    };

    /** Generate the actual metric set writing out the expected and reading it back in again
     *
     * The expected metric set is provided by the generator.
     */
    template<class MetricSet>
    class regression_test_metric_generator : public abstract_regression_test_generator< MetricSet >
    {
        typedef abstract_regression_test_generator< MetricSet > parent_t;
        typedef typename parent_t::base_t base_type;
        typedef MetricSet metric_set_t;
        enum{INCOMPLETE=250, NOT_FOUND=251};
    public:
        typedef typename abstract_regression_test_generator< MetricSet >::parent_type  parent_type;
    public:
        /** Constructor
         */
        regression_test_metric_generator() : parent_t("InterOp", 2)
        {
        }
        /** Constructor
         *
         * @param run_folder run folder
         */
        regression_test_metric_generator(const std::string& run_folder) :
                parent_t(run_folder, "InterOp", 2)
        {
        }

    protected:
        /** Read the expected data from the baseline file into the model
         *
         * @param baseline_file baseline file
         * @param expected expected model data
         * @return true if the file was found, and the read completed without failure
         */
        bool read_expected(const std::string& baseline_file, metric_set_t& expected)const
        {
            expected.clear();
            try
            {
                illumina::interop::io::read_interop(baseline_file, expected);
            }
            // Should never have an incomplete file in baseline, set sentinel to detect
            catch(const io::incomplete_file_exception&){expected.set_version(INCOMPLETE);return true;}
            // Ensure missing file is expected
            catch(const io::file_not_found_exception&){expected.set_version(NOT_FOUND); return false;}
            return true;
        }
        /** Read the actual data from the run folder
         *
         * @param run_folder run folder
         * @param actual actual model data
         */
        bool generate_actual(const std::string& run_folder,  metric_set_t& actual)const
        {
            actual.clear();
            try
            {
                illumina::interop::io::read_interop(run_folder, actual);
            }
            // Ensure missing file is expected
            catch(const io::incomplete_file_exception&){if(actual.empty()) actual.set_version(NOT_FOUND);}
            // Ensure file is missing
            catch(const io::file_not_found_exception&){actual.set_version(NOT_FOUND);}
            return !actual.empty();
        }
        /** Write the actual data to the run folder
         *
         * @param baseline_file baseline file
         * @param actual actual model data
         */
        bool write_actual(const std::string& baseline_file, const metric_set_t& actual)const
        {
            return illumina::interop::io::write_interop(baseline_file, actual);
        }
        /** Create a copy of the current object with the given run folder
         *
         * @param run_folder run folder
         * @return pointer to new copy
         */
        base_type clone(const std::string& run_folder)const
        {
            return new regression_test_metric_generator<MetricSet>(run_folder);
        }
        /** Create a copy of the current object
         *
         * @return pointer to new copy
         */
        base_type clone()const
        {
            return new regression_test_metric_generator<MetricSet>(*this);
        }
        /** Write generator info to output stream
         *
         * @param out output stream
         */
        void write(std::ostream& out)const
        {
            out << "regression_test_metric_generator< "<< metric_set_t::prefix() << metric_set_t::suffix() << "> - "
                    << io::basename(parent_t::m_run_folder);
        }
        /** Get the full path of the baseline output file
         *
         * @return full path
         */
        std::string baseline()const
        {
            return io::dirname(parent_t::baseline());
        }
    };


}}}

