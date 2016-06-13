/** Exception utilities
 *
 * The header provides a macro to throw more informative exceptions
 *
 *  @file
 *  @date 6/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/lexical_cast.h"

#define INTEROP_THROW(EXCEPTION, MESSAGE) \
        throw EXCEPTION ( static_cast<std::ostringstream&>(std::ostringstream() << MESSAGE << "\n" << __FILE__<< "::" \
            << __FUNCTION__<< " (" << __LINE__ << ")" ).str())