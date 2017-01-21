/** Logic for dynamic phasing metrics
 *
 *  @file
 *  @date  7/7/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/logic/metric/dynamic_phasing_metric.h"

namespace illumina { namespace interop { namespace logic { namespace metric
{
    /** Compute the best fit line (slope + offset) given the points defined by (x_values[i], y_values[i])
     *
     * @param x_values X-values of the list of points
     * @param y_values Y-values of the list of points
     * @param slope slope of the best fit line (calculated by this function)
     * @param offset offset of the best fit line (calculated by this function)
     * @param sample_count number of weights to use for fitting
     * @return true if linear fit could be computed, false if not
     */
    bool compute_linear_fit(const std::vector<float>& x_values, const std::vector<float>& y_values, float& slope, float& offset, size_t sample_count=0)
    {
        if(sample_count==0 || sample_count > x_values.size()) sample_count = x_values.size();
        if (x_values.size() <= 1 || x_values.size() != y_values.size())
        {
            //TODO: throw error?
            return false;
        }
        float sx = 0; // Sum(x)
        float sy = 0; // Sum(y)
        float sxx = 0; // Sum(x*x)
        float sxy = 0; // Sum(x*y)
        for (size_t i = 0; i < sample_count; i++)
        {
            sx += x_values[i];
            sy += y_values[i];
            sxy += x_values[i] * y_values[i];
            sxx += x_values[i] * x_values[i];
        }
        const float denominator = sample_count * sxx - sx * sx;
        if (denominator > std::numeric_limits<float>::epsilon())
        {
            slope = (sample_count * sxy - sx * sy) / denominator;
            offset = (sy * sxx - sx * sxy) / denominator;
            return true;
        }
        return false;
    }


    /** Populate the dynamic phasing metrics (slope & offset for phasing/prephasing) data structure given the phasing metrics data
     *
     * @param phasing_metrics phasing metric set
     * @param cycle_to_read map of cycle to read information
     * @param dynamic_phasing_metrics dynamic phasing metric set (to be populated)
     */
    void populate_dynamic_phasing_metrics(const model::metric_base::metric_set<model::metrics::phasing_metric>& phasing_metrics,
                                          const logic::summary::read_cycle_vector_t& cycle_to_read,
                                          model::metric_base::metric_set<model::metrics::dynamic_phasing_metric>& dynamic_phasing_metrics,
                                          model::metric_base::metric_set<model::metrics::tile_metric>& tile_metrics)
    {
        typedef model::metric_base::metric_set<model::metrics::phasing_metric> metric_set;
        if (phasing_metrics.size() == 0)return;
        if (phasing_metrics.max_cycle() < 25) return;
        std::vector<float> cycles;
        std::vector<float> phasing_values;
        std::vector<float> prephasing_values;
        const metric_set::id_vector lanes = phasing_metrics.lanes();
        //loop through all lane/tiles
        for (uint32_t lane_index = 0; lane_index < lanes.size(); ++lane_index)
        {
            const metric_set::uint_t lane = lanes[lane_index];
            const metric_set::id_vector tiles = phasing_metrics.tile_numbers_for_lane(lane);
            for (uint32_t tile_index = 0; tile_index < tiles.size(); ++tile_index)
            {
                const metric_set::uint_t tile = tiles[tile_index];
                size_t max_cycle_relative_to_read = 0;
                //loop through all cycles in a read and populate the 3 arrays
                for (uint32_t cycle = 0; cycle < cycle_to_read.size(); ++cycle)
                {
                    if (phasing_metrics.has_metric(lane, tile, cycle + 1))
                    {
                        const model::metrics::phasing_metric metric = phasing_metrics.get_metric(lane, tile, cycle + 1);

                        max_cycle_relative_to_read = cycle_to_read[cycle].cycle_within_read;
                        cycles.push_back(static_cast<float>(max_cycle_relative_to_read));
                        phasing_values.push_back(metric.phasing_weight());
                        prephasing_values.push_back(metric.prephasing_weight());
                    }
                    if(cycle_to_read[cycle].is_last_cycle_in_read)
                    {
                        //we wait until we have reached C25 before calculating anything
                        //compute linear fits and populate the dynamic phasing metric as appropriate
                        if (max_cycle_relative_to_read >= 25 && cycles.size() >= 2)
                        {

                            const uint32_t read = static_cast<uint32_t>(cycle_to_read[cycle].number);
                            float phasing_slope = 0, phasing_offset = 0, prephasing_slope = 0, prephasing_offset = 0;
                            const size_t tile_offset = tile_metrics.find(lane, tile);
                            if(tile_offset < tile_metrics.size())
                            {
                                model::metrics::tile_metric& tile_metric = tile_metrics[tile_offset];
                                compute_linear_fit(cycles, phasing_values, phasing_slope, phasing_offset, 25);
                                compute_linear_fit(cycles, prephasing_values, prephasing_slope, prephasing_offset, 25);
                                tile_metric.update_phasing_if_missing(read, phasing_slope, prephasing_slope);
                            }

                            if (!compute_linear_fit(cycles, phasing_values, phasing_slope, phasing_offset))
                                continue;
                            if (!compute_linear_fit(cycles, prephasing_values, prephasing_slope, prephasing_offset))
                                continue;
                            model::metrics::dynamic_phasing_metric metric(lane,
                                                                          tile,
                                                                          read,
                                                                          phasing_slope,
                                                                          phasing_offset,
                                                                          prephasing_slope,
                                                                          prephasing_offset);
                            dynamic_phasing_metrics.insert(metric);
                        }
                        //clear all the arrays
                        cycles.clear();
                        phasing_values.clear();
                        prephasing_values.clear();
                        max_cycle_relative_to_read = 0;
                    }
                }
            }
        }
    }


}}}}

