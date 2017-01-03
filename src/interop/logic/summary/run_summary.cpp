/** Summary logic for run metrics
 *
 *  @file
 *  @date  3/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/summary/run_summary.h"
#include "interop/logic/summary/error_summary.h"
#include "interop/logic/summary/tile_summary.h"
#include "interop/logic/summary/extraction_summary.h"
#include "interop/logic/summary/quality_summary.h"
#include "interop/logic/utils/channel.h"
#include "interop/logic/metric/q_metric.h"


namespace illumina { namespace interop { namespace logic { namespace summary
{
    /** Determine maximum number of tiles among all metrics for each lane
     *
     * @param metrics run metrics
     * @param summary run summary
     */
    void summarize_tile_count(const model::metrics::run_metrics& metrics, model::summary::run_summary& summary)
    {
        using namespace model::metrics;
        model::metrics::run_metrics::id_set_t tile_count_set;
        const size_t surface_count = metrics.run_info().flowcell().surface_count();
        const constants::tile_naming_method naming_convention = metrics.run_info().flowcell().naming_method();
        for(unsigned int lane=0;lane<summary.lane_count();++lane)
        {
            size_t tile_count_for_lane = 0;
            for(unsigned int surface=0;surface < surface_count;++surface)
            {
                metrics.get<tile_metric>().populate_tile_numbers_for_lane_surface(tile_count_set,
                                                                             lane+1,
                                                                             surface+1,
                                                                             naming_convention);
                metrics.get<error_metric>().populate_tile_numbers_for_lane_surface(tile_count_set,
                                                                             lane+1,
                                                                             surface+1,
                                                                             naming_convention);
                metrics.get<extraction_metric>().populate_tile_numbers_for_lane_surface(tile_count_set,
                                                                             lane+1,
                                                                             surface+1,
                                                                             naming_convention);
                metrics.get<q_metric>().populate_tile_numbers_for_lane_surface(tile_count_set,
                                                                             lane+1,
                                                                             surface+1,
                                                                             naming_convention);

                if(surface_count > 1)
                {
                    for (size_t read = 0; read < summary.size(); ++read)
                        summary[read][lane][surface].tile_count(tile_count_set.size());
                }
                tile_count_for_lane += tile_count_set.size();
                tile_count_set.clear();
            }
            for(size_t read=0;read<summary.size();++read)
                summary[read][lane].tile_count(tile_count_for_lane);
        }
    }

    namespace detail
    {
        /** Predicate for std::partition to shuffle all non empty summaries to the start of the array
         *
         * @param summary lane summary
         * @return true if the summary is not empty
         */
        inline bool not_empty(const model::summary::lane_summary& summary)
        {
            return summary.tile_count() > 0;
        }
        /** Predicate to sort lane summaries by lane number
         *
         * @param lhs left hand side summary
         * @param rhs right hand side summary
         * @return true if lhs < rhs
         */
        inline bool less_than(const model::summary::lane_summary& lhs, const model::summary::lane_summary& rhs)
        {
            return lhs.lane() < rhs.lane();
        }
    }

    /** Summarize a collection run metrics
     *
     * TODO speed up calculation by adding no_median flag
     *
     * @ingroup summary_logic
     * @param metrics source collection of all metrics
     * @param summary destination run summary
     * @param skip_median skip the median calculation
     */
    void summarize_run_metrics(model::metrics::run_metrics& metrics,
                               model::summary::run_summary& summary,
                               const bool skip_median,
                               const bool trim)
    throw( model::index_out_of_bounds_exception,
    model::invalid_channel_exception )
    {
        using namespace model::metrics;
        if(metrics.empty())
        {
            summary.clear();
            return;
        }
        summary.initialize(metrics.run_info());

        read_cycle_vector_t cycle_to_read;
        const constants::tile_naming_method naming_method = metrics.run_info().flowcell().naming_method();
        map_read_to_cycle_number(summary.begin(), summary.end(), cycle_to_read);
        summarize_tile_metrics(metrics.get<tile_metric>().begin(),
                               metrics.get<tile_metric>().end(),
                               naming_method,
                               summary);
        summarize_error_metrics(metrics.get<error_metric>().begin(),
                                metrics.get<error_metric>().end(),
                                cycle_to_read,
                                naming_method,
                                summary,
                                skip_median);
        INTEROP_ASSERT(metrics.run_info().channels().size()>0);
        const size_t intensity_channel = utils::expected2actual_map(metrics.run_info().channels())[0];
        summarize_extraction_metrics(metrics.get<extraction_metric>().begin(),
                                     metrics.get<extraction_metric>().end(),
                                     cycle_to_read,
                                     intensity_channel,
                                     naming_method,
                                     summary,
                                     skip_median);

        if(0 == metrics.get<q_collapsed_metric>().size())
            logic::metric::create_collapse_q_metrics(metrics.get<q_metric>(),
                                                     metrics.get<q_collapsed_metric>());
        summarize_collapsed_quality_metrics(metrics.get<q_collapsed_metric>().begin(),
                                            metrics.get<q_collapsed_metric>().end(),
                                            cycle_to_read,
                                            naming_method,
                                            summary);
        summarize_tile_count(metrics, summary);

        summarize_cycle_state(metrics.get<tile_metric>(),
                              metrics.get<error_metric>(),
                              cycle_to_read,
                              &model::summary::cycle_state_summary::error_cycle_range,
                              summary);
        summarize_cycle_state(metrics.get<tile_metric>(),
                              metrics.get<extraction_metric>(),
                              cycle_to_read,
                              &model::summary::cycle_state_summary::extracted_cycle_range,
                              summary);
        summarize_cycle_state(metrics.get<tile_metric>(),
                              metrics.get<q_metric>(),
                              cycle_to_read,
                              &model::summary::cycle_state_summary::qscored_cycle_range,
                              summary);
        // Summarize called cycle state
        summarize_cycle_state(metrics.get<tile_metric>(),
                              metrics.get<corrected_intensity_metric>(),
                              cycle_to_read,
                              &model::summary::cycle_state_summary::called_cycle_range,
                              summary);

        if(trim)
        {
            // Remove the empty lane summary entries
            size_t max_lane_count = 0;
            for (size_t read = 0; read < summary.size(); ++read)
            {
                // Shuffle all non-zero tile_count models to beginning of the array
                summary[read].resize(std::distance(summary[read].begin(),
                                                   std::partition(summary[read].begin(), summary[read].end(),
                                                                  detail::not_empty)));
                std::sort(summary[read].begin(), summary[read].end(), detail::less_than);
                max_lane_count = std::max(summary[read].size(), max_lane_count);
            }
            summary.lane_count(max_lane_count);
        }
    }

}}}}

