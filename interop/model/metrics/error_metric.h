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
 *
 *  @date 8/19/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <assert.h>
#include <cstring>
#include "interop/model/metric_base/base_cycle_metric.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/io/format/generic_layout.h"

namespace illumina {
    namespace interop {
        namespace model {
            namespace metrics {
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
                    enum{
                        /** Maximum number of mismatches */
                        MAX_MISMATCH=5,
                        /** Unique type code for metric */
                        TYPE=constants::Error,
                        /** Latest version of the InterOp format */
                        LATEST_VERSION=3
                    };
                    /** Define a uint array using an underlying vector
                     */
                    typedef std::vector<uint_t> uint_array_t;
                public:
                    /** Constructor
                     */
                    error_metric() :
                            metric_base::base_cycle_metric(0,0,0),
                            m_errorRate(0),
                            m_totalReads(0),
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
                    error_metric(uint_t lane,
                                 uint_t tile,
                                 uint_t cycle,
                                 float error) :
                            metric_base::base_cycle_metric(lane,tile,cycle),
                            m_errorRate(error),
                            m_totalReads(0),
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
                     * @{
                     */
                    /** Calculated error rate, as determined by a spiked in PhiX control sample.
                     *
                     * @note Supported by all versions
                     * @return error rate
                     */
                    float errorRate()const
                    {
                        return m_errorRate;
                    }
                    /** Total number of reads
                     *
                     * @note Supported by all versions
                     * @return total number of reads
                     */
                    uint_t totalReads()const
                    {
                        return m_totalReads;
                    }
                    /** Number of clusters at given number of mismatches
                     *
                     * 0: no mismatches
                     * 1: 1 mismatch
                     * etc.
                     *
                     * @note Unsupported on most platforms (TODO: MiSeq supported?)
                     * @param n index of read
                     * @return total number of errors
                     */
                    uint_t mismatch_cluster_count(size_t n)const
                    {
                        assert(n<MAX_MISMATCH);
                        return m_mismatch_cluster_count[n];
                    }
                    /** Size of mismatch array
                     *
                     * @note Unsupported on most platforms (TODO: MiSeq supported?)
                     * @return total number of errors
                     */
                    uint_t mismatch_count()const
                    {
                        return MAX_MISMATCH;
                    }
                    /** Get an array of mismatch cluster counts
                     *
                     * 0: no mismatches
                     * 1: 1 mismatch
                     * etc.
                     *
                     * @return vector of mismatch cluster counts
                     */
                    const uint_array_t& mismatch_cluster_counts()const
                    {
                        return m_mismatch_cluster_count;
                    }
                    /** @} */

                public:
                    /** Get the prefix of the InterOp filename
                     *
                     * @return "Error"
                     */
                    static const char* prefix(){return "Error";}

                private:
                    float m_errorRate;
                    uint_t m_totalReads;
                    uint_array_t m_mismatch_cluster_count;
                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
            }
        }
    }
}
