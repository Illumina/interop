/** Summary statistics compiled by surface
 *
 *  @file
 *  @date  9/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/model/summary/stat_summary.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace summary
{

    /**  Summary statistics compiled by surface
     *
     * @note The reported standard deviation is always between tiles, not cycles. For metrics like error metrics,
     * the mean over all cycles is used for each tile, before calculating the standard deviation.
     *
     */
    class surface_summary : public stat_summary
    {
    public:
        /** Collection of metric statistics type */
        typedef metric_stat metric_stat_t;
    public:
        /** Constructor
         *
         * @param surface surface number
         * @param channel_count number of channels
         */
        surface_summary(const size_t surface = 0, const size_t channel_count=0) :
                stat_summary(channel_count),
                m_surface(surface),
                m_tile_count(0)
        {
        }

    public:
        /** @defgroup surface_summary Read/Lane summary
         *
         * Summary of key metrics in each read/lane/surface
         *
         * @ingroup read_summary
         * @ref illumina::interop::model::summary::surface_summary "See full class description"
         * @ref illumina::interop::model::summary::stat_summary "See full class description"
         * @{
         */
        /** Get surface number
         *
         * @return surface number
         */
        size_t surface() const
        {
            return m_surface;
        }
        /** Get number of tiles on the surface
         *
         * @return number of tiles on the surface
         */
        size_t tile_count() const
        {
            return m_tile_count;
        }
        /** @} */

    public:
        /** Set surface number
         *
         * @param val surface number
         */
        void surface(const size_t val)
        {
            m_surface = val;
        }
        /** Set number of tiles on the surface
         *
         * @param val number of tiles in the surface
         */
        void tile_count(const size_t val)
        {
            m_tile_count = val;
        }

    private:
        size_t m_surface;
        size_t m_tile_count;
        template<class MetricType, int Version>
        friend struct io::generic_layout;
    };

}}}}

