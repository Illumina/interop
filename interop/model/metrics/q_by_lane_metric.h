/** Q-score metric by lane
 *
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/QMetricsByLane.bin
 *  - InterOp/QMetricsByLaneOut.bin
 *
 *  @file
 *  @date 5/17/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/model/metrics/q_metric.h"
#include "interop/io/format/metric_format_factory.h"

namespace illumina { namespace interop { namespace model { namespace metrics {
    /** Total histogram by lane
     */
    class q_by_lane_metric : public q_metric
    {
    public:
        enum {
            /** Unique type code for metric */
            TYPE = constants::QByLane
        };
    public:
        /** Constructor
         */
        q_by_lane_metric() {}
        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param qscore_hist q-score histogram
         */
        q_by_lane_metric(const uint_t lane,
                 const uint_t tile,
                 const uint_t cycle,
                 const uint_vector& qscore_hist) : q_metric(lane, tile, cycle, qscore_hist){}

        /** @defgroup q_metric_by_lane Quality Metrics By Lane
         *
         * Per lane per cycle quality metrics
         *
         * @ref illumina::interop::model::metrics::q_by_lane_metric "See full class description"
         *
         * @note All metrics in this class are supported by all versions
         * @ingroup run_metrics
         * @see illumina::interop::model::metrics::q_metric
         */

    public:
        /** Accummulate another q_metric from the same lane/cycle
         *
         * @param metric q_metric from same lane/cycle
         */
        void accumulate_by_lane(const q_metric& metric)
        {
            typedef uint_vector::const_iterator const_iterator;
            typedef uint_vector::iterator iterator;
            iterator it = m_qscore_hist.begin();
            for (const_iterator beg = metric.qscore_hist().begin(), end = metric.qscore_hist().end();
                 beg != end; ++beg, ++it)
                *it += *beg;
        }

    public:
        /** Get the suffix of the InterOp filename
         *
         * @return suffix
         */
        static const char* suffix(){return "ByLane";}
    };

}}}}

namespace illumina { namespace interop { namespace io
{
    /** Specialization of metric type adapter
     *
     * This class allows a metric derived from another metric to use it's format
     * For example, q_by_lane_metric uses the q_metric format
     */
    template<>
    struct metric_format_adapter<model::metrics::q_by_lane_metric>
    {
        /** Define the template parameter as the target type */
        typedef model::metrics::q_metric metric_t;
    };
}}}
