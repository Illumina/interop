/** Tile metric
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/TileMetrics.bin
 *  - InterOp/TileMetricsOut.bin
 *
 *  @file
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <cstring>
#include <fstream>
#include <map>
#include "interop/util/math.h"
#include "interop/io/format/generic_layout.h"
#include "interop/io/layout/base_metric.h"
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** Read metrics
     */
    class read_metric
    {
    public:
        /** Unsigned int */
        typedef ::uint32_t uint_t;
    public:
        /** Constructor
         *
         * @param read read number
         * @param percent_aligned percent of PhiX aligned in read
         * @param percent_phasing percent phasing
         * @param percent_prephasing percent pre-phasing
         */
        read_metric(const uint_t read = 0,
                    const float percent_aligned = std::numeric_limits<float>::quiet_NaN(),
                    const float percent_phasing = std::numeric_limits<float>::quiet_NaN(),
                    const float percent_prephasing = std::numeric_limits<float>::quiet_NaN()) :
                m_read(read),
                m_percent_aligned(percent_aligned),
                m_percent_phasing(percent_phasing),
                m_percent_prephasing(percent_prephasing)
        {
        }

    public:
        /** @defgroup read_metric Read Metrics
         *
         * Per tile per read metrics
         *
         * @ref illumina::interop::model::metrics::tile_metric "See full class description"
         * @ingroup tile_metric
         * @ref tile_metric "See read metrics"
         * @{
         */
        /** Read number
         *
         * @return read number
         */
        uint_t read() const
        { return m_read; }

        /** Percent aligned for read
         *
         * @note If percent aligned was never estimated, then it will be NaN
         *
         * @return percent aligned
         */
        float percent_aligned() const
        { return m_percent_aligned; }

        /** Percent phasing for read
         *
         * @return percent phasing
         */
        float percent_phasing() const
        { return m_percent_phasing; }

        /** Percent prephasing for read
         *
         * @return percent prephasing
         */
        float percent_prephasing() const
        { return m_percent_prephasing; }
        /** @} */
        /** Set percent aligned for read
         *
         * @param val percent aligned
         */
        void percent_aligned(const float val)
        { m_percent_aligned = val; }

        /** Set percent phasing for read
         *
         * @param val percent phasing
         */
        void percent_phasing(const float val)
        { m_percent_phasing = val; }

        /** Set percent prephasing for read
         *
         * @param val percent prephasing
         */
        void percent_prephasing(const float val)
        { m_percent_prephasing = val; }

    private:
        uint_t m_read;
        float m_percent_aligned;
        float m_percent_phasing;
        float m_percent_prephasing;
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

    /** Tile metrics
     *
     * @note Supported versions: 2
     */
    class tile_metric : public metric_base::base_metric
    {
    public:
        enum
        {
            /** Unique type code for metric */
            TYPE = constants::Tile,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 2
        };
        /** Define map between read ids and read metrics
         */
        typedef std::vector<read_metric> read_metric_vector;
        /** Define a const read iterator */
        typedef read_metric_vector::const_iterator const_iterator;
        /** Read metric type */
        typedef read_metric read_metric_type;
    private:
        typedef read_metric_vector::iterator read_iterator;
    public:
        /** Constructor
         */
        tile_metric() : metric_base::base_metric(0, 0),
                        m_cluster_density(std::numeric_limits<float>::quiet_NaN()),
                        m_cluster_density_pf(std::numeric_limits<float>::quiet_NaN()),
                        m_cluster_count(std::numeric_limits<float>::quiet_NaN()),
                        m_cluster_count_pf(std::numeric_limits<float>::quiet_NaN())
        { }
        /** Constructor
         */
        tile_metric(const header_type&) : metric_base::base_metric(0, 0),
                m_cluster_density(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_density_pf(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_count(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_count_pf(std::numeric_limits<float>::quiet_NaN())
        { }

        /** Constructor
         *
         * @note Version 2, used for writing
         * @param lane lane number
         * @param tile tile number
         * @param cluster_density density of clusters for each tile (in thousands per mm2)
         * @param cluster_density_pf density of clusters passing filter for each tile (in thousands per mm2)
         * @param cluster_count number of clusters for each tile
         * @param cluster_count_pf number of clusters passing filter for each tile
         * @param read_metrics vector of metrics for each read on the tile
         */
        tile_metric(const uint_t lane,
                    const uint_t tile,
                    const float cluster_density,
                    const float cluster_density_pf,
                    const float cluster_count,
                    const float cluster_count_pf,
                    const read_metric_vector &read_metrics = read_metric_vector()) :
                metric_base::base_metric(lane, tile),
                m_cluster_density(cluster_density),
                m_cluster_density_pf(cluster_density_pf),
                m_cluster_count(cluster_count),
                m_cluster_count_pf(cluster_count_pf),
                m_read_metrics(read_metrics)
        { }

        /** Constructor
         *
         * @note Version 2, used for writing
         * @param lane lane number
         * @param tile tile number
         * @param read_metrics vector of metrics for each read on the tile
         */
        tile_metric(const uint_t lane,
                    const uint_t tile,
                    const read_metric_vector &read_metrics = read_metric_vector()) :
                metric_base::base_metric(lane, tile),
                m_cluster_density(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_density_pf(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_count(std::numeric_limits<float>::quiet_NaN()),
                m_cluster_count_pf(std::numeric_limits<float>::quiet_NaN()),
                m_read_metrics(read_metrics)
        { }

        /** Constructor
         *
         * @param metric tile metric
         * @param read_metrics vector of metrics for each read on the tile
         */
        tile_metric(const tile_metric &metric,
                    const read_metric_vector &read_metrics = read_metric_vector()) :
                metric_base::base_metric(metric),
                m_cluster_density(metric.m_cluster_density),
                m_cluster_density_pf(metric.m_cluster_density_pf),
                m_cluster_count(metric.m_cluster_count),
                m_cluster_count_pf(metric.m_cluster_count_pf),
                m_read_metrics(read_metrics.size() > 0 ? read_metrics : metric.read_metrics())
        { }

    public:
        /** @defgroup tile_metric Tile Metrics
         *
         * Per tile tile metrics:
         *  1. Clusters passing filter
         *  2. Density passing filter
         *  3. Phasing per read
         *  4. Prephasing per read
         *  5. Percent aligned per read
         *
         * @ref illumina::interop::model::metrics::tile_metric "See full class description"
         *
         * @ref read_metric "See read metrics"
         *
         * @ingroup run_metrics
         * @note All metrics in this class are supported by all versions
         * @{
         */
        /** Density of clusters for each tile (in clusters per mm2)
         *
         * @return cluster density
         */
        float cluster_density() const
        { return m_cluster_density; }

        /** Density of clusters for each tile (in kilo-clusters per mm2)
         *
         * @return cluster density in kilo-clusters per mm2
         */
        float cluster_density_k() const
        { return m_cluster_density/1000.0f; }

        /** Density of clusters passing filter for each tile (in clusters per mm2)
         *
         * @return cluster density passing filter
         */
        float cluster_density_pf() const
        { return m_cluster_density_pf; }

        /** Density of clusters passing filter for each tile (in kilo-clusters per mm2)
         *
         * @return cluster density passing filter (in kilo-clusters per mm2)
         */
        float cluster_density_pf_k() const
        { return m_cluster_density_pf/1000.0f; }

        /** Number of clusters for each tile
         *
         * @return number of clusters
         */
        float cluster_count() const
        { return m_cluster_count; }

        /** Number of kilo-clusters for each tile
         *
         * @return number of kilo-clusters
         */
        float cluster_count_k() const
        { return m_cluster_count/1000.0f; }

        /** Number of clusters passing filter for each tile
         *
         * @return number of clusters passing filter
         */
        float cluster_count_pf() const
        { return m_cluster_count_pf; }

        /** Number of kilo-clusters passing filter for each tile
         *
         * @return number of kilo-clusters passing filter
         */
        float cluster_count_pf_k() const
        { return m_cluster_count_pf/1000.0f; }

        /** Percent of clusters passing filter
         *
         * @return percent of clusters passing filter
         */
        float percent_pf() const
        {
            return 100 * m_cluster_count_pf / m_cluster_count;
        }

        /** Metrics for each read on the tile
         *
         * @return vector of metrics for each read
         */
        const read_metric_vector &read_metrics() const
        { return m_read_metrics; }

        /** Percent aligned for read at specified index
         *
         * @note If percent aligned was never estimated, then it will be NaN
         *
         * @param n index of read
         * @return percent aligned (or NaN is out of bounds)
         */
        float percent_aligned(const size_t n) const
        {
            if (n >= m_read_metrics.size())
                return std::numeric_limits<float>::quiet_NaN();
            return m_read_metrics[n].percent_aligned();
        }

        /** Percent phasing for read at specified index
         *
         * @param n index of read
         * @return percent phasing (or NaN is out of bounds)
         */
        float percent_phasing(const size_t n) const
        {
            if (n >= m_read_metrics.size())
                return std::numeric_limits<float>::quiet_NaN();
            return m_read_metrics[n].percent_phasing();
        }

        /** Percent prephasing for read at specified index
         *
         * @param n index of read
         * @return percent prephasing (or NaN is out of bounds)
         */
        float percent_prephasing(const size_t n) const
        {
            if (n >= m_read_metrics.size())
                return std::numeric_limits<float>::quiet_NaN();
            return m_read_metrics[n].percent_prephasing();
        }

        /** Percent aligned for read read number
         *
         * @note If percent aligned was never estimated, then it will be NaN
         *
         * @param number number of read
         * @return percent aligned (or NaN is out of bounds)
         */
        float percent_aligned_at(const size_t number) const
        {
            for (const_iterator b = m_read_metrics.begin(), e = m_read_metrics.end(); b != e; ++b)
                if (b->read() == number) return b->percent_aligned();
            return std::numeric_limits<float>::quiet_NaN();
        }

        /** Percent phasing for read read number
         *
         * @param number number of read
         * @return percent phasing (or NaN is out of bounds)
         */
        float percent_phasing_at(const size_t number) const
        {
            for (const_iterator b = m_read_metrics.begin(), e = m_read_metrics.end(); b != e; ++b)
                if (b->read() == number) return b->percent_phasing();
            return std::numeric_limits<float>::quiet_NaN();
        }

        /** Percent prephasing for read number
         *
         * @param number number of read
         * @return percent prephasing (or NaN is out of bounds)
         */
        float percent_prephasing_at(const size_t number) const
        {
            for (const_iterator b = m_read_metrics.begin(), e = m_read_metrics.end(); b != e; ++b)
                if (b->read() == number) return b->percent_prephasing();
            return std::numeric_limits<float>::quiet_NaN();
        }

        /** Number of reads
         *
         * @return number of reads
         */
        size_t read_count() const
        {
            return m_read_metrics.size();
        }
        /* @} */
        /** Update the phasing/prephasing slope if they don't exist
         *
         * @param number read number
         * @param phasing phasing slope
         * @param prephasing prephasing slope
         */
        void update_phasing_if_missing(const size_t number, const float phasing, const float prephasing)
        {
            for (read_iterator b = m_read_metrics.begin(); b != m_read_metrics.end(); ++b)
            {
                if (b->read() == static_cast<uint_t>(number))
                {
                    if(std::isnan(b->percent_phasing()))
                    {
                        b->percent_phasing(phasing);
                    }
                    if(std::isnan(b->percent_prephasing()))
                    {
                        b->percent_prephasing(prephasing);
                    }
                    return;
                }
            }
            m_read_metrics.push_back(read_metric(static_cast<uint_t>(number),
                                                 std::numeric_limits<float>::quiet_NaN(),
                                                 phasing,
                                                 prephasing));
        }
        /** Density of clusters for each tile (in clusters per mm2)
         *
         * @deprecated Will be removed in 1.1.x (use cluster_density instead)
         * @return cluster density
         */
        float clusterDensity() const
        { return m_cluster_density; }

        /** Density of clusters passing filter for each tile (in clusters per mm2)
         *
         * @deprecated Will be removed in 1.1.x (use cluster_density_pf instead)
         * @return cluster density passing filter
         */
        float clusterDensityPf() const
        { return m_cluster_density_pf; }

        /** Number of clusters for each tile
         *
         * @deprecated Will be removed in 1.1.x (use cluster_count instead)
         * @return number of clusters
         */
        float clusterCount() const
        { return m_cluster_count; }

        /** Number of clusters passing filter for each tile
         *
         * @deprecated Will be removed in 1.1.x (use cluster_count_pf instead)
         * @return number of clusters passing filter
         */
        float clusterCountPf() const
        { return m_cluster_count_pf; }

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return prefix
         */
        static const char *prefix()
        { return "Tile"; }

    private:
        float m_cluster_density;
        float m_cluster_density_pf;
        float m_cluster_count;
        float m_cluster_count_pf;
        read_metric_vector m_read_metrics;

        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}

