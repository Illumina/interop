/** Base information shared by tile metrics
 *
 * Tile metrics include any metric calculated for a specific tile.
 *
 *  @file
 *
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <ostream>
#include <stdexcept>
#include <limits>
#include <cmath>
#include "interop/io/layout/base_metric.h"
#include "interop/constants/enums.h"

namespace illumina {
    namespace interop {
        namespace model {
            namespace metric_base {
                class base_metric;
                /** Defines default base header for metric */
                class base_metric_header
                {
                public:
                    /** Generate a default header
                     *
                     * @return default header
                     */
                    static base_metric_header default_header()
                    {
                        return base_metric_header();
                    }
                protected:
                    /** Update max cycle
                     *
                     * This does nothing, and is here for compatibility.
                     *
                     * @todo remove this method
                     */
                    void update_max_cycle(const base_metric&){}
                };
                /** Base class for InterOp classes that contain tile specific metrics
                 *
                 * These classes define both a lane and tile identifier.
                 */
                class base_metric
                {
                public:
                    /** Unsigned long
                     */
                    typedef ::uint64_t ulong_t;
                    /** Defines the type of a unique identifier
                     */
                    typedef ulong_t id_t;
                    /** Unsigned int16_t
                     */
                    typedef ::uint16_t ushort_t;
                    /** Unsigned int
                     */
                    typedef ::uint32_t uint_t;
                    /** Base metric header that adds no functionality
                     */
                    typedef base_metric_header header_type;
                    enum
                    {
                        LANE_BIT_COUNT=6, // Supports up to 63 lanes
                        TILE_BIT_COUNT=32,
                        LANE_BIT_SHIFT=LANE_BIT_COUNT, // Supports up to 63 lanes
                        TILE_BIT_SHIFT=LANE_BIT_COUNT+TILE_BIT_COUNT
                    };
                public:
                    /** Constructor
                     *
                     * @param lane lane number
                     * @param tile tile number
                     */
                    base_metric(const uint_t lane, const uint_t tile) :
                            m_lane(lane), m_tile(tile)
                    { }

                public:
                    /** Set the base metric identifiers
                     *
                     * @param base layout base
                     */
                    void set_base(const io::layout::base_metric &base)
                    {
                        m_lane = base.lane;
                        m_tile = base.tile;
                    }
                    /** Unique id created from both the lane and tile
                     *
                     * @return unique identifier
                     */
                    id_t id()const
                    {
                        return id(m_lane, m_tile);
                    }
                    /** Unique id created from both the lane and tile
                     *
                     * @param lane lane number
                     * @param tile tile number
                     * @return unique id
                     */
                    static id_t id(const id_t lane, const id_t tile, const id_t=0)// TODO: remove hack (const id_t=0)
                    {
                        return lane | (tile << LANE_BIT_SHIFT);
                    }
                    /** Get the lane from the unique lane/tile id
                     *
                     * @param id unique lane/tile id
                     * @return lane number
                     */
                    static id_t lane_from_id(const id_t id)
                    {
                        return id & ~((~0u) << LANE_BIT_SHIFT);
                    }
                    /** Lane number
                     *
                     * @return lane number
                     */
                    uint_t lane()const{return m_lane;}
                    /** Tile number
                     *
                     * @return tile number
                     */
                    uint_t tile()const{return m_tile;}
                    /** Tile number
                     *
                     * Calculates the number of the tile from the tile id.
                     *
                     * @return number of the tile.
                     */
                    uint_t number()const{return m_tile%100;}
                    /** Section number
                     *
                     * Calculates the section of the tile from the tile id.
                     *
                     * @return section of the tile.
                     */
                    uint_t section(const illumina::interop::constants::tile_naming_method method)const
                    {
                        if(method == constants::five_digit)
                            return (m_tile / 100) % 10;
                        return std::numeric_limits<uint_t>::min();
                    }
                    /** Surface number
                     *
                     * Calculates the surface of the tile from the tile id.
                     *
                     * @return surface of the tile.
                     */
                    uint_t surface(const illumina::interop::constants::tile_naming_method method)const
                    {
                        if(method == constants::five_digit)
                            return (m_tile / 10000);
                        return m_tile / 1000;
                    }
                    /** Swath number
                     *
                     * Calculates the surface of the tile from the tile id.
                     *
                     * @return surface of the tile.
                     */
                    uint_t swath(const illumina::interop::constants::tile_naming_method method)const
                    {
                        if(method == constants::five_digit)
                            return (m_tile / 1000) % 10;
                        if(method == constants::four_digit)
                            return (m_tile / 100) % 10;
                        return 0;
                    }
                    /** Index of the physical location of tile within the flowcell
                     *
                     * @param method the tile naming method
                     * @param section_per_lane number of sections per lane
                     * @param tile_count number of tiles
                     * @param swath_count number of swaths
                     * @param all_surfaces layout all surfaces of the flowcell
                     * @return index of the physical location within the flowcell
                     */
                    uint_t phyiscalLocationIndex(const illumina::interop::constants::tile_naming_method method,
                                                 const uint_t section_per_lane,
                                                 const uint_t tile_count,
                                                 const uint_t swath_count,
                                                 const bool all_surfaces)const
                    {
                        return phyiscalLocationColumn(method, swath_count, all_surfaces)
                               * (section_per_lane * tile_count)
                               + phyiscalLocationRow(method, section_per_lane, tile_count);
                    }
                    /** Column of the physical location of tile within the flowcell
                     *
                     * @param method the tile naming method
                     * @param swath_count number of swaths
                     * @param all_surfaces layout all surfaces of the flowcell
                     * @return column of the physical location within the flowcell
                     */
                    uint_t phyiscalLocationColumn(const illumina::interop::constants::tile_naming_method method,
                                                  const uint_t swath_count,
                                                  const bool all_surfaces)const
                    {
                        if(!(method == constants::five_digit||method == constants::four_digit)) return 0;
                        uint_t col = swath(method);
                        if(all_surfaces && surface(method)==2) col += swath_count;
                        return col;
                    }
                    /** Row of the physical location of tile within the flowcell
                     *
                     * @param method the tile naming method
                     * @param section_per_lane number of sections per lane
                     * @param tile_count number of tiles
                     * @return row of the physical location within the flowcell
                     */
                    uint_t phyiscalLocationRow(const illumina::interop::constants::tile_naming_method method,
                                               const uint_t section_per_lane,
                                               const uint_t tile_count)const
                    {
                        uint_t section;
                        switch(method)
                        {
                            case constants::five_digit:
                                section = ((m_tile % 1000) - (m_tile % 100)) / 100;
                                if(section == 4) section = 6;
                                else if(section == 6) section = 4;
                                section = (section-1) * section_per_lane;
                                return (section * tile_count) + (m_tile % 100);
                            case constants::four_digit:
                                return m_tile % 100;
                            default:
                                return m_tile;
                        }
                    }

                private:
                    uint_t m_lane;
                    uint_t m_tile;
                };
            }
        }
    }
}
