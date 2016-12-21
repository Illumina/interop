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

namespace illumina { namespace interop { namespace unittest
{
    /** Generate the plot from an empty metric set
     *
     * The expected plot is empty
     */
    template<class PlotData>
    class empty_plot_test_generator : public abstract_generator<PlotData>
    {
        typedef typename abstract_generator<PlotData>::parent_type base_type;
    public:
        /** Constructor
         *
         * @param plot_type type of the plot
         */
        empty_plot_test_generator(const constants::plot_types plot_type=constants::UnknownPlotType) :
                m_info(model::run::flowcell_layout(1, 2)),
                m_plot_type(plot_type)
        {
            std::string channels[] = {"Red", "Green"};
            m_info.channels(util::to_vector(channels));
            m_info.set_naming_method(constants::FourDigit);
            const model::run::read_info reads[] = {model::run::read_info(1, 1, 3, false)};
            m_info.reads(util::to_vector(reads));
            if(plot_type != constants::UnknownPlotType) m_metric_iterator.reset(m_info, plot_type);
        }
        /** Clone the concrete implementation TODO: Remove
         *
         * @param plot_type type of plot
         * @return copy of this object
         */
        base_type operator()(const constants::plot_types plot_type)const
        {
            return new empty_plot_test_generator(plot_type);
        }

        /** Generate the expected and actual metric sets
         *
         * @note  expected plot data is empty
         * @param actual actual plot data
         */
        ::testing::AssertionResult generate(PlotData&, PlotData &actual, bool*) const
        {
            model::metrics::run_metrics metrics;
            metrics.run_info(m_info);
            m_metric_iterator.plot(metrics, actual);
            return ::testing::AssertionSuccess();
        }

        /** Create a copy of this object
         *
         * @return pointer to an abstract_generator
         */
        base_type clone() const
        {
            return new empty_plot_test_generator(*this);
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

    private:
        metric_filter_iterator m_metric_iterator;
        model::run::info m_info;
        constants::plot_types m_plot_type;
    };


}}}

