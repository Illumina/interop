/** Common typedefs used throughout InterOp
 *
 *  @file
 *  @date 4/28/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include "interop/util/type_traits.h"
#include "interop/constants/enums.h"


namespace illumina { namespace interop { namespace constants
{

    /** Define base type for tile metrics */
    typedef constant_type<metric_base_type, BaseTileType> base_tile_t;
    /** Define base type for cycle metrics */
    typedef constant_type<metric_base_type, BaseCycleType> base_cycle_t;
    /** Define base type for read metrics */
    typedef constant_type<metric_base_type, BaseReadType> base_read_t;
    /** Define base type for lane metrics */
    typedef constant_type<metric_base_type, BaseLaneType> base_lane_t;

}}}

