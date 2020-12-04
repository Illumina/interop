/** Plot an arbitrary cycle metric by cycle
 *
 *  @file
 *  @date  4/28/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/plot/plot_by_cycle.h"
#include "interop/logic/plot/plot_point.h"
#include "interop/logic/plot/plot_data.h"
#include "interop/logic/metric/q_metric.h"
#include "interop/logic/plot/plot_metric_proxy.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{
    /** Plot the average over all tiles of a specific metric by cycle
     */
    template<class Point>
    class by_cycle_average_plot
    {
    public:
        /** Constructor
         *
         * @param points reference to collection of points
         */
        by_cycle_average_plot(model::plot::data_point_collection<Point>&  points) :
                m_points(points), m_max_cycle(0), m_empty(true){}

        /** Plot the average over all tiles of a specific metric by cycle
         *
         * @param metrics set of metric records
         * @param options filter for metric records
         * @param proxy functor that takes a metric record and returns a metric value
         */
        template<typename MetricSet, typename MetricProxy>
        void operator()(const MetricSet& metrics,
                        const model::plot::filter_options& options,
                        const MetricProxy& proxy)
        {
            typedef typename MetricSet::base_t base_t;
            plot(metrics, options, proxy, base_t::null());
        }
        /** Get the maximum cycle in the metric set
         *
         * @return maximum cycle
         */
        size_t max_cycle()const
        {
            return m_max_cycle;
        }
        /** Test whether metric set was empty
         *
         * @return true if metric set was empty
         */
        bool empty()const
        {
            return m_empty;
        }

    private:
        template<typename MetricSet, typename MetricProxy>
        void plot(const MetricSet& metrics,
                  const model::plot::filter_options& options,
                  const MetricProxy& proxy,
                  const constants::base_cycle_t*)
        {
            m_max_cycle = metrics.max_cycle();
            m_empty = metrics.empty();
            m_points.assign(m_max_cycle, Point());
            const float dummy_x = 1;
            for(typename MetricSet::const_iterator b = metrics.begin(), e = metrics.end();b != e;++b)
            {
                if(!options.valid_tile(*b)) continue;
                const float val = proxy(*b);
                if(std::isnan(val) || std::isinf(val)) continue;
                m_points[b->cycle()-1].add(dummy_x, val);
            }
            size_t index = 0;
            for(size_t cycle=0;cycle<m_max_cycle;++cycle)
            {
                if(static_cast<size_t>(m_points[cycle].x()) == 0) continue;
                const float avg = m_points[cycle].y()/m_points[cycle].x();
                m_points[index].set(static_cast<float>(cycle+1), avg);
                ++index;
            }
            m_points.resize(index);
        }
        template<typename MetricSet, typename MetricProxy>
        void plot(const MetricSet&,
                  const model::plot::filter_options&,
                  const MetricProxy&,
                  const void*){}


    private:
        model::plot::data_point_collection<Point>& m_points;
        size_t m_max_cycle;
        bool m_empty;
    };


    /** Plot the candle stick over all tiles of a specific metric by cycle
     *
     * @param metrics set of metric records
     * @param proxy functor that takes a metric record and returns a metric value
     * @param options filter for metric records
     * @param type type of metric to extract using the proxy functor
     * @param points collection of points where x is cycle number and y is the candle stick metric values
     * @return last populated cycle
     */
    template<class Point>
    class by_cycle_candle_stick_plot
    {
    public:
        /** Constructor
         *
         * @param points reference to collection of points
         */
        by_cycle_candle_stick_plot(model::plot::data_point_collection<Point>&  points) :
            m_points(points), m_max_cycle(0), m_empty(true){}


        /** Plot the candle stick over all tiles of a specific metric by cycle
         *
         * @param metrics set of metric records
         * @param options filter for metric records
         * @param proxy functor that takes a metric record and returns a metric value
         */
        template<typename MetricSet, typename MetricProxy>
        void operator()(const MetricSet& metrics,
                        const model::plot::filter_options& options,
                        const MetricProxy& proxy)
        {
            typedef typename MetricSet::base_t base_t;
            plot(metrics, options, proxy, base_t::null());
        }
        /** Get the maximum cycle in the metric set
         *
         * @return maximum cycle
         */
        size_t max_cycle()const
        {
            return m_max_cycle;
        }
        /** Test whether metric set was empty
         *
         * @return true if metric set was empty
         */
        bool empty()const
        {
            return m_empty;
        }
    private:
        /** Plot the candle stick over all tiles of a specific metric by cycle
         *
         * @param metrics set of metric records
         * @param options filter for metric records
         * @param proxy functor that takes a metric record and returns a metric value
         */
        template<typename MetricSet, typename MetricProxy>
        void plot(const MetricSet& metrics,
                  const model::plot::filter_options& options,
                  const MetricProxy& proxy,
                  const constants::base_cycle_t*)
        {
            m_max_cycle= metrics.max_cycle();
            m_empty = metrics.empty();
            const size_t tile_count = static_cast<size_t>(std::ceil(static_cast<float>(metrics.size())/m_max_cycle));
            std::vector< std::vector<float> > tile_by_cycle(m_max_cycle);
            for(size_t i=0;i<m_max_cycle;++i) tile_by_cycle[i].reserve(tile_count);
            std::vector<float> outliers;
            outliers.reserve(10); // TODO: use as flag for keeping outliers

            for(typename MetricSet::const_iterator b = metrics.begin(), e = metrics.end();b != e;++b)
            {
                if(!options.valid_tile(*b)) continue;
                const float val = proxy(*b);
                if(std::isnan(val) || std::isinf(val)) continue;
                tile_by_cycle[b->cycle()-1].push_back(val);
            }
            m_points.resize(m_max_cycle);
            size_t j=0;
            for(size_t cycle=0;cycle<m_max_cycle;++cycle)
            {
                if(tile_by_cycle[cycle].empty())
                    continue;
                plot_candle_stick(m_points[j],
                                  tile_by_cycle[cycle].begin(),
                                  tile_by_cycle[cycle].end(),
                                  static_cast<float>(cycle+1),
                                  outliers);
                ++j;
            }
            m_points.resize(j);
        }
        template<typename MetricSet, typename MetricProxy>
        void plot(const MetricSet&,
                  const model::plot::filter_options&,
                  const MetricProxy&,
                  const void*){}
    private:
        model::plot::data_point_collection<Point>& m_points;
        size_t m_max_cycle;
        bool m_empty;
    };

    /** Generate meta data for multiple plot series that compare data by channel
     *
     * @param channels collection of channel names
     * @param data plot data that holds a collection of plot series
     */
    template<class Point>
    void setup_series_by_channel(const std::vector<std::string>& channels, model::plot::plot_data<Point>& data)
    {
        typedef model::plot::series<Point> series_t;
        data.resize(channels.size());
        std::vector<size_t> expected_order(channels.size());
        utils::actual2expected(channels, expected_order);
        for(size_t i=0;i<data.size();++i)
        {
            INTEROP_ASSERT(expected_order[i] < constants::PlotColorCount);
            data[i] = series_t(
                    channels[i],
                    constants::to_string(static_cast<constants::plot_colors>(expected_order[i])),
                    series_t::Line);
        }
    }
    /** Generate meta data for multiple plot series that compare data by base
     *
     * @param data plot data that holds a collection of plot series
     */
    template<class Point>
    void setup_series_by_base(model::plot::plot_data<Point>& data)
    {
        typedef model::plot::series<Point> series_t;
        data.resize(constants::NUM_OF_BASES);
        for(size_t i=0;i<data.size();++i)
        {
            INTEROP_ASSERT(i<constants::PlotColorCount);
            data[i] = series_t(
                    constants::to_string(static_cast<constants::dna_bases>(i)),
                    constants::to_string(static_cast<constants::plot_colors>(i)),
                    series_t::Line);
        }
    }
    /** Plot a specified metric value by cycle
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param type specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output plot data
     * @param skip_empty set false for testing purposes
     */
    template<class Point>
    void plot_by_cycle_t(model::metrics::run_metrics& metrics,
                       const constants::metric_type type,
                       const model::plot::filter_options& options,
                       model::plot::plot_data<Point>& data,
                         const bool skip_empty)
    {
        data.clear();
        if(skip_empty && metrics.empty()) return;
        if(!options.all_cycles())
            INTEROP_THROW(model::invalid_filter_option, "Filtering by cycle is not supported");// TODO: Remove this!
        if(!options.all_reads())
            INTEROP_THROW(model::invalid_filter_option, "Filtering by read is not supported");// TODO: Remove this!
        if(!utils::is_cycle_metric(type))
            INTEROP_THROW(model::invalid_filter_option, "Only cycle metrics are supported");
        options.validate(type, metrics.run_info()); // TODO: Check ignored?
        size_t max_cycle=0;
        bool is_empty = true;

        if(logic::utils::to_group(type) == constants::Q)
        {
            if(0 == metrics.get<model::metrics::q_collapsed_metric>().size())
                logic::metric::create_collapse_q_metrics(metrics.get<model::metrics::q_metric>(),
                                                         metrics.get<model::metrics::q_collapsed_metric>());
        }
        if(options.all_channels(type))
        {
            setup_series_by_channel(metrics.run_info().channels(), data);
            model::plot::filter_options updated_options(options);
            for(size_t i=0;i<data.size();++i)
            {
                by_cycle_average_plot<Point> plot(data[i]);
                updated_options.channel(static_cast<model::plot::filter_options::channel_t>(i));
                plot_metric_proxy::select(metrics, updated_options, type, plot);
                max_cycle = plot.max_cycle();
                is_empty = plot.empty();
            }
        }
        else if(options.all_bases(type))
        {
            setup_series_by_base(data);
            model::plot::filter_options updated_options(options);
            for(size_t i=0;i<data.size();++i)
            {
                by_cycle_average_plot<Point> plot(data[i]);
                updated_options.dna_base(static_cast<constants::dna_bases >(i));
                plot_metric_proxy::select(metrics, updated_options, type, plot);
                max_cycle = plot.max_cycle();
                is_empty = plot.empty();
            }
        }
        else
        {
            data.assign(1, model::plot::series<Point>());
            by_cycle_candle_stick_plot<Point> plot(data[0]);
            plot_metric_proxy::select(metrics, options, type, plot);
            max_cycle = plot.max_cycle();
            is_empty = plot.empty();
        }

        if(is_empty)
        {
            data.clear();
            return;
        }
        if(type != constants::FWHM)
        {
            auto_scale_y(data);
            if(type == constants::ErrorRate)
            {
                data.set_yrange(0, std::min(5.0f, data.y_axis().max()));
            }
            else if(type == constants::BasePercent)
            {
                data.set_yrange(0, std::max(50.0f, data.y_axis().max()));
            }
        }
        else data.set_yrange(0, 6);
        data.set_xrange(0, static_cast<float>(max_cycle+2));
        data.set_xlabel("Cycle");
        data.set_ylabel(utils::to_description(type));

        std::string title = metrics.run_info().flowcell().barcode();
        if(title != "") title += " ";
        title += options.lane_description();
        if(logic::utils::is_channel_metric(type))
            title += " " + options.channel_description(metrics.run_info().channels());
        if(logic::utils::is_base_metric(type))
            title += " " + options.base_description();
        if(metrics.run_info().flowcell().surface_count()>1 && options.is_specific_surface())
            title += " " + options.surface_description();
        data.set_title(title);
    }

    /** Plot a specified metric value by cycle
     *
     * @ingroup plot_logic
     * @todo Is this temporary?
     * @param metrics run metrics
     * @param metric_name name of metric value to plot by cycle
     * @param options options to filter the data
     * @param data output plot data
     * @param skip_empty set false for testing purposes
     */
    template<class Point>
    void plot_by_cycle_t(model::metrics::run_metrics& metrics,
                         const std::string& metric_name,
                         const model::plot::filter_options& options,
                         model::plot::plot_data<Point>& data,
                         const bool skip_empty)
    {
        const constants::metric_type type = constants::parse<constants::metric_type>(metric_name);
        if(type == constants::UnknownMetricType)
            INTEROP_THROW(model::invalid_metric_type, "Unsupported metric type: " << metric_name);
        plot_by_cycle_t(metrics, type, options, data, skip_empty);
    }

    /** Plot a specified metric value by cycle
    *
    * @ingroup plot_logic
    * @param metrics run metrics
    * @param type specific metric value to plot by cycle
    * @param options options to filter the data
    * @param data output plot data
    * @param skip_empty set false for testing purposes
    */
    void plot_by_cycle(model::metrics::run_metrics& metrics,
                       const constants::metric_type type,
                       const model::plot::filter_options& options,
                       model::plot::plot_data<model::plot::candle_stick_point>& data,
                       const bool skip_empty)
            INTEROP_THROW_SPEC((model::index_out_of_bounds_exception,
            model::invalid_metric_type,
            model::invalid_channel_exception,
            model::invalid_filter_option,
            model::invalid_read_exception))
    {
        plot_by_cycle_t(metrics, type, options, data, skip_empty);
    }

    /** Plot a specified metric value by cycle using the candle stick model
     *
     * @ingroup plot_logic
     * @todo Is this temporary?
     * @param metrics run metrics
     * @param metric_name name of metric value to plot by cycle
     * @param options options to filter the data
     * @param data output plot data
     * @param skip_empty set false for testing purposes
     */
    void plot_by_cycle(model::metrics::run_metrics& metrics,
                       const std::string& metric_name,
                       const model::plot::filter_options& options,
                       model::plot::plot_data<model::plot::candle_stick_point>& data,
                       const bool skip_empty)
            INTEROP_THROW_SPEC((model::index_out_of_bounds_exception,
            model::invalid_filter_option,
            model::invalid_channel_exception,
            model::invalid_metric_type))
    {
        plot_by_cycle_t(metrics, metric_name, options, data, skip_empty);
    }

    /** List metric types available for by cycle plots
     *
     * @param types destination vector to fill with metric types
     * @param ignore_accumulated if true, ignore accumulated Q20 and Q30
     */
    void list_by_cycle_metrics(std::vector< logic::utils::metric_type_description_t >& types,
                               const bool ignore_accumulated)
    {
        utils::list_descriptions(types);
        filter_by_cycle_metrics(types, ignore_accumulated);
    }

    /** Filter metric types available for by cycle plots
     *
     * @param types destination vector to fill with metric types
     * @param ignore_accumulated if true, ignore accumulated Q20 and Q30
     */
    void filter_by_cycle_metrics(std::vector< logic::utils::metric_type_description_t >& types,
                               const bool ignore_accumulated)
    {
        std::vector< logic::utils::metric_type_description_t >::iterator dst = types.begin();
        for(std::vector< logic::utils::metric_type_description_t >::iterator src = types.begin();src != types.end();++src)
        {
            const constants::metric_type type = *src;
            if(utils::to_feature(type) == constants::UnknownMetricFeature) continue;
            if(ignore_accumulated)
            {
                if (type == constants::AccumPercentQ20) continue;
                if (type == constants::AccumPercentQ30)continue;
            }
            if(!utils::is_cycle_metric(type)) continue;
            if(src != dst) std::swap(*src, *dst);
            ++dst;
        }
        types.resize(std::distance(types.begin(), dst));
    }

}}}}

