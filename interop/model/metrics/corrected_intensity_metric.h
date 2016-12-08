/** Set of corrected intensity metrics
 *
 * The InterOp files parsed by this class include:
 *  - InterOp/CorrectedIntMetrics.bin
 *  - InterOp/CorrectedIntMetricsOut.bin
 *
 * Corrected intensities are generated once phasing and prephasing are calculated. Note, the software generates
 * phasing/prephasing estimates after cycle 25 (for most platforms).
 *
 * For 4-dye systems, the intensity corrected for cross-talk between the color channels and phasing and prephasing
 *
 * For 2-dye systems, the values are calculated is another way and aid in accessing the progress of the run.
 *
 *
 *  @file
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <limits>
#include <numeric>
#include <cstring>
#include <fstream>
#include "interop/util/exception.h"
#include "interop/constants/enums.h"
#include "interop/io/format/generic_layout.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/model/metric_base/base_cycle_metric.h"

namespace illumina { namespace interop { namespace model { namespace metrics
{
    /** @brief Intensity and count metrics for each base
     *
     * The corrected intensity metrics provide the percent base plot and the intensity plots.
     *
     * For 4-dye systems, the intensity corrected for cross-talk between the color channels and phasing
     * and prephasing. These are used in base calling. Note, the software generates phasing/prephasing
     * estimates after cycle 25 (for most platforms).
     *
     * For 2-dye systems, the values are calculated is another way and aid in accessing the progress
     * of the run.
     *
     * @note Supported versions: 2 and 3
     */
    class corrected_intensity_metric : public metric_base::base_cycle_metric
    {
    public:
        enum
        {
            /** Unique type code for metric */
            TYPE = constants::CorrectedInt,
            /** Latest version of the InterOp format */
            LATEST_VERSION = 3
        };
        /** Define a diffference type
         */
        typedef ::intptr_t difference_type;
        /** Define a uint16_t array using an underlying vector
         */
        typedef std::vector<ushort_t> ushort_array_t;
        /** Define a uint array using an underlying vector
         */
        typedef std::vector<uint_t> uint_array_t;
        /** Define a float array using an underlying vector
         */
        typedef std::vector<float> float_array_t;
        /** Define a uint16_t pointer to a uint16_t array
         */
        typedef ::uint16_t *ushort_pointer_t;
        /** Define a uint pointer to a uint array
         */
        typedef ::uint32_t *uint_pointer_t;
    public:
        /** Constructor
         */
        corrected_intensity_metric() :
                metric_base::base_cycle_metric(0, 0, 0),
                m_average_cycle_intensity(0),
                m_corrected_int_all(constants::NUM_OF_BASES, std::numeric_limits<ushort_t>::max()),
                m_corrected_int_called(constants::NUM_OF_BASES, std::numeric_limits<ushort_t>::max()),
                m_called_counts(constants::NUM_OF_BASES_AND_NC, std::numeric_limits<uint_t>::max()),
                m_signal_to_noise(std::numeric_limits<float>::quiet_NaN())
        { }
        /** Constructor
         */
        corrected_intensity_metric(const header_type&) :
                metric_base::base_cycle_metric(0, 0, 0),
                m_average_cycle_intensity(0),
                m_corrected_int_all(constants::NUM_OF_BASES, std::numeric_limits<ushort_t>::max()),
                m_corrected_int_called(constants::NUM_OF_BASES, std::numeric_limits<ushort_t>::max()),
                m_called_counts(constants::NUM_OF_BASES_AND_NC, std::numeric_limits<uint_t>::max()),
                m_signal_to_noise(std::numeric_limits<float>::quiet_NaN())
        { }

        /** Constructor
         *
         * @note Version 2, used for unit testing
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param average_cycle_intensity average intensity for the tile on that cycle
         * @param signal_to_noise signal to noise for given tile at given cycle
         * @param corrected_int_called average corrected intensity per PF base
         * @param corrected_int_all average corrected intensity per base
         * @param called_counts number of clusters called per base
         */
        corrected_intensity_metric(const uint_t lane,
                                   const uint_t tile,
                                   const uint_t cycle,
                                   const ushort_t average_cycle_intensity,
                                   const float signal_to_noise,
                                   const ushort_array_t &corrected_int_called,
                                   const ushort_array_t &corrected_int_all,
                                   const uint_array_t &called_counts) :
                metric_base::base_cycle_metric(lane, tile, cycle),
                m_average_cycle_intensity(average_cycle_intensity),
                m_corrected_int_all(corrected_int_all),
                m_corrected_int_called(corrected_int_called),
                m_called_counts(called_counts),
                m_signal_to_noise(signal_to_noise)
        {
            INTEROP_ASSERT(m_corrected_int_all.size() == constants::NUM_OF_BASES);
            INTEROP_ASSERT(m_corrected_int_called.size() == constants::NUM_OF_BASES);
            INTEROP_ASSERT(m_called_counts.size() == constants::NUM_OF_BASES_AND_NC);
        }

        /** Constructor
         *
         * @note Version 2, used for unit testing
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param average_cycle_intensity average intensity for the tile on that cycle
         * @param signal_to_noise signal to noise for given tile at given cycle
         * @param corrected_int_called average corrected intensity per PF base
         * @param corrected_int_all average corrected intensity per base
         * @param called_counts number of clusters called per base
         */
        corrected_intensity_metric(const uint_t lane,
                                   const uint_t tile,
                                   const uint_t cycle,
                                   const ushort_t average_cycle_intensity,
                                   const float signal_to_noise,
                                   const ushort_pointer_t corrected_int_called,
                                   const ushort_pointer_t corrected_int_all,
                                   const uint_pointer_t called_counts) :
                metric_base::base_cycle_metric(lane, tile, cycle),
                m_average_cycle_intensity(average_cycle_intensity),
                m_corrected_int_all(corrected_int_all, corrected_int_all + constants::NUM_OF_BASES),
                m_corrected_int_called(corrected_int_called, corrected_int_called + constants::NUM_OF_BASES),
                m_called_counts(called_counts, called_counts + constants::NUM_OF_BASES_AND_NC),
                m_signal_to_noise(signal_to_noise)
        {
            INTEROP_ASSERT(m_corrected_int_all.size() == constants::NUM_OF_BASES);
            INTEROP_ASSERT(m_corrected_int_called.size() == constants::NUM_OF_BASES);
            INTEROP_ASSERT(m_called_counts.size() == constants::NUM_OF_BASES_AND_NC);
        }

        /** Constructor
         *
         * @note Version 3
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param corrected_int_called average corrected intensity per PF base
         * @param called_counts number of clusters called per base
         */
        corrected_intensity_metric(const uint_t lane,
                                   const uint_t tile,
                                   const uint_t cycle,
                                   const ushort_array_t &corrected_int_called,
                                   const uint_array_t &called_counts) :
                metric_base::base_cycle_metric(lane, tile, cycle),
                m_average_cycle_intensity(std::numeric_limits<ushort_t>::max()),
                m_corrected_int_all(constants::NUM_OF_BASES, std::numeric_limits<ushort_t>::max()),
                m_corrected_int_called(corrected_int_called),
                m_called_counts(called_counts),
                m_signal_to_noise(std::numeric_limits<float>::quiet_NaN())
        {
            INTEROP_ASSERT(m_corrected_int_all.size() == constants::NUM_OF_BASES);
            INTEROP_ASSERT(m_called_counts.size() == constants::NUM_OF_BASES_AND_NC);
        }

        /** Constructor
         *
         * @note Version 3
         * @param lane lane number
         * @param tile tile number
         * @param cycle cycle number
         * @param corrected_int_called average corrected intensity per PF base
         * @param called_counts number of clusters called per base
         */
        corrected_intensity_metric(const uint_t lane,
                                   const uint_t tile,
                                   const uint_t cycle,
                                   const ushort_pointer_t corrected_int_called,
                                   const uint_pointer_t called_counts) :
                metric_base::base_cycle_metric(lane, tile, cycle),
                m_average_cycle_intensity(std::numeric_limits<ushort_t>::max()),
                m_corrected_int_all(constants::NUM_OF_BASES, std::numeric_limits<ushort_t>::max()),
                m_corrected_int_called(corrected_int_called, corrected_int_called + constants::NUM_OF_BASES),
                m_called_counts(called_counts, called_counts + constants::NUM_OF_BASES_AND_NC),
                m_signal_to_noise(std::numeric_limits<float>::quiet_NaN())
        {// TODO: Add array safety to this constructor
            INTEROP_ASSERT(m_corrected_int_called.size() == constants::NUM_OF_BASES);
            INTEROP_ASSERT(m_called_counts.size() == constants::NUM_OF_BASES_AND_NC);
        }

    public:
        /** @defgroup corrected_intensity Corrected Intensity Metrics
         *
         * Per tile per cycle per channel average intensity values
         *
         * @ingroup run_metrics
         * @ref illumina::interop::model::metrics::corrected_intensity_metric "See full class description"
         * @{
         */
        /** Average intensity over all clusters
         *
         * @note Supported version  2, not supported by 3 or later
         * @return average intensity
         */
        ushort_t average_cycle_intensity() const
        {
            return m_average_cycle_intensity;
        }

        /** Average corrected intensity for each type of base: A, C, G and T
         *
         * @note Supported version 2, not supported by 3 or later
         * @param index index of the base (A=0, C=1, G=2, T=3)
         * @return average corrected intensity for specified base
         */
        ushort_t corrected_int_all(const constants::dna_bases index) const throw(index_out_of_bounds_exception)
        {
            if(index >= static_cast<constants::dna_bases>(m_corrected_int_all.size()))
                INTEROP_THROW( index_out_of_bounds_exception, "Base out of bounds");
            return m_corrected_int_all[index];
        }

        /** Average corrected intensity for only base called clusters: A, C, G and T
         *
         * @note Supported by all versions
         * @param index index of the base (A=0, C=1, G=2, T=3)
         * @return average corrected intensity over only base called clusters
         */
        ushort_t corrected_int_called(const constants::dna_bases index) const throw(index_out_of_bounds_exception)
        {
            if(index >= static_cast<constants::dna_bases>(m_corrected_int_called.size()))
                INTEROP_THROW(index_out_of_bounds_exception, "Base out of bounds");
            return m_corrected_int_called[index];
        }

        /** Number of clusters called per called base (including No Calls)
         *
         * @return vector of called counts
         */
        const uint_array_t &called_counts_array() const
        { return m_called_counts; }

        /** Average corrected intensity for only base called clusters: A, C, G and T
         *
         * @return vector of corrected called intensities
         */
        const ushort_array_t &corrected_int_called_array() const
        { return m_corrected_int_called; }

        /** Average corrected intensity for each type of base: A, C, G and T
         *
         * @note Supported version 2, not supported by 3 or later
         * @return vector of corrected called intensities
         */
        const ushort_array_t &corrected_int_all_array() const
        { return m_corrected_int_all; }

        /** Number of clusters called per called base (including No Calls)
         *
         * This member accumulates cluster counts over all four bases and no calls (NC). This is used to
         * calculate the percentage of clusters for which the selected base has been called.
         *
         * @note Supported by all versions
         * @param index index of the base (NC=-1, A=0, C=1, G=2, T=3)
         * @return number of clusters called per base
         */
        uint_t called_counts(const constants::dna_bases index) const throw(index_out_of_bounds_exception)
        {
            if(index >= static_cast<constants::dna_bases>(m_called_counts.size()))
                INTEROP_THROW( index_out_of_bounds_exception, "Base out of bounds");
            return m_called_counts[static_cast<uint_t>(index + 1)];
        }

        /** Number of clusters per no call
         *
         * @note Supported by all versions
         * @return number of clusters not called
         */
        uint_t no_calls() const
        {
            if(m_called_counts.empty()) return 0;
            return m_called_counts[0];
        }

        /** The signal to noise ratio is calculated as mean called intensity divided by standard deviation
         * of non called intensities.
         *
         * @note Only supported by version 2, not supported by version 3 or later
         * @return signal to noise calculated from mean called intensity
         */
        float signal_to_noise() const
        {
            return m_signal_to_noise;
        }

        /** Get the total number of clusters called
         *
         * @note Supported by all versions
         * @param nocalls if true include no calls
         * @return total number of clusters called
         */
        uint_t total_calls(const bool nocalls = false) const
        {
            if (nocalls)
                return std::accumulate(m_called_counts.begin(), m_called_counts.end(), 0);
            return std::accumulate(m_called_counts.begin() + 1, m_called_counts.end(), 0);
        }

        /** Get the total summed intensity for all clusters
         *
         * @note Supported version 2, not supported by 3 or later
         * @return total summed intensity
         */
        uint_t total_intensity() const
        {
            return std::accumulate(m_corrected_int_all.begin(), m_corrected_int_all.end(), 0);
        }

        /** Get the total summed intensity for only called clusters
         *
         * @note Supported by all versions
         * @return total summed intensity
         */
        uint_t total_called_intensity() const
        {
            return std::accumulate(m_corrected_int_called.begin(), m_corrected_int_called.end(), 0);
        }

        /** Get the percentage per base
         *
         * @note Supported by all versions
         * @param index index of the base (NC=-1, A=0, C=1, G=2, T=3)
         * @return percentage for given base
         */
        float percent_base(const constants::dna_bases index) const throw(index_out_of_bounds_exception)
        {
            uint_t total = total_calls(index == constants::NC);
            if (total == 0)
                return std::numeric_limits<float>::quiet_NaN();
            return called_counts(index) / static_cast<float>(total) * 100;
        }

        /** Get the percentage per base (does not include no calls)
         *
         * @note Supported by all versions
         * @return percentage for given base
         */
        float_array_t percent_bases() const
        {
            uint_t total = total_calls();
            std::vector<float> percent_bases(called_counts_array().size() - 1);
            for (size_t i = 0; i < percent_bases.size(); ++i)
                percent_bases[i] = (total == 0) ? std::numeric_limits<float>::quiet_NaN() :
                                   called_counts_array()[i + 1] / static_cast<float>(total) * 100;
            return percent_bases;
        }

        /** Get the percentage of no calls
         *
         * @note Supported by all versions
         * @return percentage for no calls
         */
        float percent_nocall() const throw(index_out_of_bounds_exception)
        {
            return percent_base(constants::NC);
        }
        /** @} */
        /** Average intensity over all clusters
         *
         * @deprecated Will be removed in 1.1.x (use average_cycle_intensity instead)
         * @note Supported version  2, not supported by 3 or later
         * @return average intensity
         */
        ushort_t averageCycleIntensity() const
        {
            return m_average_cycle_intensity;
        }

        /** Average corrected intensity for each type of base: A, C, G and T
         *
         * @deprecated Will be removed in 1.1.x (use corrected_int_all instead)
         * @note Supported version 2, not supported by 3 or later
         * @param index index of the base (A=0, C=1, G=2, T=3)
         * @return average corrected intensity for specified base
         */
        ushort_t correctedIntAll(size_t index) const
        {
            INTEROP_ASSERT(index < static_cast<size_t>(constants::NUM_OF_BASES));
            return m_corrected_int_all[index];
        }

        /** Average corrected intensity for only base called clusters: A, C, G and T
         *
         * @deprecated Will be removed in 1.1.x (use corrected_int_called instead)
         * @note Supported by all versions
         * @param index index of the base (A=0, C=1, G=2, T=3)
         * @return average corrected intensity over only base called clusters
         */
        ushort_t correctedIntCalled(size_t index) const
        {
            INTEROP_ASSERT(index < static_cast<size_t>(constants::NUM_OF_BASES));
            return m_corrected_int_called[index];
        }

        /** Average corrected intensity for only base called clusters: A, C, G and T
         *
         * @deprecated Will be removed in 1.1.x (use corrected_int_called_array instead)
         * @return vector of corrected called intensities
         */
        const ushort_array_t &correctedIntCalled() const
        { return m_corrected_int_called; }

        /** Average corrected intensity for each type of base: A, C, G and T
         *
         * @deprecated Will be removed in 1.1.x (use corrected_int_all_array instead)
         * @note Supported version 2, not supported by 3 or later
         * @return vector of corrected called intensities
         */
        const ushort_array_t &correctedIntAll() const
        { return m_corrected_int_all; }

        /** Number of clusters called per called base (including No Calls)
         *
         * @deprecated Will be removed in 1.1.x (use called_counts_array instead)
         * @return vector of called counts
         */
        const uint_array_t &calledCounts() const
        { return m_called_counts; }

        /** Number of clusters per no call
         *
         * @deprecated Will be removed in 1.1.x (use no_calls instead)
         * @note Supported by all versions
         * @return number of clusters not called
         */
        uint_t noCalls() const
        {
            return m_called_counts[0];
        }

        /** Get the percentage per base
         *
         * @deprecated Will be removed in 1.1.x (use percent_base instead)
         * @note Supported by all versions
         * @param index index of the base (NC=-1, A=0, C=1, G=2, T=3)
         * @return percentage for given base
         */
        float percentBase(difference_type index) const
        {
            uint_t total = totalCalls(static_cast<constants::dna_bases>(index) == constants::NC);
            if (total == 0) return std::numeric_limits<float>::quiet_NaN();
            return calledCounts(index) / static_cast<float>(total) * 100;
        }

        /** Number of clusters called per called base (including No Calls)
         *
         * This member accumulates cluster counts over all four bases and no calls (NC). This is used to
         * calculate the percentage of clusters for which the selected base has been called.
         *
         * @deprecated Will be removed in 1.1.x (use called_counts instead)
         * @note Supported by all versions
         * @param index index of the base (NC=-1, A=0, C=1, G=2, T=3)
         * @return number of clusters called per base
         */
        uint_t calledCounts(difference_type index) const
        {
            INTEROP_ASSERT((index + 1) < static_cast<difference_type>(constants::NUM_OF_BASES_AND_NC));
            return m_called_counts[static_cast<uint_t>(index + 1)];
        }

        /** The signal to noise ratio is calculated as mean called intensity divided by standard deviation
         * of non called intensities.
         *
         * @deprecated Will be removed in 1.1.x (use signal_to_noise instead)
         * @note Only supported by version 2, not supported by version 3 or later
         * @return signal to noise calculated from mean called intensity
         */
        float signalToNoise() const
        {
            return m_signal_to_noise;
        }

        /** Get the total number of clusters called
         *
         * @deprecated Will be removed in 1.1.x (use total_calls instead)
         * @note Supported by all versions
         * @return total number of clusters called
         */
        uint_t totalCalls(const bool nocalls = false) const
        {
            if (nocalls)
                return std::accumulate(m_called_counts.begin(), m_called_counts.end(), 0);
            return std::accumulate(m_called_counts.begin() + 1, m_called_counts.end(), 0);
        }

        /** Get the total summed intensity for all clusters
         *
         * @deprecated Will be removed in 1.1.x (use total_intensity instead)
         * @note Supported version 2, not supported by 3 or later
         * @return total summed intensity
         */
        uint_t totalIntensity() const
        {
            return std::accumulate(m_corrected_int_all.begin(), m_corrected_int_all.end(), 0);
        }

        /** Get the total summed intensity for only called clusters
         *
         * @deprecated Will be removed in 1.1.x (use total_called_intensity instead)
         * @note Supported by all versions
         * @return total summed intensity
         */
        uint_t totalCalledIntensity() const
        {
            return std::accumulate(m_corrected_int_called.begin(), m_corrected_int_called.end(), 0);
        }

        /** Get the percentage of intensity over all clusters
         *
         * @deprecated Will be removed in 1.1.x
         * @note Supported version 2, not supported by 3 or later
         * @param index index of the base (A=0, C=1, G=2, T=3)
         * @return percentage of intensity for each base
         */
        float percentIntensity(size_t index) const
        {
            uint_t total = totalIntensity();
            if (total == 0) return std::numeric_limits<float>::quiet_NaN();
            return correctedIntAll(index) / static_cast<float>(total) * 100;
        }

        /** Get the percentage of intensity over only called clusters
         *
         * @deprecated Will be removed in 1.1.x
         * @note Supported by all versions
         * @param index index of the base (A=0, C=1, G=2, T=3)
         * @return percentage of intensity for each base
         */
        float percentCalledIntensity(const size_t index) const
        {
            uint_t total = totalCalledIntensity();
            if (total == 0) return std::numeric_limits<float>::quiet_NaN();
            return correctedIntCalled(index) / static_cast<float>(total) * 100;
        }

    public:
        /** Get the prefix of the InterOp filename
         *
         * @return "CorrectedInt"
         */
        static const char *prefix()
        { return "CorrectedInt"; }

    private:
        ushort_t m_average_cycle_intensity; // Version 1 & 2
        ushort_array_t m_corrected_int_all; // Version 1 & 2
        ushort_array_t m_corrected_int_called;
        // All
        uint_array_t m_called_counts; // All
        float m_signal_to_noise; // Version 2

        template<class MetricType, int Version>
        friend
        struct io::generic_layout;
    };
}}}}






