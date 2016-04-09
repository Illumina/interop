/** Exception specification
 *
 * The header provides exceptions for specific errors that may be raised while reading a binary InterOp file.
 *
 * @todo Expand and use this funcationality
 *  @file
 *  @date 4/6/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#ifndef _INTEROP_NO_EXCEPTION_SPECIFICATION
#define _INTEROP_HELPER(X) X
#define _INTEROP_THROWS(X) throw(X)
#define _INTEROP_THROWS2(X, Y) throw(X, Y)
#else
#define _INTEROP_THROWS(X)
#define _INTEROP_THROWS2(X, Y)
#endif
