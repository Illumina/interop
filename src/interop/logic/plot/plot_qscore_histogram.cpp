/** Plot the Q-score histogram
 *
 *  @file
 *  @date 5/5/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/plot/plot_qscore_histogram.h"
#include "interop/logic/metric/q_metric.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{


    /** Populate the q-score histogram based on the filter options
     *
     * @param beg iterator to start of q-metric collection
     * @param end iterator to end of q-metric collection
     * @param options filter for metric records
     * @param first_cycle first cycle to keep
     * @param last_cycle last cycle to keep
     * @param histogram q-score histogram
     */
    template<typename I>
    void populate_distribution(I beg,
                               I end,
                               const model::plot::filter_options &options,
                               const size_t first_cycle,
                               const size_t last_cycle,
                               std::vector<float>& histogram)
    {
        if(beg == end) return;
        histogram.resize(beg->size(), 0);
        for (;beg != end;++beg)
        {
            if( !options.valid_tile(*beg) || beg->cycle() < first_cycle || beg->cycle() > last_cycle) continue;
            beg->accumulate_into(histogram);
        }
    }
    /** Scale the histogram if necessary and provide the scale label
     *
     * @param histogram q-score histogram
     * @return label of the scale
     */
    inline std::string scale_histogram(std::vector<float>& histogram)
    {
        float max_height = 0;
        for(size_t i=0;i<histogram.size();++i)
        {
            histogram[i] /= 1e6f;
            max_height = std::max(max_height, histogram[i]);
        }
        if(max_height < 10000) return "million";
        for(size_t i=0;i<histogram.size();++i) histogram[i] /= 1000;
        return "billion";
    }

    /** Get the last cycle based on the filter options
     *
     * @param run_info run info with reads information
     * @param options options to filter the data
     * @param max_cycle maximum cycle for q_metrics
     * @return last cycle based on run info and filter options
     */
    inline size_t get_last_filtered_cycle(const model::run::info &run_info,
                                          const model::plot::filter_options &options,
                                          const size_t max_cycle) throw(model::invalid_read_exception)
    {
        size_t last_cycle = options.all_reads() ? max_cycle : run_info.read(options.read()).last_cycle();
        if(!options.all_cycles()) last_cycle = std::min(last_cycle, static_cast<size_t>(options.cycle()));
        return last_cycle;
    }
    /** Plot an unbinned histogram
     *
     * @param histogram q-score histogram
     * @param points collection of points where x is lane number and y is the candle stick metric values
     * @return max x-value
     */
    template<typename Point>
    float plot_unbinned_histogram(const std::vector<float>& histogram,
                                  model::plot::data_point_collection<Point>& points)
    {
        points.resize(histogram.size());
        size_t point_index = 0;
        for(size_t i=0;i<histogram.size();++i)
        {
            if(histogram[i] == 0) continue;
            points[point_index].set(static_cast<float>(i+1), histogram[i]);
            ++point_index;
        }
        points.resize(point_index);
        if(point_index == 0) return 0;
        return static_cast<float>(points[point_index-1].x()+1);
    }
    /** Plot a binned histogram
     *
     * @param beg iterator to start of the histogram bins
     * @param end iterator to end of the histogram bins
     * @param histogram q-score histogram
     * @param points collection of points where x is lane number and y is the candle stick metric values
     * @return max x-value
     */
    template<typename I, typename Point>
    float plot_binned_histogram(I beg,
                                I end,
                                const std::vector<float>& histogram,
                                model::plot::data_point_collection<Point>& points)
    {
        float max_x_value = 0;
        points.resize(std::distance(beg, end));
        size_t point_index = 0;
        if(points.size() == histogram.size()) //Compressed
        {
            for (size_t i=0; beg != end; ++beg, ++i)
            {
                INTEROP_ASSERT(i < histogram.size());
                if(histogram[i] == 0)continue;
                points[point_index].set(beg->lower(), histogram[i], static_cast<float>(beg->upper()-beg->lower()+1));
                max_x_value = std::max(max_x_value, points[point_index].x()+points[point_index].width());
                ++point_index;
            }
        }
        else // Uncompressed
        {
            for (size_t i=0; beg != end; ++beg, ++i)
            {
                const size_t bin = static_cast<size_t>(beg->value())-1;
                INTEROP_ASSERTMSG(bin < histogram.size(), bin << " < " << histogram.size());
                if(histogram[bin] == 0)continue;
                points[point_index].set(beg->lower(), histogram[bin], static_cast<float>(beg->upper()-beg->lower()+1));
                max_x_value = std::max(max_x_value, points[point_index].x()+points[point_index].width());
                ++point_index;
            }
        }
        points.resize(point_index);
        return max_x_value;
    }

    /** Plot a histogram of q-scores
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param options options to filter the data
     * @param data output plot data
     * @param boundary index of bin to create the boundary sub plots (0 means do nothing)
     */
    void plot_qscore_histogram(model::metrics::run_metrics& metrics,
                               const model::plot::filter_options& options,
                               model::plot::plot_data<model::plot::bar_point>& data,
                               const size_t boundary)
    throw( model::invalid_read_exception,
    model::index_out_of_bounds_exception,
    model::invalid_filter_option)
    {
        typedef model::plot::bar_point Point;
        data.clear();
        if(options.is_specific_surface())
        {
            if(metrics.get< model::metrics::q_metric >().empty())return;
        }
        else
        {
            if(metrics.get< model::metrics::q_metric >().empty() &&
               metrics.get< model::metrics::q_by_lane_metric >().empty()   )return;
        }
        options.validate(constants::QScore, metrics.run_info());
        const size_t first_cycle = options.all_reads() ? 1 : metrics.run_info().read(options.read()).first_cycle();

        data.push_back(model::plot::series<Point>("Q Score", "Blue", model::plot::series<Point>::Bar));
        if(boundary>0)
        {
            data.push_back(model::plot::series<Point>("Q Score", "DarkGreen", model::plot::series<Point>::Bar));
            data.push_back(model::plot::series<Point>("Threshold(>=Q30)", "Green", model::plot::series<Point>::Line));
        }

        //"DarkGreen"

        for(size_t i=0;i<data.size();++i)
            data[i].add_option(constants::to_string(constants::Shifted));

        std::string axis_scale;
        std::vector<float> histogram;
        float max_x_value;

        if(options.is_specific_surface())
        {
            typedef model::metrics::q_metric metric_t;
            const size_t last_cycle = get_last_filtered_cycle(metrics.run_info(),
                                                              options,
                                                              metrics.get<metric_t>().max_cycle());
            if(metrics.get<metric_t>().size() == 0) return;
            populate_distribution(
                    metrics.get<metric_t>().begin(),
                    metrics.get<metric_t>().end(),
                    options,
                    first_cycle,
                    last_cycle,
                    histogram);
            axis_scale = scale_histogram(histogram);
            if(!metrics.get<metric_t>().bins().empty())
                max_x_value=plot_binned_histogram(metrics.get<metric_t>().bins().begin(),
                                                  metrics.get<metric_t>().bins().end(),
                                                  histogram,
                                                  data[0]);
            else max_x_value=plot_unbinned_histogram(histogram, data[0]);
        }
        else
        {
            typedef model::metrics::q_by_lane_metric metric_t;
            if(0 == metrics.get<metric_t>().size())
                logic::metric::create_q_metrics_by_lane(metrics.get<model::metrics::q_metric>(),
                                                        metrics.get<metric_t>());
            if(0 == metrics.get<metric_t>().size()) return;
            const size_t last_cycle = get_last_filtered_cycle(metrics.run_info(),
                                                              options,
                                                              metrics.get<metric_t>().max_cycle());
            INTEROP_ASSERT(0 != metrics.get<metric_t>().size());
            populate_distribution(
                    metrics.get<metric_t>().begin(),
                    metrics.get<metric_t>().end(),
                    options,
                    first_cycle,
                    last_cycle,
                    histogram);
            axis_scale = scale_histogram(histogram);
            if(!metrics.get<metric_t>().bins().empty())
                max_x_value=plot_binned_histogram(metrics.get<metric_t>().bins().begin(),
                                                  metrics.get<metric_t>().bins().end(),
                                                  histogram,
                                                  data[0]);
            else max_x_value=plot_unbinned_histogram(histogram, data[0]);
        }

        // split into two series, < Q30 and Q30 <=
        if(data.size() > 1)
        {
            size_t gt_eq_boundary_count = 0;
            data[1].resize(data[0].size());
            for (size_t i = 0; i < data[0].size(); ++i)
            {
                if (data[0][i].x() >= boundary)
                {
                    data[1][gt_eq_boundary_count] = data[0][i];
                    ++gt_eq_boundary_count;
                }
            }
            data[0].resize(data[0].size() - gt_eq_boundary_count);
            data[1].resize(gt_eq_boundary_count);
        }


        auto_scale_y(data, false);
        data.set_xrange(1, std::max(1.0f, max_x_value)*1.1f);
        if(data.size() > 2)
        {
            data[2].resize(2);
            data[2][0].set(static_cast<float>(boundary), data.y_axis().min());
            data[2][1].set(static_cast<float>(boundary), data.y_axis().max());
        }

        data.set_xlabel("Q Score");
        data.set_ylabel("Total ("+axis_scale+")");

        std::string title = metrics.run_info().flowcell().barcode();
        if(title != "") title += " ";
        title += options.lane_description();
        if(options.is_specific_read())
            title += " " + options.read_description();
        if(metrics.run_info().flowcell().surface_count()>1 && options.is_specific_surface())
            title += " " + options.surface_description();
        data.set_title(title);
    }


}}}}

