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

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** Total histogram by lane
     */
    class q_by_lane_metric : public q_metric
    {
    public:
        enum {

            /** Unique type code for metric */
            TYPE = constants::QByLane,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 6
        };
        /** Define the base type */
        typedef constants::base_lane_t base_t;
    public:
        /** Constructor
         */
        q_by_lane_metric() {}
        /** Constructor
         *
         * @param header header of q-metric set
         */
        q_by_lane_metric(const header_type& header) : q_metric(header)
        {
        }
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
                 const uint32_vector& qscore_hist) : q_metric(lane, tile, cycle, qscore_hist){}

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
            typedef uint32_vector::const_iterator const_iterator;
            typedef uint32_vector::iterator iterator;
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


