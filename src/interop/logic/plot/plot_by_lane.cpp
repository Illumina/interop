/** Plot an arbitrary metric by lane
 *
 *  @file
 *  @date 5/1/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/plot/plot_by_lane.h"

#include "interop/logic/metric/metric_value.h"
#include "interop/logic/plot/plot_point.h"
#include "interop/logic/plot/plot_data.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{

    /** Plot the candle stick over all tiles of a specific metric by lane
     *
     * @param metrics set of metric records
     * @param proxy functor that takes a metric record and returns a metric value
     * @param options filter for metric records
     * @param type type of metric to extract using the proxy functor
     * @param points collection of points where x is lane number and y is the candle stick metric values
     */
    template<typename MetricSet, typename MetricProxy, typename Point>
    void populate_candle_stick_by_lane(const MetricSet& metrics,
                                       MetricProxy& proxy,
                                       const model::plot::filter_options& options,
                                       const constants::metric_type type,
                                       model::plot::data_point_collection<Point>& points)
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
            const float val = proxy(*b, type);
            if(std::isnan(val)) continue;
            tile_by_lane[b->lane()-1].push_back(val);
        }
        points.resize(lane_count);
        size_t offset=0;
        for(size_t i=0;i<tile_by_lane.size();++i)
        {
            if(tile_by_lane[i].empty()) continue;
            const float lane = static_cast<float>(i+1);
            plot_candle_stick(points[offset], tile_by_lane[i].begin(), tile_by_lane[i].end(), lane, outliers);
            ++offset;
        }
        points.resize(offset);
    }

    /** Plot a specified metric value by lane
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param type specific metric value to plot by lane
     * @param options options to filter the data
     * @param data output plot data
     */
    template<class Point>
    void plot_by_lane_t(const model::metrics::run_metrics& metrics,
                      const constants::metric_type type,
                      const model::plot::filter_options& options,
                      model::plot::plot_data<Point>& data)
    throw(model::index_out_of_bounds_exception,
    model::invalid_metric_type,
    model::invalid_filter_option)
    {
        data.clear();
        if(metrics.is_group_empty(logic::utils::to_group(type))) return;
        if(utils::is_cycle_metric(type))
            INTEROP_THROW(model::invalid_metric_type, "Cycle metrics are unsupported: " << constants::to_string(type));
        options.validate(type, metrics.run_info());
        data.assign(1, model::plot::series<Point>(utils::to_description(type), "Blue"));
        metric::metric_value<model::metrics::tile_metric> proxy3(options.read());
        populate_candle_stick_by_lane(metrics.get<model::metrics::tile_metric>(), proxy3, options, type,
                                      data[0]);

        const size_t read_count = metrics.run_info().reads().size();
        if(utils::is_read_metric(type) && options.all_reads() && read_count > 1)
            INTEROP_THROW(model::invalid_filter_option, "All reads is unsupported for run with " << read_count);

        if(type == constants::ClusterCount || type == constants::Clusters )//constants::Density )
        {
            data.push_back(model::plot::series<Point>("PF", "DarkGreen"));
            const constants::metric_type second_type =
                    (type==constants::Clusters ? constants::ClustersPF : constants::ClusterCountPF);
            //(type==constants::Density ? constants::DensityPF : constants::ClusterCountPF);
            populate_candle_stick_by_lane(metrics.get<model::metrics::tile_metric>(), proxy3, options, second_type,
                                          data[1]);
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
     */
    void plot_by_lane(const model::metrics::run_metrics& metrics,
                      const constants::metric_type type,
                      const model::plot::filter_options& options,
                      model::plot::plot_data<model::plot::candle_stick_point>& data)
            throw(model::index_out_of_bounds_exception,
            model::invalid_metric_type,
            model::invalid_filter_option)
    {
        plot_by_lane_t(metrics, type, options, data);
    }

    /** Plot a specified metric value by cycle
     *
     * @ingroup plot_logic
     * @todo Is this temporary?
     * @param metrics run metrics
     * @param metric_name name of metric value to plot by cycle
     * @param options options to filter the data
     * @param data output plot data
     */
    void plot_by_lane(const model::metrics::run_metrics& metrics,
                      const std::string& metric_name,
                      const model::plot::filter_options& options,
                      model::plot::plot_data<model::plot::candle_stick_point>& data)
    throw(model::index_out_of_bounds_exception,
    model::invalid_metric_type,
    model::invalid_filter_option)
    {
        const constants::metric_type type = constants::parse<constants::metric_type>(metric_name);
        if(type == constants::UnknownMetricType)
            INTEROP_THROW(model::invalid_metric_type, "Unsupported metric type: " << metric_name);
        plot_by_lane(metrics, type, options, data);
    }

    /** List metric types available for by lane plots
     *
     * @param types destination vector to fill with metric types
     * @param ignore_pf if true, ignore density PF and cluster PF
     */
    void list_by_lane_metrics(std::vector< logic::utils::metric_type_description_t >& types, const bool ignore_pf)
    {
        utils::list_descriptions(types);
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

