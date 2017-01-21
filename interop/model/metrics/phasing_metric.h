/** Phasing metric
 *
 * The phasing metric contains the estimated phasing weights learned for each tile and cycle.
 *
 *  @file
 *  @date 10/22/15.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <cstring>
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** Phasing metric
     *
     * The phasing metric contains the estimated phasing weights learned for each tile and cycle.
     *
     * @note Supported versions: 1 and 2
     */
    class phasing_metric : public metric_base::base_cycle_metric
    {
    public:
        enum
        {
            /** Unique type code for metric */
            TYPE = constants::EmpiricalPhasing,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 2
        };
    public:
        /** Constructor
         */
        phasing_metric() :
                metric_base::base_cycle_metric(0, 0, 0), m_phasing_weight(0), m_prephasing_weight(0)
        {
        }
        /** Constructor
         */
        phasing_metric(const header_type&) :
                metric_base::base_cycle_metric(0, 0, 0), m_phasing_weight(0), m_prephasing_weight(0)
        {
        }

        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param phasing_weight phasing weight for current cycle
         * @param prephasing_weight prephasing weight for current cycle
         */
        phasing_metric(const uint_t lane,
                       const uint_t tile,
                       const uint_t cycle,
                       const float phasing_weight,
                       const float prephasing_weight) :
                metric_base::base_cycle_metric(lane, tile, cycle), m_phasing_weight(phasing_weight)
                , m_prephasing_weight(prephasing_weight)
        {
        }

    public:
        /** @defgroup phasing_metric Phasing Metrics
         *
         * Per tile per cycle phasing weights
         *
         * @ref illumina::interop::model::metrics::phasing_metric "See full class description"
         * @{
         */
        /** Weight learned to correct for phasing in the sample
         *
         * @note Supported by all versions
         * @return phasing weight
         */
        float phasing_weight() const
        {
            return m_phasing_weight;
        }

        /** Weight learned to correct for prephasing in the sample
         *
         * @note Supported by all versions
         * @return prephasing weight
         */
        float prephasing_weight() const
        {
            return m_prephasing_weight;
        }
        /** @} */

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return "EmpiricalPhasing"
         */
        static const char *prefix()
        { return "EmpiricalPhasing"; }

    private:
        float m_phasing_weight;
        float m_prephasing_weight;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}

