/** Define generic ordered and unordered maps for both C++11 and C++98
 *
 * @todo include this everywhere isnan is used
 *
 *  @file
 *  @date  11/7/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#if defined(__cplusplus) && __cplusplus < 201103L
#   include <map>
#   define INTEROP_ORDERED_MAP(key_t, value_t) std::map<key_t, value_t>
#   define INTEROP_UNORDERED_MAP(key_t, value_t) std::map<key_t, value_t>
#   define INTEROP_UNORDERED_HASHMAP(key_t, value_t, hash_t) std::map<key_t, value_t>
#else
#   include <map>
#   include <unordered_map>
#   define INTEROP_ORDERED_MAP(key_t, value_t) std::map<key_t, value_t>
#   define INTEROP_UNORDERED_MAP(key_t, value_t) std::unordered_map<key_t, value_t>
#   define INTEROP_UNORDERED_HASHMAP(key_t, value_t, hash_t) std::unordered_map<key_t, value_t, hash_t>
#endif
