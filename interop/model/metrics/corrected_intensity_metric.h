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
 * @TODO: overall description for % base
 *
 *  @file
 *
 *  @date 8/5/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include <limits>
#include <numeric>
#include <cstring>
#include <fstream>
#include "interop/constants/enums.h"
#include "interop/io/format/generic_layout.h"
#include "interop/model/metric_base/metric_set.h"
#include "interop/io/layout/base_metric.h"
#include "interop/model/metric_base/base_cycle_metric.h"

namespace illumina {
    namespace interop {
        namespace model {
            namespace metrics {
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
                    enum{
                        /** Unique type code for metric */
                        TYPE=constants::CorrectedInt,
                        /** Latest version of the InterOp format */
                        LATEST_VERSION=3
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
                    /** Define a uint16_t pointer to a uint16_t array
                     */
                    typedef ::uint16_t* ushort_pointer_t;
                    /** Define a uint pointer to a uint array
                     */
                    typedef ::uint32_t* uint_pointer_t;
                public:
                    /** Constructor
                     */
                    corrected_intensity_metric() :
                            metric_base::base_cycle_metric(0,0,0),
                            m_averageCycleIntensity(0),
                            m_correctedIntAll(constants::NUM_OF_BASES, 0),
                            m_correctedIntCalled(constants::NUM_OF_BASES, 0),
                            m_calledCounts(constants::NUM_OF_BASES_AND_NC, 0),
                            m_signalToNoise(0){}
                    /** Constructor
                     *
                     * @note Version 2, used for unit testing
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param averageCycleIntensity average intensity for the tile on that cycle
                     * @param signalToNoise signal to noise for given tile at given cycle
                     * @param correctedIntCalled average corrected intensity per PF base
                     * @param correctedIntAll average corrected intensity per base
                     * @param calledCounts number of clusters called per base
                     */
                    corrected_intensity_metric(const uint_t lane,
                                               const uint_t tile,
                                               const uint_t cycle,
                                               const ushort_t averageCycleIntensity,
                                               const float signalToNoise,
                                               const ushort_array_t& correctedIntCalled,
                                               const ushort_array_t& correctedIntAll,
                                               const uint_array_t& calledCounts) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_averageCycleIntensity(averageCycleIntensity),
                            m_correctedIntAll(correctedIntAll),
                            m_correctedIntCalled(correctedIntCalled),
                            m_calledCounts(calledCounts),
                            m_signalToNoise(signalToNoise)
                    {
                       INTEROP_ASSERT(correctedIntAll.size() == constants::NUM_OF_BASES);
                       INTEROP_ASSERT(correctedIntCalled.size() == constants::NUM_OF_BASES);
                       INTEROP_ASSERT(calledCounts.size() == constants::NUM_OF_BASES_AND_NC);
                    }
                    /** Constructor
                     *
                     * @note Version 2, used for unit testing
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param averageCycleIntensity average intensity for the tile on that cycle
                     * @param signalToNoise signal to noise for given tile at given cycle
                     * @param correctedIntCalled average corrected intensity per PF base
                     * @param correctedIntAll average corrected intensity per base
                     * @param calledCounts number of clusters called per base
                     */
                    corrected_intensity_metric(const uint_t lane,
                                               const uint_t tile,
                                               const uint_t cycle,
                                               const ushort_t averageCycleIntensity,
                                               const float signalToNoise,
                                               const ushort_pointer_t correctedIntCalled,
                                               const ushort_pointer_t correctedIntAll,
                                               const uint_pointer_t calledCounts) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_averageCycleIntensity(averageCycleIntensity),
                            m_correctedIntAll(correctedIntAll, correctedIntAll+constants::NUM_OF_BASES),
                            m_correctedIntCalled(correctedIntCalled, correctedIntCalled+constants::NUM_OF_BASES),
                            m_calledCounts(calledCounts, calledCounts+constants::NUM_OF_BASES_AND_NC),
                            m_signalToNoise(signalToNoise)
                    {
                       INTEROP_ASSERT(m_correctedIntAll.size() == constants::NUM_OF_BASES);
                       INTEROP_ASSERT(m_correctedIntCalled.size() == constants::NUM_OF_BASES);
                       INTEROP_ASSERT(m_calledCounts.size() == constants::NUM_OF_BASES_AND_NC);
                    }
                    /** Constructor
                     *
                     * @note Version 3
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param correctedIntCalled average corrected intensity per PF base
                     * @param calledCounts number of clusters called per base
                     */
                    corrected_intensity_metric(const uint_t lane,
                                               const uint_t tile,
                                               const uint_t cycle,
                                               const ushort_array_t& correctedIntCalled,
                                               const uint_array_t& calledCounts) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_averageCycleIntensity(std::numeric_limits<ushort_t>::max()),
                            m_correctedIntAll(constants::NUM_OF_BASES, 0),
                            m_correctedIntCalled(correctedIntCalled),
                            m_calledCounts(calledCounts),
                            m_signalToNoise(std::numeric_limits<float>::quiet_NaN())
                    {
                       INTEROP_ASSERT(correctedIntCalled.size() == constants::NUM_OF_BASES);
                       INTEROP_ASSERT(calledCounts.size() == constants::NUM_OF_BASES_AND_NC);
                    }
                    /** Constructor
                     *
                     * @note Version 3
                     * @param lane lane number
                     * @param tile tile number
                     * @param cycle cycle number
                     * @param correctedIntCalled average corrected intensity per PF base
                     * @param calledCounts number of clusters called per base
                     */
                    corrected_intensity_metric(const uint_t lane,
                                               const uint_t tile,
                                               const uint_t cycle,
                                               const ushort_pointer_t correctedIntCalled,
                                               const uint_pointer_t calledCounts) :
                            metric_base::base_cycle_metric(lane, tile, cycle),
                            m_averageCycleIntensity(std::numeric_limits<ushort_t>::max()),
                            m_correctedIntAll(constants::NUM_OF_BASES, 0),
                            m_correctedIntCalled(correctedIntCalled, correctedIntCalled+constants::NUM_OF_BASES),
                            m_calledCounts(calledCounts, calledCounts+constants::NUM_OF_BASES_AND_NC),
                            m_signalToNoise(std::numeric_limits<float>::quiet_NaN())
                    {// TODO: Add array safety to this constructor
                       INTEROP_ASSERT(m_correctedIntCalled.size() == constants::NUM_OF_BASES);
                       INTEROP_ASSERT(m_calledCounts.size() == constants::NUM_OF_BASES_AND_NC);
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
                    ushort_t averageCycleIntensity()const
                    {
                        return m_averageCycleIntensity;
                    }
                    /** Average corrected intensity for each type of base: A, C, G and T
                     *
                     * @note Supported version 2, not supported by 3 or later
                     * @param index index of the base (A=0, C=1, G=2, T=3)
                     * @return average corrected intensity for specified base
                     */
                    ushort_t correctedIntAll(size_t index)const
                    {
                       INTEROP_ASSERT(index < static_cast<size_t>(constants::NUM_OF_BASES));
                        return m_correctedIntAll[index];
                    }
                    /** Average corrected intensity for only base called clusters: A, C, G and T
                     *
                     * @note Supported by all versions
                     * @param index index of the base (A=0, C=1, G=2, T=3)
                     * @return average corrected intensity over only base called clusters
                     */
                    ushort_t correctedIntCalled(size_t index)const
                    {
                       INTEROP_ASSERT(index < static_cast<size_t>(constants::NUM_OF_BASES));
                        return m_correctedIntCalled[index];
                    }
                    /** Average corrected intensity for only base called clusters: A, C, G and T
                     *
                     * @return vector of corrected called intensities
                     */
                    const ushort_array_t& correctedIntCalled()const{return m_correctedIntCalled;}
                    /** Average corrected intensity for each type of base: A, C, G and T
                     *
                     * @note Supported version 2, not supported by 3 or later
                     * @return vector of corrected called intensities
                     */
                    const ushort_array_t& correctedIntAll()const{return m_correctedIntAll;}
                    /** Number of clusters called per called base (including No Calls)
                     *
                     * @return vector of called counts
                     */
                    const uint_array_t& calledCounts()const{return m_calledCounts;}
                    /** Number of clusters called per called base (including No Calls)
                     *
                     * This member accumulates cluster counts over all four bases and no calls (NC). This is used to
                     * calculate the percentage of clusters for which the selected base has been called.
                     *
                     * @note Supported by all versions
                     * @param index index of the base (NC=-1, A=0, C=1, G=2, T=3)
                     * @return number of clusters called per base
                     */
                    uint_t calledCounts(difference_type index)const
                    {
                       INTEROP_ASSERT((index+1) < static_cast<difference_type>(constants::NUM_OF_BASES_AND_NC));
                        return m_calledCounts[static_cast<uint_t>(index+1)];
                    }
                    /** Number of clusters per no call
                     *
                     * @note Supported by all versions
                     * @return number of clusters not called
                     */
                    uint_t noCalls()const
                    {
                        return m_calledCounts[0];
                    }
                    /** The signal to noise ratio is calculated as mean called intensity divided by standard deviation
                     * of non called intensities.
                     *
                     * @note Only supported by version 2, not supported by version 3 or later
                     * @return signal to noise calculated from mean called intensity
                     */
                    float signalToNoise()const
                    {
                        return m_signalToNoise;
                    }
                    /** Get the total number of clusters called
                     *
                     * @note Supported by all versions
                     * @return total number of clusters called
                     */
                    uint_t totalCalls()const
                    {
                        return std::accumulate(m_calledCounts.begin(), m_calledCounts.end(), 0);
                    }
                    /** Get the total summed intensity for all clusters
                     *
                     * @note Supported version 2, not supported by 3 or later
                     * @return total summed intensity
                     */
                    uint_t totalIntensity()const
                    {
                        return std::accumulate(m_correctedIntAll.begin(), m_correctedIntAll.end(), 0);
                    }
                    /** Get the total summed intensity for only called clusters
                     *
                     * @note Supported by all versions
                     * @return total summed intensity
                     */
                    uint_t totalCalledIntensity()const
                    {
                        return std::accumulate(m_correctedIntCalled.begin(), m_correctedIntCalled.end(), 0);
                    }
                    /** Get the percentage per base
                     *
                     * @note Supported by all versions
                     * @param index index of the base (NC=-1, A=0, C=1, G=2, T=3)
                     * @return percentage for given base
                     */
                    float percentBase(difference_type index)const
                    {
                        uint_t total = totalCalls();
                        if(total == 0) return std::numeric_limits<float>::quiet_NaN();
                        return calledCounts(index) / static_cast<float>(total) * 100;
                    }
                    /** Get the percentage of intensity over all clusters
                     *
                     * @note Supported version 2, not supported by 3 or later
                     * @param index index of the base (A=0, C=1, G=2, T=3)
                     * @return percentage of intensity for each base
                     */
                    float percentIntensity(size_t index)const
                    {
                        uint_t total = totalIntensity();
                        if(total == 0) return std::numeric_limits<float>::quiet_NaN();
                        return correctedIntAll(index) / static_cast<float>(total) * 100;
                    }
                    /** Get the percentage of intensity over only called clusters
                     *
                     * @note Supported by all versions
                     * @param index index of the base (A=0, C=1, G=2, T=3)
                     * @return percentage of intensity for each base
                     */
                    float percentCalledIntensity(const size_t index)const
                    {
                        uint_t total = totalCalledIntensity();
                        if(total == 0) return std::numeric_limits<float>::quiet_NaN();
                        return correctedIntCalled(index) / static_cast<float>(total) * 100;
                    }
                    /** @} */

                public:
                    /** Get the prefix of the InterOp filename
                     *
                     * @return "CorrectedInt"
                     */
                    static const char* prefix(){return "CorrectedInt";}

                private:
                    ushort_t m_averageCycleIntensity; // Version 1 & 2
                    ushort_array_t m_correctedIntAll; // Version 1 & 2
                    ushort_array_t m_correctedIntCalled;// All
                    uint_array_t m_calledCounts; // All
                    float m_signalToNoise; // Version 2

                    template<class MetricType, int Version>
                    friend struct io::generic_layout;
                };
            }
        }
    }
}





