/** Index metric
 *
 * The index metric holds string information describing the indexes used in the run.
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/IndexMetrics.bin
 *  - InterOp/IndexMetricsOut.bin
 *
 *  @file
 *  @date 11/2/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <string>
#include <map>
#include <vector>
#include "interop/util/exception.h"
#include "interop/util/assert.h"
#include "interop/model/metric_base/base_read_metric.h"
#include "interop/model/metric_base/metric_exceptions.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{

    /** Index information
     *
     * This class defines all the information that describes an index within a sequencing run.
     *
     * @note Supported versions: 1, 2
     */
    class index_info
    {
    public:
        /** Constructor
         *
         */
        index_info() :
                m_index_seq(""),
                m_sample_id(""),
                m_sample_proj(""),
                m_cluster_count(0)
        {
        }

        /** Constructor
         *
         * @param index_seq index sequence
         * @param sample_id sample id
         * @param sample_proj sample project
         * @param cluster_count number of index sequences
         */
        index_info(const std::string &index_seq,
                   const std::string &sample_id,
                   const std::string &sample_proj,
                   const ::uint64_t cluster_count) :
                m_index_seq(index_seq),
                m_sample_id(sample_id),
                m_sample_proj(sample_proj),
                m_cluster_count(cluster_count)
        {
        }

    public:
        /** @defgroup index_info Index Info
         *
         *
         * @ref illumina::interop::model::metrics::index_info "See full class description"
         * @ingroup index_metric
         * @{
         */
        /** Get the index sequence
         *
         * @note this can be two sequences, which are seperated by a '-' or a '+'
         * @return index sequence
         */
        const std::string &index_seq() const
        { return m_index_seq; }

        /** Get the sample id
         *
         * @return sample id
         */
        const std::string &sample_id() const
        { return m_sample_id; }

        /** Get the sample project
         *
         * @return sample project
         */
        const std::string &sample_proj() const
        { return m_sample_proj; }

        /** Get the number of clusters (per tile) that have this index sequence
         *
         * @return number of clusters
         */
        ::uint64_t cluster_count() const
        { return m_cluster_count; }

        /** Test if the sequence is a dual index
         *
         * The two sequences in a dual index are separated by a '-' or a '+'
         *
         * @return true if the sequence is a dual index
         */
        bool is_dual() const
        {
            return index_of_separator() != std::string::npos;
        }

        /** Get the first sequence in a dual index (or full sequence for single index)
         *
         * @return index 1 or full sequence for single index
         */
        std::string index1() const
        {
            const std::string::size_type pos = index_of_separator();
            if (pos != std::string::npos) return m_index_seq.substr(0, pos);
            return m_index_seq;
        }

        /** Get the second sequence in a dual index (or empty string for single index)
         *
         * @return index 2 or empty string for single index
         */
        std::string index2() const
        {
            const std::string::size_type pos = index_of_separator();
            if (pos != std::string::npos) return m_index_seq.substr(pos + 1);
            return "";
        }
        /** Get unique ID of index sequence
         *
         * @return unique ID of index sequence
         */
        std::string unique_id()const
        {
            return m_index_seq+m_sample_id;
        }
        /** @} */
    private:
        std::string::size_type index_of_separator() const
        {
            const std::string::size_type pos = m_index_seq.find('-');
            if (pos != std::string::npos) return pos;
            return m_index_seq.find('+');
        }

    private:
        std::string m_index_seq;
        std::string m_sample_id;
        std::string m_sample_proj;
        ::uint64_t m_cluster_count;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };


    /** Header information for writing an index metric set
     */
    class index_metric_header : public metric_base::base_read_metric::header_type
    {
    public:
        /** Unsigned int16_t
         */
        typedef ::uint16_t ushort_t;
    public:
        /** Constructor
         *
         */
        index_metric_header(){}

        /** Get the ordered unique indices
         *
         * @return number of channels
         */
        const std::vector<std::string>& index_order() const
        { return m_index_order; }
        /** Set the ordered unique indices
         *
         * @param order unique index order
         */
        void index_order(const std::vector<std::string>& order)
        {
            m_index_order = order;
        }

        /** Generate a default header
         *
         * @return default header
         */
        static index_metric_header default_header()
        {
            return index_metric_header();
        }
        /** Clear the data
         */
        void clear()
        {
            m_index_order.clear();
            metric_base::base_read_metric::header_type::clear();
        }

    private:
        std::vector<std::string> m_index_order;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };

    /** Index metric
     *
     * The index metric holds string information describing the indexes used in the run.
     *
     * @note Supported versions: 1 and 2
     */
    class index_metric : public metric_base::base_read_metric
    {
    public:
        enum
        {
            /** Unique type code for metric */
            TYPE = constants::Index,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 2
        };
        /** Index metric header */
        typedef index_metric_header header_type;
        /** Define a index array using an underlying vector */
        typedef std::vector<index_info> index_array_t;
        /** Define index info type */
        typedef index_info index_info_t;
        /** Define a constant iterator */
        typedef index_array_t::const_iterator const_iterator;
    public:
        /** Constructor
         */
        index_metric() :
                metric_base::base_read_metric(0, 0, 0),
                m_cluster_count(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_count_pf(std::numeric_limits<float>::quiet_NaN())
        {
        }
        /** Constructor
         */
        index_metric(const header_type&) :
                metric_base::base_read_metric(0, 0, 0),
                m_cluster_count(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_count_pf(std::numeric_limits<float>::quiet_NaN())
        {
        }

        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param read read number
         * @param indices array of indexes
         */
        index_metric(uint_t lane,
                     uint_t tile,
                     uint_t read,
                     const index_array_t &indices) :
                metric_base::base_read_metric(lane, tile, read),
                m_indices(indices),
                m_cluster_count(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_count_pf(std::numeric_limits<float>::quiet_NaN())
        {
        }

    public:
        /** @defgroup index_metric Index Metrics
         *
         * @ref illumina::interop::model::metrics::index_metric "See full class description"
         * @ingroup run_metrics
         * @{
         */
        /** Get the number of indexes
         *
         * @return number of indexes.
         */
        size_t size() const
        {
            return m_indices.size();
        }

        /** Get index info at given position
         *
         * @param n index of index
         * @return index
         */
        const index_info &indices(const size_t n) const INTEROP_THROW_SPEC((model::index_out_of_bounds_exception))
        {
            INTEROP_BOUNDS_CHECK(n, m_indices.size(), "Index out of bounds");
            return m_indices[n];
        }

        /** Get an array of indices
         *
         * @return vector of indices
         */
        const index_array_t &indices() const
        {
            return m_indices;
        }
        /** @} */
        /** Get iterator to start of indices
         *
         * @return iterator to start of indices
         */
        const_iterator begin()const
        {
            return m_indices.begin();
        }
        /** Get iterator to end of indices
         *
         * @return iterator to end of indices
         */
        const_iterator end()const
        {
            return m_indices.end();
        }
        /** Number of clusters for each tile
         *
         * @note Derived from tile_metric using `populate_indices` in `run_metrics::finalize_after_load`
         * @return number of clusters
         */
        float cluster_count() const
        { return m_cluster_count; }

        /** Number of clusters passing filter for each tile
         *
         * @note Derived from tile_metric using `populate_indices` in `run_metrics::finalize_after_load`
         * @return number of clusters passing filter
         */
        float cluster_count_pf() const
        { return m_cluster_count_pf; }

    public:
        /** Set the cluster counds dervied from tile_metric
         *
         * @param cluster_count number of clusters
         * @param cluster_count_pf number of PF clusters
         */
        void set_cluster_counts(const float cluster_count, const float cluster_count_pf)
        {
            m_cluster_count = cluster_count;
            m_cluster_count_pf = cluster_count_pf;
        }
        /** Get the prefix of the InterOp filename
         *
         * @return "Index"
         */
        static const char *prefix()
        { return "Index"; }

    private:
        index_array_t m_indices;
        float m_cluster_count; // Derived from tile metric
        float m_cluster_count_pf; // Derived from tile metric
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}

