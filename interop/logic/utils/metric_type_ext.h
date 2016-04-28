/** Extension functions for metric types
 *
 *  @file
 *  @date 4/28/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/constants/enums.h"

namespace illumina { namespace interop {  namespace logic { namespace utils {

    bool is_base_metric(const constants::metric_type type)
    {
        using namespace illumina::interop::constants;
        switch(type)
        {
            case CalledIntensity:
            case CorrectedIntensity:
            case PercentBase:
                return true;
            default:
                return false;
        }
    }

    bool is_channel_metric(const constants::metric_type type)
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

    bool is_read_metric(const constants::metric_type type)
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

    bool is_cycle_metric(const constants::metric_type type)
    {
        using namespace illumina::interop::constants;
        switch(type)
        {
            case Intensity:
            case FWHM:
            case CalledIntensity:
            case CorrectedIntensity:
            case SignalToNoise:
            case PercentBase:
            case PercentQ20:
            case PercentQ30:
            case QScore:
            case ErrorRate:
                return true;
            default:
                return false;
        }
    }

}}}}
