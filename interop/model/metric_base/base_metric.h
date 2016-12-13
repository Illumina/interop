/** Base information shared by tile metrics
 *
 * Tile metrics include any metric calculated for a specific tile.
 *
 *  @file
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <limits>
#include "interop/util/assert.h"
#include "interop/io/layout/base_metric.h"
#include "interop/constants/enums.h"
#include "interop/constants/typedefs.h"

namespace illumina { namespace interop { namespace model { namespace metric_base
{
    /** This is a sentinel used to ignore a metric set in certain situations
     *
     * @see illumina::interop::logic::metric::table_entry::update
     */
    class empty_header { };

    // Forward declaration
    class base_metric;

    /** Get attributes of the metric */
    template<class Metric>
    struct metric_attributes : public Metric{};

    /** Defines default base header for metric */
    class base_metric_header : public empty_header
    {
    public:
        /** Define the base type */
        typedef constants::base_tile_t base_t;
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
        /** Does nothing
         *
         * @todo remove this method
         */
        void clear(){}
        /** Update max cycle
         *
         * This does nothing, and is here for compatibility.
         *
         * @todo remove this method
         */
        void update_max_cycle(const base_metric &)
        { }
    };

    /** This is a sentinel used to ignore a metric in certain situations
     *
     * @see illumina::interop::logic::metric::table_entry::update
     */
    class empty_metric
    {
    };

    /** Base class for InterOp classes that contain tile specific metrics
     *
     * These classes define both a lane and tile identifier.
     */
    class base_metric : public empty_metric
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
        /** Define the base type */
        typedef constants::base_tile_t base_t;
        enum
        {
            //Compress Lane, Tile and cycle into a 64-bit Integer
            // Bit order: LTCR or Lane, Tile, Cycle/Read, Reserved
            // Reserved: Bits 0-16
            // Read/Cycle: Bits 16-32
            // Tile: Bits 32-58
            // Lane: Bits 58-64
            LANE_BIT_COUNT = 6, // Supports up to 63 lanes
            TILE_BIT_COUNT = 26, // Support 7 digit tile (up to 67108864)
            CYCLE_BIT_COUNT = 16, // Support up to 65535 cycles
            READ_BIT_COUNT = 16, // Support up to 65535 reads
            RESERVED_BIT_COUNT = 16, // Support up to 65535 values
            READ_BIT_SHIFT = RESERVED_BIT_COUNT,
            CYCLE_BIT_SHIFT = RESERVED_BIT_COUNT,
            TILE_BIT_SHIFT = CYCLE_BIT_SHIFT+CYCLE_BIT_COUNT,
            LANE_BIT_SHIFT = TILE_BIT_SHIFT+TILE_BIT_COUNT // Supports up to 63 lanes=
        };
    public:
        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         */
        base_metric(const uint_t lane=0, const uint_t tile=0) :
                m_lane(lane), m_tile(tile)
        { }

    public:
        /** Set id
         *
         * @param lane lane number
         * @param tile tile number
         */
        void set_base(const uint_t lane, const uint_t tile)
        {
            m_lane = lane;
            m_tile = tile;
        }

        /** Set the base metric identifiers
         *
         * @param base layout base
         */
        template<class BaseMetric>
        void set_base(const BaseMetric &base)
        {
            m_lane = base.lane;
            m_tile = base.tile;
        }

        /** Unique id created from both the lane and tile
         *
         * @return unique identifier
         */
        id_t id() const
        {
            return create_id(m_lane, m_tile);
        }

        /** Unique id created from both the lane and tile
         *
         * @return unique identifier
         */
        id_t tile_hash() const
        {
            return create_id(m_lane, m_tile);
        }

        /** Unique id created from both the lane and tile
         *
         * @param lane lane number
         * @param tile tile number
         * @return unique id
         */
        static id_t create_id(const id_t lane, const id_t tile, const id_t= 0)// TODO: remove hack (const id_t=0)
        {
            return (lane << LANE_BIT_SHIFT) | (tile << TILE_BIT_SHIFT);
        }

        /** Get the lane from the unique lane/tile id
         *
         * @param id unique lane/tile id
         * @return lane number
         */
        static id_t lane_from_id(const id_t id)
        {
            return id >> LANE_BIT_SHIFT;
        }
        /** Get the tile hash from the unique lane/tile/cycle id
         *
         * @param id unique lane/tile/cycle id
         * @return tile hash number
         */
        static id_t tile_hash_from_id(const id_t id)
        {
            // 1. Remove cycle and reserved information
            return (id >> TILE_BIT_SHIFT) << TILE_BIT_SHIFT;
        }
        /** Get the tile hash from the unique lane/tile/cycle id
         *
         * @param id unique lane/tile/cycle id
         * @return tile hash number
         */
        static id_t tile_from_id(const id_t id)
        {
            // 1. Shift off lane
            // 2. Shift tile id into proper position (removing cycle and reserved)
            return (id << (LANE_BIT_COUNT) >> (LANE_BIT_COUNT+CYCLE_BIT_COUNT+RESERVED_BIT_COUNT));
        }

        /** Lane number
         *
         * @return lane number
         */
        uint_t lane() const
        { return m_lane; }

        /** Tile id
         *
         * The tile id can have either a 4-digit format or a 5-digit format.
         *
         * 4-digit:
         * 1234 -> surface (1) swath (2) tile_number (34)
         *
         * 5-digit:
         * 12345 -> surface (1) swath (2) section(3) tile_number (45)
         *
         * @return tile number
         */
        uint_t tile() const
        { return m_tile; }

        /** Tile number
         *
         * Calculates the number of the tile from the tile id.
         *
         * @return number of the tile.
         */
        uint_t number(const illumina::interop::constants::tile_naming_method) const
        {
            return m_tile % 100;
        }

        /** Section number
         *
         * Calculates the section of the tile from the tile id.
         *
         * @return section of the tile.
         */
        uint_t section(const illumina::interop::constants::tile_naming_method method) const
        {
            if (method == constants::FiveDigit)
                return (m_tile / 100) % 10;
            return std::numeric_limits<uint_t>::min();
        }

        /** Surface number
         *
         * Calculates the surface of the tile from the tile id.
         *
         * @return surface of the tile.
         */
        uint_t surface(const illumina::interop::constants::tile_naming_method method) const
        {
            if (method == constants::FiveDigit)
                return (m_tile / 10000);
            if (method == constants::FourDigit)
                return m_tile / 1000;
            return 1;
        }

        /** Swath number
         *
         * Calculates the surface of the tile from the tile id.
         *
         * @return surface of the tile.
         */
        uint_t swath(const illumina::interop::constants::tile_naming_method method) const
        {
            if (method == constants::FiveDigit)
                return (m_tile / 1000) % 10;
            if (method == constants::FourDigit)
                return (m_tile / 100) % 10;
            return 1;
        }

        /** Index of the physical location of tile within the flowcell
         *
         * @deprecated Will be removed in 1.1.x (use physical_location_index instead)
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
                                     const bool all_surfaces) const
        {
            const uint_t column = phyiscalLocationColumn(method, swath_count, all_surfaces);
            const uint_t row = phyiscalLocationRow(method, section_per_lane, tile_count);
            const uint_t row_count = section_per_lane * tile_count;
            return column * row_count + row;
        }

        /** Column of the physical location of tile within the flowcell
         *
         * @deprecated Will be removed in 1.1.x (use phyiscal_location_column instead)
         * @param method the tile naming method
         * @param swath_count number of swaths
         * @param all_surfaces layout all surfaces of the flowcell
         * @return column of the physical location within the flowcell
         */
        uint_t phyiscalLocationColumn(const illumina::interop::constants::tile_naming_method method,
                                      const uint_t swath_count,
                                      const bool all_surfaces) const
        {
            if (!(method == constants::FiveDigit || method == constants::FourDigit)) return 0;
            uint_t col = swath(method);
            if (all_surfaces && surface(method) == 2) col += swath_count;
            return col;
        }

        /** Row of the physical location of tile within the flowcell
         *
         * @deprecated Will be removed in 1.1.x (use phyiscal_location_row instead)
         * @param method the tile naming method
         * @param section_per_lane number of sections per lane
         * @param tile_count number of tiles
         * @return row of the physical location within the flowcell
         */
        uint_t phyiscalLocationRow(const illumina::interop::constants::tile_naming_method method,
                                   const uint_t section_per_lane,
                                   const uint_t tile_count) const
        {
            uint_t section;
            switch (method)
            {
                case constants::FiveDigit:
                    section = ((m_tile % 1000) - (m_tile % 100)) / 100;
                    if (section == 4) section = 6;
                    else if (section == 6) section = 4;
                    section = (section - 1) * section_per_lane;
                    return (section * tile_count) + (m_tile % 100);
                case constants::FourDigit:
                    return m_tile % 100;
                default:
                    return m_tile;
            }
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
        size_t physical_location_index(const illumina::interop::constants::tile_naming_method method,
                                       const uint_t section_per_lane,
                                       const uint_t tile_count,
                                       const uint_t swath_count,
                                       const bool all_surfaces) const
        {

            INTEROP_ASSERTMSG(swath(method) <= swath_count, swath(method) << " <= " << swath_count);
            INTEROP_ASSERTMSG(number(method) <= tile_count, number(method) << " <= " << tile_count);
            const uint_t column = physical_location_column(method, swath_count, all_surfaces);
            const uint_t row = physical_location_row(method, section_per_lane, tile_count);
            const uint_t row_count = section_per_lane * tile_count;
            return static_cast<size_t>(column * row_count + row);
        }

        /** Column of the physical location of tile within the flowcell
         *
         * @param method the tile naming method
         * @param swath_count number of swaths
         * @param all_surfaces layout all surfaces of the flowcell
         * @return column of the physical location within the flowcell
         */
        uint_t physical_location_column(const illumina::interop::constants::tile_naming_method method,
                                        const uint_t swath_count,
                                        const bool all_surfaces) const
        {
            if (!(method == constants::FiveDigit || method == constants::FourDigit)) return 0;
            uint_t col = swath(method);
            if (all_surfaces && surface(method) == 2) col += swath_count;
            return col - 1;
        }

        /** Row of the physical location of tile within the flowcell
         *
         * @param method the tile naming method
         * @param section_per_lane number of sections per lane
         * @param tile_count number of tiles
         * @return row of the physical location within the flowcell
         */
        uint_t physical_location_row(const illumina::interop::constants::tile_naming_method method,
                                     const uint_t section_per_lane,
                                     const uint_t tile_count) const
        {
            uint_t section;
            switch (method)
            {
                case constants::FiveDigit:
                    section = ((m_tile % 1000) - (m_tile % 100)) / 100;
                    if (section == 4) section = 6;
                    else if (section == 6) section = 4;
                    section = (section - 1) % section_per_lane;
                    return (section * tile_count) + (m_tile % 100) - 1;
                case constants::FourDigit:
                    return m_tile % 100 - 1;
                default:
                    return m_tile - 1;
            }
        }
        /** Get the metric name suffix
         *
         * @return empty string
         */
        static const char *suffix()
        {
            return "";
        }

    private:
        uint_t m_lane;
        uint_t m_tile;
    };


    /** Specialization for base_metric providing sentinel values*/
    template<>
    struct metric_attributes<base_metric>
    {
        enum
        {
            /** Metric type identifier
             * @sa illumina::interop::constants::metric_group
             */
            TYPE = constants::UnknownMetricType,
            /** Latest version of the metric format*/
            LATEST_VERSION=0
        };

        /** Empty prefix string
         *
         * @return empty string
         */
        static const char *prefix()
        {
            return "";
        }
        /** Get the metric name suffix
         *
         * @return empty string
         */
        static const char *suffix()
        {
            return "";
        }
    };


}}}}

