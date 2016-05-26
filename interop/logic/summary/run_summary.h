/** Summary logic for run metrics
 *
 *  @file
 *  @date  3/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <vector>
#include "interop/model/model_exceptions.h"
#include "interop/logic/summary/error_summary.h"
#include "interop/logic/summary/tile_summary.h"
#include "interop/logic/summary/extraction_summary.h"
#include "interop/logic/summary/quality_summary.h"
#include "interop/model/summary/run_summary.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/utils/channel.h"


namespace illumina
{
namespace interop
{
namespace logic
{
namespace summary
{
    /** Determine maximum number of tiles among all metrics for each lane
     *
     * @param metrics run metrics
     * @param summary run summary
     */
    inline void summarize_tile_count(const model::metrics::run_metrics& metrics, model::summary::run_summary& summary)
    {
        using namespace model::metrics;
        for(unsigned int lane=0;lane<summary.lane_count();++lane)
        {
            size_t tile_count = 0;
            tile_count = std::max(tile_count, metrics.get_set<tile_metric>().tile_numbers_for_lane(lane+1).size());
            tile_count = std::max(tile_count, metrics.get_set<error_metric>().tile_numbers_for_lane(lane+1).size());
            tile_count = std::max(tile_count, metrics.get_set<extraction_metric>().tile_numbers_for_lane(lane+1).size());
            tile_count = std::max(tile_count, metrics.get_set<q_metric>().tile_numbers_for_lane(lane+1).size());
            for(size_t read=0;read<summary.size();++read)
                summary[read][lane].tile_count(tile_count);
        }
    }

    /** Summarize a collection run metrics
     *
     * @TODO speed up calculation by adding no_median flag
     *
     * @param metrics source collection of all metrics
     * @param summary destination run summary
     */
    inline void summarize_run_metrics(model::metrics::run_metrics& metrics, model::summary::run_summary& summary)
    throw( model::index_out_of_bounds_exception, model::invalid_channel_exception )
    {
        using namespace model::metrics;
        if(metrics.empty()) return;


        summary.initialize(metrics.run_info().reads(), metrics.run_info().flowcell().lane_count());


        read_cycle_vector_t cycle_to_read;
        map_read_to_cycle_number(summary.begin(), summary.end(), cycle_to_read);
        summarize_tile_metrics(metrics.get_set<tile_metric>().begin(), metrics.get_set<tile_metric>().end(), summary);
        summarize_error_metrics(metrics.get_set<error_metric>().begin(),
                                metrics.get_set<error_metric>().end(),
                                cycle_to_read,
                                summary);
        INTEROP_ASSERT(metrics.run_info().channels().size()>0);
        const size_t intensity_channel = utils::expected2actual_map(metrics.run_info().channels())[0];
        summarize_extraction_metrics(metrics.get_set<extraction_metric>().begin(),
                                     metrics.get_set<extraction_metric>().end(),
                                     cycle_to_read,
                                     intensity_channel,
                                     summary);

        if(0 == metrics.get_set<q_collapsed_metric>().size())
            logic::metric::create_collapse_q_metrics(metrics.get_set<model::metrics::q_metric>(),
                                                     metrics.get_set<q_collapsed_metric>());
        summarize_collapsed_quality_metrics(metrics.get_set<q_collapsed_metric>().begin(),
                                            metrics.get_set<q_collapsed_metric>().end(),
                                            cycle_to_read,
                                            summary);
        summarize_tile_count(metrics, summary);

        summarize_cycle_state(metrics.get_set<tile_metric>(),
                              metrics.get_set<error_metric>(),
                              cycle_to_read,
                              &model::summary::cycle_state_summary::error_cycle_range,
                              summary);
        summarize_cycle_state(metrics.get_set<tile_metric>(),
                              metrics.get_set<extraction_metric>(),
                              cycle_to_read,
                              &model::summary::cycle_state_summary::extracted_cycle_range,
                              summary);
        summarize_cycle_state(metrics.get_set<tile_metric>(),
                              metrics.get_set<q_metric>(),
                              cycle_to_read,
                              &model::summary::cycle_state_summary::qscored_cycle_range,
                              summary);
        // Summarize called cycle state
        summarize_cycle_state(metrics.get_set<tile_metric>(),
                              metrics.get_set<corrected_intensity_metric>(),
                              cycle_to_read,
                              &model::summary::cycle_state_summary::called_cycle_range,
                              summary);
    }

}
}
}
}
