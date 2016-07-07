/** Options to filter metrics
 *
 *  @file
 *  @date 4/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/model/metric_base/base_metric.h"
#include "interop/util/lexical_cast.h"
#include "interop/constants/enums.h"
#include "interop/logic/utils/metric_type_ext.h"
#include "interop/logic/utils/channel.h"
#include "interop/model/run/info.h"

namespace illumina { namespace interop { namespace model { namespace plot
{

    /** Options to filter metrics
     */
    class filter_options
    {
    public:
        /** DNA base enumerated type - Range: A,C,G,T - All: NC*/
        typedef constants::dna_bases dna_base_t;
        /** Channel type - Range: 0-N - All: -1*/
        typedef ::int16_t channel_t;
        /** ID type - Range: 1-N - All: 0*/
        typedef model::metric_base::base_metric::uint_t id_t;

    public:
        /** Sentinels for use all of a specific type */
        enum UseAll
        {
            /** All sentinel for id types */
                    ALL_IDS = 0,
            /** All sentinel for channel types */
                    ALL_CHANNELS = -1,
            /** All sentinel for base types */
                    ALL_BASES = -1
        };

    public:
        /** Constructor
         *
         * @param naming_method tile naming method
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
        filter_options(const constants::tile_naming_method naming_method,
                       const id_t lane = ALL_IDS,
                       const channel_t channel = ALL_CHANNELS,// Order based on RunInfo.xml
                       const dna_base_t base = (dna_base_t) ALL_BASES,
                       const id_t surface = ALL_IDS,
                       const id_t read = ALL_IDS,
                       const id_t cycle = ALL_IDS,
                       const id_t tile_number = ALL_IDS,
                       const id_t swath = ALL_IDS,
                       const id_t section = ALL_IDS) :
                m_lane(lane),
                m_channel(channel),
                m_base(base),
                m_surface(surface),
                m_read(read),
                m_cycle(cycle),
                m_tile_number(tile_number),
                m_swath(swath),
                m_section(section),
                m_naming_method(naming_method)
        { }

    public:
        /** Test if the filter options are valid, if not throw an exception
         */
        void validate(const constants::metric_type type, const run::info& run_info, const bool check_ignored=false)const throw(model::invalid_filter_option)
        {
            if(m_naming_method == constants::UnknownTileNamingMethod)
                INTEROP_THROW(model::invalid_filter_option, "Invalid tile naming method: Unknown");
            if(m_naming_method > constants::TileNamingMethodCount)
                INTEROP_THROW(model::invalid_filter_option, "Invalid tile naming method: exceeds total number");
            if(m_naming_method != run_info.flowcell().naming_method())
                INTEROP_THROW(model::invalid_filter_option, "Invalid tile naming method: does not match RunInfo.xml");

            if(!all_lanes() && m_lane >= run_info.flowcell().lane_count())
                INTEROP_THROW(model::invalid_filter_option, "Lane number exceeds total number of lanes" << m_lane << " > " << run_info.flowcell().lane_count());
            if(is_specific_surface() && m_surface >= run_info.flowcell().surface_count())
                INTEROP_THROW(model::invalid_filter_option, "Surface number exceeds total number of surfaces" << m_surface << " > " << run_info.flowcell().surface_count());
            if(!all_tile_numbers() && m_tile_number >= run_info.flowcell().tile_count())
                INTEROP_THROW(model::invalid_filter_option, "Tile number exceeds total number of tile numbers" << m_tile_number << " > " << run_info.flowcell().tile_count());
            if(!all_swaths() && m_swath >= run_info.flowcell().swath_count())
                INTEROP_THROW(model::invalid_filter_option, "Swath number exceeds total number of swaths" << m_swath << " > " << run_info.flowcell().swath_count());
            if(!all_sections() && m_section >= run_info.flowcell().sections_per_lane())
                INTEROP_THROW(model::invalid_filter_option, "Section number exceeds total number of sections" << m_section << " > " << run_info.flowcell().sections_per_lane());
            if(logic::utils::is_base_metric(type))
            {
                if(!all_bases() && (m_base >= static_cast<dna_base_t>(constants::NUM_OF_BASES) || m_base < 0))
                    INTEROP_THROW(model::invalid_filter_option, "Base to filter is invalid: " << to_string(m_base));
            }
            if(logic::utils::is_cycle_metric(type))
            {
                if(!all_cycles() && m_cycle > run_info.total_cycles())
                    INTEROP_THROW(model::invalid_filter_option, "Cycle number exceeds total number of cycles" << m_cycle << " > " << run_info.total_cycles());
            }
            if(logic::utils::is_read_metric(type))
            {
                if(!all_reads() && m_read > run_info.reads().size())
                    INTEROP_THROW(model::invalid_filter_option, "Read number exceeds total number of reads" << m_read << " > " << run_info.reads().size());
            }
            if(logic::utils::is_channel_metric(type))
            {
                if(!all_channels() && static_cast<size_t>(m_channel) >= run_info.channels().size())
                    INTEROP_THROW(model::invalid_filter_option, "Channel number exceeds total number of channels" << m_channel << " > " << run_info.channels().size());
            }
            if(!check_ignored) return;
            if(!logic::utils::is_base_metric(type) && !all_bases())
                INTEROP_THROW(model::invalid_filter_option, "Invalid filter option base for metric " << constants::to_string(type));
            if(!logic::utils::is_cycle_metric(type) && !all_cycles())
                INTEROP_THROW(model::invalid_filter_option, "Invalid filter option cycle for metric " << constants::to_string(type));
            if(!logic::utils::is_read_metric(type) && !all_reads())
                INTEROP_THROW(model::invalid_filter_option, "Invalid filter option read for metric " << constants::to_string(type));
            if(!logic::utils::is_channel_metric(type) && !all_channels())
                INTEROP_THROW(model::invalid_filter_option, "Invalid filter option channel for metric " << constants::to_string(type));
            //all_cycles
        }
        /** Test if metric is a valid tile
         *
         * @param metric any metric type
         * @return true if the tile should not be filtered
         */
        template<class Metric>
        bool valid_tile(const Metric &metric) const
        {
            return valid_lane(metric.lane()) &&
                   valid_surface(metric.surface(m_naming_method)) &&
                   valid_tile_number(metric.number(m_naming_method)) &&
                   valid_swath(metric.swath(m_naming_method)) &&
                   valid_section(metric.section(m_naming_method));
        }

        /** Test if metric is a valid tile
         *
         * @param metric any metric type
         * @return true if the tile should not be filtered
         */
        template<class Metric>
        bool valid_tile_cycle(const Metric &metric) const
        {
            return valid_tile(metric) && valid_cycle(metric, typename Metric::base_t());
        }

        /** Test if all channels were requested
         *
         * @param type metric type
         * @return true if metric supports channels and all channels requested
         */
        bool all_channels(const constants::metric_type type) const
        {
            return all_channels() && logic::utils::is_channel_metric(type);
        }

        /** Test if all channels were requested
         *
         * @return true if metric supports channels and all channels requested
         */
        bool all_channels() const
        {
            return m_channel == static_cast<channel_t>(ALL_CHANNELS);
        }

        /** Test if all bases were requested
         *
         * @param type metric type
         * @return true if metric type supports bases and all bases requested
         */
        bool all_bases(const constants::metric_type type) const
        {
            return all_bases() && logic::utils::is_base_metric(type);
        }
        /** Test if all bases were requested
         *
         * @return true if all bases requested
         */
        bool all_bases() const
        {
            return m_base == static_cast<dna_base_t>(ALL_BASES);
        }

        /** Test if all reads were requested
         *
         * @return true if all reads were requested
         */
        bool all_reads() const
        {
            return m_read == static_cast<id_t>(ALL_IDS);
        }

        /** Test if all cycles were requested
         *
         * @return true if all cycles were requested
         */
        bool all_cycles() const
        {
            return m_cycle == static_cast<id_t>(ALL_IDS);
        }
        /** Test if all lanes were requested
         *
         * @return true if all lanes were requested
         */
        bool all_lanes() const
        {
            return m_lane == static_cast<id_t>(ALL_IDS);
        }
        /** Test if all tile numbers were requested
         *
         * @return true if all tile numbers were requested
         */
        bool all_tile_numbers() const
        {
            return m_tile_number == static_cast<id_t>(ALL_IDS);
        }
        /** Test if all swaths were requested
         *
         * @return true if all swaths were requested
         */
        bool all_swaths() const
        {
            return m_swath == static_cast<id_t>(ALL_IDS);
        }
        /** Test if all sections were requested
         *
         * @return true if all sections were requested
         */
        bool all_sections() const
        {
            return m_section == static_cast<id_t>(ALL_IDS);
        }
        /** Test if metric is read metric and specific read is chosen
         *
         * @param type metric type
         * @return true if metric type supports reads and a specific read is requested
         */
        bool is_specific_read(const constants::metric_type type) const
        {
            return m_read != static_cast<id_t>(ALL_IDS) && logic::utils::is_read_metric(type);
        }

        /** Test if specific read is chosen
         *
         * @return true if a specific read is requested
         */
        bool is_specific_read() const
        {
            return m_read != static_cast<id_t>(ALL_IDS);
        }

        /** Test if specific surface is chosen
         *
         * @return true if  a specific surface is requested
         */
        bool is_specific_surface() const
        {
            return m_surface != static_cast<id_t>(ALL_IDS);
        }

        /** Test if the lane if valid
         *
         * @return true if lane should not be filtered
         */
        bool valid_lane(const id_t lane) const
        {
            return m_lane == static_cast<id_t>(ALL_IDS) || m_lane == lane;
        }

        /** Test if the surface if valid
         *
         * @return true if surface should not be filtered
         */
        bool valid_surface(const id_t surface) const
        {
            return m_surface == static_cast<id_t>(ALL_IDS) || m_surface == surface;
        }

        /** Test if the read if valid
         *
         * @return true if read should not be filtered
         */
        bool valid_read(const id_t read) const
        {
            return m_read == static_cast<id_t>(ALL_IDS) || m_read == read;
        }

        /** Test if the cycle if valid
         *
         * @return true if cycle should not be filtered
         */
        bool valid_cycle(const id_t cycle) const
        {
            return m_cycle == static_cast<id_t>(ALL_IDS) || m_cycle == cycle;
        }

        /** Test if the tile number if valid
         *
         * @return true if tile number should not be filtered
         */
        bool valid_tile_number(const id_t tile_number) const
        {
            return m_tile_number == static_cast<id_t>(ALL_IDS) || m_tile_number == tile_number;
        }

        /** Test if the swath if valid
         *
         * @return true if swath should not be filtered
         */
        bool valid_swath(const id_t swath) const
        {
            return m_swath == static_cast<id_t>(ALL_IDS) || m_swath == swath;
        }

        /** Test if the section if valid
         *
         * @return true if section should not be filtered
         */
        bool valid_section(const id_t section) const
        {
            return m_section == static_cast<id_t>(ALL_IDS) || m_section == section;
        }

        /** Test if the channel if valid
         *
         * @return true if channel should not be filtered
         */
        bool valid_channel(const channel_t channel) const
        {
            return m_channel == static_cast<channel_t>(ALL_CHANNELS) || m_channel == channel;
        }

        /** Test if the base if valid
         *
         * @return true if base should not be filtered
         */
        bool valid_base(const dna_base_t base) const
        {
            return m_base == static_cast<dna_base_t>(ALL_BASES) || m_base == base;
        }

    public:
        /** Set the tile naming method
         *
         * @param naming_method tile naming method enum
         */
        void tile_naming_method(const constants::tile_naming_method naming_method)
        {
            m_naming_method = naming_method;
        }
        /** Set channel to filter
         *
         * @param channel channel
         */
        void channel(const channel_t channel)
        {
            m_channel = channel;
        }

        /** Set base to filter
         *
         * @param base base
         */
        void dna_base(const dna_base_t base)
        {
            m_base = base;
        }

        /** Set read to filter
         *
         * @param r read index
         */
        void read(const id_t r)
        {
            m_read = r;
        }

        /** Set cycle to filter
         *
         * @param c cycle index
         */
        void cycle(const id_t c)
        {
            m_cycle = c;
        }

        /** Set surface to filter
         *
         * @param s surface index
         */
        void surface(const id_t s)
        {
            m_surface = s;
        }
        /** Set swath to filter
         *
         * @param s swath index
         */
        void swath(const id_t s)
        {
            m_swath = s;
        }
        /** Set section to filter
         *
         * @param s section index
         */
        void section(const id_t s)
        {
            m_section = s;
        }
        /** Set tile number to filter
         *
         * @param s tile number
         */
        void tile_number(const id_t s)
        {
            m_tile_number = s;
        }

        /** Set lane to filter
         *
         * @param l lane index
         */
        void lane(const id_t l)
        {
            m_lane = l;
        }

    public:
        /** Get the lane to display
         *
         * @return lane
         */
        id_t lane()const
        {
            return m_lane;
        }
        /** Get channel to display
         *
         * @return channel
         */
        channel_t channel() const
        {
            return m_channel;
        }

        /** Get base to display
         *
         * @return base
         */
        dna_base_t dna_base() const
        {
            return m_base;
        }

        /** Get read to display
         *
         * @return read
         */
        id_t read() const
        {
            return m_read;
        }

        /** Get cycle to display
         *
         * @return cycle
         */
        id_t cycle() const
        {
            return m_cycle;
        }

        /** Get a description for the cycle filter option
         *
         * @return description for the cycle filter option
         */
        std::string cycle_description() const
        {
            return all_cycles() ? "All Cycles" : "Cycle " + util::lexical_cast<std::string>(m_cycle);
        }

        /** Get a description of the lane filter options
         *
         * @return description
         */
        std::string lane_description() const
        {
            return all_lanes() ? "All Lanes" : "Lane " + util::lexical_cast<std::string>(m_lane);
        }

        /** Get a description of the channel filter options
         *
         * @return description
         */
        std::string channel_description(const std::vector<std::string> &channels) const
        {
            return all_channels() ? "All Channels" : channels[m_channel] +
                                                                                          " Channel";
        }

        /** Get a description of the base filter options
         *
         * @return description
         */
        std::string base_description() const
        {
            return all_bases() ? "All Bases" : "Base " +
                                                                                  constants::to_string(m_base);
        }

        /** Get a description of the surface filter options
         *
         * @return description
         */
        std::string surface_description() const
        {
            return !is_specific_surface() ? "All Surfaces" :
                   constants::to_string(static_cast<constants::surface_type >(m_surface)) + " Surface";
        }

        /** Get a description of the read filter options
         *
         * @return description
         */
        std::string read_description() const
        {
            return all_reads() ? "All Reads" : "Read " + util::lexical_cast<std::string>(m_read);
        }

    private:
        template<class Metric>
        bool valid_cycle(const Metric &metric, constants::base_cycle_t) const
        {
            return metric.cycle() == m_cycle;
        }

        template<class Metric>
        bool valid_cycle(const Metric &, constants::base_tile_t) const
        { return true; }

        template<class Metric>
        bool valid_cycle(const Metric &, constants::base_read_t) const
        { return true; }

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
        constants::tile_naming_method m_naming_method;

    };


}}}}
