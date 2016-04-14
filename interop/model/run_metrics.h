/** Collection of all metric sets for a run
 *
 *  @file
 *  @date 3/9/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/object_list.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/metrics/image_metric.h"
#include "interop/model/metrics/index_metric.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/io/metric_file_stream.h"
#include "interop/model/run/info.h"
#include "interop/model/run/parameters.h"
#include "interop/logic/metric/q_metric.h"
#include "interop/logic/utils/channel.h"

namespace illumina { namespace interop { namespace model { namespace metrics {


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
            tile_metric
    >::result_t metric_type_list_t;
    template<class T> struct create_metric_set{typedef metric_base::metric_set<T> result_t;};
    typedef transform_type_list< metric_type_list_t, create_metric_set>::result_t metric_set_list_t;
    typedef object_list< metric_set_list_t > metric_list_t;

public:
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
    run_metrics(const run::info& run_info, const run::parameters& run_param=run::parameters()) :
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
    void read(const std::string& run_folder) throw( xml::xml_file_not_found_exception,
                                                    xml::bad_xml_format_exception,
                                                    xml::empty_xml_format_exception,
                                                    xml::missing_xml_element_exception,
                                                    xml::xml_parse_exception,
                                                    io::file_not_found_exception,
                                                    io::bad_format_exception,
                                                    io::incomplete_file_exception)
    {
        read_metrics(run_folder);
        m_run_info.read(run_folder);
        const size_t count = logic::metric::count_legacy_q_score_bins(get_set<q_metric>());
        if(m_run_info.channels().empty() || logic::metric::requires_legacy_bins(count))
        {

            try {
                m_run_parameters.read(run_folder);
            }
            catch (const xml::xml_file_not_found_exception &)
            {
                if(m_run_info.channels().empty())
                    throw io::file_not_found_exception("RunParameters.xml required for legacy run folders with missing channel names");
                else
                    throw io::file_not_found_exception("RunParameters.xml required for legacy run folders and is missing");
            }
        }
        if(m_run_info.channels().empty())
        {
            m_run_info.channels(logic::utils::update_channel_from_instrument_type(m_run_parameters.instrument_type()));
            if(m_run_info.channels().empty())
                throw io::format_exception("Channel names are missing from the RunInfo.xml, and RunParameters.xml does not contain sufficient information on the instrument run.");
        }
        logic::metric::populate_legacy_q_score_bins(get_set<q_metric>().bins(), m_run_parameters.instrument_type(), count);
    }
    /** Test if all metrics are empty
     *
     * @return true if all metrics are empty
     */
    bool empty()const
    {
        is_metric_empty func;
        m_metrics.apply(func);
        return func.empty();
    }

public:
    /** Get the set of corrected intensity metrics
     *
     * @return set of corrected intensity metrics
     */
    corrected_intensity_metric_set_t& corrected_intensity_metric_set()
    {
        return get_set<corrected_intensity_metric>();
    }
    /** Get the set of error metrics
     *
     * @return set of error metrics
     */
    error_metric_set_t& error_metric_set()
    {
        return get_set<error_metric>();
    }
    /** Get the set of extraction metrics
     *
     * @return set of extraction metrics
     */
    extraction_metric_set_t& extraction_metric_set()
    {
        return get_set<extraction_metric>();
    }
    /** Get the set of image metrics
     *
     * @return set of image metrics
     */
    image_metric_set_t& image_metric_set()
    {
        return get_set<image_metric>();
    }
    /** Get the set of index metrics
     *
     * @return set of index metrics
     */
    index_metric_set_t& index_metric_set()
    {
        return get_set<index_metric>();
    }
    /** Get the set of quality metrics
     *
     * @return set of quality metrics
     */
    q_metric_set_t& q_metric_set()
    {
        return get_set<q_metric>();
    }
    /** Get the set of tile metrics
     *
     * @return set of tile metrics
     */
    tile_metric_set_t& tile_metric_set()
    {
        return get_set<tile_metric>();
    }
    /** Set the set of tile metrics
     *
     * @param set set of tile metrics
     */
    void tile_metric_set(const tile_metric_set_t& set)
    {
        get_set<tile_metric>() = set;
    }
    /** Set the set of q-metrics
     *
     * @param set set of q-metrics
     */
    void q_metric_set(const q_metric_set_t& set)
    {
        get_set<q_metric>() = set;
    }
    /** Set the set of index metrics
     *
     * @param set set of index metrics
     */
    void index_metric_set(const index_metric_set_t& set)
    {
        get_set<index_metric>() = set;
    }
    /** Set the set of image metrics
     *
     * @param set set of image metrics
     */
    void image_metric_set(const image_metric_set_t& set)
    {
        get_set<image_metric>() = set;
    }
    /** Set the set of extraction metrics
     *
     * @param set set of extraction metrics
     */
    void extraction_metric_set(const extraction_metric_set_t& set)
    {
        get_set<extraction_metric>() = set;
    }
    /** Set the set of error metrics
     *
     * @param set set of error metrics
     */
    void error_metric_set(const error_metric_set_t& set)
    {
        get_set<error_metric>() = set;
    }
    /** Set the set of corrected intensity metrics
     *
     * @param set set of corrected intensity metrics
     */
    void corrected_intensity_metric_set(const corrected_intensity_metric_set_t& set)
    {
        get_set<corrected_intensity_metric>() = set;
    }

public:
    /** Get information about the run
     *
     * @return run info
     */
    const run::info& run_info()const
    {
        return m_run_info;
    }
    /** Set information about the run
     *
     * @param info run info
     */
    void run_info(const run::info& info)
    {
        m_run_info = info;
    }
    /** @} */
    /** Get parameters describing the run
     *
     * @return run parameters
     */
    const run::parameters& run_parameters()const
    {
        return m_run_parameters;
    }
    /** Set parameters describing the run
     *
     * @param param run parameters
     */
    void run_parameters(const run::parameters& param)
    {
        m_run_parameters = param;
    }

public:
    /** Get a metric set
     *
     * @return metric set
     */
    template<class T>
    T& get()
    {
        return m_metrics.get<T>();
    }
    /** Get a metric set
     *
     * @return metric set
     */
    template<class T>
    const T& get()const
    {
        return m_metrics.get<T>();
    }
    /** Get a metric set given a metric type
     *
     * @return metric set
     */
    template<class T>
    metric_base::metric_set<T>& get_set()
    {
        return m_metrics.get< metric_base::metric_set<T> >();
    }
    /** Get a metric set given a metric type
     *
     * @return metric set
     */
    template<class T>
    const metric_base::metric_set<T>& get_set()const
    {
        return m_metrics.get< metric_base::metric_set<T> >();
    }

public:
    /** Read binary metrics from the run folder
     *
     * This function ignores:
     *  - Missing InterOp files
     *  - Incomplete InterOp files
     *  - Missing RunParameters.xml for non-legacy run folders
     *
     * @param run_folder run folder path
     */
    void read_metrics(const std::string& run_folder) throw(
                                                        io::file_not_found_exception,
                                                        io::bad_format_exception,
                                                        io::incomplete_file_exception)
    {
        m_metrics.apply(read_func(run_folder));
    }
    /** Read binary metrics and XML files from the run folder
     *
     * @param func call back for metric and XML reading
     */
    template<class ReadFunc>
    void read_callback(ReadFunc& func)
    {
        read_metrics_callback(func);
        func(m_run_info);
        const size_t count = logic::metric::count_legacy_q_score_bins(get_set<q_metric>());
        if(m_run_info.channels().empty() || logic::metric::requires_legacy_bins(count))
        {
            func(m_run_parameters);
        }
        if(m_run_info.channels().empty())
        {
            m_run_info.channels(logic::utils::update_channel_from_instrument_type(m_run_parameters.instrument_type()));
            if(m_run_info.channels().empty())
                throw io::format_exception("Channel names are missing from the RunInfo.xml, and RunParameters.xml does not contain sufficient information on the instrument run.");

        }
        logic::metric::populate_legacy_q_score_bins(get_set<q_metric>().bins(), m_run_parameters.instrument_type(), count);
    }
    /** Read binary metrics from the run folder
     *
     * @param func call back for metric reading
     */
    template<class ReadFunc>
    void read_metrics_callback(ReadFunc& func)
    {
        m_metrics.apply(func);
    }

private:
    metric_list_t m_metrics;
    run::info m_run_info;
    run::parameters m_run_parameters;

private:
    struct is_metric_empty
    {
        is_metric_empty(): m_empty(true){}
        template<class MetricSet>
        void operator()(const MetricSet& metrics)
        {
            if(metrics.size()>0) m_empty = false;
        }
        bool empty()const
        {
            return m_empty;
        }
        bool m_empty;
    };
    struct read_func
    {
        read_func(const std::string& f) : m_run_folder(f){}
        template<class MetricSet>
        int operator()(MetricSet& metrics)const
        {
            try
            {
                io::read_interop(m_run_folder, metrics);
            }
            catch(const io::file_not_found_exception&){return 1;}
            catch(const io::incomplete_file_exception&){return 2;}
            return 0;
        }
        std::string m_run_folder;
    };
};


}}}}
