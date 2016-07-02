/** Encapsulates all the data for a flowcell heat map
 *
 *  @file
 *  @date 5/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/exception.h"
#include "interop/util/assert.h"
#include "interop/model/plot/series.h"
#include "interop/model/plot/axes.h"
#include "interop/model/plot/heatmap_data.h"

namespace illumina { namespace interop { namespace model { namespace plot {

/** Encapsulates all data for a flowcell heat map
 *
 * @todo refactor so it does not depend on 2-axes (from heatmap/chart_data)
 */
class flowcell_data : public heatmap_data
{
public:
    /** Constructor */
    flowcell_data() : m_swath_count(0), m_tile_count(0){}
public:
    /** Resize the flowcell heat map to the given number of rows and columns
     *
     * @param lanes number of lanes
     * @param swaths number of swaths
     * @param tiles number of tiles
     */
    void resize(const size_t lanes, const size_t swaths, const size_t tiles)
    {
        m_swath_count = swaths;
        m_tile_count = tiles;
        heatmap_data::resize(lanes, swaths*tiles);
        m_data.resize(heatmap_data::length());
    }
    /** Clear the data
     */
    void clear()
    {
        heatmap_data::clear();
        m_data.clear();
    }
    /** Set data at given location in the flowcell
     *
     * @param lane_idx lane index
     * @param loc physical tile location
     * @param tile_id id of the tile
     * @param value value of the metric
     */
    void set_data(const size_t lane_idx, const size_t loc, const ::uint32_t tile_id, const float value)
    throw(model::index_out_of_bounds_exception)
    {
        if(lane_idx >= lane_count())
            INTEROP_THROW( model::index_out_of_bounds_exception, "Lane index out of bounds");
        if(loc >= column_count())
            INTEROP_THROW(model::index_out_of_bounds_exception, "Location index out of bounds");
        //INTEROP_ASSERT(lane < m_data.size());
        heatmap_data::operator()(lane_idx, loc) = value;
        m_data[index_of(lane_idx, loc)] = tile_id;
    }
    /** Get the tile id associated with the location
     *
     * @param lane_idx
     * @param loc
     * @return tile id
     */
    ::uint32_t tile_id(const size_t lane_idx, const size_t loc)const throw(model::index_out_of_bounds_exception)
    {
        if(lane_idx >= lane_count())
            INTEROP_THROW(model::index_out_of_bounds_exception, "Lane index out of bounds");
        if(loc >= column_count())
            INTEROP_THROW(model::index_out_of_bounds_exception, "Location index out of bounds");
        INTEROP_ASSERT(index_of(lane_idx, loc) < m_data.size());
        return m_data[index_of(lane_idx, loc)];
    }
    /** Set the axis
     *
     * @param plot_axis single axis
     */
    void set_saxis(const plot::axis& plot_axis)
    {
        set_yaxis(plot_axis);
    }
    /** Set the label of the axis
     *
     * @param label text label
     */
    void set_label(const std::string& label)
    {
        set_ylabel(label);
    }
    /** Get the single axis
     *
     * @return single axis
     */
    const plot::axis& saxis()const
    {
        return y_axis();
    }
    /** Set the sub title
     *
     * @param subtitle label string
     */
    void set_subtitle(const std::string& subtitle)
    {
        m_subtitle = subtitle;
    }
    /** Set the limits of the axis
     *
     * @param vmin minimum value
     * @param vmax maximum value
     */
    void set_range(const float vmin, const float vmax)
    {
        set_yrange(vmin, vmax);
    }
    /** Get the sub title
     *
     * @return sub title
     */
    const std::string& subtitle()const
    {
        return m_subtitle;
    }
    /** Get number of lanes
     *
     * @return number of lanes
     */
    size_t lane_count()const
    {
        return row_count();
    }
    /** Get number of swaths
     *
     * @return number of swaths
     */
    size_t swath_count()const
    {
        return m_swath_count;
    }
    /** Get number of tiles
     *
     * @return number of tiles
     */
    size_t tile_count()const
    {
        return m_tile_count;
    }

private:
    std::vector< ::uint32_t > m_data;
    std::string m_subtitle;
    size_t m_swath_count;
    size_t m_tile_count;
};

}}}}
