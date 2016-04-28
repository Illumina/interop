/** Options to filter metrics
 *
 *  @file
 *  @date 4/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/constants/enums.h"

namespace illumina { namespace interop { namespace model { namespace plot {

/** Options to filter metrics
 */
class filter_options
{
public:
    /** DNA base enumerated type - Range: A,C,G,T - All: NC*/
    typedef constants::dna_base dna_base_t;
    /** Channel type - Range: 0-N - All: -1*/
    typedef ::int16_t channel_t;
    /** ID type - Range: 1-N - All: 0*/
    typedef ::uint16_t id_t;

public:
    enum UseAll
    {
        /** All sentinel for id types */
        ALL_IDS=0,
        /** All sentinel for channel types */
        ALL_CHANNELS=-1,
        /** All sentinel for base types */
        ALL_BASES=constants::NC
    };

public:
    /** Constructor
     *
     * @param lane lane number
     * @param channel channel number (order based on RunInfo.xml)
     * @param base enumerate type
     * @param surface surface number
     * @param read read number
     * @param cycle cycle number
     * @param tile_number tile number
     * @param swath swath number
     * @param section section number
     */
    filter_options(const id_t lane,
                   const channel_t channel,// Order based on RunInfo.xml
                   const dna_base_t base,
                   const id_t surface,
                   const id_t read,
                   const id_t cycle,
                   const id_t tile_number,
                   const id_t swath,
                   const id_t section) :
            m_lane(lane),
            m_channel(channel),
            m_base(base),
            m_surface(surface),
            m_read(read),
            m_cycle(cycle),
            m_tile_number(tile_number),
            m_swath(swath),
            m_section(section)
    {
    }

public:
    /** Test if all channels were requested
     *
     * @param type metric type
     * @return true if metric supports channels and all channels requested
     */
    //bool all_channels(const metric_type type)
    /** Test if the lane if valid
     *
     * @return true if lane should not be filtered
     */
    bool valid_lane(const id_t lane)const
    {
        return m_lane == static_cast<id_t>(ALL_IDS) || m_lane == lane;
    }
    /** Test if the surface if valid
     *
     * @return true if surface should not be filtered
     */
    bool valid_surface(const id_t surface)const
    {
        return m_surface == static_cast<id_t>(ALL_IDS) || m_surface == surface;
    }
    /** Test if the read if valid
     *
     * @return true if read should not be filtered
     */
    bool valid_read(const id_t read)const
    {
        return m_read == static_cast<id_t>(ALL_IDS) || m_read == read;
    }
    /** Test if the cycle if valid
     *
     * @return true if cycle should not be filtered
     */
    bool valid_cycle(const id_t cycle)const
    {
        return m_cycle == static_cast<id_t>(ALL_IDS) || m_cycle == cycle;
    }
    /** Test if the tile number if valid
     *
     * @return true if tile number should not be filtered
     */
    bool valid_tile_number(const id_t tile_number)const
    {
        return m_tile_number == static_cast<id_t>(ALL_IDS) || m_tile_number == tile_number;
    }
    /** Test if the swath if valid
     *
     * @return true if swath should not be filtered
     */
    bool valid_swath(const id_t swath)const
    {
        return m_swath == static_cast<id_t>(ALL_IDS) || m_swath == swath;
    }
    /** Test if the section if valid
     *
     * @return true if section should not be filtered
     */
    bool valid_section(const id_t section)const
    {
        return m_section == static_cast<id_t>(ALL_IDS) || m_section == section;
    }
    /** Test if the channel if valid
     *
     * @return true if channel should not be filtered
     */
    bool valid_channel(const channel_t channel)const
    {
        return m_channel == static_cast<channel_t>(ALL_CHANNELS) || m_channel == channel;
    }
    /** Test if the base if valid
     *
     * @return true if base should not be filtered
     */
    bool valid_base(const dna_base_t base)const
    {
        return m_base == static_cast<dna_base_t>(ALL_BASES) || m_base == base;
    }

private:
    id_t m_lane;
    channel_t m_channel;
    dna_base_t m_base;
    id_t m_surface;
    id_t m_read;
    id_t m_cycle;
    id_t m_tile_number;
    id_t m_swath;
    id_t m_section;

};


}}}}
