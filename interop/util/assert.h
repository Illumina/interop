/** Assert macros
 *
 * This header file provides enhanced assert macros to make debugging easier.
 *
 *  @file
 *  @date 10/28/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#if !defined(NDEBUG) || defined(DEBUG) || defined(_DEBUG)
#include <iostream>
#include <cstdlib>

/** Assert if test is true
 *
 * If the test fails, then print the file, function and number number along with the test and call abort.
 * @note Do nothing in release mode.
 */
#define INTEROP_ASSERT(TST)	\
    if(!(TST)) {\
        std::cerr << __FILE__<< "::" << __FUNCTION__<< " (" << __LINE__ << ")" << "  " #TST << std::endl;\
        std::abort();\
    }
/** Assert if test is true
 *
 * If the test fails, then print the file, function and number number along with the test and a
 * message; then call abort.
 * @note Do nothing in release mode.
 */
#define INTEROP_ASSERTMSG(TST, MSG)	\
    if(!(TST)) {\
        std::cerr << __FILE__<< "::" << __FUNCTION__<< " (" << __LINE__ << ")" << "  " #TST << "  " << MSG << std::endl;\
        std::abort();\
    }

#else

/** Assert if test is true
 *
 * If the test fails, then print the file, function and number number along with the test and call abort.
 * @note Do nothing in release mode.
 */
#define INTEROP_ASSERT(TST)	(void)0;
/** Assert if test is true
 *
 * If the test fails, then print the file, function and number number along with the test and a
 * message; then call abort.
 * @note Do nothing in release mode.
 */
#define INTEROP_ASSERTMSG(TST,MSG)  (void)0;

#endif

