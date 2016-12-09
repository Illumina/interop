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
 *  @date 7/30/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <cstddef>
#include "interop/util/static_assert.h"
#include "interop/util/cstdint.h"
#include "interop/constants/typedefs.h"

namespace illumina { namespace interop { namespace io { namespace layout
{
#pragma pack(1)

    /** Base class for InterOp records that contain tile specific metrics
     *
     * These records contain both a lane and tile identifier.
     *
     * @note These classes are packed such that there is not padding. Their size reflects the accumulation of their
     * member fields.
     */
    template<class T>
    struct base_metric
    {
        /** Lane integral type */
        typedef ::uint16_t lane_t;
        /** Tile integral type */
        typedef T tile_t;
        /** Define a record size type */
        typedef ::uint8_t record_size_t;
        /** Define base type */
        typedef constants::base_tile_t base_t;

        /** Constructor
         *
         * @param lane_ lane number
         * @param tile_ tile number
         */
        base_metric(const lane_t lane_ = 0, const tile_t tile_ = 0) :
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
        void set(const BaseMetric &metric)
        {
            lane = static_cast< lane_t >(metric.lane());
            tile = static_cast< tile_t >(metric.tile());
        }

        /** Test if the layout contains valid data
         *
         * @return true if data is valid
         */
        bool is_valid() const
        {
            return tile > 0 && lane > 0;
        }

        /** Lane number
         */
        lane_t lane;
        /** Tile number
         */
        tile_t tile;
    };

    /** Base class for InterOp records that contain tile specific metrics per cycle
     *
     * These records contain both a lane, tile and cycle identifier.
     *
     * @note These classes are packed such that there is not padding. Their size reflects the accumulation of their
     * member fields.
     */
    template<class T>
    struct base_cycle_metric : base_metric<T>
    {
        /** Lane integral type */
        typedef typename base_metric<T>::lane_t lane_t;
        /** Tile integral type */
        typedef typename base_metric<T>::tile_t tile_t;
        /** Cycle intergral type */
        typedef ::uint16_t cycle_t;
        /** Define base type */
        typedef constants::base_cycle_t base_t;
        /** Constructor
         *
         * @param lane_ lane number
         * @param tile_ tile number
         * @param cycle_ cycle number
         */
        base_cycle_metric(const lane_t lane_ = 0, const tile_t tile_ = 0, const cycle_t cycle_ = 0) :
                base_metric<T>(lane_, tile_), cycle(cycle_)
        {
        }

        /** Set the lane, tile and cycle id from a base metric
         *
         * @param metric a base_metric from the model
         */
        template<class BaseMetric>
        void set(const BaseMetric &metric)
        {
            base_metric<T>::set(metric);
            cycle = static_cast< cycle_t >(metric.cycle());
        }

        /** Test if the layout contains valid data
         *
         * @return true if data is valid
         */
        bool is_valid() const
        {
            return base_metric<T>::is_valid() && cycle > 0;
        }

        /** Cycle number
         */
        cycle_t cycle;
    };

    /** Base class for InterOp records that contain tile specific metrics per read
     *
     * These records contain both a lane, tile and read identifier.
     *
     * @note These classes are packed such that there is not padding. Their size reflects the accumulation
     * of their member fields.
     */
    template<class T>
    struct base_read_metric : base_metric<T>
    {
        /** Lane integral type */
        typedef typename base_metric<T>::lane_t lane_t;
        /** Tile integral type */
        typedef typename base_metric<T>::tile_t tile_t;
        /** Read integral type */
        typedef ::uint16_t read_t;
        /** Define base type */
        typedef constants::base_read_t base_t;
        /** Constructor
         *
         * @param lane_ lane number
         * @param tile_ tile number
         * @param read_ read number
         */
        base_read_metric(const lane_t lane_ = 0, const tile_t tile_ = 0, const read_t read_ = 0) :
                base_metric<T>(lane_, tile_), read(read_)
        {
        }

        /** Set the lane, tile and read id from a base metric
         *
         * @param metric a base_metric from the model
         */
        template<class BaseMetric>
        void set(const BaseMetric &metric)
        {
            base_metric<T>::set(metric);
            read = static_cast< read_t >(metric.read());
        }

        /** Test if the layout contains valid data
         *
         * @return true if data is valid
         */
        bool is_valid() const
        {
            return base_metric<T>::is_valid() && read > 0;
        }

        /** Read number
         */
        read_t read;
    };

#pragma pack()
}}}}


