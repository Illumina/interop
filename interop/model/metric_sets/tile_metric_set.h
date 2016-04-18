/** Tile Metric Set
 *
 *  @file
 *
 *  @date 10/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/model/metrics/tile_metric.h"

namespace illumina {
namespace interop {
namespace model {
namespace metrics {


/** Tile Metric Set
 *
 * @deprecated Will be removed in 1.1.x (use metric_set<tile_metric> instead)
 */
class tile_metrics : public metric_base::metric_set<tile_metric>
{
    typedef metric_base::metric_set<tile_metric> parent_type;
public:
    /** Constructor
     */
    tile_metrics() : parent_type(header_type())
    {
    }

    /** Constructor
     *
     * @param vec array of metrics
     * @param version of the metric format
     * @param header header information for the metric set
     */
    tile_metrics(const metric_array_t &vec, const ::int16_t version = -1, const header_type &header = header_type()) :
            parent_type(vec, version, header)
    {
    }

public:
    /** Collect all possible tile numbers
     */
    void populate_tiles()
    {
        std::set<uint_t> tiles;
        for(const_iterator beg = m_data.begin(), end=m_data.end();beg != end;++beg)
            tiles.insert(beg->tile());
        m_tiles.assign(tiles.begin(), tiles.end());
    }

public:
    /** Get list of all tile numbers
     *
     * @return list of available tile numbers
     */
    const std::vector<uint_t>& tiles()const
    {
        return m_tiles;
    }

private:
    std::vector<uint_t> m_tiles;
};
}
}
}
}
