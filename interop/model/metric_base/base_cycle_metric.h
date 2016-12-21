/** Base information shared by per cycle tile metrics
 *
 * Per cycle tile metrics include any metric calculated for a specific tile at a given cycle.
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
    /** Defines default base header for metric */
    class base_cycle_metric_header : public empty_header
    {
    public:
        /** Define the base type */
        typedef constants::base_cycle_t base_t;
    public:
        /** Constructor */
        base_cycle_metric_header() : m_max_cycle(0)
        { }

    public:
        /** Generate a default header
         *
         * @return default header
         */
        static base_cycle_metric_header default_header()
        {
            return base_cycle_metric_header();
        }

    public:
        /** Get cycle
         *
         * @return cycle
         */
        base_metric::uint_t max_cycle() const
        { return m_max_cycle; }
        /** Clear the max_cycle
         */
        void clear()
        {
            m_max_cycle=0;
        }

    protected:
        /** Update the max cycle
         *
         * @param metric cycle metric
         */
        template<class Metric>
        inline void update_max_cycle(const Metric &metric)
        {
            m_max_cycle = std::max(m_max_cycle, metric.cycle());
        }

    private:
        /** Maximum cycle read **/
        base_metric::uint_t m_max_cycle;
    };
    /** Base class for InterOp classes that contain per cycle tile specific metrics
     *
     * These classes define both a lane, tile and cycle identifier.
     */
    class base_cycle_metric : public base_metric
    {
    public:
        /** A cycle metric_set header
         */
        typedef base_cycle_metric_header header_type;
        /** Define the base type */
        typedef constants::base_cycle_t base_t;
    public:
        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         */
        base_cycle_metric(const uint_t lane=0, const uint_t tile=0, const uint_t cycle=0) :
                base_metric(lane, tile), m_cycle(cycle)
        { }

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
        /** Set id
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         */
        void set_base(const uint_t lane, const uint_t tile, const uint_t cycle)
        {
            base_metric::set_base(lane, tile);
            m_cycle = cycle;
        }
        /** Set the base metric identifiers
         *
         * @param base layout base
         */
        template<class BaseCycleMetric>
        void set_base(const BaseCycleMetric &base)
        {
            base_metric::set_base(base);
            m_cycle = base.cycle;
        }

        /** Cycle number
         *
         * @return cycle number
         */
        uint_t cycle() const
        { return m_cycle; }

        /** Unique id created from the lane, tile and cycle
         *
         * @return unique id
         */
        id_t id() const
        {
            return create_id(lane(), tile(), m_cycle);
        }
        /** Unique id created from the lane, tile and cycle
         *
         * @return unique id
         */
        id_t cycle_hash() const
        {
            return create_id(lane(), tile(), m_cycle);
        }
        /** Get the cycle from the unique lane/tile/cycle id
         *
         * @param id unique lane/tile/cycle id
         * @return cycle number
         */
        static id_t cycle_from_id(const id_t id)
        {
            // 1. Shift off lane and tile bits
            // 2. Shift back, while shifting off reserved bits
            return (id << (LANE_BIT_COUNT+TILE_BIT_COUNT)) >> (LANE_BIT_COUNT+TILE_BIT_COUNT+RESERVED_BIT_COUNT);
        }
        /** Get the reserved number from the unique lane/tile/cycle id
         *
         * @param id unique lane/tile/cycle id
         * @return reserved number
         */
        static id_t reserved_from_id(const id_t id)
        {
            // 1. Shift off lane, tile and cycle bits
            // 2. Shift back
            return (id << (LANE_BIT_COUNT+TILE_BIT_COUNT+CYCLE_BIT_COUNT)) >> (LANE_BIT_COUNT+TILE_BIT_COUNT+CYCLE_BIT_COUNT);
        }

        /** Create unique id from the lane, tile and cycle
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @return unique id
         */
        static id_t create_id(const id_t lane, const id_t tile, const id_t cycle)
        {
            return base_metric::create_id(lane, tile) | (cycle << CYCLE_BIT_SHIFT);
        }

    private:
        uint_t m_cycle;
    };



    /** Specialization for base_cycle_metric providing sentinel values*/
    template<>
    struct metric_attributes<base_cycle_metric> : public metric_attributes<base_metric>{};

}}}}


