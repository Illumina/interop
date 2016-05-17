/** Plot an arbitrary cycle metric by cycle
 *
 *  @file
 *  @date  4/28/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/util/statistics.h"
#include "interop/constants/enums.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/model/model_exceptions.h"
#include "interop/model/plot/filter_options.h"
#include "interop/model/plot/series.h"
#include "interop/model/plot/plot_data.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/utils/metric_type_ext.h"
#include "interop/logic/utils/channel.h"
#include "interop/logic/metric/metric_value.h"
#include "interop/logic/plot/plot_point.h"
#include "interop/logic/plot/plot_data.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{

    /** Plot the average over all tiles of a specific metric by cycle
     *
     * @param metrics set of metric records
     * @param proxy functor that takes a metric record and returns a metric value
     * @param options filter for metric records
     * @param type type of metric to extract using the proxy functor
     * @param points collection of points where x is cycle number and y is the mean metric value
     */
    template<typename MetricSet, typename MetricProxy, typename Point>
    void populate_metric_average_by_cycle(const MetricSet& metrics,
                                          MetricProxy& proxy,
                                          const model::plot::filter_options& options,
                                          const constants::metric_type type,
                                          model::plot::data_point_collection<Point>& points)
    {
        const size_t max_cycle = metrics.max_cycle();
        points.assign(max_cycle, Point());
        for(typename MetricSet::const_iterator b = metrics.begin(), e = metrics.end();b != e;++b)
        {
            if(!options.valid_tile(*b)) continue;
            points[b->cycle()-1].add(1, proxy(*b, type));
        }
        for(size_t cycle=0;cycle<max_cycle;++cycle)
        {
            const float avg = (points[cycle].x()>0) ? points[cycle].y()/points[cycle].x() : 0;
            points[cycle].set(static_cast<float>(cycle+1), avg);
        }
    }
    /** Plot the candle stick over all tiles of a specific metric by cycle
     *
     * @param metrics set of metric records
     * @param proxy functor that takes a metric record and returns a metric value
     * @param options filter for metric records
     * @param type type of metric to extract using the proxy functor
     * @param points collection of points where x is cycle number and y is the candle stick metric values
     */
    template<typename MetricSet, typename MetricProxy>
    void populate_candle_stick_by_cycle(const MetricSet& metrics,
                                        MetricProxy& proxy,
                                        const model::plot::filter_options& options,
                                        const constants::metric_type type,
                                        model::plot::data_point_collection<model::plot::candle_stick_point>& points)
    {
        const size_t max_cycle = metrics.max_cycle();
        const size_t tile_count = static_cast<size_t>(std::ceil(static_cast<float>(metrics.size())/max_cycle));
        std::vector< std::vector<float> > tile_by_cycle(max_cycle);
        for(size_t i=0;i<max_cycle;++i) tile_by_cycle[i].reserve(tile_count);
        std::vector<float> outliers;
        outliers.reserve(10); // TODO: use as flag for keeping outliers

        for(typename MetricSet::const_iterator b = metrics.begin(), e = metrics.end();b != e;++b)
        {
            if(!options.valid_tile(*b)) continue;
            tile_by_cycle[b->cycle()-1].push_back(proxy(*b, type));
        }
        points.resize(max_cycle);
        for(size_t i=0;i<max_cycle;++i)
        {
            if(tile_by_cycle[i].empty())continue;
            plot_candle_stick(points[i], tile_by_cycle[i].begin(), tile_by_cycle[i].end(), static_cast<float>(i+1), outliers);
        }
    }
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
            data[i] = series_t(
                    constants::to_string(static_cast<constants::dna_bases>(i)),
                    constants::to_string(static_cast<constants::plot_colors>(i)),
                    series_t::Line);
        }
    }
    /** Plot a specified metric value by cycle
     *
     * @param metrics run metrics
     * @param type specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output plot data
     */
    template<class Point>
    void plot_by_cycle(model::metrics::run_metrics& metrics,
                       const constants::metric_type type,
                       const model::plot::filter_options& options,
                       model::plot::plot_data<Point>& data)
    {
        data.clear();
        if(!utils::is_cycle_metric(type)) throw model::invalid_metric_type("Only cycle metrics are supported");
        switch(logic::utils::to_group(type))
        {
            case constants::Extraction:
            {
                if(options.all_channels(type))
                {
                    setup_series_by_channel(metrics.run_info().channels(), data);
                    for(size_t i=0;i<data.size();++i) // TODO: Added to inner call?
                    {
                        metric::metric_value<model::metrics::extraction_metric> proxy(i);
                        populate_metric_average_by_cycle(
                                metrics.get_set<model::metrics::extraction_metric>(),
                                proxy,
                                options,
                                type,
                                data[i]
                        );
                    }
                }
                else
                {
                    data.assign(1, model::plot::series<Point>());
                    const size_t channel = options.channel();
                    metric::metric_value<model::metrics::extraction_metric> proxy(channel);
                    populate_candle_stick_by_cycle(
                            metrics.get_set<model::metrics::extraction_metric>(),
                            proxy,
                            options,
                            type,
                            data[0]);
                }
                break;
            }
            case constants::CorrectedInt:
            {
                if(options.all_bases(type))
                {
                    setup_series_by_base(data);
                    for(size_t i=0;i<data.size();++i) // TODO: Added to inner call?
                    {
                        metric::metric_value<model::metrics::corrected_intensity_metric> proxy(
                                static_cast<constants::dna_bases>(i));
                        populate_metric_average_by_cycle(
                                metrics.get_set<model::metrics::corrected_intensity_metric>(),
                                proxy,
                                options,
                                type,
                                data[i]
                        );
                    }
                }
                else
                {
                    data.assign(1, model::plot::series<Point>());
                    const constants::dna_bases base = options.dna_base();
                    metric::metric_value<model::metrics::corrected_intensity_metric> proxy(base);
                    populate_candle_stick_by_cycle(
                            metrics.get_set<model::metrics::corrected_intensity_metric>(),
                            proxy,
                            options,
                            type,
                            data[0]);
                }
                break;
            }
            case constants::Q:
            {
                data.assign(1, model::plot::series<Point>());
                if(options.is_specific_surface())
                {
                    typedef model::metrics::q_collapsed_metric metric_t;
                    metric::metric_value<metric_t> proxy2;
                    if(0 == metrics.get_set<metric_t>().size())
                        logic::metric::create_collapse_q_metrics(metrics.get_set<model::metrics::q_metric>(),
                                                                 metrics.get_set<metric_t>());
                    populate_candle_stick_by_cycle(
                            metrics.get_set<metric_t>(),
                            proxy2,
                            options,
                            type,
                            data[0]);
                }
                else
                {
                    typedef model::metrics::q_by_lane_metric metric_t;
                    if(0 == metrics.get_set<metric_t>().size())
                        logic::metric::create_q_metrics_by_lane(metrics.get_set<model::metrics::q_metric>(),
                                                                 metrics.get_set<metric_t>());
                    const size_t qbin = metric::index_for_q_value(metrics.get_set<metric_t>(),
                                                               (type == constants::PercentQ20 ||
                                                                type == constants::AccumPercentQ20) ? 20 : 30);
                    metric::metric_value<metric_t> proxy2(qbin,metrics.get_set<metric_t>().bins());
                    populate_candle_stick_by_cycle(
                            metrics.get_set<metric_t>(),
                            proxy2,
                            options,
                            type,
                            data[0]);
                }
                break;
            }
            case constants::Error://TODO: skip last cycle of read for error metric
            {
                data.assign(1, model::plot::series<Point>());
                metric::metric_value<model::metrics::error_metric> proxy3;
                populate_candle_stick_by_cycle(
                        metrics.get_set<model::metrics::error_metric>(),
                        proxy3,
                        options,
                        type,
                        data[0]);
                break;
            }
            default:
                throw model::invalid_metric_type("Invalid metric group");
        }
        if(type != constants::FWHM)
        {
            auto_scale_y(data);
            if(type == constants::ErrorRate)
            {
                data.set_yrange(0, std::min(5.0f, data.y_axis().max()));
            }
            else if(type == constants::PercentBase)
            {
                data.set_yrange(0, std::max(50.0f, data.y_axis().max()));
            }
        }
        else data.set_yrange(0, 6);
        data.set_xrange(0, static_cast<float>(data[0].size()+2));
        data.set_xlabel("Cycle");
        data.set_ylabel(utils::to_description(type));

        std::string title = metrics.run_info().flowcell().barcode();
        if(title != "") title += " ";
        title += options.lane_description();
        if(logic::utils::is_channel_metric(type))
            title += " " + options.channel_description(metrics.run_info().channels());
        if(logic::utils::is_base_metric(type))
            title += " " + options.base_description();
        if(metrics.run_info().flowcell().surface_count()>1)
            title += " " + options.surface_description();
        data.set_title(title);
    }

    /** Plot a specified metric value by cycle
     *
     * @todo Is this temporary?
     * @param metrics run metrics
     * @param metric_name name of metric value to plot by cycle
     * @param options options to filter the data
     * @param data output plot data
     */
    template<class Point>
    void plot_by_cycle(model::metrics::run_metrics& metrics,
                       const std::string& metric_name,
                       const model::plot::filter_options& options,
                       model::plot::plot_data<Point>& data)
    {
        plot_by_cycle(metrics, constants::parse<constants::metric_type>(metric_name), options, data);
    }
}
}}}
