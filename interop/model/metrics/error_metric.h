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
#include "interop/model/metric_base/metric_exceptions.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/io/format/generic_layout.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** Header information for an error metric file, used for format version 6
     */
    class error_metric_header : public metric_base::base_cycle_metric::header_type
    {
    public:
        /** Constructor
         */
        error_metric_header(const std::vector<std::string>& adapters) :
            m_number_adapters(static_cast<uint16_t>(adapters.size())),
            m_adapter_length(0)
        {
            if(adapters.size()>0)
                m_adapter_length = static_cast<uint16_t>(adapters[0].size());
            for(size_t i = 0; i < adapters.size(); i++) {
                INTEROP_ASSERTMSG(adapters[i].size() == m_adapter_length,
                                  "Adapter Sequence (" << adapters[i] << ") length doesn't match expected adapter length");
                m_adapter_sequences.push_back(adapters[i]);
            }
        }

        error_metric_header() :
            m_number_adapters(0),
            m_adapter_length(0)
        {}

        /** number of adapter sequences in model
         *
         * @return m_number_adapters
         */
        uint16_t number_adapters() const
        {
            return m_number_adapters;
        }

        /** Length of each adapter sequence
         *
         * @return m_adapter_length
         */
        uint16_t adapter_length() const
        {
            return m_adapter_length;
        }

        /** Adapter sequences
         *
         * @return m_adapter_sequences
         */
        const std::vector<std::string>& adapter_sequences() const
        {
            return m_adapter_sequences;
        }

        void clear()
        {
            m_number_adapters = 0;
            m_adapter_length = 0;
            m_adapter_sequences.clear();
            metric_base::base_cycle_metric::header_type::clear();
        }
        /** Generate a default header
         *
         * @return default header
         */
        static error_metric_header default_header()
        {
            return error_metric_header(std::vector<std::string>());
        }

    private:
        uint16_t m_number_adapters;
        uint16_t m_adapter_length;
        std::vector<std::string> m_adapter_sequences;

        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };

    /** @brief Error rate for a spiked in PhiX control sample
     *
     * The error metric is the calculated error rate, as determined by a spiked in PhiX control sample.
     * This metric is available for each lane and tile for every cycle.
     *
     * @note Supported versions: 3, 4, 5 and 6
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
            /** Latest version of the I
             * nterOp format */
            LATEST_VERSION = 6
        };
        /** Define a uint array using an underlying vector
         */
        typedef std::vector<uint_t> uint_array_t;
        /** Vector of floats points */
        typedef std::vector<float> float_vector;
        /** Error metric set header */
        typedef error_metric_header header_type;
    public:
        /** Constructor
         */
        error_metric() :
            metric_base::base_cycle_metric(0, 0, 0),
            m_error_rate(std::numeric_limits<float>::quiet_NaN()),
            m_phix_adapter_rate(std::numeric_limits<float>::quiet_NaN()),
            m_mismatch_cluster_count(MAX_MISMATCH, 0)
        {
        }
        /** Constructor
         */
        error_metric(const header_type& header) :
            metric_base::base_cycle_metric(0, 0, 0),
            m_error_rate(std::numeric_limits<float>::quiet_NaN()),
            m_phix_adapter_rate(std::numeric_limits<float>::quiet_NaN()),
            m_phix_adapter_rates(header.number_adapters(), 0),
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
                     const float error,
                     const float phix_adapter_rate) :
            metric_base::base_cycle_metric(lane, tile, cycle),
            m_error_rate(error),
            m_phix_adapter_rate(phix_adapter_rate),
            m_mismatch_cluster_count(MAX_MISMATCH, 0)
        {
        }

        /** Constructor
         *
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param error error rate for current cycle
         * @param phix_adapter_rates rates of each adapter
         */
        error_metric(const uint_t lane,
                     const uint_t tile,
                     const uint_t cycle,
                     const float error,
                     const float_vector &phix_adapter_rates) :
            metric_base::base_cycle_metric(lane, tile, cycle),
            m_error_rate(error),
            m_phix_adapter_rates(phix_adapter_rates),
            m_mismatch_cluster_count(MAX_MISMATCH, 0)
        {
            set_mean_adapter_rate();
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

        /** Calculated adapter trim rate of PhiX clusters
         *
         * @note Supported by v5
         * @return adapter trim rate
         */
        float phix_adapter_rate() const
        {
            return m_phix_adapter_rate;
        }
        /* Calculated adapter trim rate per adapter of PhiX clusters
         *
         * @note Supported by v6
         * @return vector of adapter rates per adapter
         */
        const float_vector& phix_adapter_rates() const
        {
            return m_phix_adapter_rates;
        }
        /* Calculated adapter trim rate per adapter of PhiX clusters
         *
         * @note Supported by v6
         * @return adapter rate for given adapter index
         */
        float phix_adapter_rate_at(const size_t n) const INTEROP_THROW_SPEC((model::index_out_of_bounds_exception))
        {
            INTEROP_BOUNDS_CHECK(n, m_phix_adapter_rates.size(), "Index out of bounds");
            return m_phix_adapter_rates[n];
        }
        /* Number of adapters on PhiX
         *
         * @note Supported by v6
         * @return number of adapters
         */
        size_t phix_adapter_count() const
        {
            return m_phix_adapter_rates.size();
        }

        /** Number of clusters at given number of mismatches
         *
         * 0: no mismatches
         * 1: 1 mismatch
         * etc.
         *
         * @note Suppored by version 3, really unsupported on most platforms, some older MiSeq and HiSeq support
         * @param n index of read
         * @return total number of errors
         */
        uint_t mismatch_cluster_count(const size_t n) const INTEROP_THROW_SPEC((model::index_out_of_bounds_exception))
        {
            INTEROP_BOUNDS_CHECK(n, static_cast<size_t>(MAX_MISMATCH), "Index out of bounds");
            return m_mismatch_cluster_count[n];
        }

        /** Size of mismatch array
         *
         * @note Suppored by version 3, really unsupported on most platforms, some older MiSeq and HiSeq support
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
         * @note Suppored by version 3, really unsupported on most platforms, some older MiSeq and HiSeq support
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

        void set_mean_adapter_rate() {
            const size_t size = m_phix_adapter_rates.size();
            if(size == 0) return;
            m_phix_adapter_rate = 0;
            for(size_t i = 0; i < size; i++)
                m_phix_adapter_rate += m_phix_adapter_rates[i];
            m_phix_adapter_rate /= size;
        }

        float m_error_rate;
        float m_phix_adapter_rate;
        float_vector m_phix_adapter_rates;
        uint_array_t m_mismatch_cluster_count;
        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}

