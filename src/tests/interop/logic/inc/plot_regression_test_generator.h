/** Generate plots for regression testing
 *
 *
 *  @file
 *  @date 11/3/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "src/tests/interop/logic/inc/metric_filter_iterator.h"
#include "src/tests/interop/inc/abstract_regression_test_generator.h"

namespace illumina { namespace interop { namespace unittest
{

    /** Plot regression test fixture
     *
     */
    template<class PlotData>
    class plot_regression_test_generator : public abstract_regression_test_generator< PlotData >
    {
        typedef abstract_regression_test_generator< PlotData > parent_t;
        typedef typename parent_t::base_t base_type;
    public:
        /** Constructor
         *
         * @param test_dir sub directory containing the tests
         * @param plot_type type of the plot
         */
        plot_regression_test_generator(const std::string& test_dir, const constants::plot_types plot_type) :
                parent_t(test_dir), m_plot_type(plot_type)
        {
            //regression_test_data::instance().add_subdir(test_dir);
        }
        /** Constructor
         *
         * @param run_folder run folder
         * @param test_dir sub directory containing the tests
         * @param plot_type type of the plot
         */
        plot_regression_test_generator(const std::string& run_folder,
                                     const std::string& test_dir,
                                     const constants::plot_types plot_type) :
                parent_t(run_folder, test_dir), m_plot_type(plot_type)
        {
            m_metric_iterator.reset(parent_t::m_run_folder, plot_type);
        }

    public:
        /** Get the full path of the baseline output file
         *
         * @return full path
         */
        std::string baseline()const
        {
            return parent_t::baseline()+"_"+name();
        }

    protected:
        /** Read the actual data from the run folder
         *
         * @param run_folder run folder
         * @param actual actual model data
         * @return true if data was generated
         */
        bool generate_actual(const std::string& run_folder, PlotData& actual)const
        {
            model::metrics::run_metrics& actual_metrics = get_metrics(run_folder);
            if( actual_metrics.empty() ) return false;
            try
            {
                m_metric_iterator.plot(actual_metrics, actual);
            }
            catch(const std::exception& ex)
            {
                std::cerr << "generate_actual: " << ex.what() << std::endl;
                throw;
            }
            return !actual.empty();
        }
        /** Write generator info to output stream
         *
         * @param out output stream
         */
        void write(std::ostream& out)const
        {
            out << name();
        }
        /** Advance to the next type
         *
         * @return true when the generator has finished, and the next parameter can be obtained
         */
        bool advance()
        {
            return m_metric_iterator.advance();
        }
        /** Create a copy of this object
         *
         * @return pointer to an abstract_generator
         */
        base_type clone() const
        {
            return new plot_regression_test_generator(*this);
        }
        /** Create a copy of this object
         *
         * @return pointer to an abstract_generator
         */
        base_type clone(const std::string& run_folder) const
        {
            return new plot_regression_test_generator(run_folder, parent_t::m_test_dir, m_plot_type);
        }
        /** Read the expected data from the baseline file into the model
         *
         * @param baseline_file baseline file
         * @param expected expected model data
         * @return true if the file was found, and the read completed without failure
         */
        bool read_expected(const std::string& baseline_file, PlotData& expected)const
        {
            std::ifstream fin(baseline_file.c_str());
            if( !fin.good() ) return false;
            try
            {
                fin >> expected;
            }
            catch(const std::exception& ex)
            {
                std::cerr << "Failed with exception: " << ex.what() << std::endl;
                return false;
            }
            if( fin.eof() ) return !expected.empty();
            return !fin.fail() && !expected.empty();
        }
        /** Write the actual data to the run folder
         *
         * @param baseline_file baseline file
         * @param actual actual model data
         */
        bool write_actual(const std::string& baseline_file, const PlotData& actual)const
        {
            std::ofstream fout(baseline_file.c_str());
            fout << actual;
            return fout.good();
        }

    private:
        static model::metrics::run_metrics& get_metrics(const std::string& run_folder)
        {
            static std::string current;
            static model::metrics::run_metrics actual_metrics;
            if( current != run_folder )
            {
                current = run_folder;
                actual_metrics.read(run_folder);
            }

            return actual_metrics;
        }

    private:
        std::string name()const
        {
            return constants::to_string(m_plot_type) + "_"
                   + io::basename(parent_t::m_run_folder)
                   +  "_Metric_" + constants::to_string(m_metric_iterator.metric())
                   + "_" + util::lexical_cast<std::string>(m_metric_iterator.options());
        }

    private:
        constants::plot_types m_plot_type;
        metric_filter_iterator m_metric_iterator;
    };


}}}
