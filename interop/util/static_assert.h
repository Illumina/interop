/** Back port of static assert
 *
 * This is a compile time check for debugging purposes.
 *
 *  @file
 *  @date 9/26/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#if (!defined(__cplusplus) || __cplusplus < 201103L) && (!defined(_MSC_VER) || _MSC_VER < 1600)
    #if !defined(_LIBCPP_HAS_NO_STATIC_ASSERT) && !defined(static_assert)
        #undef static_assert
        /** Define a broken static_assert_ class for the false condition */
        template <bool> struct static_assert_;
        /** Define a working static_assert_ class for the true condition */
        template <> struct static_assert_<true> {};
        /** Test condition and print message
         *
         * @param x condition
         * @param m message
         */
        #define  static_assert(x, m) static_assert_<(x)>()
    #endif
#endif


