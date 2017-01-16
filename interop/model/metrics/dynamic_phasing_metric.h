/** Dynamic phasing metric
 *
 * The dynamic phasing metric contains the estimated phasing and prephasing slope/offsets learned for each tile and read.
 *
 *  @note This is not actually an InterOp written to disk, but is calculated from EmpiricalPhasingMetricsOut.bin
 *  @file
 *  @date 10/22/15.
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <cstring>
#include "interop/model/metric_base/base_read_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** Dynamic phasing metric
     *
     * The dynamic phasing metric contains the estimated phasing and prephasing slope/offsets learned for each tile and read.
     *
     * @note This is not actually an InterOp written to disk, but is calculated from EmpiricalPhasingMetricsOut.bin
     * @note Supported versions: 1
     */
    class dynamic_phasing_metric : public metric_base::base_read_metric
    {
    public:
        enum
        {
            /** Unique type code for metric */
                    TYPE = constants::DynamicPhasing,
            /** Latest version of the InterOp format */
                    LATEST_VERSION = 1
        };
    public:
        /** Constructor
         */
        dynamic_phasing_metric() :
                metric_base::base_read_metric(0, 0, 0),
                m_phasing_slope(0), m_phasing_offset(0), m_prephasing_slope(0), m_prephasing_offset(0)
        {
        }
        /** Constructor
         */
        dynamic_phasing_metric(const header_type&) :
                metric_base::base_read_metric(0, 0, 0),
                m_phasing_slope(0), m_phasing_offset(0), m_prephasing_slope(0), m_prephasing_offset(0)
        {
        }

        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param read read number
         * @param phasing_slope phasing slope for current read
         * @param phasing_offset phasing offset for current read
         * @param prephasing_slope prephasing slope for current read
         * @param prephasing_offset prephasing offset for current read
         */
        dynamic_phasing_metric(const uint_t lane,
                               const uint_t tile,
                               const uint_t read,
                               const float phasing_slope,
                               const float phasing_offset,
                               const float prephasing_slope,
                               const float prephasing_offset) :
                metric_base::base_read_metric(lane, tile, read), m_phasing_slope(phasing_slope)
                , m_phasing_offset(phasing_offset), m_prephasing_slope(prephasing_slope),
                m_prephasing_offset(prephasing_offset)
        {
        }

    public:
        /** @defgroup dynamic_phasing_metric Dynamic Phasing Metrics
         *
         * Per tile per read phasing slopes/offsets
         *
         * @note This is not actually an InterOp written to disk, but is calculated from EmpiricalPhasingMetricsOut.bin
         * @ref illumina::interop::model::metrics::dynamic_phasing_metric "See full class description"
         * @{
         */
        /** Slope of the best-fit line by cycle of all phasing metrics in the tile
         *
         * @return phasing slope
         */
        float phasing_slope() const
        {
            return m_phasing_slope;
        }

        /** Slope of the best-fit line by cycle of all prephasing metrics in the tile
         *
         * @return prephasing slope
         */
        float prephasing_slope() const
        {
            return m_prephasing_slope;
        }

        /** Offset of the best-fit line by cycle of all phasing metrics in the tile
         *
         * @return phasing offset
         */
        float phasing_offset() const
        {
            return m_phasing_offset;
        }

        /** Offset of the best-fit line by cycle of all prephasing metrics in the tile
         *
         * @return prephasing offset
         */
        float prephasing_offset() const
        {
            return m_prephasing_offset;
        }
        /** @} */

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return "DynamicPhasing"
         */
        static const char *prefix()
        { return "DynamicPhasing"; }

    private:
        float m_phasing_slope;
        float m_phasing_offset;
        float m_prephasing_slope;
        float m_prephasing_offset;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}
