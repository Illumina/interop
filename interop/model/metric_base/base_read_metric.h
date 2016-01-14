/** Base information shared by per read tile metrics
 *
 * Per read tile metrics include any metric calculated for a specific tile at a given read.
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
                /** Base class for InterOp classes that contain per read tile specific metrics
                 *
                 * These classes define both a lane, tile and read identifier.
                 */
                class base_read_metric : public base_metric
                {
                public:
                    /** Constructor
                     *
                     * @param lane lane number
                     * @param tile tile number
                     * @param read read number
                     */
                    base_read_metric(const uint_t lane, const uint_t tile, const uint_t read) :
                            base_metric(lane, tile), m_read(read){}

                public:
                    /** Set the base metric identifiers
                     *
                     * @param base layout base
                     */
                    void set_base(const io::layout::base_read_metric &base)
                    {
                        base_metric::set_base(base);
                        m_read = base.read;
                    }
                    /** Read number
                     *
                     * @return read number
                     */
                    uint_t read()const{return m_read;}
                    /** Unique id created from the lane, tile and read
                     *
                     * @return unique id
                     */
                    id_t id()const
                    {
                        return id(lane(), tile(), m_read);
                    }
                    /** Create unique id from the lane, tile and read
                     *
                     * @param lane lane number
                     * @param tile tile number
                     * @param read read number
                     * @return unique id
                     */
                    static id_t id(const id_t lane, const id_t tile, const id_t read)
                    {
                        return io::layout::base_read_metric::id(lane, tile, read);
                    }

                private:
                    uint_t m_read;
                };
            }
        }
    }
}

