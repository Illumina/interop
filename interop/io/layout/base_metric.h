/** Basic layouts describing metric identifiers
 *
 * The layout of a binary InterOp generally has the following parts:
 *      1. Header
 *      2. Record(s)
 *
 * The classes contained in this file define two types of metric identifiers: Per Tile and Per Tile Per Cycle.
 *
 * @note These classes are packed such that there is not padding. Their size reflects the accumulation of their member fields.
 *
 *  @file
 *
 *  @date 7/30/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <cstddef>
#include "interop/util/static_assert.h"
#include "interop/util/cstdint.h"

namespace illumina
{
    namespace interop
    {
        namespace io
        {
                namespace layout
                {
#pragma pack(1)
                    /** Base class for InterOp records that contain tile specific metrics
                     *
                     * These records contain both a lane and tile identifier.
                     *
                     * @note These classes are packed such that there is not padding. Their size reflects the accumulation of their member fields.
                     */
                    struct base_metric
                    {
                        /** Define an ID type */
                        typedef ::uint64_t id_t;
                        /** Define a record size type */
                        typedef ::uint8_t record_size_t;
                        /** Constructor
                     * @note These classes are packed such that there is not padding. Their size reflects the accumulation of their member fields.
                         *
                         * @param lane_ lane number
                         * @param tile_ tile number
                         */
                        base_metric(::uint16_t lane_=0, ::uint16_t tile_=0) :
                                lane(lane_), tile(tile_)
                        {
                            static_assert(sizeof(::uint16_t) == 2, "16-bit int not supported");
                            static_assert(sizeof(::uint32_t) == 4, "32-bit int not supported");
                        }
                        /** Set the lane and tile id from a base metric
                         *
                         * @param metric a base_metric from the model
                         */
                        template<class BaseMetric>
                        void set(const BaseMetric& metric)
                        {
                            lane = static_cast< ::uint16_t >(metric.lane());
                            tile = static_cast< ::uint16_t >(metric.tile());
                        }
                        /** Unique id created from both the lane and tile
                         *
                         * @return unique id
                         */
                        id_t id()const
                        {
                            return id(lane, tile);
                        }
                        /** Unique id created from both the lane and tile
                         *
                         * @param lane lane number
                         * @param tile tile number
                         * @return unique id
                         */
                        static id_t id(id_t lane, id_t tile)
                        {
                            return lane | (tile << 6);
                        }
                        /** Get the lane from the unique lane/tile id
                         *
                         * @param id unique lane/tile id
                         * @return lane number
                         */
                        static id_t lane_from_id(const id_t id)
                        {
                            return id & ~((~0u) << 6);
                        }
                        /** Test if the layout contains valid data
                         *
                         * @return true if data is valid
                         */
                        bool is_valid()const
                        {
                            return tile > 0 && lane > 0;
                        }
                        /** Lane number
                         */
                        ::uint16_t lane;
                        /** Tile number
                         */
                        ::uint16_t tile;
                    };
                    /** Base class for InterOp records that contain tile specific metrics per cycle
                     *
                     * These records contain both a lane, tile and cycle identifier.
                     *
                     * @note These classes are packed such that there is not padding. Their size reflects the accumulation of their member fields.
                     */
                    struct base_cycle_metric : base_metric
                    {
                        /** Constructor
                         *
                         * @param lane_ lane number
                         * @param tile_ tile number
                         * @param cycle_ cycle number
                         */
                        base_cycle_metric(::uint16_t lane_=0, ::uint16_t tile_=0, ::uint16_t cycle_=0) :
                                base_metric(lane_,tile_), cycle(cycle_)
                        {
                        }
                        /** Set the lane, tile and cycle id from a base metric
                         *
                         * @param metric a base_metric from the model
                         */
                        template<class BaseMetric>
                        void set(const BaseMetric& metric)
                        {
                            base_metric::set(metric);
                            cycle = static_cast< ::uint16_t >(metric.cycle());
                        }
                        /** Unique id created from the lane, tile and cycle
                         *
                         * @return unique id
                         */
                        id_t id()const
                        {
                            return id(lane, tile, cycle);
                        }
                        /** Unique id created from both the lane, tile and cycle
                         *
                         * @param lane lane number
                         * @param tile tile number
                         * @param cycle cycle number
                         * @return unique id
                         */
                        static id_t id(id_t lane, id_t tile, id_t cycle)
                        {
                            return base_metric::id(lane, tile) | (cycle << 32);
                        }
                        /** Test if the layout contains valid data
                         *
                         * @return true if data is valid
                         */
                        bool is_valid()const
                        {
                            return tile > 0 && lane > 0 && cycle > 0;
                        }
                        /** Cycle number
                         */
                        ::uint16_t cycle;
                    };
                    /** Base class for InterOp records that contain tile specific metrics per read
                     *
                     * These records contain both a lane, tile and read identifier.
                     *
                     * @note These classes are packed such that there is not padding. Their size reflects the accumulation
                     * of their member fields.
                     */
                    struct base_read_metric : base_metric
                    {
                        /** Constructor
                         *
                         * @param lane_ lane number
                         * @param tile_ tile number
                         * @param read_ read number
                         */
                        base_read_metric(::uint16_t lane_=0, ::uint16_t tile_=0, ::uint16_t read_=0) :
                                base_metric(lane_,tile_), read(read_)
                        {
                        }
                        /** Set the lane, tile and read id from a base metric
                         *
                         * @param metric a base_metric from the model
                         */
                        template<class BaseMetric>
                        void set(const BaseMetric& metric)
                        {
                            base_metric::set(metric);
                            read = static_cast< ::uint16_t >(metric.read());
                        }
                        /** Unique id created from the lane, tile and read
                         *
                         * @return unique id
                         */
                        id_t id()const
                        {
                            return id(lane, tile, read);
                        }
                        /** Unique id created from both the lane, tile and read
                         *
                         * @param lane lane number
                         * @param tile tile number
                         * @param read_ read number
                         * @return unique id
                         */
                        static id_t id(id_t lane, id_t tile, id_t read_)
                        {
                            return base_metric::id(lane, tile) | (read_ << 32);
                        }
                        /** Test if the layout contains valid data
                         *
                         * @return true if data is valid
                         */
                        bool is_valid()const
                        {
                            return tile > 0 && lane > 0 && read > 0;
                        }
                        /** Read number
                         */
                        ::uint16_t read;
                    };

#pragma pack()
                }
        }
    }
}

