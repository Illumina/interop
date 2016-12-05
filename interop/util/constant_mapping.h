/** Map a constant of one type to another type
 *
 * This is designed to simplify mapping between enums, enums to strings or strings built using define statements.
 *
 *  @file
 *  @date 8/9/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once

#include <vector>
#include <functional>
#include "interop/util/map.h"
#include "interop/util/length_of.h"

namespace illumina { namespace interop { namespace util
{

    namespace detail
    {
        /** Convert enum to proper hash
         */
        struct enum_hash
        {
            /** Convert enum to hash
             *
             * @param val enum value
             * @return hash value
             */
            template<typename T>
            std::size_t operator()(const T val)const
            {
                return static_cast<std::size_t>(val);
            }
#if defined(__cplusplus) && __cplusplus >= 201103L
            /** Convert enum to hash
             *
             * @param val enum value
             * @return hash value
             */
            std::size_t operator()(const std::string& val)const
            {
                return std::hash<std::string>{}(val);
            }
#endif
        };
    }

    /** A singleton that maps a key to a value
     */
    template<class Key, class Value>
    class constant_mapping
    {
        typedef INTEROP_UNORDERED_HASHMAP(Key, Value, detail::enum_hash) constant_map_t;
    private:
        constant_mapping(const std::pair<Key, Value>* pairs, size_t n)
        {
            for (size_t i = 0; i < n; ++i)
            {
                m_mapping.insert(std::make_pair(pairs[i].first, pairs[i].second));
            }
        }
        constant_mapping(const std::pair<Value, Key>* pairs, size_t n)
        {
            for (size_t i = 0; i < n; ++i)
            {
                m_mapping.insert(std::make_pair(pairs[i].second, pairs[i].first));
            }
        }

    public:
        /** Get instance of singleton
         *
         * @param pairs array of values mapping Key to Value
         * @param pair_count number of pairs in array
         * @return instance of singleton
         */
        static const constant_mapping<Key, Value> &fmapping(const std::pair<Key, Value>* pairs, size_t pair_count)
        {
            static constant_mapping<Key, Value> singleton(pairs, pair_count); // Only called once
            return singleton;
        }
        /** Get instance of singleton
         *
         * @note This reverse the input pair to setup a reverse mapping
         * @param pairs array of values mapping Key to Value
         * @param pair_count number of pairs in array
         * @return instance of singleton
         */
        static const constant_mapping<Key, Value> &rmapping(const std::pair<Value, Key>* pairs, size_t pair_count)
        {
            static constant_mapping<Key, Value> singleton(pairs, pair_count); // Only called once
            return singleton;
        }
        /** Get the value corresponding to the key or default_value if none exists
         *
         * @param key key to search for in mapping
         * @param default_value value to return if the key is not found
         * @return value found (or default_value)
         */
        const Value &get(const Key &key, const Value &default_value) const
        {
            typename constant_map_t::const_iterator it = m_mapping.find(key);
            if (it == m_mapping.end()) return default_value;
            return it->second;
        }

    private:
        constant_map_t m_mapping;
    };

    /** Get the value corresponding to the key or default_value if none exists
     *
     * @param pairs array of values mapping Key to Value
     * @param key key to search for in mapping
     * @param default_value value to return if the key is not found
     * @return value found (or default_value)
     */
    template<class Key, class Value, size_t N>
    const Value &constant_mapping_get(const std::pair<Key, Value> (&pairs)[N], const Key &key,
                                      const Value &default_value)
    {
        return constant_mapping<Key, Value>::fmapping(pairs, N).get(key, default_value);
    }

}}}

