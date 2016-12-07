/** Iterator over all possible filters and metrics
 *
 *
 *  @file
 *  @date 10/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "interop/model/run/info.h"
#include "interop/model/plot/filter_options.h"
#include "interop/logic/plot/plot_by_cycle.h"
#include "interop/logic/plot/plot_by_lane.h"
#include "interop/logic/plot/plot_flowcell_map.h"
#include "interop/logic/plot/plot_qscore_heatmap.h"
#include "interop/logic/plot/plot_qscore_histogram.h"
#include "interop/logic/plot/plot_sample_qc.h"

namespace illumina { namespace interop { namespace unittest
{
    /** Iterator over metric types and filter options
     */
    class metric_filter_iterator
    {
        typedef std::vector <logic::utils::metric_type_description_t> metric_type_vector_t;
        typedef metric_type_vector_t::const_iterator const_metric_type_iterator_t;
    public:
        /** Constructor */
        metric_filter_iterator() : m_options(constants::UnknownTileNamingMethod),
                                   m_current_metric(m_metric_types.begin()),
                                   m_plot_type(constants::UnknownPlotType)
        {
        }

    public:
        /** Copy constructor
         *
         * @param source source iterator to copy
         */
        metric_filter_iterator(const metric_filter_iterator& source) :
                m_metric_types(source.m_metric_types),
                m_options(source.m_options),
                m_run_info(source.m_run_info),
                m_plot_type(source.m_plot_type)
        {
            m_current_metric = m_metric_types.begin() + std::distance(source.m_metric_types.begin(), source.m_current_metric);
            m_current_filter = m_options.option_iterator(m_run_info, *m_current_metric, m_plot_type, true);
        }

    public:
        /** Reset iterator ranges and set to start
         *
         * @param run_info_filename path to run info xml
         * @param plot_type type of the plot
         */
        void reset(const std::string &run_info_filename, const constants::plot_types plot_type)
        {
            model::metrics::run_metrics run;
            try
            {
                run.read(run_info_filename);
            }
            catch (const std::exception &){}
            reset(run.run_info(), plot_type);
        }
        /** Plot a run by lane or cycle
         *
         * @param run run metrics
         * @param data candle stick plot data
         */
        void plot(model::metrics::run_metrics& run, model::plot::plot_data<model::plot::candle_stick_point>& data)const
        {
            model::plot::plot_data<model::plot::candle_stick_point> unused;
            switch(m_plot_type)
            {
                case constants::ByCyclePlot:
                    logic::plot::plot_by_cycle(run, *m_current_metric, m_options, data);
                    break;
                case constants::ByLanePlot:
                    logic::plot::plot_by_lane(run, *m_current_metric, m_options, data);
                    break;
                default:
                    INTEROP_THROW(std::runtime_error, "Plot type unsupported");
            };
        }
        /** Plot a histogram of the q-scores
         *
         * @param run run metrics
         * @param data bar plot data
         */
        void plot(model::metrics::run_metrics& run, model::plot::plot_data<model::plot::bar_point>& data)const
        {
            switch(m_plot_type)
            {
                case constants::QHistogramPlot:
                    logic::plot::plot_qscore_histogram(run, m_options, data);
                    break;
                case constants::SampleQCPlot:
                    logic::plot::plot_sample_qc(run, m_options.lane(), data);
                    break;
                default:
                    INTEROP_THROW(std::runtime_error, "Plot type unsupported");
            };
        }
        /** Plot a q-score heat map of the run
         *
         * @param run run metrics
         * @param data heatmap plot data
         */
        void plot(model::metrics::run_metrics& run, model::plot::heatmap_data& data)const
        {
            switch(m_plot_type)
            {
                case constants::QHeatmapPlot:
                    logic::plot::plot_qscore_heatmap(run, m_options, data);
                    break;
                default:
                    INTEROP_THROW(std::runtime_error, "Plot type unsupported");
            };
        }
        /** Plot a flow cell of the run
         *
         * @param run run metrics
         * @param data flowcell plot data
         */
        void plot(model::metrics::run_metrics& run, model::plot::flowcell_data& data)const
        {
            switch(m_plot_type)
            {
                case constants::FlowcellPlot:
                    logic::plot::plot_flowcell_map(run, *m_current_metric, m_options, data);
                    break;
                default:
                    INTEROP_THROW(std::runtime_error, "Plot type unsupported");
            };
        }

        /** Reset iterator ranges and set to start
         *
         * @param run_info run info
         * @param plot_type type of the plot
         */
        void reset(const model::run::info &run_info, const constants::plot_types plot_type)
        {
            m_run_info = run_info;
            m_plot_type = plot_type;

            m_metric_types.clear();
            switch(plot_type)
            {
                case constants::ByCyclePlot:
                    logic::plot::list_by_cycle_metrics(m_metric_types);
                    break;
                case constants::ByLanePlot:
                    logic::plot::list_by_lane_metrics(m_metric_types);
                    break;
                case constants::FlowcellPlot:
                    logic::plot::list_flowcell_metrics(m_metric_types);
                    break;
                case constants::QHeatmapPlot:
                case constants::QHistogramPlot:
                    m_metric_types.push_back(logic::utils::metric_type_description_t(constants::Q30Percent, ""));
                    break;
                case constants::SampleQCPlot:
                    // Sentinel (turn off surface filtering)
                    m_metric_types.push_back(logic::utils::metric_type_description_t(constants::UnknownMetricType, ""));
                    break;
                default:
                    INTEROP_THROW(std::runtime_error, "Plot type unsupported");
            };

            // Reduce the number of filters!
            model::run::flowcell_layout layout = run_info.flowcell();
            layout.lane_count(1); // TODO: increase to test filtering?
            layout.swath_count(1);
            layout.tile_count(1);
            layout.sections_per_lane(1);
            layout.lanes_per_section(1);
            m_run_info.flowcell(layout);
            std::string channels[] = {"Red", "Green"};
            m_run_info.channels(util::to_vector(channels));
            const model::run::read_info reads[] = {model::run::read_info(1, 1, 1, false)};
            m_run_info.reads(util::to_vector(reads));

            m_plot_type = plot_type;
            m_options.tile_naming_method(m_run_info.flowcell().naming_method());
            m_current_metric = m_metric_types.begin();
            if (m_current_metric == m_metric_types.end())
                INTEROP_THROW(std::runtime_error, "Metric iterator started exhausted");
            m_current_filter = m_options.option_iterator(m_run_info, *m_current_metric, plot_type);
        }
        /** Get current metric
         *
         * @return  current metric
         */
        constants::metric_type metric() const
        {
            if (m_current_metric == m_metric_types.end())
                INTEROP_THROW(std::runtime_error, "Metric iterator started exhausted");
            return *m_current_metric;
        }

        /** Get filter options
         *
         * @return filter options
         */
        const model::plot::filter_options &options() const
        {
            return m_options;
        }

        /** Advance to the next type
         *
         * @return true when the generator has finished, and the next parameter can be obtained
         */
        bool advance()
        {
            if (m_current_metric == m_metric_types.end())
                return true;
            ++m_current_filter;
            if (m_current_filter.is_done())
            {
                ++m_current_metric;
                if( m_current_metric != m_metric_types.end())
                    m_current_filter = m_options.option_iterator(m_run_info, *m_current_metric, m_plot_type);
            }
            return m_current_metric == m_metric_types.end();
        }
        /** Test if the iterator is exhausted
         *
         * @return true when the generator has finished
         */
        bool is_done()
        {
            return m_current_metric == m_metric_types.end();
        }
        /** Get a vector of metric types
         *
         * @return vector of metric types
         */
        metric_type_vector_t& metric_types()
        {
            return m_metric_types;
        }

    protected:
        /** Reset the metric iterator */
        void reset()
        {
            m_current_metric = m_metric_types.begin();
        }

    protected:
        /** Vector of metrics */
        metric_type_vector_t m_metric_types;
        /** Filter options */
        model::plot::filter_options m_options;
    private:
        util::chain_range_iterator m_current_filter;
        const_metric_type_iterator_t m_current_metric;
        model::run::info m_run_info;
        constants::plot_types m_plot_type;
    };

}}}


