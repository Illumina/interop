/** Plot the Q-score histogram
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
#include "interop/model/plot/series.h"
#include "interop/model/plot/bar_point.h"
#include "interop/logic/metric/metric_value.h"
#include "interop/logic/plot/plot_point.h"
#include "interop/logic/plot/plot_data.h"

namespace illumina { namespace interop { namespace logic { namespace plot {


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
        for(size_t i=0;i<histogram.size();++i) histogram[i] /= 1e6f;
    }
    /** Scale the histogram if necessary and provide the scale label
     *
     * @param histogram q-score histogram
     * @return label of the scale
     */
    inline std::string scale_histogram(std::vector<float>& histogram)
    {
        float max_height = 0;
        for(size_t i=0;i<histogram.size();++i) max_height = std::max(max_height, histogram[i]);
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
                             const size_t max_cycle)
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
        for(size_t i=0;i<histogram.size();++i) points[i].set(static_cast<float>(i+1), histogram[i]);
        return static_cast<float>(histogram.size()+1);
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
        if(points.size() == histogram.size()) //Compressed
        {
            for (size_t i=0; beg != end; ++beg, ++i)
            {
                points[i].set(beg->lower(), histogram[i], static_cast<float>(beg->upper()-beg->lower()+1));
                max_x_value = std::max(max_x_value, points[i].x()+points[i].width());
            }
        }
        else // Uncompressed
        {
            for (size_t i=0; beg != end; ++beg, ++i)
            {
                const size_t bin = static_cast<size_t>(beg->value());
                points[i].set(beg->lower(), histogram[bin], static_cast<float>(beg->upper()-beg->lower()+1));
                max_x_value = std::max(max_x_value, points[i].x()+points[i].width());
            }
        }
        return max_x_value;
    }

    /** Plot a histogram of q-scores
     *
     * @param metrics run metrics
     * @param type specific metric value to plot by lane
     * @param options options to filter the data
     * @param data output plot data
     */
    template<class Point>
    void plot_qscore_histogram(const model::metrics::run_metrics& metrics,
                               const model::plot::filter_options& options,
                               model::plot::plot_data<Point>& data)
    {
        typedef constants::enumeration<constants::bar_plot_options> bar_plot_options_t;
        typedef model::metric_base::metric_set<model::metrics::q_metric> q_metric_set_t;

        const q_metric_set_t& q_metric_set = metrics.get_set<model::metrics::q_metric>();
        const size_t first_cycle = options.all_reads() ? 1 : metrics.run_info().read(options.read()).first_cycle();
        const size_t last_cycle = get_last_filtered_cycle(metrics.run_info(),
                                                          options,
                                                          metrics.get_set<model::metrics::q_metric>().max_cycle());

        data.assign(1, model::plot::series<Point>("Q Score", "", model::plot::series<Point>::Bar));
        data[0].add_option(bar_plot_options_t::to_key(constants::ShiftedBar));

        std::vector<float> histogram;
        populate_distribution(
                q_metric_set.begin(),
                q_metric_set.end(),
                options,
                first_cycle,
                last_cycle,
                histogram);
        const std::string axis_scale = scale_histogram(histogram);
        float max_x_value;
        if(!q_metric_set.bins().empty())
            max_x_value=plot_binned_histogram(q_metric_set.bins().begin(), q_metric_set.bins().end(), histogram, data[0]);
        else max_x_value=plot_unbinned_histogram(histogram, data[0]);

        auto_scale_y(data, false);
        data.set_xrange(1, max_x_value*1.1f);

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
