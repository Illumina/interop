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
#include "interop/constants/enum_description.h"
#include "interop/logic/utils/enums.h"

namespace illumina { namespace interop {  namespace logic { namespace utils
{

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
#       define INTEROP_TUPLE4(Metric, Description, Group, Feature) mapped_t(Metric,Group)
        static const mapped_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
#       undef INTEROP_TUPLE4
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
#       define INTEROP_TUPLE4(Metric, Description, Group, Feature) mapped_t(Metric,Description)
        static const mapped_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
#       undef INTEROP_TUPLE4
        return util::constant_mapping_get(name_types, type, std::string("UnknownDescription"));
    }
    /** Convert metric type to string description
     *
     * @param types destination vector of metric types
     */
    inline void list_descriptions(std::vector< constants::enum_description< constants::metric_type> >& types )
    {
        using namespace constants;
        // TODO: This can be reduced to a single macro define
        typedef std::pair<metric_type, std::string > mapped_t;
#       define INTEROP_TUPLE4(Metric, Description, Group, Feature) mapped_t(Metric,Description)
        static const mapped_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
#       undef INTEROP_TUPLE4
        types.assign(name_types, name_types+util::length_of(name_types));
    }
    /** Convert metric type to metric group
     *
     * @param type metric type
     * @return metric feature type
     */
    inline constants::metric_feature_type to_feature(const constants::metric_type type)
    {
        using namespace constants;
        // TODO: This can be reduced to a single macro define
        typedef std::pair<metric_type, metric_feature_type > mapped_t;
#       define INTEROP_TUPLE4(Metric, Description, Group, Feature) mapped_t(Metric,static_cast<metric_feature_type>(Feature))
        static const mapped_t name_types[] = {INTEROP_ENUM_METRIC_TYPES};
#       undef INTEROP_TUPLE4
        return util::constant_mapping_get(name_types, type, UnknownMetricFeature);
    }

    /** Test if metric type is indexed by DNA base
     *
     * @param type metric type
     * @return true if metric can be indexed by DNA base
     */
    inline bool is_base_metric(const constants::metric_type type)
    {
        return (to_feature(type) & constants::BaseFeature) == constants::BaseFeature;
    }

    /** Test if metric type is indexed by channel
     *
     * @param type metric type
     * @return true if metric can be indexed by channel
     */
    inline bool is_channel_metric(const constants::metric_type type)
    {
        return (to_feature(type) & constants::ChannelFeature) == constants::ChannelFeature;
    }

    /** Test if metric type is produced every read
     *
     * @param type metric type
     * @return true if metric is produced every read
     */
    inline bool is_read_metric(const constants::metric_type type)
    {
        return (to_feature(type) & constants::ReadFeature) == constants::ReadFeature;
    }

    /** Test if metric type is produced every cycle
     *
     * @param type metric type
     * @return true if metric is produced every cycle
     */
    inline bool is_cycle_metric(const constants::metric_type type)
    {
        return (to_feature(type) & constants::CycleFeature) == constants::CycleFeature;
    }

    /** Test if metric type is produced for a tile
     *
     * @param type metric type
     * @return true if metric is produced for every tile
     */
    inline bool is_tile_metric(const constants::metric_type type)
    {
        return (to_feature(type) & constants::TileFeature) == constants::TileFeature;
    }

}}}}

