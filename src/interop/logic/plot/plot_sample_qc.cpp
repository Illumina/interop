/** Plot reads identified versus index
 *
 *  @file
 *  @date 5/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/plot/plot_sample_qc.h"

#include "interop/logic/utils/enums.h"
#include "interop/logic/metric/index_metric.h"

namespace illumina { namespace interop { namespace logic { namespace plot
{

    /** Populate reads identified versus the index
     *
     * @param index_metrics set of metric records
     * @param tile_metrics source collection of tile metrics
     * @param lane lane index
     * @param points collection of points where x is lane number and y is the candle stick metric values
     */
    template<typename Point>
    float populate_reads_identified(model::metric_base::metric_set<model::metrics::index_metric> &index_metrics,
                                    const model::metric_base::metric_set<model::metrics::tile_metric> &tile_metrics,
                                    const size_t lane,
                                    model::plot::data_point_collection<Point> &points)
    {
        typedef model::metric_base::metric_set<model::metrics::index_metric> index_metric_set_t;
        typedef INTEROP_UNORDERED_MAP(std::string, ::uint64_t) index_count_map_t;
        typedef typename index_count_map_t::iterator map_iterator;
        typedef typename model::metrics::index_metric::const_iterator const_index_iterator;
        const size_t kAllLanes = 0;

        logic::metric::populate_indices(tile_metrics, index_metrics);
        index_count_map_t index_count_map;
        ::uint64_t pf_cluster_count_total = 0;
        for (typename index_metric_set_t::const_iterator b = index_metrics.begin(), e = index_metrics.end();
             b != e; ++b)
        {
            if (lane != kAllLanes && lane != b->lane()) continue;
            if(std::isnan(b->cluster_count_pf())) continue;// TODO: check better?
            pf_cluster_count_total += static_cast< ::uint64_t >( b->cluster_count_pf());
            for (const_index_iterator ib = b->indices().begin(), ie = b->indices().end(); ib != ie; ++ib)
            {
                const std::string index_id = ib->index_seq() + ib->sample_id();
                map_iterator found_index = index_count_map.find(index_id);
                if (found_index == index_count_map.end()) index_count_map[index_id] = ib->cluster_count();
                else found_index->second += ib->cluster_count();
            }
        }
        points.resize(index_count_map.size());
        float max_height = 0;
        std::vector<float> heights;
        heights.reserve(index_count_map.size());
        size_t i = 0;
        if(!index_count_map.empty())
        {
            for (typename std::vector<std::string>::const_iterator b = index_metrics.index_order().begin(),
                         e = index_metrics.index_order().end(); b != e; ++b)
            {
                if(index_count_map.find(*b) == index_count_map.end()) continue;
                const float height = (pf_cluster_count_total == 0) ? 0.0f : 100.0f * index_count_map[*b] /
                                                                            pf_cluster_count_total;
                points[i].set(i + 1.0f, height, 1.0f);
                max_height = std::max(max_height, height);
                ++i;
            }
        }
        return max_height;
    }

    /** Plot reads identified versus index
     *
     * @ingroup plot_logic
     * @param metrics run metrics
     * @param lane lane number
     * @param data output plot data
     */
    void plot_sample_qc(model::metrics::run_metrics &metrics,
                        const size_t lane,
                        model::plot::plot_data<model::plot::bar_point> &data)
    throw(model::index_out_of_bounds_exception)
    {
        typedef model::plot::series<model::plot::bar_point> bar_series_t;
        data.clear();
        if (metrics.is_group_empty(constants::Tile) ||
            metrics.is_group_empty(constants::Index))
            return;
        data.set_xlabel("Index Number");
        data.set_ylabel("% Reads Identified (PF)");
        data.assign(1, bar_series_t("% reads", "Green", bar_series_t::Bar));
        data[0].add_option(constants::to_string(constants::Centered));

        if (metrics.get<model::metrics::index_metric>().size() == 0)
        {
            if (metrics.run_info().is_indexed())
            {
                data.set_range(data.x_axis().min(),
                               1,
                               data.y_axis().min(),
                               5);
            }
            //data.clear(); // TODO: Remove below and uncomment this line

            return;
        }

        const float max_height = populate_reads_identified(metrics.get<model::metrics::index_metric>(),
                                                           metrics.get<model::metrics::tile_metric>(),
                                                           lane,
                                                           data[0]);
        auto_scale(data);
        data.set_range(data.x_axis().min(),
                       static_cast<float>(data[0].size() + 1),
                       data.y_axis().min(),
                       roundf(max_height + 5));
    }


}}}}

