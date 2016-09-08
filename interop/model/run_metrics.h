/** Collection of all metric sets for a run
 *
 *  @file
 *  @date 3/9/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/util/exception.h"
#include "interop/util/object_list.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/stream_exceptions.h"
#include "interop/model/run/info.h"
#include "interop/model/run/parameters.h"

//Metrics
#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/metrics/image_metric.h"
#include "interop/model/metrics/index_metric.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/model/metrics/q_by_lane_metric.h"
#include "interop/model/metrics/q_collapsed_metric.h"
#include "interop/model/metrics/tile_metric.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{

    /**  Collection of all metric sets for a run
     *
     * @ingroup run_metrics
     * @see corrected_intensity_metrics
     * @see error_metrics
     * @see extraction_metrics
     * @see image_metrics
     * @see index_metrics
     * @see q_metrics
     * @see tile_metrics
     * @see q_by_lane_metric
     * @see q_collapsed_metric
     */
    class run_metrics
    {
        typedef make_type_list<
                corrected_intensity_metric,
                error_metric,
                extraction_metric,
                image_metric,
                index_metric,
                q_metric,
                q_by_lane_metric,
                q_collapsed_metric,
                tile_metric
        >::result_t metric_type_list_t;
        template<class T>
        struct create_metric_set
        {
            typedef metric_base::metric_set<T> result_t;
        };
        typedef transform_type_list<metric_type_list_t, create_metric_set>::result_t metric_set_list_t;
        typedef object_list<metric_set_list_t> metric_list_t;

    public:
        /** Define an id type */
        typedef metric_base::base_metric::id_t id_t;
        /** Define corrected intensity metric set */
        typedef metric_base::metric_set<corrected_intensity_metric> corrected_intensity_metric_set_t;
        /** Define error metric set */
        typedef metric_base::metric_set<error_metric> error_metric_set_t;
        /** Define extraction metric set */
        typedef metric_base::metric_set<extraction_metric> extraction_metric_set_t;
        /** Define image metric set */
        typedef metric_base::metric_set<image_metric> image_metric_set_t;
        /** Define index metric set */
        typedef metric_base::metric_set<index_metric> index_metric_set_t;
        /** Define q-metric set */
        typedef metric_base::metric_set<q_metric> q_metric_set_t;
        /** Define tile metric set */
        typedef metric_base::metric_set<tile_metric> tile_metric_set_t;
        /** Define collapsed q-metric set */
        typedef metric_base::metric_set<q_collapsed_metric> q_collapsed_metric_set_t;
        /** Define by lane q-metric set */
        typedef metric_base::metric_set<q_by_lane_metric> q_by_lane_metric_set_t;
        /** Define a map of ids to a base metric */
        typedef std::map<id_t, metric_base::base_metric> tile_metric_map_t;
        /** Define a map of ids to a base cycle metric */
        typedef std::map<id_t, metric_base::base_cycle_metric> cycle_metric_map_t;


    public:
        /** Constructor
         */
        run_metrics()
        {
        }

        /** Constructor
         *
         * @param run_info run information
         * @param run_param run parameters
         */
        run_metrics(const run::info &run_info, const run::parameters &run_param = run::parameters()) :
                m_run_info(run_info),
                m_run_parameters(run_param)
        {
        }

    public:
        /** @defgroup run_metrics Run Metrics
         *
         * All the InterOp data as well as the RunInfo
         *
         * @ref illumina::interop::model::metrics::run_metrics "See full class description"
         * @{
         */
        /** Read binary metrics and XML files from the run folder
         *
         * @param run_folder run folder path
         */
        void read(const std::string &run_folder) throw(xml::xml_file_not_found_exception,
        xml::bad_xml_format_exception,
        xml::empty_xml_format_exception,
        xml::missing_xml_element_exception,
        xml::xml_parse_exception,
        io::file_not_found_exception,
        io::bad_format_exception,
        io::incomplete_file_exception,
        io::format_exception,
        model::index_out_of_bounds_exception,
        model::invalid_tile_naming_method,
        model::invalid_run_info_exception);
        /** Read binary metrics and XML files from the run folder
         *
         * @param run_folder run folder path
         * @param valid_to_load list of metrics to load
         */
        void read(const std::string &run_folder, const std::vector<unsigned char>& valid_to_load)
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
        model::invalid_tile_naming_method,
        model::invalid_run_info_exception,
        invalid_parameter);

        /** Read XML files: RunInfo.xml and possibly RunParameters.xml
         *
         * @param run_folder run folder path
         */
        size_t read_xml(const std::string &run_folder) throw(io::file_not_found_exception,
        xml::xml_file_not_found_exception,
        xml::bad_xml_format_exception,
        xml::empty_xml_format_exception,
        xml::missing_xml_element_exception,
        xml::xml_parse_exception);

        /** Read RunInfo.xml
         *
         * @param run_folder run folder path
         */
        void read_run_info(const std::string &run_folder) throw(xml::xml_file_not_found_exception,
        xml::bad_xml_format_exception,
        xml::empty_xml_format_exception,
        xml::missing_xml_element_exception,
        xml::xml_parse_exception);

        /** Read RunParameters.xml if necessary
         *
         * @param run_folder run folder path
         */
        size_t read_run_parameters(const std::string &run_folder) throw(io::file_not_found_exception,
        xml::xml_file_not_found_exception,
        xml::bad_xml_format_exception,
        xml::empty_xml_format_exception,
        xml::missing_xml_element_exception,
        xml::xml_parse_exception);

        /** Finalize the metric sets after loading from disk
         *
         * @param count number of bins for legacy q-metrics
         */
        void finalize_after_load(size_t count = std::numeric_limits<size_t>::max()) throw(io::format_exception,
        model::invalid_tile_naming_method,
        model::index_out_of_bounds_exception,
        model::invalid_run_info_exception);

        /** Test if all metrics are empty
         *
         * @return true if all metrics are empty
         */
        bool empty() const;

        /** Update channels for legacy runs
         *
         * @param type instrument type
         */
        void legacy_channel_update(const constants::instrument_type type);

        /** Set the tile naming method
         *
         * @param naming_method tile naming method
         */
        void set_naming_method(const constants::tile_naming_method naming_method);

    public:
        /** Get the set of corrected intensity metrics
         *
         * @return set of corrected intensity metrics
         */
        corrected_intensity_metric_set_t &corrected_intensity_metric_set()
        {
            return get_set<corrected_intensity_metric>();
        }

        /** Get the set of error metrics
         *
         * @return set of error metrics
         */
        error_metric_set_t &error_metric_set()
        {
            return get_set<error_metric>();
        }

        /** Get the set of extraction metrics
         *
         * @return set of extraction metrics
         */
        extraction_metric_set_t &extraction_metric_set()
        {
            return get_set<extraction_metric>();
        }

        /** Get the set of image metrics
         *
         * @return set of image metrics
         */
        image_metric_set_t &image_metric_set()
        {
            return get_set<image_metric>();
        }

        /** Get the set of index metrics
         *
         * @return set of index metrics
         */
        index_metric_set_t &index_metric_set()
        {
            return get_set<index_metric>();
        }

        /** Get the set of quality metrics
         *
         * @return set of quality metrics
         */
        q_metric_set_t &q_metric_set()
        {
            return get_set<q_metric>();
        }

        /** Get the set of collapsed quality metrics
         *
         * @return set of collapsed quality metrics
         */
        q_collapsed_metric_set_t &q_collapsed_metric_set()
        {
            return get_set<q_collapsed_metric>();
        }

        /** Get the set of by lane quality metrics
         *
         * @return set of by lane quality metrics
         */
        q_by_lane_metric_set_t &q_by_lane_metric_set()
        {
            return get_set<q_by_lane_metric>();
        }

        /** Get the set of tile metrics
         *
         * @return set of tile metrics
         */
        tile_metric_set_t &tile_metric_set()
        {
            return get_set<tile_metric>();
        }

        /** Set the set of tile metrics
         *
         * @param set set of tile metrics
         */
        void tile_metric_set(const tile_metric_set_t &set)
        {
            get_set<tile_metric>() = set;
        }

        /** Set the set of collapsed q-metrics
         *
         * @param set set of collapsed q-metrics
         */
        void q_collapsed_metric_set(const q_collapsed_metric_set_t &set)
        {
            get_set<q_collapsed_metric>() = set;
        }

        /** Set the set of by lane q-metrics
         *
         * @param set set of by lane q-metrics
         */
        void q_by_lane_metric_set(const q_by_lane_metric_set_t &set)
        {
            get_set<q_by_lane_metric>() = set;
        }

        /** Set the set of q-metrics
         *
         * @param set set of q-metrics
         */
        void q_metric_set(const q_metric_set_t &set)
        {
            get_set<q_metric>() = set;
        }

        /** Set the set of index metrics
         *
         * @param set set of index metrics
         */
        void index_metric_set(const index_metric_set_t &set)
        {
            get_set<index_metric>() = set;
        }

        /** Set the set of image metrics
         *
         * @param set set of image metrics
         */
        void image_metric_set(const image_metric_set_t &set)
        {
            get_set<image_metric>() = set;
        }

        /** Set the set of extraction metrics
         *
         * @param set set of extraction metrics
         */
        void extraction_metric_set(const extraction_metric_set_t &set)
        {
            get_set<extraction_metric>() = set;
        }

        /** Set the set of error metrics
         *
         * @param set set of error metrics
         */
        void error_metric_set(const error_metric_set_t &set)
        {
            get_set<error_metric>() = set;
        }

        /** Set the set of corrected intensity metrics
         *
         * @param set set of corrected intensity metrics
         */
        void corrected_intensity_metric_set(const corrected_intensity_metric_set_t &set)
        {
            get_set<corrected_intensity_metric>() = set;
        }

    public:
        /** Get information about the run
         *
         * @return run info
         */
        const run::info &run_info() const
        {
            return m_run_info;
        }

        /** Set information about the run
         *
         * @param info run info
         */
        void run_info(const run::info &info)
        {
            m_run_info = info;
        }
        /** @} */
        /** Get parameters describing the run
         *
         * @return run parameters
         */
        const run::parameters &run_parameters() const
        {
            return m_run_parameters;
        }

        /** Set parameters describing the run
         *
         * @param param run parameters
         */
        void run_parameters(const run::parameters &param)
        {
            m_run_parameters = param;
        }

    public:
        /** Get a metric set
         *
         * @return metric set
         */
        template<class T>
        T &get()
        {
            return m_metrics.get<T>();
        }

        /** Get a metric set
         *
         * @return metric set
         */
        template<class T>
        const T &get() const
        {
            return m_metrics.get<T>();
        }

        /** Get a metric set given a metric type
         *
         * @return metric set
         */
        template<class T>
        metric_base::metric_set<T> &get_set()
        {
            return m_metrics.get<metric_base::metric_set<T> >();
        }

        /** Get a metric set given a metric type
         *
         * @return metric set
         */
        template<class T>
        const metric_base::metric_set<T> &get_set() const
        {
            return m_metrics.get<metric_base::metric_set<T> >();
        }

    public:
        /** Check if the InterOp file for each metric set exists
         *
         * This will set the `metric_set::data_source_exists` flag.
         *
         * @param run_folder run folder path
         */
        void check_for_data_sources(const std::string &run_folder);
        /** Read binary metrics from the run folder
         *
         * This function ignores:
         *  - Missing InterOp files
         *  - Incomplete InterOp files
         *  - Missing RunParameters.xml for non-legacy run folders
         *
         * @param run_folder run folder path
         */
        void read_metrics(const std::string &run_folder) throw(
        io::file_not_found_exception,
        io::bad_format_exception,
        io::incomplete_file_exception);
        /** Read binary metrics from the run folder
         *
         * This function ignores:
         *  - Missing InterOp files
         *  - Incomplete InterOp files
         *  - Missing RunParameters.xml for non-legacy run folders
         *
         * @param run_folder run folder path
         * @param valid_to_load list of metrics to load
         */
        void read_metrics(const std::string &run_folder, const std::vector<unsigned char>& valid_to_load) throw(
        io::file_not_found_exception,
        io::bad_format_exception,
        io::incomplete_file_exception,
        invalid_parameter);
        /** Write binary metrics to the run folder
         *
         * @param run_folder run folder path
         */
        void write_metrics(const std::string &run_folder)const throw(
        io::file_not_found_exception,
        io::bad_format_exception);

        /** Validate whether the RunInfo.xml matches the InterOp files
         *
         * @throws invalid_run_info_exception
         */
        void validate() throw(invalid_run_info_exception);

        /** Read binary metrics and XML files from the run folder
         *
         * @param func call back for metric and XML reading
         */
        template<class ReadFunc>
        void read_callback(ReadFunc &func)
        {
            metrics_callback(func);
            func(m_run_info);
            finalize_after_load();
        }

        /** Read binary metrics from the run folder
         *
         * @param func call back for metric reading
         */
        template<class Func>
        void metrics_callback(Func &func)
        {
            m_metrics.apply(func);
        }
        /** Check if the metric group is empty
         *
         * @param group_name prefix of interop group metric
         * @return true if metric is empty
         */
        bool is_group_empty(const std::string& group_name) const;
        /** Check if the metric group is empty
         *
         * @param group_id prefix of interop group metric id
         * @return true if metric is empty
         */
        bool is_group_empty(const constants::metric_group group_id) const;

        /** Populate a map of valid tiles
         *
         * @param map mapping between tile has and base_metric
         */
        void populate_id_map(tile_metric_map_t &map) const;

        /** Populate a map of valid tiles and cycles
         *
         * @param map mapping between tile has and base_metric
         */
        void populate_id_map(cycle_metric_map_t &map) const;
        /** Sort the metrics by id
         */
        void sort();

        /** Clear all the metrics
         */
         void clear();

    private:
        metric_list_t m_metrics;
        run::info m_run_info;
        run::parameters m_run_parameters;

    };


}}}}
