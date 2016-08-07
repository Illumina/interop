/** Collection of all metric sets for a run
 *
 *  @file
 *  @date 3/9/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include "interop/model/run_metrics.h"
#include "interop/io/metric_file_stream.h"

#include "interop/logic/metric/q_metric.h"
#include "interop/logic/metric/tile_metric.h"
#include "interop/logic/utils/channel.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    struct populate_tile_list
    {
        populate_tile_list(run_metrics::tile_metric_map_t& map) : m_map(map){}

        template<class MetricSet>
        void operator()(const MetricSet &metrics)const
        {
            typedef typename MetricSet::base_t base_t;
            populate_id(metrics, base_t::null());
        }
    private:
        template<class MetricSet>
        void populate_id(const MetricSet &metrics, const void*)const
        {
            for(typename MetricSet::const_iterator it = metrics.begin();it != metrics.end();++it)
            {
                INTEROP_ASSERTMSG(it->tile()>0, it->lane() << "_" << it->tile());
                m_map[it->tile_hash()] = *it;
            }
        }
        template<class MetricSet>
        void populate_id(const MetricSet &, const constants::base_lane_t*)const{}

        run_metrics::tile_metric_map_t& m_map;
    };
    struct populate_tile_cycle_list
    {
        populate_tile_cycle_list(run_metrics::cycle_metric_map_t& map) : m_map(map){}

        template<class MetricSet>
        void operator()(const MetricSet &metrics)const
        {
            typedef typename MetricSet::base_t base_t;
            populate_id(metrics, base_t::null());
        }

    private:
        template<class MetricSet>
        void populate_id(const MetricSet &metrics, const constants::base_cycle_t*)const
        {
            for(typename MetricSet::const_iterator it = metrics.begin();it != metrics.end();++it)
            {
                INTEROP_ASSERTMSG(it->tile()>0, it->lane() << "_" << it->tile() << " @ " << it->cycle());
                m_map[it->id()] = *it;
            }
        }
        template<class MetricSet>
        void populate_id(const MetricSet &, const void*)const{}
        run_metrics::cycle_metric_map_t& m_map;
    };
    struct is_metric_empty
    {
        is_metric_empty() : m_empty(true)
        { }

        template<class MetricSet>
        void operator()(const MetricSet &metrics)
        {
            if (metrics.size() > 0) m_empty = false;
        }

        bool empty() const
        {
            return m_empty;
        }

        bool m_empty;
    };

    struct read_func
    {
        read_func(const std::string &f) : m_run_folder(f)
        { }

        template<class MetricSet>
        int operator()(MetricSet &metrics) const
        {
            try
            {
                io::read_interop(m_run_folder, metrics);
            }
            catch (const io::file_not_found_exception &)
            {
                try
                {
                    io::read_interop(m_run_folder, metrics,
                                     false /** Search for XMetrics.bin not XMetricsOut.bin */);
                }
                catch (const io::file_not_found_exception &)
                { return 1; }
                catch (const io::incomplete_file_exception &)
                { return 2; }
            }
            catch (const io::incomplete_file_exception &)
            { return 2; }
            return 0;
        }

        std::string m_run_folder;
    };
    /** Read binary metrics and XML files from the run folder
     *
     * @param run_folder run folder path
     */
    void run_metrics::read(const std::string &run_folder)
                                            throw(xml::xml_file_not_found_exception,
                                            xml::bad_xml_format_exception,
                                            xml::empty_xml_format_exception,
                                            xml::missing_xml_element_exception,
                                            xml::xml_parse_exception,
                                            io::file_not_found_exception,
                                            io::bad_format_exception,
                                            io::incomplete_file_exception,
                                            io::format_exception,
                                            model::index_out_of_bounds_exception,
                                            model::invalid_tile_naming_method)
    {
        read_metrics(run_folder);
        const size_t count = read_xml(run_folder);
        finalize_after_load(count);
    }

    /** Read XML files: RunInfo.xml and possibly RunParameters.xml
     *
     * @param run_folder run folder path
     */
    size_t run_metrics::read_xml(const std::string &run_folder)
                                    throw(io::file_not_found_exception,
                                    xml::xml_file_not_found_exception,
                                    xml::bad_xml_format_exception,
                                    xml::empty_xml_format_exception,
                                    xml::missing_xml_element_exception,
                                    xml::xml_parse_exception)
    {
        read_run_info(run_folder);
        return read_run_parameters(run_folder);
    }

    /** Read RunInfo.xml
     *
     * @param run_folder run folder path
     */
    void run_metrics::read_run_info(const std::string &run_folder)
                                    throw(xml::xml_file_not_found_exception,
                                    xml::bad_xml_format_exception,
                                    xml::empty_xml_format_exception,
                                    xml::missing_xml_element_exception,
                                    xml::xml_parse_exception)
    {
        m_run_info.read(run_folder);
    }

    /** Read RunParameters.xml if necessary
     *
     * @param run_folder run folder path
     */
    size_t run_metrics::read_run_parameters(const std::string &run_folder) throw(io::file_not_found_exception,
    xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception)
    {
        const size_t count = logic::metric::count_legacy_q_score_bins(get_set<q_metric>());
        if (m_run_info.channels().empty() || logic::metric::requires_legacy_bins(count))
        {

            try
            {
                m_run_parameters.read(run_folder);
            }
            catch (const xml::xml_file_not_found_exception &)
            {
                if (m_run_info.channels().empty())
                    INTEROP_THROW(io::file_not_found_exception,
                                  "RunParameters.xml required for legacy run folders with missing channel names");
                else
                    INTEROP_THROW(io::file_not_found_exception,
                                  "RunParameters.xml required for legacy run folders and is missing");
            }
        }
        return count;
    }

    /** Finalize the metric sets after loading from disk
     *
     * @param count number of bins for legacy q-metrics
     */
    void run_metrics::finalize_after_load(size_t count)
                                        throw(io::format_exception,
                                        model::invalid_tile_naming_method,
                                        model::index_out_of_bounds_exception)
    {
        if (m_run_info.flowcell().naming_method() == constants::UnknownTileNamingMethod)
        {
            m_run_info.set_naming_method(
                    logic::metric::tile_naming_method_from_metric(get_set<metrics::tile_metric>()));
            if (m_run_info.flowcell().naming_method() == constants::UnknownTileNamingMethod)
                m_run_info.set_naming_method(
                        logic::metric::tile_naming_method_from_metric(get_set<metrics::extraction_metric>()));
            if (m_run_info.flowcell().naming_method() == constants::UnknownTileNamingMethod)
                m_run_info.set_naming_method(
                        logic::metric::tile_naming_method_from_metric(get_set<metrics::q_metric>()));
        }
        if (count == std::numeric_limits<size_t>::max())
        {
            if(get_set<q_metric>().size() > 0)
                count = logic::metric::count_legacy_q_score_bins(get_set<q_metric>());
            else if(get_set<q_by_lane_metric>().size())
                count = logic::metric::count_legacy_q_score_bins(get_set<q_by_lane_metric>());
        }
        logic::metric::populate_legacy_q_score_bins(get_set<q_metric>().bins(), m_run_parameters.instrument_type(),
                                                    count);
        if (get_set<q_metric>().size() > 0 && get_set<q_collapsed_metric>().size() == 0)
            logic::metric::create_collapse_q_metrics(get_set<q_metric>(), get_set<q_collapsed_metric>());
        if (get_set<q_metric>().size() > 0 && get_set<q_by_lane_metric>().size() == 0)
            logic::metric::create_q_metrics_by_lane(get_set<q_metric>(), get_set<q_by_lane_metric>());
        logic::metric::populate_cumulative_distribution(get_set<q_metric>());
        logic::metric::populate_cumulative_distribution(get_set<q_by_lane_metric>());
        logic::metric::populate_cumulative_distribution(get_set<q_collapsed_metric>());
        INTEROP_ASSERT(get_set<q_metric>().size() == 0 || get_set<q_metric>().size() == get_set<q_collapsed_metric>().size());
        if (m_run_info.channels().empty())
        {
            legacy_channel_update(m_run_parameters.instrument_type());
            if (m_run_info.channels().empty())
                INTEROP_THROW(io::format_exception,
                              "Channel names are missing from the RunInfo.xml, and RunParameters.xml does not contain sufficient information on the instrument run.");
        }
        if (!empty() && run_info().flowcell().naming_method() == constants::UnknownTileNamingMethod)
            INTEROP_THROW(model::invalid_tile_naming_method, "Unknown tile naming method - update your RunInfo.xml");
        extraction_metric_set_t& extraction_metrics = get_set<extraction_metric>();
        // Trim excess channel data for imaging table
        for(extraction_metric_set_t::iterator it = extraction_metrics.begin(); it != extraction_metrics.end();++it)
            it->trim(run_info().channels().size());
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

    /** Update channels for legacy runs
     *
     * @param type instrument type
     */
    void run_metrics::legacy_channel_update(const constants::instrument_type type)
    {
        m_run_info.channels(logic::utils::update_channel_from_instrument_type(type));
    }

    /** Set the tile naming method
     *
     * @param naming_method tile naming method
     */
    void run_metrics::set_naming_method(const constants::tile_naming_method naming_method)
    {
        m_run_info.set_naming_method(naming_method);
    }

    /** Read binary metrics from the run folder
     *
     * This function ignores:
     *  - Missing InterOp files
     *  - Incomplete InterOp files
     *  - Missing RunParameters.xml for non-legacy run folders
     *
     * @param run_folder run folder path
     */
    void run_metrics::read_metrics(const std::string &run_folder) throw(
    io::file_not_found_exception,
    io::bad_format_exception,
    io::incomplete_file_exception)
    {
        m_metrics.apply(read_func(run_folder));
    }

    /** Populate a map of valid tiles
     *
     * @param map mapping between tile has and base_metric
     */
    void run_metrics::populate_id_map(tile_metric_map_t& map)const
    {
        m_metrics.apply(populate_tile_list(map));
    }
    /** Populate a map of valid tiles and cycles
     *
     * @param map mapping between tile has and base_metric
     */
    void run_metrics::populate_id_map(cycle_metric_map_t& map)const
    {
        m_metrics.apply(populate_tile_cycle_list(map));
    }

}}}}