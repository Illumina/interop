/** Tile metric
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/TileMetrics.bin
 *  - InterOp/TileMetricsOut.bin
 *
 *  @file
 *
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <assert.h>
#include <cstring>
#include <fstream>
#include <map>
#include "interop/io/format/generic_layout.h"
#include "interop/io/layout/base_metric.h"
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"

namespace illumina {
    namespace interop {
        namespace model {
            namespace metrics {
                /** Read metrics
                 */
                class read_metric
                {
                public:
                    /** Unsigned int
                     */
                    typedef ::uint32_t uint_t;
                public:
                    /** Constructor
                     *
                     * @param read read number
                     * @param percentAligned percent of PhiX aligned in read
                     * @param percentPhasing percent phasing
                     * @param percentPrePhasing percent pre-phasing
                     */
                    read_metric(uint_t read=0,
                                float percentAligned=0,
                                float percentPhasing=0,
                                float percentPrePhasing=0) :
                            m_read(read),
                            m_percent_aligned(percentAligned),
                            m_percent_phasing(percentPhasing),
                            m_percent_prephasing(percentPrePhasing)
                    {
                    }
                public:
                    /** @defgroup read_metric Read Metrics
                     *
                     * Per tile per read metrics
                     *
                     * @ref illumina::interop::model::metrics::tile_metric "See full class description"
                     *
                     * @ref tile_metric "See read metrics"
                     * @{
                     */
                    /** Read number
                     *
                     * @return read number
                     */
                    uint_t read()const{return m_read;}
                    /** Percent aligned for read
                     *
                     * @return percent aligned
                     */
                    float percent_aligned()const{return m_percent_aligned;}
                    /** Percent phasing for read
                     *
                     * @return percent phasing
                     */
                    float percent_phasing()const{return m_percent_phasing;}
                    /** Percent prephasing for read
                     *
                     * @return percent prephasing
                     */
                    float percent_prephasing()const{return m_percent_prephasing;}
                    /** @} */
                    /** Set percent aligned for read
                     *
                     * @param val percent aligned
                     */
                    void percent_aligned(const float val){m_percent_aligned=val;}
                    /** Set percent phasing for read
                     *
                     * @param val percent phasing
                     */
                    void percent_phasing(const float val){m_percent_phasing=val;}
                    /** Set percent prephasing for read
                     *
                     * @param val percent prephasing
                     */
                    void percent_prephasing(const float val){m_percent_prephasing=val;}

                private:
                    uint_t m_read;
                    float m_percent_aligned;
                    float m_percent_phasing;
                    float m_percent_prephasing;
                };
                /** Tile metrics
                 *
                 * @note Supported versions: 2
                 */
                class tile_metric : public metric_base::base_metric
                {
                public:
                    enum{
                        /** Unique type code for metric */
                        TYPE=constants::Tile,
                        /** Latest version of the InterOp format */
                        LATEST_VERSION=2
                    };
                    /** Define map between read ids and read metrics
                     */
                    typedef std::vector<read_metric> read_metric_vector;
                    /** Read metric type
                     */
                    typedef read_metric read_metric_type;
                public:
                    /** Constructor
                     */
                    tile_metric() : metric_base::base_metric(0,0),
                                    m_clusterDensity(0),
                                    m_clusterDensityPf(0),
                                    m_clusterCount(0),
                                    m_clusterCountPf(0)
                    {}
                    /** Constructor
                     *
                     * @note Version 2, used for writing
                     * @param lane lane number
                     * @param tile tile number
                     * @param clusterDensity density of clusters for each tile (in thousands per mm2)
                     * @param clusterDensityPf density of clusters passing filter for each tile (in thousands per mm2)
                     * @param clusterCount number of clusters for each tile
                     * @param clusterCountPf number of clusters passing filter for each tile
                     * @param readMetrics vector of metrics for each read on the tile
                     */
                    tile_metric(const uint_t lane,
                                const uint_t tile,
                                const float clusterDensity,
                                const float clusterDensityPf,
                                const float clusterCount,
                                const float clusterCountPf,
                                const read_metric_vector & readMetrics=read_metric_vector()) :
                            metric_base::base_metric(lane,tile),
                                    m_clusterDensity(clusterDensity),
                                    m_clusterDensityPf(clusterDensityPf),
                                    m_clusterCount(clusterCount),
                                    m_clusterCountPf(clusterCountPf),
                                    m_read_metrics(readMetrics)
                    {}
                    /** Constructor
                     *
                     * @note Version 2, used for writing
                     * @param lane lane number
                     * @param tile tile number
                     * @param readMetrics vector of metrics for each read on the tile
                     */
                    tile_metric(const uint_t lane,
                                const uint_t tile,
                                const read_metric_vector & readMetrics=read_metric_vector()) :
                            metric_base::base_metric(lane,tile),
                            m_clusterDensity(std::numeric_limits<float>::quiet_NaN()),
                            m_clusterDensityPf(std::numeric_limits<float>::quiet_NaN()),
                            m_clusterCount(std::numeric_limits<float>::quiet_NaN()),
                            m_clusterCountPf(std::numeric_limits<float>::quiet_NaN()),
                            m_read_metrics(readMetrics)
                    {}
                    /** Constructor
                     *
                     * @param metric tile metric
                     * @param readMetrics vector of metrics for each read on the tile
                     */
                    tile_metric(const tile_metric& metric,
                                const read_metric_vector & readMetrics=read_metric_vector()) :
                            metric_base::base_metric(metric),
                            m_clusterDensity(metric.m_clusterDensity),
                            m_clusterDensityPf(metric.m_clusterDensityPf),
                            m_clusterCount(metric.m_clusterCount),
                            m_clusterCountPf(metric.m_clusterCountPf),
                            m_read_metrics(readMetrics.size() > 0 ? readMetrics : metric.read_metrics())
                    {}

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
                     * @note All metrics in this class are supported by all versions
                     * @{
                     */
                    /** Density of clusters for each tile (in thousands per mm2)
                     *
                     * @return cluster density
                     */
                    float clusterDensity()const{return m_clusterDensity;}
                    /** Density of clusters passing filter for each tile (in thousands per mm2)
                     *
                     * @return cluster density passing filter
                     */
                    float clusterDensityPf()const{return m_clusterDensityPf;}
                    /** Number of clusters for each tile
                     *
                     * @return number of clusters
                     */
                    float clusterCount()const{return m_clusterCount;}
                    /** Number of clusters passing filter for each tile
                     *
                     * @return number of clusters passing filter
                     */
                    float clusterCountPf()const{return m_clusterCountPf;}
                    /** Metrics for each read on the tile
                     *
                     * @return vector of metrics for each read
                     */
                    const read_metric_vector & read_metrics()const{return m_read_metrics;}
                    /* @} */

                public:
                    /** Get the prefix of the InterOp filename
                     *
                     * @return prefix
                     */
                    static const char* prefix(){return "Tile";}

                private:
                    float m_clusterDensity;
                    float m_clusterDensityPf;
                    float m_clusterCount;
                    float m_clusterCountPf;
                    read_metric_vector m_read_metrics;

                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
            }
        }
    }
}
