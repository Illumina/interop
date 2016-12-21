/** Logic for tile metrics
 *
 *  @file
 *  @date  5/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/constants/enums.h"
#include "interop/model/run/flowcell_layout.h"


namespace illumina { namespace interop { namespace logic { namespace metric
{

    /** Convert string lane/tile identifier (1_1111) to lane number
     *
     * @param tile_name lane/tile identifier (1_1111)
     * @return lane number (1)
     */
    inline ::uint32_t lane_from_name(const std::string& tile_name)
    {
        if(tile_name=="") return 0;
        const size_t n = tile_name.find('_');
        if(n == std::string::npos) return 0;
        return util::lexical_cast< ::uint32_t >(tile_name.substr(0, n));
    }
    /** Convert string lane/tile identifier (1_1111) to tile hash
     *
     * @param tile_name lane/tile identifier (1_1111)
     * @return tile hash (1111)
     */
    inline ::uint32_t tile_from_name(const std::string& tile_name)
    {
        if(tile_name=="") return 0;
        const size_t n = tile_name.find('_');
        if(n == std::string::npos) return 0;
        return util::lexical_cast< ::uint32_t >(tile_name.substr(n+1));
    }

    /** Determine the tile naming method from a metric tile ID
     *
     * @param metric tile metric
     * @return tile_naming_method
     */
    inline constants::tile_naming_method tile_naming_method_from_metric(const model::metric_base::base_metric& metric)
    {
        if( metric.tile() > 9999) return constants::FiveDigit;
        if( metric.tile() > 999) return constants::FourDigit;
        if( metric.tile() <= 99) return constants::Absolute;
        return constants::UnknownTileNamingMethod;
    }
    /** Determine the tile naming method from a metric set
     *
     * @param metric_set metric set
     * @return tile_naming_method
     */
    template<class Metric>
    constants::tile_naming_method tile_naming_method_from_metric(const model::metric_base::metric_set<Metric>& metric_set)
    {
        if(metric_set.size() == 0 ) return constants::UnknownTileNamingMethod;
        return tile_naming_method_from_metric(metric_set[0]);
    }
    /** Tile number
     *
     * Calculates the number of the tile from the tile id.
     *
     * @return number of the tile.
     */
    inline ::uint32_t number(const ::uint32_t tile_id, const constants::tile_naming_method)
    {
        return tile_id%100;
    }
    /** Section number
     *
     * Calculates the section of the tile from the tile id.
     *
     * @return section of the tile.
     */
    inline ::uint32_t section(const ::uint32_t tile_id, const constants::tile_naming_method method)
    {
        if( method == constants::FiveDigit ) return (tile_id / 100) % 10;
        return std::numeric_limits< ::uint32_t >::min();
    }
    /** Surface number
     *
     * Calculates the surface of the tile from the tile id.
     *
     * @return surface of the tile.
     */
    inline ::uint32_t surface(const ::uint32_t tile_id, const constants::tile_naming_method method)
    {
        if(method == constants::FiveDigit) return (tile_id / 10000);
        if(method == constants::FourDigit) return tile_id / 1000;
        return 1;
    }
    /** Swath number
     *
     * Calculates the surface of the tile from the tile id.
     *
     * @return surface of the tile.
     */
    inline ::uint32_t swath(const ::uint32_t tile_id, const constants::tile_naming_method method)
    {
        if(method == constants::FiveDigit) return (tile_id / 1000) % 10;
        if(method == constants::FourDigit) return (tile_id / 100) % 10;
        return 1;
    }
    /** Determine the physical location (in terms of its column number) of the tile on the flow cell
     *
     * @param tile_id tile id
     * @param method tile naming method (4-digit or 5-digit or Absolute)
     * @param swath_count number of swathes per lane
     * @param all_surfaces if true, then find the proper column assuming all surfaces are being displayed
     * @return physical location of the tile in terms of its column number
     */
    inline ::uint32_t physical_location_column(const ::uint32_t tile_id,
                                               const constants::tile_naming_method method,
                                               const ::uint32_t swath_count,
                                               const bool all_surfaces)
    {
        if(!(method == constants::FiveDigit || method == constants::FourDigit)) return 0;
        ::uint32_t col = swath(tile_id, method);
        if(all_surfaces && surface(tile_id, method)==2) col += swath_count;
        return col-1;
    }
    /** Determine the physical location (in terms of its row number) of the tile on the flow cell
     *
     * @param tile_id tile id
     * @param method tile naming method (4-digit or 5-digit or Absolute)
     * @param section_per_lane number of sections per lane (5-digit only)
     * @param tile_count number of tiles per swath
     * @return physical location of the tile in terms of its row number
     */
    inline ::uint32_t physical_location_row(const ::uint32_t tile_id,
                                            const constants::tile_naming_method method,
                                            const ::uint32_t section_per_lane,
                                            const ::uint32_t tile_count)
    {
        switch(method)
        {
            case constants::FiveDigit:
            {
                ::uint32_t section = ((tile_id % 1000) - (tile_id % 100)) / 100;
                if (section == 4) section = 6;
                else if (section == 6) section = 4;
                section = (section - 1) * section_per_lane;
                return (section * tile_count) + (tile_id % 100) - 1;
            }
            case constants::FourDigit:
                return tile_id % 100-1;
            default:
                return tile_id-1;
        }
    }
    /** Determine the physical location (in terms of its column number) of the tile on the flow cell
     *
     * @param tile_id tile id
     * @param layout flow cell layout
     * @param all_surfaces if true, then find the proper column assuming all surfaces are being displayed
     * @return physical location of the tile in terms of its column number
     */
    inline ::uint32_t physical_location_column(const ::uint32_t tile_id,
                                               const model::run::flowcell_layout& layout,
                                               const bool all_surfaces)
    {
        return physical_location_column(tile_id,
                                        layout.naming_method(),
                                        layout.swath_count(),
                                        all_surfaces);
    }
    /** Determine the physical location (in terms of its row number) of the tile on the flow cell
     *
     * @param tile_id tile id
     * @param layout flow cell layout
     * @return physical location of the tile in terms of its row number
     */
    inline ::uint32_t physical_location_row(const ::uint32_t tile_id,
                                            const model::run::flowcell_layout& layout)
    {
        return physical_location_row(tile_id, layout.naming_method(), layout.sections_per_lane(), layout.tile_count());
    }

}}}}



