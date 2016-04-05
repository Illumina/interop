/** Summary logic for extraction metrics
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

    /** Summarize a collection extraction metrics
     *
     * @TODO speed up calculation by adding no_median flag
     *
     * @param metrics source collection of all metrics
     * @param summary destination run summary
     */
    inline void summarize_run_metrics(const model::metrics::run_metrics& metrics, model::summary::run_summary& summary) _INTEROP_MODEL_THROWS
    {
        using namespace model::metrics;
        if(metrics.empty()) return;
        summary.initialize(metrics.run_info().reads(), metrics.run_info().flowcell().lane_count());
        summarize_tile_metrics(metrics.get_set<tile_metric>().begin(), metrics.get_set<tile_metric>().end(), summary);
        summarize_error_metrics(metrics.get_set<error_metric>().begin(), metrics.get_set<error_metric>().end(), summary);
        INTEROP_ASSERT(metrics.run_info().channels().size()>0);
        const size_t intensity_channel = utils::expected2actual_map(metrics.run_info().channels())[0];
        summarize_extraction_metrics(metrics.get_set<extraction_metric>().begin(), metrics.get_set<extraction_metric>().end(), intensity_channel, summary);
        const size_t q30_index = metrics.get_set<q_metric>().index_for_q_value(30);
        summarize_quality_metrics(metrics.get_set<q_metric>().begin(), metrics.get_set<q_metric>().end(), q30_index, summary);
        summarize_tile_count(metrics, summary);
    }

}
}
}
}
