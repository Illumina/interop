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
#include "interop/util/exception_specification.h"
#include "interop/util/lexical_cast.h"

/** Create a formatted message for the given exception
 *
 * @note The .flush() is a workaround for clang
 */
#define INTEROP_THROW(EXCEPTION, MESSAGE) \
        throw EXCEPTION ( static_cast<std::ostringstream&>(std::ostringstream().flush() << MESSAGE << "\n" << __FILE__<< "::" \
            << __FUNCTION__<< " (" << __LINE__ << ")" ).str())

/** Throw exception if value exceeds bounds
 */
#define INTEROP_BOUNDS_CHECK(VALUE, RANGE, MESSAGE) \
    if((VALUE) >= (RANGE)) throw illumina::interop::model::index_out_of_bounds_exception ( \
            static_cast<std::ostringstream&>(std::ostringstream().flush() << MESSAGE << " - " << VALUE << " >= " << RANGE << "\n" << __FILE__ << "::" \
            << __FUNCTION__<< " (" << __LINE__ << ")" ).str())

/** Throw exception if value exceeds bounds
 */
#define INTEROP_RANGE_CHECK(VALUE, RANGE, EXCEPTION, MESSAGE) \
    if((VALUE) >= (RANGE)) throw EXCEPTION ( \
            static_cast<std::ostringstream&>(std::ostringstream().flush() << MESSAGE << " - " << VALUE << " >= " << RANGE << "\n" << __FILE__ << "::" \
            << __FUNCTION__<< " (" << __LINE__ << ")" ).str())

/** Throw exception if value exceeds bounds
 */
#define INTEROP_RANGE_CHECK_GT(VALUE, RANGE, EXCEPTION, MESSAGE) \
    if((VALUE) > (RANGE)) throw EXCEPTION ( \
            static_cast<std::ostringstream&>(std::ostringstream().flush() << MESSAGE << " - " << VALUE << " > " << RANGE << "\n" << __FILE__ << "::" \
            << __FUNCTION__<< " (" << __LINE__ << ")" ).str())


/** Throw exception if value exceeds bounds
 */
#define INTEROP_RANGE_CHECK_NE(VALUE, RANGE, EXCEPTION, MESSAGE) \
    if((VALUE) > (RANGE)) throw EXCEPTION ( \
            static_cast<std::ostringstream&>(std::ostringstream().flush() << MESSAGE << " - " << VALUE << " != " << RANGE << "\n" << __FILE__ << "::" \
            << __FUNCTION__<< " (" << __LINE__ << ")" ).str())

