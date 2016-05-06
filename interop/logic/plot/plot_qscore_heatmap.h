/** Plot the Q-score heat map
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
#include "interop/model/plot/heatmap_data.h"
#include "interop/logic/metric/q_metric.h"

namespace illumina { namespace interop { namespace logic { namespace plot {


    /** Populate the q-score heat map based on the filter options
     *
     * @param beg iterator to start of q-metric collection
     * @param end iterator to end of q-metric collection
     * @param bins q-score bins
     * @param options filter for metric records
     * @param data q-score heatmap
     */
    template<typename I, typename B>
    void populate_heatmap_from_compressed(I beg,
                                          I end,
                                          const std::vector<B>& bins,
                                          const model::plot::filter_options &options,
                                          model::plot::heatmap_data& data)
    {
        for (;beg != end;++beg)
        {
            if( !options.valid_tile(*beg) ) continue;
            for(size_t bin =0;bin < bins.size();++bin)
                data(beg->cycle()-1, bins[bin].value()) += beg->qscore_hist(bin);
        }
    }
    /** Populate the q-score heatmap based on the filter options
     *
     * @param beg iterator to start of q-metric collection
     * @param end iterator to end of q-metric collection
     * @param options filter for metric records
     * @param data q-score heatmap
     */
    template<typename I>
    void populate_heatmap_from_uncompressed(I beg,
                                            I end,
                                            const model::plot::filter_options &options,
                                            model::plot::heatmap_data& data)
    {
        for (;beg != end;++beg)
        {
            if( !options.valid_tile(*beg) ) continue;

            for(size_t bin =0;bin < beg->size();++bin)
                data(beg->cycle()-1, bin) += beg->qscore_hist(bin);
        }
    }
    /** Normalize the heat map to a percent
     *
     * @param data output heat map data
     */
    inline void normalize_heatmap(model::plot::heatmap_data& data)
    {
        float max_value = 0;
        for(size_t r=0;r<data.row_count();++r)
            for(size_t c=0;c<data.column_count();++c)
                max_value = std::max(max_value, data(r,c));
        for(size_t r=0;r<data.row_count();++r)
            for(size_t c=0;c<data.column_count();++c)
                data(r,c) = 100 * data(r,c) / max_value;
    }
    /** Spread the bins out
     *
     *
     * @param beg iterator to start of bins
     * @param end iterator to end of bins
     * @param max_cycle maximum cycle number
     * @param data output heat map data
     */
    template<typename I>
    void remap_to_bins(I beg, I end, const size_t max_cycle, model::plot::heatmap_data& data)
    {
        for(;beg != end;++beg)
        {
            for(size_t bin = std::max(0, beg->lower()-1), upper=beg->upper();bin < upper;++bin)
            {
                for(size_t cycle = 0;cycle < max_cycle;++cycle)
                {
                    data(cycle, bin) = data(cycle, beg->value());
                }
            }
        }
    }

    /** Plot a heat map of q-scores
     *
     * @param metrics run metrics
     * @param options options to filter the data
     * @param data output heat map data
     */
    inline void plot_qscore_heatmap(const model::metrics::run_metrics& metrics,
                                    const model::plot::filter_options& options,
                                    model::plot::heatmap_data& data)
    {
        typedef model::metric_base::metric_set<model::metrics::q_metric> q_metric_set_t;

        const q_metric_set_t& q_metric_set = metrics.get_set<model::metrics::q_metric>();
        data.clear();
        if(q_metric_set.size()==0)return;

        const size_t max_q_val = logic::metric::max_qval(q_metric_set);
        data.resize(q_metric_set.max_cycle()+1, max_q_val);
        if(logic::metric::is_compressed(q_metric_set))
            populate_heatmap_from_compressed(q_metric_set.begin(),
                                             q_metric_set.end(),
                                             q_metric_set.bins(),
                                             options,
                                             data);
        else
            populate_heatmap_from_uncompressed(q_metric_set.begin(), q_metric_set.end(), options, data);
        normalize_heatmap(data);
        remap_to_bins(q_metric_set.bins().begin(), q_metric_set.bins().end(), q_metric_set.max_cycle(), data);
        data.set_xrange(0, static_cast<float>(data.row_count()));
        data.set_yrange(0, static_cast<float>(data.column_count()));

        data.set_xlabel("Cycle");
        data.set_ylabel("Q Score");

        std::string title = metrics.run_info().flowcell().barcode();
        if(title != "") title += " ";
        title += options.lane_description();
        if(metrics.run_info().flowcell().surface_count()>1 && options.is_specific_surface())
            title += " " + options.surface_description();
        data.set_title(title);
    }


}}}}
