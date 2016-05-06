/** Plot the flowcell map
 *
 *  @file
 *  @date 5/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/util/statistics.h"
#include "interop/constants/enums.h"
#include "interop/model/model_exceptions.h"
#include "interop/model/run_metrics.h"
#include "interop/model/plot/filter_options.h"
#include "interop/model/plot/flowcell_data.h"
#include "interop/logic/utils/metric_type_ext.h"
#include "interop/logic/utils/channel.h"
#include "interop/logic/metric/metric_value.h"

namespace illumina { namespace interop { namespace logic { namespace plot {


    /** Populate the flowcell map based on the filter options
     *
     * @param beg iterator to start of q-metric collection
     * @param end iterator to end of q-metric collection
     * @param proxy functor that takes a metric record and returns a metric value
     * @param layout layout of the flowcell
     * @param options filter for metric records
     * @param data flowcell map
     */
    template<typename I, typename MetricProxy>
    void populate_flowcell_map(I beg,
                               I end,
                               MetricProxy& proxy,
                               const constants::metric_type type,
                               const model::run::flowcell_layout& layout,
                               const model::plot::filter_options &options,
                               model::plot::flowcell_data& data,
                               std::vector<float>& values_for_scaling)
    {
        if(beg == end) return;
        const bool all_surfaces = !options.is_specific_surface();
        for (;beg != end;++beg)
        {
            if( !options.valid_tile_cycle(*beg) ) continue;
            const float val = proxy(*beg, type);
            if(std::isnan(val)) continue;
            data.set_data(beg->lane(),
                          beg->physical_location_index(
                                  layout.naming_method(),
                                  layout.sections_per_lane(),
                                  layout.tile_count(),
                                  layout.swath_count(),
                                  all_surfaces),
                          beg->tile(),
                          val);
            values_for_scaling.push_back(val);
        }
    }

    /** Plot a flowcell map
     *
     * @param metrics run metrics
     * @param type specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output flowcell map
     */
    inline void plot_flowcell_map(const model::metrics::run_metrics& metrics,
                                  const constants::metric_type type,
                                  const model::plot::filter_options& options,
                                  model::plot::flowcell_data& data) throw(std::invalid_argument)
    {
        const model::run::flowcell_layout& layout = metrics.run_info().flowcell();
        data.clear();
        data.resize(layout.lane_count(), layout.total_swaths(!options.is_specific_surface()), layout.tiles_per_lane());
        std::vector<float> values_for_scaling;
        values_for_scaling.reserve(data.length());
        if(options.all_cycles())
            throw std::invalid_argument("All cycles is unsupported");
        if(options.all_reads())
            throw std::invalid_argument("All reads is unsupported");
        switch(logic::utils::to_group(type))
        {
            case constants::MetricCount:
            case constants::UnknownMetricGroup:
            case constants::Index:
            case constants::Image:
                throw std::invalid_argument("Unsupported metric type: "+constants::to_string(type));
            case constants::Tile:
            {
                typedef model::metrics::tile_metric metric_t;
                typedef model::metric_base::metric_set<metric_t> metric_set_t;
                const metric_set_t& metric_set = metrics.get_set<metric_t>();
                metric::metric_value<metric_t> proxy(options.read());
                populate_flowcell_map(metric_set.begin(), metric_set.end(), proxy, type, layout, options, data,
                                      values_for_scaling);
                break;
            }
            case constants::Extraction:
            {
                typedef model::metrics::extraction_metric metric_t;
                typedef model::metric_base::metric_set<metric_t> metric_set_t;
                const metric_set_t& metric_set = metrics.get_set<metric_t>();
                const size_t channel = options.channel();
                if(options.all_channels(type))
                    throw std::invalid_argument("All channels is unsupported");
                metric::metric_value<metric_t> proxy(channel);
                populate_flowcell_map(metric_set.begin(), metric_set.end(), proxy, type, layout, options, data,
                                      values_for_scaling);
                break;
            }
            case constants::CorrectedInt:
            {
                typedef model::metrics::corrected_intensity_metric metric_t;
                typedef model::metric_base::metric_set<metric_t> metric_set_t;
                const metric_set_t& metric_set = metrics.get_set<metric_t>();
                const constants::dna_bases base = options.dna_base();
                if(options.all_bases(type))
                    throw std::invalid_argument("All bases is unsupported");
                metric::metric_value<metric_t> proxy(base);
                populate_flowcell_map(metric_set.begin(), metric_set.end(), proxy, type, layout, options, data,
                        values_for_scaling);
                break;
            }
            case constants::Q:
            {
                typedef model::metrics::q_metric metric_t;
                typedef model::metric_base::metric_set<metric_t> metric_set_t;
                const metric_set_t& metric_set = metrics.get_set<metric_t>();
                const size_t qbin = metric::index_for_q_value(metric_set,
                                                              (type == constants::PercentQ20 ||
                                                               type == constants::AccumPercentQ20) ? 20 : 30);
                metric::metric_value<metric_t> proxy(qbin, metric_set.bins());
                populate_flowcell_map(metric_set.begin(), metric_set.end(), proxy, type, layout, options, data,
                                      values_for_scaling);
                break;
            }
            case constants::Error:
            {
                typedef model::metrics::error_metric metric_t;
                typedef model::metric_base::metric_set<metric_t> metric_set_t;
                const metric_set_t& metric_set = metrics.get_set<metric_t>();
                metric::metric_value<metric_t> proxy;
                populate_flowcell_map(metric_set.begin(), metric_set.end(), proxy, type, layout, options, data,
                                      values_for_scaling);
                break;
            }
        };

        if(!values_for_scaling.empty())
        {
            std::sort(values_for_scaling.begin(), values_for_scaling.end());
            const float lower = util::percentile_sorted<float>(values_for_scaling.begin(), values_for_scaling.end(),
                                                               25);
            const float upper = util::percentile_sorted<float>(values_for_scaling.begin(), values_for_scaling.end(),
                                                               75);
            data.set_range(std::max(lower - 2 * (upper - lower), values_for_scaling[0]),
                           std::min(values_for_scaling.back(), upper + 2 * (upper - lower)));
        }

        std::string title = metrics.run_info().flowcell().barcode();
        if(title != "") title += " ";
        title += utils::to_description(type);
        data.set_title(title);

        std::string subtitle;
        if(metrics.run_info().flowcell().surface_count()>1)
            subtitle += " " + options.surface_description();
        subtitle += " " + options.cycle_description();
        if(logic::utils::is_channel_metric(type))
            subtitle += " " + options.channel_description(metrics.run_info().channels());
        if(logic::utils::is_base_metric(type))
            subtitle += " " + options.base_description();
        if(logic::utils::is_read_metric(type))
            subtitle += " " + options.read_description();
        data.set_subtitle(subtitle);
        data.set_label(utils::to_description(type));
    }


}}}}
