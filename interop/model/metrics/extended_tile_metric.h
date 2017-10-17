/** Extended tile metric
 *
 * The extended tile metric reports tile metrics such as the occupancy aka number
 * of empty wells on a tile.
 *
 *  @file
 *  @date 10/22/15
 *  @version 1.0
 *  @copyright GNU Public License
 */
#pragma once

#include <cstring>
#include <limits>
#include "interop/model/metric_base/base_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/model/metric_base/point2d.h"
#include "interop/io/layout/base_metric.h"
#include "interop/io/format/generic_layout.h"
#include "interop/model/metrics/tile_metric.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** Extended tile metric
     *
     * The extended tile metric reports tile metrics such as the occupancy aka number
     * of empty wells on a tile.
     *
     * @note Supported versions: 2 and 3
     */
    class extended_tile_metric : public metric_base::base_metric
    {
    public:
        enum
        {
            /** Unique type code for metric */
            TYPE = constants::ExtendedTile,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 3
        };
    public:
        /** Constructor
         */
        extended_tile_metric() :
                metric_base::base_metric(0, 0),
                m_cluster_count_occupied(std::numeric_limits<float>::quiet_NaN()),
                m_percent_occupied(std::numeric_limits<float>::quiet_NaN())
        {
        }
        /** Constructor
         */
        extended_tile_metric(const header_type&) :
                metric_base::base_metric(0, 0),
                m_cluster_count_occupied(std::numeric_limits<float>::quiet_NaN()),
                m_percent_occupied(std::numeric_limits<float>::quiet_NaN())
        {
        }
        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param occupied number of occupied wells
         * @param upper_left coordinate of upper left fiducial
         */
        extended_tile_metric(const uint_t lane,
                             const uint_t tile,
                             const float occupied,
                             const point2d& upper_left=point2d()) :
                metric_base::base_metric(lane, tile),
                m_cluster_count_occupied(occupied),
                m_percent_occupied(std::numeric_limits<float>::quiet_NaN()),
                m_upper_left(upper_left)
        {
        }

    public:
        /** Setter
         *
         * @note Version 1 and 2
         * @param lane lane number
         * @param tile tile number
         * @param occupied number of occupied wells
         * @param upper_left coordinate of upper left fiducial
         */
        void set(const uint_t lane,
                 const uint_t tile,
                 const float occupied,
                 const point2d& upper_left=point2d())
        {
            metric_base::base_metric::set_base(lane, tile);
            m_cluster_count_occupied = occupied;
            m_upper_left = upper_left;
        }
        /** @defgroup extended_tile_metric Extended Tile Metrics
         *
         * Per tile occupancy metrics
         *
         * @ingroup run_metrics
         * @ref illumina::interop::model::metrics::extended_tile_metric "See full class description"
         * @{
         */
        /** Number of clusters on the tile that are occupied
         *
         * @note Supported by all versions
         * @return number of occupied wells
         */
        float cluster_count_occupied() const
        {
            return m_cluster_count_occupied;
        }

        /** Number of thousands of clusters on the tile that are occupied
         *
         * @note Supported by all versions
         * @return number of occupied wells (in thousands)
         */
        float cluster_count_occupied_k() const
        {
            return m_cluster_count_occupied / 1000;
        }

        /** Percentage of clusters on the tile that are occupied
         *
         * Derived from tile_metric
         *
         * @note Supported by all versions
         * @return percent of occupied wells
         */
        float percent_occupied() const
        {
            return m_percent_occupied;
        }
        /** Get the coodinate of the upper left fiducial found on the first cycle
         *
         * @note supported in version 3 or later
         * @return upper left
         */
        const point2d& upper_left()const
        {
            return m_upper_left;
        }

        /** @} */
        /** Set the cluster count in kilo bases
         *
         * Derived from tile_metric using `populate_percent_occupied` in `run_metrics::finalize_after_load`
         *
         * @param cluster_count_k cluster count in kilo bases
         */
        void set_cluster_count_k(const float cluster_count_k)
        {
            m_percent_occupied = (m_cluster_count_occupied / 1000) / cluster_count_k * 100;
        }

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return "ExtendedTile"
         */
        static const char *prefix()
        { return "ExtendedTile"; }

    private:
        float m_cluster_count_occupied;
        float m_percent_occupied;//Derived from tile_metric
        point2d m_upper_left;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}


