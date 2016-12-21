/** Generic layout defined for a specific metric and version
 *
 * This file defines a generic layout for all metric formats.  This class will be specialized for a specific InterOp
 * interface class.
 *
 *  @file
 *  @date 8/15/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <ios>

namespace illumina { namespace interop { namespace io
{
    /** A POD-struct that defines the layout of a binary InterOp record
     *
     * The MetricType is the class that holds the actual record data.
     * The Version is the version of the format.
     */
    template<class MetricType, int Version>
    struct generic_layout;

    /** Define a text layout of the metric
     *
     * This is currently only used for writing metrics
     */
    template<class MetricType, int Version>
    struct text_layout;


}}}


