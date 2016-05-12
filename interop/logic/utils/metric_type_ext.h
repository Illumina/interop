/** Extension functions for metric types
 *
 *  @file
 *  @date 4/28/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/constants/enums.h"
#include "interop/logic/utils/enums.h"

namespace illumina { namespace interop {  namespace logic { namespace utils {

    namespace detail
    {
            /** Specialization that maps metric group types to their string names */
            template<typename E>
            class metric_to_group
            {
            public:
                /** Define string key type */
                typedef constants::metric_group key_t;
            private:
                typedef std::pair<key_t, E> name_type_pair_t;
                typedef std::vector< name_type_pair_t > name_type_vector_t;

            public:
                /** Get vector that maps enumeration string names to enumeration values
                 *
                 * @return vector of enumeration string names and enumeration values
                 */
                static const name_type_vector_t& key_type_pair_vector()
                {
                    using namespace constants;
                    /** This macro maps an enum description to a string/enum pair */
#           define INTEROP_ENUM_DESCRIPTION(X, Y, Z) name_type_pair_t(Z,X)
                    static const name_type_pair_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
#           undef INTEROP_ENUM_DESCRIPTION
                    static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
                    return tmp;
                }
                /** Get the unknown type
                 *
                 * @return unknown type
                 */
                static E unknown(){return constants::UnknownMetricType;}
            };
            /** Specialization that maps metric group types to their string names
             *
             * @todo replace by macro?
             */
            template<typename E>
            class metric_to_description
            {
            public:
                /** Define string key type */
                typedef std::string key_t;
            private:
                typedef std::pair<key_t, E> name_type_pair_t;
                typedef std::vector< name_type_pair_t > name_type_vector_t;

            public:
                /** Get vector that maps enumeration string names to enumeration values
                 *
                 * @return vector of enumeration string names and enumeration values
                 */
                static const name_type_vector_t& key_type_pair_vector()
                {
                    using namespace constants;
                    /** This macro maps an enum description to a string/enum pair */
#           define INTEROP_ENUM_DESCRIPTION(X, Y, Z) name_type_pair_t(Y,X)
                    static const name_type_pair_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
#           undef INTEROP_ENUM_DESCRIPTION
                    static const name_type_vector_t tmp(name_types, name_types+util::length_of(name_types));
                    return tmp;
                }
                /** Get the unknown type
                 *
                 * @return unknown type
                 */
                static E unknown(){return constants::UnknownMetricType;}
            };
    }
    /** Convert metric type to metric group
     *
     * @param type metric type
     * @return metric group
     */
    inline constants::metric_group to_group(const constants::metric_type type)
    {
        return constants::enumeration<constants::metric_type, detail::metric_to_group>::to_key(type);
    }
    /** Convert metric type to string description
     *
     * @param type metric type
     * @return string description
     */
    inline const std::string to_description(const constants::metric_type type)
    {
        return constants::enumeration<constants::metric_type, detail::metric_to_description>::to_key(type);
    }
    /** Test if metric type is indexed by DNA base
     *
     * @TODO: replace with above enumeration framework
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
            case PercentBase:
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
