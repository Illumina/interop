/** Collection of all metric sets for a run
 *
 *  @file
 *  @date 3/9/16
 *  @version 1.0
 *  @copyright Illumina Use Only
 */
#include "interop/model/run_metrics.h"

#include "interop/logic/metric/q_metric.h"
#include "interop/logic/utils/channel.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
     struct populate_tile_list
     {
         populate_tile_list(run_metrics::tile_metric_map_t &map) : m_map(map) {}

         template<class MetricSet>
         void operator()(const MetricSet &metrics) const {
             typedef typename MetricSet::base_t base_t;
             populate_id(metrics, base_t::null());
         }

     private:
         template<class MetricSet>
         void populate_id(const MetricSet &metrics, const void *) const {
             for (typename MetricSet::const_iterator it = metrics.begin(); it != metrics.end(); ++it) {
                 INTEROP_ASSERTMSG(it->tile() > 0, it->lane() << "_" << it->tile());
                 m_map[it->tile_hash()] = *it;
             }
         }

         template<class MetricSet>
         void populate_id(const MetricSet &, const constants::base_lane_t *) const {}

         run_metrics::tile_metric_map_t &m_map;
     };


     struct populate_tile_cycle_list
     {
         populate_tile_cycle_list(run_metrics::cycle_metric_map_t &map) : m_map(map) {}

         template<class MetricSet>
         void operator()(const MetricSet &metrics) const {
             typedef typename MetricSet::base_t base_t;
             populate_id(metrics, base_t::null());
         }

     private:
         template<class MetricSet>
         void populate_id(const MetricSet &metrics, const constants::base_cycle_t *) const {
             for (typename MetricSet::const_iterator it = metrics.begin(); it != metrics.end(); ++it) {
                 INTEROP_ASSERTMSG(it->tile() > 0, it->lane() << "_" << it->tile() << " @ " << it->cycle());
                 m_map[it->cycle_hash()] = *it;
             }
         }

         template<class MetricSet>
         void populate_id(const MetricSet &, const void *) const {}

         run_metrics::cycle_metric_map_t &m_map;
     };

     struct is_metric_empty
     {
         is_metric_empty() : m_empty(true) {}

         template<class MetricSet>
         void operator()(const MetricSet &metrics) {
             if (metrics.size() > 0) m_empty = false;
         }

         bool empty() const {
             return m_empty;
         }

         bool m_empty;
     };

     struct check_if_groupid_is_empty
     {
         check_if_groupid_is_empty(const constants::metric_group group) : m_empty(true), m_group(group) {}

         template<class MetricSet>
         void operator()(const MetricSet &metrics) {
             if (m_group == static_cast<constants::metric_group>(MetricSet::TYPE)) {
                 m_empty = metrics.empty();
             }
         }

         bool empty() const {
             return m_empty;
         }

         bool m_empty;
         constants::metric_group m_group;
     };

     struct check_if_group_is_empty
     {
         check_if_group_is_empty(const std::string &name) : m_empty(true), m_prefix(name) {}

         template<class MetricSet>
         void operator()(const MetricSet &metrics) {
             if (m_prefix == metrics.prefix()) {
                 m_empty = metrics.empty();
             }
         }

         bool empty() const {
             return m_empty;
         }

         bool m_empty;
         std::string m_prefix;
     };

     struct sort_by_lane_tile_cycle
     {
         template<class MetricSet>
         void operator()(MetricSet &metrics) const {
             typedef typename MetricSet::metric_type metric_type;
             std::sort(metrics.begin(), metrics.end(), is_less < metric_type > );
         }

         template<class T>
         static bool is_less(const T &lhs, const T &rhs) {
             return lhs.id() < rhs.id();
         }
     };

     struct check_for_each_data_source
     {
         check_for_each_data_source(const std::string &f, const size_t last_cycle) :
                 m_run_folder(f), m_last_cycle(last_cycle) {}

         template<class MetricSet>
         void operator()(MetricSet &metrics) const {
             metrics.data_source_exists(io::interop_exists(m_run_folder, metrics, m_last_cycle));
         }

         std::string m_run_folder;
         size_t m_last_cycle;
     };

     class calculate_metric_set_buffer_size
     {
     public:
         calculate_metric_set_buffer_size(const constants::metric_group group) :
                 m_group(group), m_buffer_size(0) {}

         template<class MetricSet>
         void operator()(const MetricSet &metrics) {
             if (m_group == static_cast<constants::metric_group>(MetricSet::TYPE)) {
                 m_buffer_size = io::compute_buffer_size(metrics);
             }
         }

         size_t buffer_size() const {
             return m_buffer_size;
         }

     private:
         constants::metric_group m_group;
         size_t m_buffer_size;
     };

     class list_interop_filenames
     {
     public:
         list_interop_filenames(const constants::metric_group group,
                                std::vector<std::string> &files,
                                const std::string &run_folder,
                                const size_t last_cycle,
                                const bool use_out) :
                 m_group(group),
                 m_files(files),
                 m_run_folder(run_folder),
                 m_last_cycle(last_cycle),
                 m_use_out(use_out) {}

         template<class MetricSet>
         void operator()(const MetricSet &) const
         {
             if (m_group == static_cast<constants::metric_group>(MetricSet::TYPE) ||
                     m_group == constants::UnknownMetricGroup)
             {
                 io::list_interop_filenames<MetricSet>(m_files, m_run_folder, m_last_cycle, m_use_out,
                                                       m_group == constants::UnknownMetricGroup);
             }
         }

     private:
         constants::metric_group m_group;
         std::vector<std::string> &m_files;
         std::string m_run_folder;
         size_t m_last_cycle;
         bool m_use_out;
     };




                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Definitions
                ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                /** Test whether run parameters must be loaded
                 *
                 * This is used to determine channel count and legacy q-score bins
                 *
                 * @param legacy_bin_count known number of bins
                 * @return true if run parameters is required
                 */
                bool run_metrics::is_run_parameters_required(const size_t legacy_bin_count)const
                {
                    return m_run_info.channels().empty() || logic::metric::requires_legacy_bins(count_legacy_bins(legacy_bin_count));
                }
                /** Get number of legacy bins
                 *
                 * @param legacy_bin_count known number of bins
                 * @return number of legacy bins
                 */
                size_t run_metrics::count_legacy_bins(const size_t legacy_bin_count)const
                {
                    if( legacy_bin_count < std::numeric_limits<size_t>::max() ) return legacy_bin_count;
                    if( !get<q_metric>().empty() )
                    {
                        return logic::metric::count_legacy_q_score_bins(get<q_metric>());
                    }
                    else if( !get<q_by_lane_metric>().empty() )
                    {
                        return logic::metric::count_legacy_q_score_bins(get<q_by_lane_metric>());
                    }
                    return std::numeric_limits<size_t>::max();
                }

                /** Test if all metrics are empty
                 *
                 * @return true if all metrics are empty
                 */
                bool run_metrics::empty() const
                {
                    is_metric_empty func;
                    m_metrics.apply(func);
                    return func.empty();
                }

                /** List all filenames for a specific metric
                 *
                 * @param group metric group type
                 * @param files destination interop file names (first one is legacy, all subsequent are by cycle)
                 * @param run_folder run folder location
                 * @param use_out use the copied version
                 */
                void run_metrics::list_filenames(const constants::metric_group group,
                                                 std::vector<std::string>& files,
                                                 const std::string& run_folder,
                                                 const bool use_out)
                throw(invalid_run_info_exception)
                {
                    const size_t last_cycle = run_info().total_cycles();
                    if( last_cycle == 0 ) INTEROP_THROW(invalid_run_info_exception, "RunInfo is empty");
                    m_metrics.apply(list_interop_filenames(group, files, run_folder, last_cycle, use_out));
                }

                /** List all filenames for all metrics
                 *
                 * @param files destination interop file names (first one is legacy, all subsequent are by cycle)
                 * @param run_folder run folder location
                 * @param bycycle if true, list all by cycle file name
                 * @param use_out use the copied version
                 */
                void run_metrics::list_filenames(std::vector<std::string>& files,
                                                 const std::string& run_folder,
                                                 const bool bycycle,
                                                 const bool use_out)
                throw(invalid_run_info_exception)
                {
                    const size_t last_cycle = run_info().total_cycles();
                    if( last_cycle == 0 && bycycle)
                        INTEROP_THROW(invalid_run_info_exception, "RunInfo is empty: " << last_cycle);
                    if(bycycle)
                    {
                        files.clear();
                        m_metrics.apply(
                                list_interop_filenames(constants::UnknownMetricGroup, files, run_folder, last_cycle,
                                                       use_out));
                    }
                    else
                    {
                        files.clear();
                        files.reserve(static_cast<size_t>(constants::MetricCount));
                        m_metrics.apply(
                                list_interop_filenames(constants::UnknownMetricGroup, files, run_folder, 0, use_out));
                    }
                }
                /** Calculate the required size of the buffer for writing
                 *
                 * @param group metric set to write
                 * @return required size of the binary buffer
                 */
                size_t run_metrics::calculate_buffer_size(const constants::metric_group group)const throw(
                io::invalid_argument, io::bad_format_exception)
                {
                    calculate_metric_set_buffer_size calc(group);
                    m_metrics.apply(calc);
                    return calc.buffer_size();
                }

                /** Populate a map of valid tiles
                 *
                 * @param map mapping between tile has and base_metric
                 */
                void run_metrics::populate_id_map(tile_metric_map_t &map) const
                {
                    m_metrics.apply(populate_tile_list(map));
                }

                /** Check if the metric group is empty
                 *
                 * @param group_name prefix of interop group metric
                 * @return true if metric is empty
                 */
                bool run_metrics::is_group_empty(const std::string& group_name) const
                {
                    check_if_group_is_empty func(group_name);
                    m_metrics.apply(func);
                    return func.empty();
                }
                /** Check if the metric group is empty
                 *
                 * @param group_id id of interop group metric
                 * @return true if metric is empty
                 */
                bool run_metrics::is_group_empty(const constants::metric_group group_id) const
                {
                    check_if_groupid_is_empty func(group_id);
                    m_metrics.apply(func);
                    return func.empty();
                }

                /** Populate a map of valid tiles and cycles
                 *
                 * @param map mapping between tile has and base_metric
                 */
                void run_metrics::populate_id_map(cycle_metric_map_t &map) const
                {
                    m_metrics.apply(populate_tile_cycle_list(map));
                }

                /** Sort the metrics by lane, then tile, then cycle
                 *
                 */
                void run_metrics::sort()
                {
                    m_metrics.apply(sort_by_lane_tile_cycle());
                }

                /** Check if the InterOp file for each metric set exists
                 *
                 * This will set the `metric_set::data_source_exists` flag.
                 * @param run_folder run folder path
                 * @param last_cycle last cycle to search for by cycle interops
                 */
                void run_metrics::check_for_data_sources(const std::string &run_folder, const size_t last_cycle)
                {
                    m_metrics.apply(check_for_each_data_source(run_folder, last_cycle));
                }


}}}}
