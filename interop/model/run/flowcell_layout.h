/** Layout of the flow cell
 *
 *  @file
 *  @date 11/7/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include <vector>
#include "interop/constants/enums.h"

namespace illumina
{
namespace interop
{
namespace model
{
namespace run
{

/** Layout of the flow cell
 *
 */
class flowcell_layout
{
public:
    /** Define tile naming method type */
    typedef constants::tile_naming_method tile_naming_method_t;
    /** Define string vector type */
    typedef std::vector <std::string> str_vector_t;

public:
    /** Constructor
     *
     * @param lane_count number of lanes
     * @param surface_count number of surfaces
     * @param swath_count number of swaths
     * @param tile_count number of tiles
     * @param sections_per_lane number of sections per lane
     * @param lanes_per_section number of lanes per section
     * @param tiles vector of tile names
     * @param naming_method tile naming method
     * @param barcode flowcell barcode
     */
    flowcell_layout(const size_t lane_count=1,
                    const size_t surface_count=1,
                    const size_t swath_count=1,
                    const size_t tile_count=1,
                    const size_t sections_per_lane=1,
                    const size_t lanes_per_section=1,
                    const str_vector_t& tiles=str_vector_t(),
                    const tile_naming_method_t naming_method=constants::unknown,
                    const std::string& barcode="") :
            m_naming_method(naming_method),
            m_lane_count(lane_count),
            m_surface_count(surface_count),
            m_swath_count(swath_count),
            m_tile_count(tile_count),
            m_sections_per_lane(sections_per_lane),
            m_lanes_per_section(lanes_per_section),
            m_tiles(tiles),
            m_barcode(barcode)
    {
    }

public:
    /** @defgroup flowcell_layout Flowcell Layout
     *
     * Information describing the layout of the flowcell
     *
     * @ingroup run_info
     * @ref illumina::interop::model::run::flowcell_layout "See full class description"
     * @{
     */
    /** Get number of lanes
     *
     * @return number of lanes
     */
    size_t lane_count()const{ return m_lane_count;}
    /** Get number of surfaces
     *
     * @return number of surfaces
     */
    size_t surface_count()const{ return m_surface_count;}
    /** Get number of swaths
     *
     * @return number of swaths
     */
    size_t swath_count()const{ return m_swath_count;}
    /** Get number of tiles
     *
     * @return number of tiles
     */
    size_t tile_count()const{ return m_tile_count;}
    /** Get number of sections per lane
     *
     * @return number of sections per lane
     */
    size_t sections_per_lane()const{ return m_sections_per_lane;}
    /** Get number of lanes per section
     *
     * @return number of lanes per section
     */
    size_t lanes_per_section()const{ return m_lanes_per_section;}
    /** Get the tile naming convention
     *
     * @return tile naming convention
     */
    tile_naming_method_t naming_method()const{return m_naming_method;}
    /** Get a list of tile names
     *
     * @return vector of tile names
     */
    const str_vector_t& tiles()const{ return m_tiles; }
    /** Get the flowcell bar code
     *
     * @return flowcell bar code as a string
     */
    const std::string& barcode()const{ return m_barcode; }
    /** @} */

private:
    tile_naming_method_t m_naming_method;
    size_t m_lane_count;
    size_t m_surface_count;
    size_t m_swath_count;
    size_t m_tile_count;
    size_t m_sections_per_lane;
    size_t m_lanes_per_section;
    str_vector_t m_tiles;
    std::string m_barcode;
    friend class info;
};
}
}
}
}

