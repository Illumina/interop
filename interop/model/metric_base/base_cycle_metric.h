/** Base information shared by per cycle tile metrics
 *
 * Per cycle tile metrics include any metric calculated for a specific tile at a given cycle.
 *
 *  @file
 *
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <algorithm>
#include "interop/model/metric_base/base_metric.h"


namespace illumina {
    namespace interop {
        namespace model {
            namespace metric_base {
                /** Defines default base header for metric */
                class base_cycle_metric_header
                {
                public:
                    /** Constructor */
                    base_cycle_metric_header() : m_max_cycle(0){}
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
                    base_metric::uint_t max_cycle()const{return m_max_cycle;}
                protected:
                    /** Update the max cycle
                     *
                     * @param metric cycle metric
                     */
                    template<class Metric>
                    inline void update_max_cycle(const Metric& metric)
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
                public:
                    /** Constructor
                     *
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     */
                    base_cycle_metric(const uint_t lane, const uint_t tile, const uint_t cycle) :
                            base_metric(lane, tile), m_cycle(cycle){}

                public:
                    /** Set the base metric identifiers
                     *
                     * @param base layout base
                     */
                    void set_base(const io::layout::base_cycle_metric &base)
                    {
                        base_metric::set_base(base);
                        m_cycle = base.cycle;
                    }
                    /** Cycle number
                     *
                     * @return cycle number
                     */
                    uint_t cycle()const{return m_cycle;}
                    /** Unique id created from the lane, tile and cycle
                     *
                     * @return unique id
                     */
                    id_t id()const
                    {
                        return id(lane(), tile(), m_cycle);
                    }
                    /** Create unique id from the lane, tile and cycle
                     *
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @return unique id
                     */
                    static id_t id(const id_t lane, const id_t tile, const id_t cycle)
                    {
                        return base_metric::id(lane, tile) | (cycle << TILE_BIT_SHIFT);
                    }

                private:
                    uint_t m_cycle;
                };
            }
        }
    }
}

