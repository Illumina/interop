/** Collection of utilities for metric listing
 *
 *  @file
 *  @date  4/28/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/util/exception.h"
#include "interop/logic/utils/metrics_to_load.h"
#include "interop/logic/utils/metric_type_ext.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/plot/plot_flowcell_map.h"

namespace illumina { namespace interop { namespace logic { namespace utils
{

    /** List the required on demand metrics
     *
     * @param group specific metric group to load
     * @param valid_to_load list of metrics to load on demand
     */
    void list_metrics_to_load(const constants::metric_group group, std::vector<unsigned char>& valid_to_load)
    {
        if(valid_to_load.size() != constants::MetricCount) valid_to_load.assign(constants::MetricCount, 0);
        if(group < constants::MetricCount)
        {
            valid_to_load[group] = static_cast<unsigned char>(1);
        }
    }

    /** List the required on demand metrics
     *
     * @param type specific metric type to load
     * @param valid_to_load list of metrics to load on demand
     */
    void list_metrics_to_load(const constants::metric_type type, std::vector<unsigned char>& valid_to_load)
    {
        list_metrics_to_load(utils::to_group(type), valid_to_load);
    }
    /** List the required on demand metrics
     *
     * @param groups collection of specific metric groups to load
     * @param valid_to_load list of metrics to load on demand
     */
    void list_metrics_to_load(const std::vector<constants::metric_group>& groups,
                              std::vector<unsigned char>& valid_to_load)
    {
        for(std::vector<constants::metric_group>::const_iterator it = groups.begin();it != groups.end();++it)
            list_metrics_to_load(*it, valid_to_load);
    }
    /** List the required on demand metrics
     *
     * @param types collection of specific metric types to load
     * @param valid_to_load list of metrics to load on demand
     */
    void list_metrics_to_load(const std::vector<constants::metric_type>& types,
                              std::vector<unsigned char>& valid_to_load)
    {
        for(std::vector<constants::metric_type>::const_iterator it = types.begin();it != types.end();++it)
            list_metrics_to_load(*it, valid_to_load);
    }
    /** List the required on demand metrics
     *
     * @param metric_name name of metric value to load
     * @param valid_to_load list of metrics to load on demand
     */
    void list_metrics_to_load(const std::string& metric_name, std::vector<unsigned char>& valid_to_load)
    throw(model::invalid_metric_type)
    {
        const constants::metric_type type = constants::parse<constants::metric_type>(metric_name);
        if(type == constants::UnknownMetricType)
            INTEROP_THROW(model::invalid_metric_type, "Unsupported metric type: " << metric_name);
        list_metrics_to_load(type, valid_to_load);
    }

    /** List all required metric groups
     *
     * @param groups destination group list
     */
    void list_index_summary_metric_groups(std::vector<constants::metric_group>& groups)
    {
        using namespace model::metrics;
        groups.clear();
        const constants::metric_group group_set[] = {
                static_cast<constants::metric_group >(index_metric::TYPE),
                static_cast<constants::metric_group >(tile_metric::TYPE)
        };
        groups.assign(group_set, group_set+util::length_of(group_set));
    }

    /** List all required metric groups
     *
     * @param groups destination group list
     */
    void list_summary_metric_groups(std::vector<constants::metric_group>& groups)
    {
        using namespace model::metrics;
        groups.clear();
        const constants::metric_group group_set[] = {
                static_cast<constants::metric_group >(q_metric::TYPE),
                static_cast<constants::metric_group >(tile_metric::TYPE),
                static_cast<constants::metric_group >(error_metric::TYPE),
                static_cast<constants::metric_group >(extraction_metric::TYPE),
                static_cast<constants::metric_group >(corrected_intensity_metric::TYPE)
        };
        groups.assign(group_set, group_set+util::length_of(group_set));
    }
    /** List all required metric groups
     *
     * @param valid_to_load list of metrics to load on demand
     */
    void list_summary_metrics_to_load(std::vector<unsigned char>& valid_to_load)
    {
        std::vector<constants::metric_group> groups;
        list_summary_metric_groups(groups);
        logic::utils::list_metrics_to_load(groups, valid_to_load); // Only load the InterOp files required
    }

    /** List all required metric groups
     *
     * @param valid_to_load list of metrics to load on demand
     */
    void list_index_metrics_to_load(std::vector<unsigned char>& valid_to_load)
    {
        std::vector<constants::metric_group> groups;
        list_index_summary_metric_groups(groups);
        logic::utils::list_metrics_to_load(groups, valid_to_load); // Only load the InterOp files required
    }
    /** List all required metric groups for the analysis tab
     *
     * @param valid_to_load list of metrics to load on demand
     */
    void list_analysis_metrics_to_load(std::vector<unsigned char>& valid_to_load)
    {
        typedef std::vector< metric_type_description_t > description_vector_t;
        description_vector_t types;
        logic::plot::list_flowcell_metrics(types);
        for(description_vector_t::const_iterator it = types.begin();it != types.end();++it)
            list_metrics_to_load(*it, valid_to_load);
    }


}}}}
