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
    flowcell_data() : m_data(0), m_swath_count(0), m_tile_count(0), m_free(false){}
    /** Constructor
     *
     * @param data_buffer data value buffer
     * @param id_buffer data tile id buffer
     * @param lanes lane count
     * @param swaths swath count
     * @param tiles tile count
     */
    flowcell_data(float* data_buffer,
                  ::uint32_t* id_buffer,
                  const size_t lanes,
                  const size_t swaths,
                  const size_t tiles) : m_data(0), m_swath_count(0), m_tile_count(0), m_free(false)
    {
        set_buffer(data_buffer, id_buffer, lanes, swaths, tiles);
    }
    /** Destructor */
    virtual ~flowcell_data()
    {
        clear();
    }
public:
    /** Resize the heat map to the given number of rows and columns
     *
     * @param data_buffer buffer to hold the flow cell values
     * @param id_buffer buffer to hold the tile ids
     * @param lanes number of lanes
     * @param swaths number of swaths
     * @param tiles number of tiles
     */
    void set_buffer(float* data_buffer,
                    ::uint32_t* id_buffer,
                    const size_t lanes,
                    const size_t swaths,
                    const size_t tiles)
    {
        heatmap_data::set_buffer(data_buffer, lanes, swaths * tiles);
        set_buffer(id_buffer, swaths, tiles);
    }
    /** Resize the flowcell heat map to the given number of rows and columns
     *
     * @param lanes number of lanes
     * @param swaths number of swaths
     * @param tiles number of tiles
     */
    void resize(const size_t lanes, const size_t swaths, const size_t tiles)
    {
        if( lanes != row_count() && swaths != m_swath_count &&  tiles != m_tile_count)
        {
            heatmap_data::resize(lanes, swaths * tiles);
            resize(swaths, tiles);
        }
    }
    /** Clear the data
     */
    void clear()
    {
        heatmap_data::clear();
        if(m_free)
        {
            delete[] m_data;
            m_data=0;
            m_free = false;
        }
        m_swath_count = 0;
        m_tile_count = 0;
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
        heatmap_data::operator()(lane_idx, loc) = value;
        INTEROP_ASSERT(m_data != 0);
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
        INTEROP_ASSERT(index_of(lane_idx, loc) < length());
        INTEROP_ASSERT(m_data != 0);
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

protected:
    /** Resize the heat map to the given number of rows and columns
     *
     * @param id_buffer buffer to hold the tile ids
     * @param swaths number of swaths
     * @param tiles number of tiles
     */
    void set_buffer(::uint32_t* id_buffer,
                    const size_t swaths,
                    const size_t tiles)
    {
        if(m_free) delete[] m_data;
        m_data = id_buffer;
        m_swath_count = swaths;
        m_tile_count = tiles;
        m_free=false;
    }
    /** Resize the tile ID map to the given number of rows and columns
     *
     * @param swaths number of swaths
     * @param tiles number of tiles
     */
    void resize(const size_t swaths,
                const size_t tiles)
    {
        if (m_free) delete[] m_data;
        m_swath_count = swaths;
        m_tile_count = tiles;
        m_data = new ::uint32_t[heatmap_data::length()];
        std::memset(reinterpret_cast<char*>(m_data), 0, sizeof(::uint32_t)*heatmap_data::length());
        m_free = true;
    }

protected:
    /** Array of tile numbers for each tile */
    ::uint32_t* m_data;
    /** Sub title */
    std::string m_subtitle;
    /** Number of swaths per lane */
    size_t m_swath_count;
    /** Number of tiles per swath */
    size_t m_tile_count;
    bool m_free;
};

}}}}
