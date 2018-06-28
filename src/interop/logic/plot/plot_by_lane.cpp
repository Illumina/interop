/** Plot an arbitrary metric by lane
 *
 *  @file
 *  @date 5/1/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/plot/plot_by_lane.h"

#include "interop/logic/plot/plot_point.h"
#include "interop/logic/plot/plot_data.h"
#include "interop/logic/plot/plot_metric_proxy.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{
   /** Plot the candle stick over all tiles of a specific metric by lane
    */
    template<class Point>
    class by_lane_candle_stick_plot
    {
    public:
        /** Constructor
         *
         * @param points reference to collection of points
         */
        by_lane_candle_stick_plot(model::plot::data_point_collection<Point>&  points) :
                m_points(points){}


        /** Plot the candle stick over all tiles of a specific metric by lane
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
    private:
        /** Plot the candle stick over all tiles of a specific metric by lane
         *
         * @param metrics set of metric records
         * @param options filter for metric records
         * @param proxy functor that takes a metric record and returns a metric value
         */
        template<typename MetricSet, typename MetricProxy>
        void plot(const MetricSet& metrics,
                  const model::plot::filter_options& options,
                  const MetricProxy& proxy,
                  const constants::base_tile_t*)
        {
            if(metrics.max_lane() == 0) return;
            const size_t lane_count = metrics.max_lane();
            const size_t tile_count = static_cast<size_t>(std::ceil(static_cast<float>(metrics.size())/lane_count));
            std::vector< std::vector<float> > tile_by_lane(metrics.max_lane());
            for(size_t i=0;i<tile_by_lane.size();++i) tile_by_lane[i].reserve(tile_count); // optimize using lane ids
            std::vector<float> outliers;
            outliers.reserve(10);

            for(typename MetricSet::const_iterator b = metrics.begin(), e = metrics.end();b != e;++b)
            {
                if(!options.valid_tile(*b)) continue;
                const float val = proxy(*b);
                if(std::isnan(val)) continue;
                tile_by_lane[b->lane()-1].push_back(val);
            }
            m_points.resize(lane_count);
            size_t offset=0;
            for(size_t i=0;i<tile_by_lane.size();++i)
            {
                if(tile_by_lane[i].empty()) continue;
                const float lane = static_cast<float>(i+1);
                plot_candle_stick(m_points[offset], tile_by_lane[i].begin(), tile_by_lane[i].end(), lane, outliers);
                ++offset;
            }
            m_points.resize(offset);
        }
        template<typename MetricSet, typename MetricProxy>
        void plot(const MetricSet&,
                  const model::plot::filter_options&,
                  const MetricProxy&,
                  const void*){}
    private:
        model::plot::data_point_collection<Point>& m_points;
    };

    /** Plot a specified metric value by lane
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param type specific metric value to plot by lane
     * @param options options to filter the data
     * @param data output plot data
     * @param skip_empty set false for testing purposes
     */
    template<class Point>
    void plot_by_lane_t(const model::metrics::run_metrics& metrics,
                      const constants::metric_type type,
                      const model::plot::filter_options& options,
                      model::plot::plot_data<Point>& data,
                        const bool skip_empty)
    INTEROP_THROW_SPEC((model::index_out_of_bounds_exception,
    model::invalid_metric_type,
    model::invalid_filter_option))
    {
        data.clear();
        if(skip_empty && metrics.empty()) return;
        if(utils::is_cycle_metric(type))
            INTEROP_THROW(model::invalid_metric_type, "Cycle metrics are unsupported: " << constants::to_string(type));
        options.validate(type, metrics.run_info());
        if(metrics.is_group_empty(logic::utils::to_group(type))) return;
        data.assign(1, model::plot::series<Point>(utils::to_description(type), "Blue"));


        by_lane_candle_stick_plot<Point> plot(data[0]);
        plot_metric_proxy::select(metrics, options, type, plot);
        if (type == constants::ClusterCount || type == constants::Clusters)//constants::Density )
        {
            data.push_back(model::plot::series<Point>("PF", "DarkGreen"));
            const constants::metric_type second_type =
                    (type == constants::Clusters ? constants::ClustersPF : constants::ClusterCountPF);

            by_lane_candle_stick_plot<Point> plot2(data[1]);
            plot_metric_proxy::select(metrics, options, second_type, plot2);
        }

        auto_scale(data, true, 1.2f);
        if(type == constants::PercentPrephasing || type == constants::PercentPhasing)
            data.set_yrange(0, 1);
        data.set_xrange(0, data.x_axis().max()+1);

        data.set_xlabel("Lane");
        data.set_ylabel(utils::to_description(type));

        std::string title = metrics.run_info().flowcell().barcode();
        if(options.is_specific_read(type))
        {
            if(title != "") title += " ";
            title += options.read_description();
        }
        if(metrics.run_info().flowcell().surface_count()>1 && options.is_specific_surface())
        {
            if(title != "") title += " ";
            title += options.surface_description();
        }
        data.set_title(title);
    }
    /** Plot a specified metric value by lane
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param type specific metric value to plot by lane
     * @param options options to filter the data
     * @param data output plot data
     * @param skip_empty set false for testing purposes
     */
    void plot_by_lane(const model::metrics::run_metrics& metrics,
                      const constants::metric_type type,
                      const model::plot::filter_options& options,
                      model::plot::plot_data<model::plot::candle_stick_point>& data,
                      const bool skip_empty)
            INTEROP_THROW_SPEC((model::index_out_of_bounds_exception,
            model::invalid_metric_type,
            model::invalid_filter_option))
    {
        plot_by_lane_t(metrics, type, options, data, skip_empty);
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
    void plot_by_lane(const model::metrics::run_metrics& metrics,
                      const std::string& metric_name,
                      const model::plot::filter_options& options,
                      model::plot::plot_data<model::plot::candle_stick_point>& data,
                      const bool skip_empty)
    INTEROP_THROW_SPEC((model::index_out_of_bounds_exception,
    model::invalid_metric_type,
    model::invalid_filter_option))
    {
        const constants::metric_type type = constants::parse<constants::metric_type>(metric_name);
        if(type == constants::UnknownMetricType)
            INTEROP_THROW(model::invalid_metric_type, "Unsupported metric type: " << metric_name);
        plot_by_lane(metrics, type, options, data, skip_empty);
    }

    /** List metric types available for by lane plots
     *
     * @param types destination vector to fill with metric types
     * @param ignore_pf if true, ignore density PF and cluster PF
     */
    void list_by_lane_metrics(std::vector< logic::utils::metric_type_description_t >& types,
                              const bool ignore_pf)
    {
        utils::list_descriptions(types);
        filter_by_lane_metrics(types, ignore_pf);
    }
    /** Filter metric types available for by lane plots
     *
     * @param types destination vector to fill with metric types
     * @param ignore_pf if true, ignore density PF and cluster PF
     */
    void filter_by_lane_metrics(std::vector< logic::utils::metric_type_description_t >& types,
                              const bool ignore_pf)
    {
        std::vector< logic::utils::metric_type_description_t >::iterator dst = types.begin();
        for(std::vector< logic::utils::metric_type_description_t >::iterator src = types.begin();src != types.end();++src)
        {
            const constants::metric_type type = *src;
            if(utils::to_feature(type) == constants::UnknownMetricFeature) continue;
            if(ignore_pf)
            {
                if(type == constants::ClustersPF) continue;
                if(type == constants::ClusterCountPF) continue;
            }
            if(utils::is_cycle_metric(type)) continue;
            if(src != dst) std::swap(*src, *dst);
            ++dst;
        }
        types.resize(std::distance(types.begin(), dst));
    }


}}}}

