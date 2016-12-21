/** Ensure self registration classes properly link
 *
 *  @file
 *  @date 12/19/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

/** Ensure that static libraries are properly linked
 *  This must be used in a function that will definitely be linked.
 *
 *  Tested for Microsoft Visual C++, GCC and CLang
 */
#define INTEROP_FORCE_LINK_USE(X) void force_link_metric_format(X*);  force_link_metric_format(0);
/** Ensure that static libraries are properly linked
 *  This must be used in a file that may not be linked.
 *
 *  Tested for Microsoft Visual C++, GCC and CLang
 */
#define INTEROP_FORCE_LINK_DEF(X) namespace illumina{namespace interop{namespace io{ void force_link_metric_format(X*){} }}} \
    void force_link_metric_format(X*){} // For Microsoft Visual C++
