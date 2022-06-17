/** Collection of all metric sets for a run
 *
 *  @file
 *  @date 3/9/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#ifdef _OPENMP
#include <omp.h>
#endif

#include "interop/model/run_metrics.h"

#include "interop/logic/metric/q_metric.h"
#include "interop/logic/metric/tile_metric.h"
#include "interop/logic/metric/index_metric.h"
#include "interop/logic/utils/channel.h"
#include "interop/logic/metric/dynamic_phasing_metric.h"
#include "interop/logic/metric/extended_tile_metric.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{

    struct clear_metric
    {
        template<class MetricSet>
        void operator()(MetricSet &metrics)const
        {
            metrics.clear();
        }
    };

    struct read_func
    {
        typedef const unsigned char* bool_pointer;
        read_func(const std::string &f, bool_pointer load_metric_check=0, const bool skip_loaded=false) :
                m_run_folder(f),
                m_load_metric_check(load_metric_check),
                m_are_all_files_missing(true),
                m_skip_loaded(skip_loaded)
        {}

        template<class MetricSet>
        int operator()(MetricSet &metrics) const
        {
            // If the m_load_metric_check is not set, read in the metric
            // Otherwise, check if the metric should be read and that it is not empty
            // This logic is for SAV OnDemand (TM) loading
            const constants::metric_group group = static_cast<constants::metric_group>(MetricSet::TYPE);
            const bool is_aggregated_always = (group == constants::Index || group == constants::QByLane || group == constants::QCollapsed);
            if(m_load_metric_check != 0 && (m_load_metric_check[MetricSet::TYPE] == 0 || !metrics.empty()))
            {
                return 0;
            }
            else if(m_skip_loaded && !metrics.empty())
            {
                return 0;
            }
            else if (m_load_metric_check == 0 || m_load_metric_check[MetricSet::TYPE] != 0)
            {
                metrics.clear();
            }
            try
            {
                io::read_interop(m_run_folder, metrics);
                if(m_are_all_files_missing && !is_aggregated_always) m_are_all_files_missing=false;
            }
            catch (const io::file_not_found_exception &)
            {
                return 1;
            }
            catch (const io::incomplete_file_exception &)
            {
                if(m_are_all_files_missing && !is_aggregated_always) m_are_all_files_missing=false;
                return 2;
            }
            return 0;
        }

        bool are_all_files_missing()const
        {
            return m_are_all_files_missing;
        }

        std::string m_run_folder;
        bool_pointer m_load_metric_check;
        mutable bool m_are_all_files_missing;
        bool m_skip_loaded;
    };

    struct write_func
    {
        write_func(const std::string &f, const bool use_out) : m_run_folder(f), m_use_out(use_out)
        {}

        template<class MetricSet>
        void operator()(const MetricSet &metrics) const
        {
            io::write_interop(m_run_folder, metrics, m_use_out);
        }

        std::string m_run_folder;
        bool m_use_out;
    };

    struct validate_run_info
    {
        validate_run_info(const run::info& info) : m_info(info){}

        template<class MetricSet>
        void operator()(MetricSet &metrics)const
        {
            typedef typename MetricSet::base_t base_t;
            validate(metrics, base_t::null());
        }
    private:
        template<class MetricSet>
        void validate(const MetricSet &metrics, const constants::base_tile_t*)const
        {
            const std::string name =  io::interop_basename<MetricSet>();
            for(typename MetricSet::const_iterator it = metrics.begin();it != metrics.end();++it)
            {
                m_info.validate(it->lane(), it->tile(), name);
            }
        }
        template<class MetricSet>
        void validate(MetricSet &metrics, const constants::base_cycle_t*)const
        {
            const std::string name =  io::interop_basename<MetricSet>();
            bool exception_is_thrown = false;
            std::string exception_string = "";
            for(typename MetricSet::iterator it = metrics.begin();it != metrics.end();++it)
            {
                try
                {
                    m_info.validate_cycle(it->lane(), it->tile(), it->cycle(), name);
                }
                catch(const model::invalid_run_info_cycle_exception& ex)
                {
                    metrics.remove(it);
                    --it;
                    exception_is_thrown = true;
                    exception_string = ex.what();
                }
            }
            if(exception_is_thrown)
                INTEROP_THROW(model::invalid_run_info_cycle_exception, exception_string + ": truncating invalid entries");
        }
        template<class MetricSet>
        void validate(const MetricSet &metrics, const constants::base_read_t*)const
        {
            const std::string name = io::interop_basename<MetricSet>();
            for(typename MetricSet::const_iterator it = metrics.begin();it != metrics.end();++it)
            {
                m_info.validate_read(it->lane(), it->tile(), it->read(), name);
            }
        }
        template<class MetricSet>
        void validate(const MetricSet &, const void*)const{}

        const run::info& m_info;
    };

    class rebuild_index
    {
    public:
        template<class MetricSet>
        void operator()(MetricSet &metrics)const
        {
            metrics.rebuild_index();
        }
    };

    class determine_tile_naming_method
    {
    public:
        determine_tile_naming_method() : m_naming_method(constants::UnknownTileNamingMethod){}
        template<class MetricSet>
        void operator()(const MetricSet &metrics)
        {
            typedef typename MetricSet::base_t base_t;
            determine(metrics, base_t::null());
        }
        constants::tile_naming_method naming_method()const
        {
            return m_naming_method;
        }

    private:
        template<class MetricSet>
        void determine(const MetricSet &metrics, const void*)
        {
            if (!metrics.empty() && m_naming_method == constants::UnknownTileNamingMethod)
                m_naming_method = logic::metric::tile_naming_method_from_metric(metrics);
        }
        template<class MetricSet>
        void determine(const MetricSet &, const constants::base_run_t *)
        {
        }

    private:
        constants::tile_naming_method m_naming_method;
    };

    struct read_by_cycle_func
    {
        typedef const unsigned char* bool_pointer;

        read_by_cycle_func(const std::string &f, const size_t last_cycle, bool_pointer load_metric_check=0) :
                m_run_folder(f), m_last_cycle(last_cycle), m_load_metric_check(load_metric_check)
        {}

        template<class MetricSet>
        int operator()(MetricSet &metrics) const
        {
            // If the m_load_metric_check is not set, read in the metric
            // Otherwise, check if the metric should be read and that it is not empty
            // This logic is for SAV OnDemand (TM) loading
            if(m_load_metric_check != 0 && (m_load_metric_check[MetricSet::TYPE] == 0 || !metrics.empty()))
            {
                return 0;
            }
            io::read_interop_by_cycle(m_run_folder, metrics, m_last_cycle);
            return 0;
        }

        std::string m_run_folder;
        size_t m_last_cycle;
        bool_pointer m_load_metric_check;
    };

    class read_metric_set_from_binary_buffer
    {
    public:
        read_metric_set_from_binary_buffer(const constants::metric_group group,
                                           ::uint8_t* buffer,
                                           const size_t buffer_size) :
                m_group(group),
                m_buffer(buffer),
                m_buffer_size(buffer_size){}
        template<class MetricSet>
        void operator()(MetricSet &metrics) const
        {
            if(m_group == static_cast<constants::metric_group>(MetricSet::TYPE))
            {
                io::read_interop_from_buffer(m_buffer, m_buffer_size, metrics);
            }
        }
    private:
        constants::metric_group m_group;
        uint8_t* m_buffer;
        size_t m_buffer_size;
    };

    class write_metric_set_to_binary_buffer
    {
    public:
        write_metric_set_to_binary_buffer(const constants::metric_group group,
                                          uint8_t* buffer,
                                          const size_t buffer_size) :
                m_group(group),
                m_buffer(buffer),
                m_buffer_size(buffer_size){}
        template<class MetricSet>
        void operator()(const MetricSet &metrics) const
        {
            if(m_group == static_cast<constants::metric_group>(MetricSet::TYPE))
            {
                io::write_interop_to_buffer(metrics, m_buffer, m_buffer_size);
            }
        }

    private:
        constants::metric_group m_group;
        uint8_t* m_buffer;
        size_t m_buffer_size;
    };

    class append_tiles_functor
    {
    public:
        append_tiles_functor(const run_metrics& metrics, const metric_base::base_metric& tile_id) :
                m_metrics(metrics), m_tile_id(tile_id)
        {
        }

    public:
        template<class MetricSet>
        void operator()(MetricSet &metrics) const
        {
            metrics.append_tiles(m_metrics.get<MetricSet>(), m_tile_id);
        }

    private:
        const run_metrics& m_metrics;
        const metric_base::base_metric m_tile_id;
    };

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Definitions
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    /** Copy records only for a specific tile
     *
     * @param origin full metric set
     * @param tile_id selected tile
     */
    void run_metrics::copy_tile(const run_metrics& metrics, const metric_base::base_metric& tile_id)
    {
        clear();
        append_tiles(metrics, tile_id);
    }
    /** Adds records only for a specific tile
     *
     * @param origin full metric set
     * @param tile_id selected tile
     */
    void run_metrics::append_tiles(const run_metrics& metrics, const metric_base::base_metric& tile_id)
    {
        m_metrics.apply(append_tiles_functor(metrics, tile_id));
    }

    /** Read binary metrics and XML files from the run folder
     *
     * @param run_folder run folder path
     * @param thread_count number of threads to use for network loading
     */
    void run_metrics::read(const std::string &run_folder, const size_t thread_count)
    INTEROP_THROW_SPEC((xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception,
    io::file_not_found_exception,
    io::bad_format_exception,
    io::incomplete_file_exception,
    model::invalid_channel_exception,
    model::index_out_of_bounds_exception,
    model::invalid_tile_naming_method,
    model::invalid_tile_list_exception,
    model::invalid_run_info_exception,
    model::invalid_run_info_cycle_exception,
    model::invalid_parameter))
    {
        clear();
        const size_t count = read_xml(run_folder);
        read_metrics(run_folder, run_info().total_cycles(), thread_count);
        finalize_after_load(count);
    }
    /** Read binary metrics and XML files from the run folder
     *
     * @note This function does not clear
     * @param run_folder run folder path
     * @param valid_to_load list of metrics to load
     * @param thread_count number of threads to use for network loading
     * @param skip_loaded skip metrics that are already loaded
     */
    void run_metrics::read(const std::string &run_folder,
                           const std::vector<unsigned char>& valid_to_load,
                           const size_t thread_count,
                           const bool skip_loaded)
    INTEROP_THROW_SPEC((xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception,
    io::file_not_found_exception,
    io::bad_format_exception,
    io::incomplete_file_exception,
    model::invalid_channel_exception,
    model::index_out_of_bounds_exception,
    model::invalid_tile_naming_method,
    model::invalid_tile_list_exception,
    model::invalid_run_info_exception,
    model::invalid_run_info_cycle_exception,
    invalid_parameter))
    {
        read_run_info(run_folder);
        read_metrics(run_folder, run_info().total_cycles(), valid_to_load, thread_count, skip_loaded);
        const size_t count = read_run_parameters(run_folder);
        finalize_after_load(count);
        check_for_data_sources(run_folder, run_info().total_cycles());
    }

    /** Read XML files: RunInfo.xml and possibly RunParameters.xml
     *
     * @param run_folder run folder path
     */
    size_t run_metrics::read_xml(const std::string &run_folder)
    INTEROP_THROW_SPEC((io::file_not_found_exception,
    xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception))
    {
        read_run_info(run_folder);
        return read_run_parameters(run_folder);
    }

    /** Read RunInfo.xml
     *
     * @param run_folder run folder path
     */
    void run_metrics::read_run_info(const std::string &run_folder)
    INTEROP_THROW_SPEC((xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception))
    {
        m_run_info.read(run_folder);
    }

    /** Read RunParameters.xml if necessary
     *
     * @param run_folder run folder path
     * @param force_load force loading of run parameters
     */
    size_t run_metrics::read_run_parameters(const std::string &run_folder, const bool force_load) INTEROP_THROW_SPEC((io::file_not_found_exception,
    xml::xml_file_not_found_exception,
    xml::bad_xml_format_exception,
    xml::empty_xml_format_exception,
    xml::missing_xml_element_exception,
    xml::xml_parse_exception))
    {
        const size_t count = count_legacy_bins();
        if (m_run_info.channels().empty() || logic::metric::requires_legacy_bins(count) || force_load)
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
                else if(logic::metric::requires_legacy_bins(count))
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
    INTEROP_THROW_SPEC((model::invalid_channel_exception,
    model::invalid_parameter,
    model::invalid_tile_naming_method,
    model::index_out_of_bounds_exception,
    model::invalid_tile_list_exception,
    model::invalid_run_info_exception,
    model::invalid_run_info_cycle_exception))
    {
        if (m_run_info.flowcell().naming_method() == constants::UnknownTileNamingMethod)
        {
            determine_tile_naming_method naming_method_determinator;
            m_metrics.apply(naming_method_determinator);
            m_run_info.set_naming_method( naming_method_determinator.naming_method());
        }
        if(!get<model::metrics::index_metric>().empty())
        {
            logic::metric::populate_indices(get<model::metrics::tile_metric>(), get<model::metrics::index_metric>());
        }
        if (count == std::numeric_limits<size_t>::max())
        {
            count = count_legacy_bins();

            // BaseSpace calls finalize_after_load with the default argument (SAV does not)
            // We need to ensure rebuild index is done for BaseSpace
            // This is already taken care of for SAV by the read_by_cycle function
            m_metrics.apply(rebuild_index());
        }
        if(logic::metric::requires_legacy_bins(count))
        {
            logic::metric::populate_legacy_q_score_bins(get<q_metric>().bins(), m_run_parameters.instrument_type(),
                                                        count);
            logic::metric::populate_legacy_q_score_bins(get<q_by_lane_metric>().bins(),
                                                        m_run_parameters.instrument_type(),
                                                        count);
            logic::metric::compress_q_metrics(get<q_metric>());
            logic::metric::compress_q_metrics(get<q_by_lane_metric>());
        }
        if (get<q_metric>().size() > 0 && get<q_collapsed_metric>().size() == 0)
        {
            logic::metric::create_collapse_q_metrics(get<q_metric>(), get<q_collapsed_metric>());
        }
        INTEROP_ASSERTMSG(
                get<q_metric>().size() == 0 ||
                get<q_metric>().size() == get<q_collapsed_metric>().size(),
                get<q_metric>().size() << " == " << get<q_collapsed_metric>().size());
        if (get<q_metric>().size() > 0 && get<q_by_lane_metric>().size() == 0)
            logic::metric::create_q_metrics_by_lane(get<q_metric>(),
                                                    get<q_by_lane_metric>(),
                                                    m_run_parameters.instrument_type());
        logic::metric::populate_cumulative_distribution(get<q_metric>());
        logic::metric::populate_cumulative_distribution(get<q_by_lane_metric>());
        logic::metric::populate_cumulative_distribution(get<q_collapsed_metric>());
        if(!get<model::metrics::extended_tile_metric>().empty() && !get<model::metrics::tile_metric>().empty())
        {
            logic::metric::populate_percent_occupied(get<model::metrics::tile_metric>(),
                                                     get<model::metrics::extended_tile_metric>());
        }

        if (m_run_info.channels().empty())
        {
            legacy_channel_update(m_run_parameters.instrument_type());
            if (m_run_info.channels().empty())
                INTEROP_THROW(model::invalid_channel_exception,
                              "Channel names are missing from the RunInfo.xml, and RunParameters.xml does not contain sufficient information on the instrument run.");
        }
        typedef metric_base::metric_set< extraction_metric > extraction_metric_set_t;
        extraction_metric_set_t &extraction_metrics = get<extraction_metric>();
        // Trim excess channel data for imaging table
        extraction_metrics.channel_count(run_info().channels().size());
        for (extraction_metric_set_t::iterator it = extraction_metrics.begin(); it != extraction_metrics.end(); ++it)
            it->trim(run_info().channels().size());
        typedef metric_base::metric_set<image_metric> image_metric_set_t;
        image_metric_set_t &image_metrics = get<image_metric>();
        if(run_info().channels().size() < image_metrics.channel_count())
        {
            image_metrics.channel_count(run_info().channels().size());
            for (image_metric_set_t::iterator it = image_metrics.begin(); it != image_metrics.end(); ++it)
                it->trim(run_info().channels().size());
        }

        if (!empty())
        {
            if(run_info().flowcell().naming_method() == constants::UnknownTileNamingMethod)
                INTEROP_THROW(model::invalid_tile_naming_method, "Unknown tile naming method - update your RunInfo.xml");
            m_run_info.validate();
            validate();
            m_run_info.validate_tiles();
        }

        if(!get<model::metrics::phasing_metric>().empty())
        {
            logic::summary::read_cycle_vector_t cycle_to_read;
            logic::summary::map_read_to_cycle_number(run_info().reads().begin(),
                                                     run_info().reads().end(),
                                                     cycle_to_read);
            logic::metric::populate_dynamic_phasing_metrics(get<model::metrics::phasing_metric>(),
                                                            cycle_to_read,
                                                            get<model::metrics::dynamic_phasing_metric>(),
                                                            get<model::metrics::tile_metric>());
        }
    }

    /** Clear all the metrics
     */
    void run_metrics::clear()
    {
        m_run_info = run::info();
        m_run_parameters = run::parameters();
        m_metrics.apply(clear_metric());
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
     * @param last_cycle last cycle to search for by cycle interops
     * @param thread_count number of threads to use for network loading
     */
    void run_metrics::read_metrics(const std::string &run_folder, const size_t last_cycle, const size_t thread_count)
    INTEROP_THROW_SPEC((
    io::file_not_found_exception,
    io::bad_format_exception,
    io::incomplete_file_exception))
    {
        (void)thread_count;
#ifdef _OPENMP
        if(thread_count > 1)
        {
            std::vector<unsigned char> valid_to_load(constants::MetricCount, 1);
            read_metrics(run_folder, last_cycle, valid_to_load, thread_count);
        }
        else{
#endif
            read_func read_functor(run_folder);
            m_metrics.apply(read_functor);
            if (read_functor.are_all_files_missing())
            {
                m_metrics.apply(read_by_cycle_func(run_folder, last_cycle));
            }
#ifdef _OPENMP
        }
#endif
    }

    /** Read binary metrics from the run folder
     *
     * This function ignores:
     *  - Missing InterOp files
     *  - Incomplete InterOp files
     *  - Missing RunParameters.xml for non-legacy run folders
     *
     * @param run_folder run folder path
     * @param last_cycle last cycle to search for by cycle interops
     * @param valid_to_load list of metrics to load
     * @param thread_count number of threads to use for network loading
     * @param skip_loaded skip metrics that are already loaded
     */
    void run_metrics::read_metrics(const std::string &run_folder,
                                   const size_t last_cycle,
                                   const std::vector<unsigned char>& valid_to_load,
                                   const size_t thread_count,
                                   const bool skip_loaded)
    INTEROP_THROW_SPEC((io::file_not_found_exception,
    io::bad_format_exception,
    io::incomplete_file_exception,
    model::invalid_parameter))
    {
        (void)thread_count;
        if(valid_to_load.empty()) return;
        if(valid_to_load.size() != constants::MetricCount)
            INTEROP_THROW(invalid_parameter, "Boolean array valid_to_load does not match expected number of metrics: "
                    << valid_to_load.size() << " != " << constants::MetricCount);

        bool all_files_are_missing = true;
#ifdef _OPENMP
        if(thread_count > 1)
        {
            std::vector<bool> local_files_missing(thread_count, true);
            std::vector<size_t> offset;
            offset.reserve(valid_to_load.size());
            for(size_t i=0;i<valid_to_load.size();++i)
                if(valid_to_load[i]) offset.push_back(i);
            std::vector< std::vector<unsigned char> > valid_to_load_local(thread_count, std::vector<unsigned char>(valid_to_load.size(), 0));
            bool exception_thrown = false;
            std::string exception_msg;
#           pragma omp parallel for default(shared) num_threads(static_cast<int>(thread_count)) schedule(dynamic)
            for(int i=0;i<static_cast<int>(offset.size());++i)
            {
#               pragma omp flush(exception_thrown)
                if(exception_thrown) continue;
                valid_to_load_local[ omp_get_thread_num() ][offset[i]] = 1;
                read_func read_functor_l(run_folder, &valid_to_load_local[ omp_get_thread_num() ].front(), skip_loaded);
                try{
                    m_metrics.apply(read_functor_l);
                }
                catch(const std::exception& ex)
                {
#pragma             omp critical(SaveMessage)
                    exception_msg = ex.what();

                    exception_thrown = true;
#pragma             omp flush(exception_thrown)
                }
                valid_to_load_local[ omp_get_thread_num() ][offset[i]] = 0;
                local_files_missing[omp_get_thread_num()] = local_files_missing[omp_get_thread_num()] && read_functor_l.are_all_files_missing();
            }
            if(exception_thrown)
                throw io::bad_format_exception(exception_msg);
            for(size_t i=0;i<local_files_missing.size();++i)
                all_files_are_missing = all_files_are_missing && local_files_missing[i];
        }
        else{
#endif
            read_func read_functor(run_folder, &valid_to_load.front(), skip_loaded);
            m_metrics.apply(read_functor);
            all_files_are_missing = read_functor.are_all_files_missing();
#ifdef _OPENMP
        }
#endif
        if (all_files_are_missing)
        {
#ifdef _OPENMP
            if(thread_count > 1)
            {
                std::vector<size_t> offset;
                offset.reserve(valid_to_load.size());
                for(size_t i=0;i<valid_to_load.size();++i)
                    if(valid_to_load[i]) offset.push_back(i);
                std::vector< std::vector<unsigned char> > valid_to_load_local(thread_count, std::vector<unsigned char>(valid_to_load.size(), 0));
                bool exception_thrown = false;
                std::string exception_msg;
#               pragma omp parallel for default(shared) num_threads(static_cast<int>(thread_count)) schedule(dynamic)
                for(int i=0;i<static_cast<int>(offset.size());++i)
                {
#               pragma omp flush(exception_thrown)
                    valid_to_load_local[ omp_get_thread_num() ][offset[i]] = 1;
                    try{
                        m_metrics.apply(read_by_cycle_func(run_folder, last_cycle, &valid_to_load_local[ omp_get_thread_num() ].front()));
                    }
                    catch(const std::exception& ex)
                    {
#pragma                 omp critical(SaveMessage)
                        exception_msg = ex.what();

                        exception_thrown = true;
#pragma                 omp flush(exception_thrown)
                    }
                    valid_to_load_local[ omp_get_thread_num() ][offset[i]] = 0;
                }
                if(exception_thrown)
                    throw io::bad_format_exception(exception_msg);
            }
            else
            {
#endif
                m_metrics.apply(read_by_cycle_func(run_folder, last_cycle, &valid_to_load.front()));
#ifdef _OPENMP
            }
#endif
        }
    }

    /** Write binary metrics to the run folder
     *
     * @param run_folder run folder path
         * @param use_out use the copied version
     */
    void run_metrics::write_metrics(const std::string &run_folder, const bool use_out) const
    INTEROP_THROW_SPEC((io::file_not_found_exception,
    io::bad_format_exception))
    {
        m_metrics.apply(write_func(run_folder, use_out));
    }

    /** Read a single metric set from a binary buffer
     *
     * @param group metric set to write
     * @param buffer binary buffer
     * @param buffer_size size of binary buffer
     */
    void run_metrics::read_metrics_from_buffer(const constants::metric_group group,
                                               ::uint8_t* buffer,
                                               const size_t buffer_size) INTEROP_THROW_SPEC((
    io::file_not_found_exception,
    io::bad_format_exception,
    io::incomplete_file_exception,
    model::index_out_of_bounds_exception))
    {
        m_metrics.apply(read_metric_set_from_binary_buffer(group, buffer, buffer_size));
    }
    /** Write a single metric set to a binary buffer
     *
     * @param group metric set to write
     * @param buffer binary buffer
     * @param buffer_size size of binary buffer
     */
    void run_metrics::write_metrics_to_buffer(const constants::metric_group group,
                                              ::uint8_t* buffer,
                                              const size_t buffer_size)const INTEROP_THROW_SPEC((
    io::invalid_argument,
    io::bad_format_exception,
    io::incomplete_file_exception))
    {
        m_metrics.apply(write_metric_set_to_binary_buffer(group, buffer, buffer_size));
    }

    /** Validate whether the RunInfo.xml matches the InterOp files
     *
     * @throws invalid_run_info_exception
     */
    void run_metrics::validate() INTEROP_THROW_SPEC((invalid_run_info_exception, invalid_run_info_cycle_exception))
    {
        m_metrics.apply(validate_run_info(m_run_info));
    }

}}}}

