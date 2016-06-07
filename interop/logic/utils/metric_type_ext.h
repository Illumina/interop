/** Extension functions for metric types
 *
 *  @file
 *  @date 4/28/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/constant_mapping.h"
#include "interop/constants/enums.h"
#include "interop/logic/utils/enums.h"

namespace illumina { namespace interop {  namespace logic { namespace utils {

    /** Convert metric type to metric group
     *
     * @param type metric type
     * @return metric group
     */
    inline constants::metric_group to_group(const constants::metric_type type)
    {
        using namespace constants;
        // TODO: This can be reduced to a single macro define
        typedef std::pair<metric_type, metric_group > mapped_t;
#       define INTEROP_TUPLE3(Metric, Description, Group) mapped_t(Metric,Group)
        static const mapped_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
#       undef INTEROP_TUPLE3
        return util::constant_mapping_get(name_types, type, UnknownMetricGroup);
    }
    /** Convert metric type to string description
     *
     * @param type metric type
     * @return string description
     */
    inline const std::string to_description(const constants::metric_type type)
    {
        using namespace constants;
        // TODO: This can be reduced to a single macro define
        typedef std::pair<metric_type, std::string > mapped_t;
#       define INTEROP_TUPLE3(Metric, Description, Group) mapped_t(Metric,Description)
        static const mapped_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
#       undef INTEROP_TUPLE3
        return util::constant_mapping_get(name_types, type, std::string("UnknownDescription"));
    }
    /** Test if metric type is indexed by DNA base
     *
     * @TODO: replace with above enumeration framework (to_group)
     *
     * @param type metric type
     * @return true if metric can be indexed by DNA base
     */
    inline bool is_base_metric(const constants::metric_type type)
    {
        using namespace illumina::interop::constants;
        switch(type)
        {
            case CalledIntensity:
            case CorrectedIntensity:
            case BasePercent:
                return true;
            default:
                return false;
        }
    }

    /** Test if metric type is indexed by channel
     *
     * @TODO: replace with above enumeration framework
     *
     * @param type metric type
     * @return true if metric can be indexed by channel
     */
    inline bool is_channel_metric(const constants::metric_type type)
    {
        using namespace illumina::interop::constants;
        switch(type)
        {
            case Intensity:
            case FWHM:
                return true;
            default:
                return false;
        }
    }

    /** Test if metric type is produced every read
     *
     * @TODO: replace with above enumeration framework
     *
     * @param type metric type
     * @return true if metric is produced every read
     */
    inline bool is_read_metric(const constants::metric_type type)
    {
        using namespace illumina::interop::constants;
        switch(type)
        {
            case PercentAligned:
            case PercentPhasing:
            case PercentPrephasing:
                return true;
            default:
                return false;
        }
    }

    /** Test if metric type is produced every cycle
     *
     * @TODO: replace with above enumeration framework
     *
     * @param type metric type
     * @return true if metric is produced every cycle
     */
    inline bool is_cycle_metric(const constants::metric_type type)
    {
        using namespace illumina::interop::constants;
        switch(type)
        {
            case Intensity:
            case FWHM:
            case CalledIntensity:
            case CorrectedIntensity:
            case SignalToNoise:
            case BasePercent:
            case Q20Percent:
            case Q30Percent:
            case QScore:
            case ErrorRate:
                return true;
            default:
                return false;
        }
    }

}}}}
