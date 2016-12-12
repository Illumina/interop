/** Options to filter metrics
 *
 *  @file
 *  @date 4/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#ifdef _MSC_VER
#pragma warning(disable:4290)// MSVC warns that it ignores the exception specification.
#endif
#include "interop/model/metric_base/base_metric.h"
#include "interop/util/indirect_range_iterator.h"
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
                       const dna_base_t base = static_cast<dna_base_t>(ALL_BASES),
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
        /** Reset all options to default values (except naming_method)
         */
        void reset()
        {
            m_lane = ALL_IDS;
            m_channel = ALL_CHANNELS;
            m_base =  static_cast<dna_base_t>(ALL_BASES);
            m_surface = ALL_IDS;
            m_read = ALL_IDS;
            m_cycle = ALL_IDS;
            m_tile_number = ALL_IDS;
            m_swath = ALL_IDS;
            m_section = ALL_IDS;
        }

    public:
        /** Test if the filter options are valid, if not throw an exception
         */
        void validate(const constants::metric_type type,
                      const run::info& run_info,
                      const bool check_ignored=false)const throw(model::invalid_filter_option)
        {
            if(m_naming_method == constants::UnknownTileNamingMethod)
                INTEROP_THROW(model::invalid_filter_option, "Invalid tile naming method: Unknown");
            if(m_naming_method > constants::TileNamingMethodCount)
                INTEROP_THROW(model::invalid_filter_option, "Invalid tile naming method: exceeds total number");
            if(m_naming_method != run_info.flowcell().naming_method())
                INTEROP_THROW(model::invalid_filter_option, "Invalid tile naming method: does not match RunInfo.xml");

            if(!all_lanes() && m_lane > run_info.flowcell().lane_count())
                INTEROP_THROW(model::invalid_filter_option,
                              "Lane number exceeds total number of lanes: "
                                      << m_lane << " > " << run_info.flowcell().lane_count());
            if(is_specific_surface() && m_surface > run_info.flowcell().surface_count())
                INTEROP_THROW(model::invalid_filter_option,
                              "Surface number exceeds total number of surfaces: "
                                      << m_surface << " > " << run_info.flowcell().surface_count());
            if(!all_tile_numbers() && m_tile_number > run_info.flowcell().tile_count())
                INTEROP_THROW(model::invalid_filter_option,
                              "Tile number exceeds total number of tile numbers: "
                                      << m_tile_number << " > " << run_info.flowcell().tile_count());
            if(!all_swaths() && m_swath > run_info.flowcell().swath_count())
                INTEROP_THROW(model::invalid_filter_option,
                              "Swath number exceeds total number of swaths: "
                                      << m_swath << " > " << run_info.flowcell().swath_count());
            if(!all_sections() && m_section > run_info.flowcell().total_number_of_sections())
                INTEROP_THROW(model::invalid_filter_option,
                              "Section number exceeds total number of sections: "
                                      << m_section << " > " << run_info.flowcell().total_number_of_sections());
            if(logic::utils::is_base_metric(type))
            {
                if(!all_bases() && (m_base >= static_cast<dna_base_t>(constants::NUM_OF_BASES) || m_base < 0))
                    INTEROP_THROW(model::invalid_filter_option, "Base to filter is invalid: " << to_string(m_base));
            }
            if(logic::utils::is_cycle_metric(type))
            {
                if(!all_cycles() && m_cycle > run_info.total_cycles())
                    INTEROP_THROW(model::invalid_filter_option,
                                  "Cycle number exceeds total number of cycles: "
                                          << m_cycle << " > " << run_info.total_cycles()
                                          << " reads: " << run_info.reads().size());
            }
            if(logic::utils::is_read_metric(type))
            {
                if(!all_reads() && m_read > run_info.reads().size())
                    INTEROP_THROW(model::invalid_filter_option,
                                  "Read number exceeds total number of reads: "
                                          << m_read << " > " << run_info.reads().size());
            }
            if(logic::utils::is_channel_metric(type))
            {
                if(!all_channels() && static_cast<size_t>(m_channel) >= run_info.channels().size())
                    INTEROP_THROW(model::invalid_filter_option,
                                  "Channel number exceeds total number of channels: "
                                          << m_channel << " > " << run_info.channels().size());
            }
            if(!check_ignored) return;
            if(!logic::utils::is_base_metric(type) && !all_bases())
                INTEROP_THROW(model::invalid_filter_option,
                              "Invalid filter option base for metric " << constants::to_string(type));
            if(!logic::utils::is_cycle_metric(type) && !all_cycles())
                INTEROP_THROW(model::invalid_filter_option,
                              "Invalid filter option cycle for metric " << constants::to_string(type));
            if(!logic::utils::is_read_metric(type) && !all_reads())
                INTEROP_THROW(model::invalid_filter_option,
                              "Invalid filter option read for metric " << constants::to_string(type));
            if(!logic::utils::is_channel_metric(type) && !all_channels())
                INTEROP_THROW(model::invalid_filter_option,
                              "Invalid filter option channel for metric " << constants::to_string(type));
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
            typedef typename Metric::base_t base_t;
            return valid_tile(metric) && valid_cycle(metric, base_t::null());
        }

        /** Test if metric is a valid read
         *
         * @param metric any metric type
         * @return true if the tile should not be filtered
         */
        template<class Metric>
        bool valid_read(const Metric &metric) const
        {
            typedef typename Metric::base_t base_t;
            return valid_read(metric, base_t::null());
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

        /** Get surface to display
         *
         * @return cycle
         */
        id_t surface() const
        {
            return m_surface;
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
        /** Get the tile naming convention
         *
         * @return tile naming convention
         */
        constants::tile_naming_method naming_method()const
        {
            return m_naming_method;
        }

    public:
        /** Create an iterator that updates the current object
         *
         * flowcell - does not support lanes
         *
         *
         * @param info run info
         * @param metric_type metric type
         * @param plot_type type of the plot
         * @param keep_state keep the current state of the options
         * @return iterator
         */
        util::chain_range_iterator option_iterator(const model::run::info& info,
                                                   const constants::metric_type metric_type,
                                                   const constants::plot_types plot_type,
                                                   const bool keep_state=false)
        {
            const id_t lane_beg = !supports_lane(plot_type) || supports_all_lanes(plot_type) ?
                                  static_cast<id_t>(ALL_IDS) : 1;
            const id_t lane_end = !supports_lane(plot_type) ?
                                  static_cast<id_t>(ALL_IDS) : static_cast<id_t>(info.flowcell().lane_count()+1);
            const channel_t channel_beg = !supports_channel(metric_type) || supports_all_channels(plot_type) ?
                                          static_cast<channel_t>(ALL_CHANNELS) : 0;
            const channel_t channel_end = !supports_channel(metric_type) ?
                                          static_cast<channel_t>(ALL_CHANNELS) :
                                          static_cast<channel_t>(info.channels().size());
            const dna_base_t base_beg = !supports_base(metric_type) || supports_all_bases(plot_type) ?
                                        static_cast<dna_base_t>(ALL_BASES) : constants::A;
            const dna_base_t base_end = !supports_base(metric_type) ?
                                        static_cast<dna_base_t>(ALL_BASES) : constants::NUM_OF_BASES;
            const id_t surface_beg = static_cast<id_t>(ALL_IDS); // All surfaces is always supported
            const id_t surface_end = !supports_surface(metric_type, info) ?
                                     static_cast<id_t>(ALL_IDS) : info.flowcell().surface_count()+1;
            const id_t read_beg = !supports_read(metric_type, plot_type) || supports_all_reads(plot_type) ?
                                  static_cast<id_t>(ALL_IDS) : 1;
            const id_t read_end = !supports_read(metric_type, plot_type) ?
                                  static_cast<id_t>(ALL_IDS) : static_cast<id_t>(info.reads().size()+1);
            const id_t cycle_beg = !supports_cycle(metric_type, plot_type) || supports_all_cycles(plot_type) ?
                                   static_cast<id_t>(ALL_IDS) : 1u;
            const id_t cycle_end = !supports_cycle(metric_type, plot_type) ?
                                   static_cast<id_t>(ALL_IDS) : static_cast<id_t>(info.total_cycles()+1);
            const id_t swath_beg = static_cast<id_t>(ALL_IDS);
            const id_t swath_end = !supports_swath(plot_type) ? static_cast<id_t>(ALL_IDS) :
                                   static_cast<id_t>(info.flowcell().swath_count()+1);
            const id_t section_beg = static_cast<id_t>(ALL_IDS);
            const id_t section_end = !supports_section(plot_type, info) ? static_cast<id_t>(ALL_IDS) :
                                   static_cast<id_t>(info.flowcell().total_number_of_sections()+1);
            const id_t tile_beg = static_cast<id_t>(ALL_IDS);
            const id_t tile_end = !supports_tile(plot_type) ? static_cast<id_t>(ALL_IDS) :
                                   static_cast<id_t>(info.flowcell().tile_count()+1);
            util::abstract_range_iterator* iterators[] =
            {
                new util::indirect_range_iterator<id_t>(m_lane, lane_beg, lane_end, !keep_state),
                new util::indirect_range_iterator<channel_t>(m_channel, channel_beg, channel_end, !keep_state),
                new util::indirect_range_iterator<dna_base_t, int>(m_base, base_beg, base_end, !keep_state),
                new util::indirect_range_iterator<id_t>(m_surface,  surface_beg, surface_end, !keep_state),
                new util::indirect_range_iterator<id_t>(m_read, read_beg, read_end, !keep_state),
                new util::indirect_range_iterator<id_t>(m_cycle, cycle_beg, cycle_end, !keep_state),
                new util::indirect_range_iterator<id_t>(m_swath, swath_beg, swath_end, !keep_state),
                new util::indirect_range_iterator<id_t>(m_section, section_beg, section_end, !keep_state),
                new util::indirect_range_iterator<id_t>(m_tile_number, tile_beg, tile_end, !keep_state)
            };
            return util::chain_range_iterator(iterators);
        }

    public:

        /** Test if the combination of the plot and run info support filtering by section
         *
         * param plot_type type of plot
         * @param info run info
         * @return true if metric supports filtering by swath
         */
        bool supports_section(const constants::plot_types /*plot_type*/, const model::run::info& info)const
        {
            if(info.flowcell().naming_method() != constants::FiveDigit) return false;
            return false;
        }
        /** Test if plot supports filtering by swath
         *
         * param plot_type type of plot
         * @return true if metric supports filtering by swath
         */
        bool supports_swath(const constants::plot_types /*plot_type*/)const
        {
            return false;
        }
        /**  Test if plot supports filtering by tile
         *
         * param plot_type type of plot
         * @return true if metric supports filtering by tile
         */
        bool supports_tile(const constants::plot_types /*plot_type*/)const
        {
            return false;
        }
        /** Test if plot supports filtering by all lanes
         *
         * @param plot_type type of plot
         * @return true if metric supports filtering by all lanes
         */
        bool supports_all_lanes(const constants::plot_types plot_type)const
        {
            return plot_type != constants::SampleQCPlot;
        }
        /** Test if plot supports filtering by lane
         *
         * @param plot_type type of plot
         * @return true if metric supports filtering by lane
         */
        bool supports_lane(const constants::plot_types plot_type)const
        {
            return plot_type != constants::FlowcellPlot &&
                   plot_type != constants::ByLanePlot;
        }
        /** Test if plot supports filtering by all bases
         *
         * @param plot_type plot type
         * @return true if plot supports filtering by base
         */
        bool supports_all_bases(const constants::plot_types plot_type)const
        {
            return plot_type == constants::ByCyclePlot;
        }
        /** Test if metric supports filtering by base
         *
         * @param metric_type metric type
         * @return true if metric supports filtering by base
         */
        bool supports_base(const constants::metric_type metric_type)const
        {
            return logic::utils::is_base_metric(metric_type);
        }
        /** Test if plot supports filtering by all channels
         *
         * @param plot_type plot type
         * @return true if plot supports filtering all channels
         */
        bool supports_all_channels(const constants::plot_types plot_type)const
        {
            return plot_type == constants::ByCyclePlot;
        }
        /** Test if metric supports filtering by channel
         *
         * @param metric_type metric type
         * @return true if metric supports filtering by channel
         */
        bool supports_channel(const constants::metric_type metric_type)const
        {
            return logic::utils::is_channel_metric(metric_type);
        }
        /** Test if plot supports filtering by all cycles
         *
         * @param plot_type plot type
         * @return true if plot supports filtering by all cycles
         */
        bool supports_all_cycles(const constants::plot_types plot_type)const
        {
            return plot_type == constants::QHistogramPlot;
        }
        /** Test if metric and plot combination supports filtering by cycle
         *
         * @param metric_type metric type
         * @param plot_type plot type
         * @return true if metric supports filtering by cycle
         */
        bool supports_cycle(const constants::metric_type metric_type, const constants::plot_types plot_type)const
        {
            return logic::utils::is_cycle_metric(metric_type) &&
                    plot_type != constants::ByCyclePlot &&
                    plot_type != constants::QHeatmapPlot;
        }
        /** Test if plot supports filtering by all reads
         *
         * @param plot_type plot type
         * @return true if plot supports filtering by all reads
         */
        bool supports_all_reads(const constants::plot_types plot_type)const
        {
            return plot_type == constants::QHistogramPlot;
        }
        /** Test if metric and plot combination supports filtering by read
         *
         * @param metric_type metric type
         * @param plot_type type of plot
         * @return true if metric supports filtering by read
         */
        bool supports_read(const constants::metric_type metric_type, const constants::plot_types plot_type)const
        {
            return logic::utils::is_read_metric(metric_type) || plot_type == constants::QHistogramPlot;
        }
        /** Test if metric and run info combination supports filtering by surface
         *
         * @param metric_type metric type
         * @param info run info
         * @return true if metric supports filtering by surface
         */
        bool supports_surface(const constants::metric_type metric_type, const model::run::info& info)const
        {
            if(metric_type == constants::UnknownMetricType) return false;
            const size_t surface_count = info.flowcell().surface_count();
            return surface_count > 1;
        }

    private:
        template<class Metric>
        bool valid_cycle(const Metric &metric, const constants::base_cycle_t*) const
        {
            return m_cycle == static_cast<id_t>(ALL_IDS) || metric.cycle() == m_cycle;
        }

        template<class Metric>
        bool valid_cycle(const Metric &, const void*) const
        { return true; }

        template<class Metric>
        bool valid_read(const Metric &metric, const constants::base_read_t*) const
        { return m_read == static_cast<id_t>(ALL_IDS) || metric.read() == m_read; }

        template<class Metric>
        bool valid_read(const Metric &, const void*) const
        { return true; }

        friend std::ostream& operator <<(std::ostream& out, const filter_options& options)
        {
            out << "Selected_filters_";
            if(!options.all_lanes()) out << "Lane_" << options.m_lane << "_";
            if(!options.all_channels()) out << "Channel_" << options.m_channel << "_";
            if(!options.all_bases()) out << "Base_" << options.m_base << "_";
            if(options.is_specific_surface()) out << "Surface_" << options.m_surface << "_";
            if(!options.all_reads()) out << "Read_" << options.m_read << "_";
            if(!options.all_cycles()) out << "Cycle_" << options.m_cycle << "_";
            if(!options.all_swaths()) out << "Swath_" << options.m_swath << "_";
            if(!options.all_sections()) out << "Section_" << options.m_section << "_";
            if(!options.all_tile_numbers()) out << "Tile_" << options.m_tile_number << "_";
            out << "_";
            return out;
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
        constants::tile_naming_method m_naming_method;

    };


}}}}

