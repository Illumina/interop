/** Base information shared by per read tile metrics
 *
 * Per read tile metrics include any metric calculated for a specific tile at a given read.
 *
 *  @file
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <algorithm>
#include "interop/util/type_traits.h"
#include "interop/model/metric_base/base_metric.h"

namespace illumina { namespace interop { namespace model { namespace metric_base
{
    class base_read_metric;

    /** Defines default base read header for metric */
    class base_read_metric_header : public empty_header
    {
    public:
        /** Define the base type */
        typedef constants::base_read_t base_t;

    public:
        /** Constructor */
        base_read_metric_header() { }

    public:
        /** Generate a default header
         *
         * @return default header
         */
        static base_read_metric_header default_header()
        {
            return base_read_metric_header();
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
        void update_max_cycle(const base_read_metric &) { }
    };

    /** Base class for InterOp classes that contain per read tile specific metrics
     *
     * These classes define both a lane, tile and read identifier.
     */
    class base_read_metric : public base_metric
    {
    public:
        /** A read metric_set header
         */
        typedef base_read_metric_header header_type;
        /** Define the base type */
        typedef constants::base_read_t base_t;
    public:
        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param read read number
         */
        base_read_metric(const uint_t lane, const uint_t tile, const uint_t read) :
                base_metric(lane, tile), m_read(read) { }

    public:
        /** Set id
         *
         * @note This is only supported to enable easier unit testing, this should not be used.
         * @param lane lane number
         * @param tile tile number
         */
        void set_base(const uint_t lane, const uint_t tile)
        {
            base_metric::set_base(lane, tile);
        }
        /**
         * Sets ID given lane-tile-read as opposed to the BaseReadMetric
         *
         * @param lane lane number
         * @param tile tile number
         * @param read read number
         */
        void set_base(const uint_t lane, const uint_t tile, const uint_t read)
        {
            base_metric::set_base(lane, tile);
            m_read = read;
        }
        /** Set the base metric identifiers
         *
         * @param base layout base
         */
        template<class BaseReadMetric>
        void set_base(const BaseReadMetric &base)
        {
            base_metric::set_base(base);
            m_read = base.read;
        }

        /** Read number
         *
         * @return read number
         */
        uint_t read() const { return m_read; }

        /** Unique id created from the lane, tile and read
         *
         * @return unique id
         */
        id_t id() const
        {
            return create_id(lane(), tile(), m_read);
        }

        /** Create unique id from the lane, tile and read
         *
         * @param lane lane number
         * @param tile tile number
         * @param read read number
         * @return unique id
         */
        static id_t create_id(const id_t lane, const id_t tile, const id_t read)
        {
            return base_metric::create_id(lane, tile) | (read << READ_BIT_SHIFT);
        }
        /** Get the read from the unique lane/tile/read id
         *
         * @param id unique lane/tile/read id
         * @return read number
         */
        static id_t read_from_id(const id_t id)
        {
            // 1. Shift off lane and tile bits
            // 2. Shift back, while shifting off reserved bits
            return (id << (LANE_BIT_COUNT+TILE_BIT_COUNT)) >> (LANE_BIT_COUNT+TILE_BIT_COUNT+RESERVED_BIT_COUNT);
        }

    private:
        uint_t m_read;
    };

    /** Specialization for base_cycle_metric providing sentinel values*/
    template<>
    struct metric_attributes<base_read_metric> : public metric_attributes<base_metric>{};


}}}}


