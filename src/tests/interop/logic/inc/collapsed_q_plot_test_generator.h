/** Generate plots for unit testing
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
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/metrics/inc/q_metrics_test.h"
#include "src/tests/interop/run/info_test.h"

namespace illumina { namespace interop { namespace unittest
{
    /** Generate the plot from an empty metric set
     *
     * The expected plot is empty
     */
    template<class PlotData>
    class collapsed_q_plot_test_generator : public abstract_generator<PlotData>
    {
        typedef typename abstract_generator<PlotData>::parent_type base_type;
    public:
        /** Constructor
         *
         * @param plot_type type of the plot
         */
        collapsed_q_plot_test_generator(const constants::plot_types plot_type=constants::UnknownPlotType) :
                m_plot_type(plot_type)
        {
            const model::run::read_info read_array[]={
                    model::run::read_info(1, 1, 26),
                    model::run::read_info(2, 27, 76)
            };
            hiseq4k_run_info::create_expected(m_info, util::to_vector(read_array));
            if(plot_type != constants::UnknownPlotType) m_metric_iterator.reset(m_info, plot_type);
        }
        /** Clone the concrete implementation TODO: Remove
         *
         * @param plot_type type of plot
         * @return copy of this object
         */
        virtual base_type operator()(const constants::plot_types plot_type)const
        {
            return new collapsed_q_plot_test_generator(plot_type);
        }

        /** Generate the expected and actual metric sets
         *
         * @note  expected plot data is empty
         * @param expected expected plot data
         * @param actual actual plot data
         */
        virtual ::testing::AssertionResult generate(PlotData& expected, PlotData &actual, bool*) const
        {
            model::metrics::run_metrics metrics(m_info);
            unittest::q_metric_v6::create_expected(metrics.get<model::metrics::q_metric>());
            metrics.finalize_after_load();
            m_metric_iterator.plot(metrics, expected);

            metrics.get<model::metrics::q_metric>().clear();
            m_metric_iterator.plot(metrics, actual);
            return ::testing::AssertionSuccess();
        }

        /** Create a copy of this object
         *
         * @return pointer to an abstract_generator
         */
        virtual  base_type clone() const
        {
            return new collapsed_q_plot_test_generator(*this);
        }

        /** Write generator info to output stream
         *
         * @param out output stream
         */
        void write(std::ostream &out) const
        {
            out << constants::to_string(m_plot_type);
            out << "Metric_" << constants::to_string(m_metric_iterator.metric()) << "_";
            out << m_metric_iterator.options();
        }
        /** Advance to the next type
         *
         * @return true when the generator has finished, and the next parameter can be obtained
         */
        bool advance()
        {
            return m_metric_iterator.advance();
        }

    protected:
        metric_filter_iterator m_metric_iterator;
        model::run::info m_info;
        constants::plot_types m_plot_type;
    };

}}}

