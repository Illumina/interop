/** Summary run metric
 *
 * The summary run metric contains run-level summary information
 *
 *  @file
 *  @date 07/06/2020
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/io/format/generic_layout.h"
#include "interop/model/metric_base/base_metric.h"
#include "interop/model/metric_base/metric_set.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{

    /** Summary run metric
     *
     * The summary run metric contains run-level summary information
     *
     * @note Supported versions: 1
     */
    class summary_run_metric : public metric_base::empty_metric
    {
    public:
        enum
        {
            /** Unique type code for metric */
            TYPE = constants::SummaryRun,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 1
        };
        /** Static run metric header */
        typedef metric_base::base_metric_header header_type;
        /** Vector of floats */
        typedef std::vector<float> float_vector;
        /** ubyte_t type */
        typedef uint8_t ubyte_t;
        /** ushort_t type */
        typedef uint16_t ushort_t;
        /** uint_t type */
        typedef uint32_t uint_t;
        /** Count type */
        typedef double count_t;

    public:
        /** Constructor */
        summary_run_metric() :
                m_occupancy_proxy_cluster_count(missing())
                , m_raw_cluster_count(missing())
                , m_occupied_cluster_count(missing())
                , m_pf_cluster_count(missing())
        { }
        /** Constructor */
        summary_run_metric(const header_type&) :
                m_occupancy_proxy_cluster_count(missing())
                , m_raw_cluster_count(missing())
                , m_occupied_cluster_count(missing())
                , m_pf_cluster_count(missing())
        { }

        /** Constructor
         *
         * @note Version 1
         * @param occupancy_proxy_cluster_count proxy for occupancy
         * @param raw_cluster_count raw cluster count
         * @param occupied_cluster_count occupied cluster count
         * @param pf_cluster_count pf cluster count
         */
        summary_run_metric(const count_t occupancy_proxy_cluster_count
                , const count_t raw_cluster_count
                , const count_t occupied_cluster_count
                , const count_t pf_cluster_count
                ) :
                m_occupancy_proxy_cluster_count(occupancy_proxy_cluster_count)
                , m_raw_cluster_count(raw_cluster_count)
                , m_occupied_cluster_count(occupied_cluster_count)
                , m_pf_cluster_count(pf_cluster_count)
        {}

    public:
        /** Setter
        *
        * @note Version 1
        * @param occupancy_proxy_cluster_count proxy for occupancy
        * @param raw_cluster_count raw cluster count
        * @param occupied_cluster_count occupied cluster count
        * @param pf_cluster_count pf cluster count
        */
        void set(const count_t occupancy_proxy_cluster_count
                , const count_t raw_cluster_count
                , const count_t occupied_cluster_count
                , const count_t pf_cluster_count)
        {
            m_occupancy_proxy_cluster_count = occupancy_proxy_cluster_count;
            m_raw_cluster_count = raw_cluster_count;
            m_occupied_cluster_count = occupied_cluster_count;
            m_pf_cluster_count = pf_cluster_count;
        }

        /** @defgroup summary_run_metric Run distortion metric
         *
         * Run summary metrics
         *
         * @ref illumina::interop::model::metrics::summary_run_metric "See full class description"
         * @ingroup run_metrics
         * @{
         */
        /** Return raw cluster count
         *
         *  @return raw cluster count
         */
        count_t raw_cluster_count() const
        {
            return m_raw_cluster_count;
        }
        /** Return occupancy cluster count
         *
         *  @return occupancy cluster count
         */
        count_t occupied_cluster_count() const
        {
            return m_occupied_cluster_count;
        }
        /** Return PF cluster count
         *
         *  @return PF cluster count
         */
        count_t pf_cluster_count() const
        {
            return m_pf_cluster_count;
        }
        /** Return occupancy proxy cluster count
         *
         *  @return occupancy proxy cluster count
         */
        count_t occupancy_proxy_cluster_count() const
        {
            return m_occupancy_proxy_cluster_count;
        }
        /** Return percent occupancy proxy
         *
         *  @return percent occupancy proxy
         */
        count_t percent_occupancy_proxy() const
        {
            if(std::isnan(m_pf_cluster_count)) return missing();
            if(std::isnan(m_occupancy_proxy_cluster_count)) return missing();
            return m_occupancy_proxy_cluster_count / m_pf_cluster_count * 100.0;
        }
        /** Return percent PF
         *
         *  @return percent PF
         */
        count_t percent_pf() const
        {
            if(std::isnan(m_raw_cluster_count)) return missing();
            if(std::isnan(m_pf_cluster_count)) return missing();
            return m_pf_cluster_count / m_raw_cluster_count * 100.0;
        }
        /** Return percent occupancy
         *
         *  @return percent occupancy
         */
        count_t percent_occupied() const
        {
            if(std::isnan(m_raw_cluster_count)) return missing();
            if(std::isnan(m_occupied_cluster_count)) return missing();
            return m_occupied_cluster_count / m_raw_cluster_count * 100.0;
        }
        /** @} */
        /** Unique id created from both the lane and tile
         *
         * @return unique identifier
         */
        id_t id() const
        {
            return 1;// There is only ever 1 per run, cannot be zero
        }

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return prefix
         */
        static const char *prefix()
        { return "SummaryRun"; }

    public:
        /** Flag if value is missing
         *
         * @return missing sentinel
         */
        static count_t missing()
        {
            return std::numeric_limits<count_t>::quiet_NaN();
        }

    private:
        count_t m_occupancy_proxy_cluster_count;
        count_t m_raw_cluster_count;
        count_t m_occupied_cluster_count;
        count_t m_pf_cluster_count;

        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}

