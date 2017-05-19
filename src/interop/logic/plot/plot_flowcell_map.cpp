/** Plot the flowcell map
 *
 *  @file
 *  @date 5/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/plot/plot_flowcell_map.h"

#include "interop/logic/metric/q_metric.h"
#include "interop/logic/plot/plot_metric_proxy.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{
    /** Plot the flowcell heatmap
     */
    class flowcell_plot
    {
    public:
        /** Constructor
         *
         * @param data reference to collection of points
         * @param values_for_scaling references to values for color bar scaling
         * @param layout flowcell layout
         */
        flowcell_plot(model::plot::flowcell_data &data,
                      std::vector<float> &values_for_scaling,
                      const model::run::flowcell_layout &layout) :
                m_data(data), m_values_for_scaling(values_for_scaling), m_layout(layout), m_empty(true)
        {}

        /** Plot the average over all tiles of a specific metric by cycle
         *
         * @param metrics set of metric records
         * @param options filter for metric records
         * @param proxy functor that takes a metric record and returns a metric value
         */
        template<typename MetricSet, typename MetricProxy>
        void operator()(const MetricSet &metrics,
                        const model::plot::filter_options &options,
                        const MetricProxy &proxy)
        {
            m_empty = metrics.empty();
            const bool all_surfaces = !options.is_specific_surface();
            for (typename MetricSet::const_iterator beg = metrics.begin(); beg != metrics.end(); ++beg)
            {
                if (!options.valid_tile_cycle(*beg)) continue;
                const float val = proxy(*beg);
                if (std::isnan(val)) continue;
                m_data.set_data(beg->lane() - 1,
                                beg->physical_location_index(
                                        m_layout.naming_method(),
                                        m_layout.sections_per_lane(),
                                        m_layout.tile_count(),
                                        m_layout.swath_count(),
                                        all_surfaces),
                                beg->tile(),
                                val);
                m_values_for_scaling.push_back(val);
            }
        }

        /** Test whether metric set was empty
         *
         * @return true if metric set was empty
         */
        bool empty() const
        {
            return m_empty;
        }


    private:
        model::plot::flowcell_data &m_data;
        std::vector<float> &m_values_for_scaling;
        model::run::flowcell_layout m_layout;
        bool m_empty;
    };

    /** Plot a flowcell map
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param type specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output flowcell map
     * @param buffer preallocated memory for data
     * @param tile_buffer preallocated memory for tile ids
     * @param skip_empty set false for testing purposes
     */
    void plot_flowcell_map(model::metrics::run_metrics &metrics,
                           const constants::metric_type type,
                           const model::plot::filter_options &options,
                           model::plot::flowcell_data &data,
                           float *buffer,
                           ::uint32_t *tile_buffer,
                           const bool skip_empty)
    throw(model::invalid_filter_option,
    model::invalid_metric_type,
    model::index_out_of_bounds_exception)
    {
        data.clear();
        if (skip_empty && metrics.empty()) return;
        options.validate(type, metrics.run_info());

        const model::run::flowcell_layout &layout = metrics.run_info().flowcell();
        if (buffer == 0 || tile_buffer == 0)
            data.resize(layout.lane_count(),
                        layout.total_swaths(layout.surface_count() > 1 && !options.is_specific_surface()),
                        layout.tiles_per_lane());
        else
        {
            const size_t buffer_size = layout.lane_count() *
                                       layout.total_swaths(
                                               layout.surface_count() > 1 && !options.is_specific_surface()) *
                                       layout.tiles_per_lane();
            if (buffer_size == 0) return;
            data.set_buffer(buffer, tile_buffer, layout.lane_count(),
                            layout.total_swaths(layout.surface_count() > 1 && !options.is_specific_surface()),
                            layout.tiles_per_lane());
        }
        std::vector<float> values_for_scaling;
        values_for_scaling.reserve(data.length());


        if (utils::is_cycle_metric(type) && options.all_cycles())
            INTEROP_THROW(model::invalid_filter_option, "All cycles is unsupported");
        if (utils::is_read_metric(type) && options.all_reads() && metrics.run_info().reads().size() > 1)
            INTEROP_THROW(model::invalid_filter_option, "All reads is unsupported");

        if(options.all_channels(type))
            INTEROP_THROW(model::invalid_filter_option, "All channels is unsupported");
        if (options.all_bases(type))
            INTEROP_THROW(model::invalid_filter_option, "All bases is unsupported");

        if(logic::utils::to_group(type) == constants::Q)
        {
            if(0 == metrics.get<model::metrics::q_collapsed_metric>().size())
                logic::metric::create_collapse_q_metrics(metrics.get<model::metrics::q_metric>(),
                                                         metrics.get<model::metrics::q_collapsed_metric>());
        }
        flowcell_plot plot(data, values_for_scaling, layout);
        plot_metric_proxy::select(metrics, options, type, plot);
        const bool is_empty = plot.empty();
        if (is_empty && !skip_empty)
        {
            data.clear();
            return;
        }

        if (!values_for_scaling.empty())
        {
            std::sort(values_for_scaling.begin(), values_for_scaling.end());
            // TODO: Put this back
            /*
            const float lower = util::percentile_sorted<float>(values_for_scaling.begin(), values_for_scaling.end(),
                                                               25);
            const float upper = util::percentile_sorted<float>(values_for_scaling.begin(), values_for_scaling.end(),
                                                               75);*/
            const float lower = values_for_scaling[size_t(0.25 * values_for_scaling.size())];
            const float upper = values_for_scaling[size_t(0.75 * values_for_scaling.size())];
            data.set_range(std::max(lower - 2 * (upper - lower), values_for_scaling[0]),
                           std::min(values_for_scaling.back(), upper + 2 * (upper - lower)));
        } else data.set_range(0, 0);
        if (type == constants::ErrorRate) data.set_range(0, std::min(5.0f, data.saxis().max()));

        std::string title = metrics.run_info().flowcell().barcode();
        if (title != "") title += " ";
        title += utils::to_description(type);
        data.set_title(title);

        std::string subtitle;
        if (metrics.run_info().flowcell().surface_count() > 1)
            subtitle += options.surface_description() + " ";
        subtitle += options.cycle_description();
        if (logic::utils::is_channel_metric(type))
            subtitle += " " + options.channel_description(metrics.run_info().channels());
        if (logic::utils::is_base_metric(type))
            subtitle += " " + options.base_description();
        if (logic::utils::is_read_metric(type))
            subtitle += " " + options.read_description();
        data.set_subtitle(subtitle);
        data.set_label(utils::to_description(type));
    }

    /** Plot a flowcell map
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param metric_name specific metric value to plot by cycle
     * @param options options to filter the data
     * @param data output flowcell map
     * @param buffer preallocated memory for data
     * @param tile_buffer preallocated memory for tile ids
     * @param skip_empty set false for testing purposes
     */
    void plot_flowcell_map(model::metrics::run_metrics &metrics,
                           const std::string &metric_name,
                           const model::plot::filter_options &options,
                           model::plot::flowcell_data &data,
                           float *buffer,
                           ::uint32_t *tile_buffer,
                           const bool skip_empty)
    throw(model::invalid_filter_option,
    model::invalid_metric_type,
    model::index_out_of_bounds_exception)
    {
        const constants::metric_type type = constants::parse<constants::metric_type>(metric_name);
        if (type == constants::UnknownMetricType)
            INTEROP_THROW(model::invalid_metric_type, "Unsupported metric type: " << metric_name);
        plot_flowcell_map(metrics, type, options, data, buffer, tile_buffer, skip_empty);
    }

    /** List metric type names available for flowcell
     *
     * @param types destination vector to fill with metric type names
     * @param ignore_accumulated exclude accumulated q-metrics
     */
    void list_flowcell_metrics(std::vector< logic::utils::metric_type_description_t > &types,
                               const bool ignore_accumulated)
    {
        utils::list_descriptions(types);
        filter_flowcell_metrics(types, ignore_accumulated);
    }
    /** Filter metric type names available for flowcell
     *
     * @param types destination vector to fill with metric type names
     * @param ignore_accumulated exclude accumulated q-metrics
     */
    void filter_flowcell_metrics(std::vector<logic::utils::metric_type_description_t> &types,
                               const bool ignore_accumulated)
    {
        std::vector<logic::utils::metric_type_description_t>::iterator dst = types.begin();
        for (std::vector<logic::utils::metric_type_description_t>::iterator src = types.begin();
             src != types.end(); ++src)
        {
            const constants::metric_type type = *src;
            if (utils::to_feature(type) == constants::UnknownMetricFeature) continue;
            if (ignore_accumulated)
            {
                if (type == constants::AccumPercentQ20) continue;
                if (type == constants::AccumPercentQ30)continue;
            }
            if(src != dst) std::swap(*src, *dst);
            ++dst;
        }
        types.resize(std::distance(types.begin(), dst));
    }

    /** Calculate the required buffer size
     *
     * @param metrics run metrics
     * @param options options to filter the data
     */
    size_t calculate_flowcell_buffer_size(const model::metrics::run_metrics &metrics,
                                          const model::plot::filter_options &options)
    {
        const model::run::flowcell_layout &layout = metrics.run_info().flowcell();
        return layout.lane_count() *
               (layout.total_swaths(layout.surface_count() > 1 && !options.is_specific_surface())) *
               layout.tiles_per_lane();
    }


}}}}

