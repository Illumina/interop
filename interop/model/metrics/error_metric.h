/** Error metric
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/ErrorMetrics.bin
 *  - InterOp/ErrorMetricsOut.bin
 *
 * The error metric is the calculated error rate, as determined by a spiked in PhiX control sample. This metric
 * is available for each lane and tile for every cycle.
 *
 *  @file
 *  @date 8/19/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <cstring>
#include "interop/util/exception.h"
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** @brief Error rate for a spiked in PhiX control sample
     *
     * The error metric is the calculated error rate, as determined by a spiked in PhiX control sample.
     * This metric is available for each lane and tile for every cycle.
     *
     * @note Supported versions: 3
     */
    class error_metric : public metric_base::base_cycle_metric
    {
    public:
        enum
        {
            /** Maximum number of mismatches */
            MAX_MISMATCH = 5,
            /** Unique type code for metric */
            TYPE = constants::Error,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 3
        };
        /** Define a uint array using an underlying vector
         */
        typedef std::vector<uint_t> uint_array_t;
    public:
        /** Constructor
         */
        error_metric() :
                metric_base::base_cycle_metric(0, 0, 0),
                m_error_rate(std::numeric_limits<float>::quiet_NaN()),
                m_mismatch_cluster_count(MAX_MISMATCH, 0)
        {
        }
        /** Constructor
         */
        error_metric(const header_type&) :
                metric_base::base_cycle_metric(0, 0, 0),
                m_error_rate(std::numeric_limits<float>::quiet_NaN()),
                m_mismatch_cluster_count(MAX_MISMATCH, 0)
        {
        }

        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param error error rate for current cycle
         */
        error_metric(const uint_t lane,
                     const uint_t tile,
                     const uint_t cycle,
                     const float error) :
                metric_base::base_cycle_metric(lane, tile, cycle),
                m_error_rate(error),
                m_mismatch_cluster_count(MAX_MISMATCH, 0)
        {
        }

    public:
        /** @defgroup error_metric Error Metrics
         *
         * Per tile per cycle PhiX sequence error
         *
         * @ref illumina::interop::model::metrics::error_metric "See full class description"
         *
         * @ingroup run_metrics
         * @{
         */
        /** Calculated error rate, as determined by a spiked in PhiX control sample.
         *
         * @note Supported by all versions
         * @return error rate
         */
        float error_rate() const
        {
            return m_error_rate;
        }

        /** Number of clusters at given number of mismatches
         *
         * 0: no mismatches
         * 1: 1 mismatch
         * etc.
         *
         * @note Unsupported on most platforms, some older MiSeq and HiSeq support
         * @param n index of read
         * @return total number of errors
         */
        uint_t mismatch_cluster_count(const size_t n) const throw(index_out_of_bounds_exception)
        {
            if(n >= m_mismatch_cluster_count.size())
                INTEROP_THROW( index_out_of_bounds_exception, "Index out of bounds");
            return m_mismatch_cluster_count[n];
        }

        /** Size of mismatch array
         *
         * @note Unsupported on most platforms, some older MiSeq and HiSeq support
         * @return total number of errors
         */
        uint_t mismatch_count() const
        {
            return MAX_MISMATCH;
        }

        /** Get an array of mismatch cluster counts
         *
         * 0: no mismatches
         * 1: 1 mismatch
         * etc.
         *
         * @note Unsupported on most platforms, some older MiSeq and HiSeq support
         * @return vector of mismatch cluster counts
         */
        const uint_array_t &mismatch_cluster_counts() const
        {
            return m_mismatch_cluster_count;
        }
        /** @} */
        /** Calculated error rate, as determined by a spiked in PhiX control sample.
         *
         * @deprecated Will be removed in 1.1.x (use error_rate instead)
         * @note Supported by all versions
         * @return error rate
         */
        float errorRate() const
        {
            return m_error_rate;
        }

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return "Error"
         */
        static const char *prefix()
        { return "Error"; }

    private:
        float m_error_rate;
        uint_array_t m_mismatch_cluster_count;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}

