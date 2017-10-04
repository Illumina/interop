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

#ifdef HAVE_UNORDERED_MAP_IN_STD
#   define INTEROP_HAS_UNORDERED_MAP 1
#   include <map>
#   include <unordered_map>
#   define INTEROP_ORDERED_MAP(key_t, value_t) std::map<key_t, value_t>
#   define INTEROP_UNORDERED_MAP(key_t, value_t) std::unordered_map<key_t, value_t>
#   define INTEROP_UNORDERED_HASHMAP(key_t, value_t, hash_t) std::unordered_map<key_t, value_t, hash_t>
#   define INTEROP_CLEAR_MAP(map_ins) offset_map_t empty_map_to_clear;std::swap(map_ins, empty_map_to_clear);
#else
#   include <map>
#   define INTEROP_ORDERED_MAP(key_t, value_t) std::map<key_t, value_t>
#   define INTEROP_UNORDERED_MAP(key_t, value_t) std::map<key_t, value_t>
#   define INTEROP_UNORDERED_HASHMAP(key_t, value_t, hash_t) std::map<key_t, value_t>
#   define INTEROP_CLEAR_MAP(map_ins) map_ins.clear();
#endif
