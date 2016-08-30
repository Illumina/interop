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

#include <map>
#include <vector>
#include "interop/util/length_of.h"

namespace illumina { namespace interop { namespace util
{

    /** A singleton that maps a key to a value
     */
    template<class Key, class Value>
    class constant_mapping
    {
    private:
        constant_mapping(const std::pair<Key, Value>* pairs, size_t n)
        {
            for (size_t i = 0; i < n; ++i) m_mapping[pairs[i].first] = pairs[i].second;
        }
        constant_mapping(const std::pair<Value, Key>* pairs, size_t n)
        {
            for (size_t i = 0; i < n; ++i) m_mapping[pairs[i].second] = pairs[i].first;
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
            typename std::map<Key, Value>::const_iterator it = m_mapping.find(key);
            if (it == m_mapping.end()) return default_value;
            return it->second;
        }

    private:
        std::map<Key, Value> m_mapping;
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
