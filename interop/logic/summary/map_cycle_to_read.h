/** Map each cycle to its read info
 *
 *  @file
 *  @date  4/20/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <vector>
#include <map>
#include "interop/logic/summary/summary_statistics.h"
#include "interop/model/metric_base/base_metric.h"

namespace illumina { namespace interop { namespace logic { namespace summary
{
    /** Read number and cycle in read
     */
    struct read_cycle
    {
        /** Constructor
         *
         * @param r read number
         * @param c cycle in read
         */
        read_cycle(const size_t r = 0, const size_t c = 0) : number(r), cycle_within_read(c),
                                                             is_last_cycle_in_read(false)
        { }

        /** Read number */
        size_t number;
        /** Cycle within read */
        size_t cycle_within_read;
        /** Is last cycle in read */
        bool is_last_cycle_in_read;
    };

    /** Vector of read_cycle objects */
    typedef std::vector<read_cycle> read_cycle_vector_t;

    /** Create a map from cycle to a pair: read number and cycle in read
     *
     * @param beg iterator to start of a collection of read infos
     * @param end iterator to end of a collection of read infos
     * @param cycle_to_read map that takes a cycle and returns the read-number cycle-in-read pair
     * @param op unary operator that takes some object and returns a read info
     */
    template<typename I, typename UnaryOp>
    void map_read_to_cycle_number(I beg, I end, read_cycle_vector_t &cycle_to_read, UnaryOp op)
    {
        cycle_to_read.resize(std::accumulate(beg, end, size_t(0), op::total_cycle_sum<UnaryOp>(op)));
        std::fill(cycle_to_read.begin(), cycle_to_read.end(), 0);
        for (; beg != end; ++beg)
        {
            for (size_t cycle = op(*beg).first_cycle() - 1, last_cycle = op(*beg).last_cycle(), cycle_in_read = 1;
                 cycle < last_cycle; ++cycle_in_read, ++cycle)
            {
                INTEROP_ASSERT(cycle < cycle_to_read.size());
                cycle_to_read[cycle] = read_cycle(op(*beg).number(), cycle_in_read);
            }
            INTEROP_ASSERT((op(*beg).last_cycle() - 1) < cycle_to_read.size());
            cycle_to_read[op(*beg).last_cycle() - 1].is_last_cycle_in_read = true;
        }
    }

    /** Create a map from cycle to a pair: read number and cycle in read
     *
     * @param beg iterator to start of a collection of read infos
     * @param end iterator to end of a collection of read infos
     * @param cycle_to_read map that takes a cycle and returns the read-number cycle-in-read pair
     */
    template<typename I>
    void map_read_to_cycle_number(I beg, I end, read_cycle_vector_t &cycle_to_read)
    {
        map_read_to_cycle_number(beg, end, cycle_to_read, op::default_get_read());
    }


}}}}

