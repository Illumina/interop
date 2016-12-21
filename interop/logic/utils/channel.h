/** Channel order logic
 *
 *  @file
 *  @date  3/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4290)// MSVC warns that it ignores the exception specification.
#endif

#include <string>
#include <cctype>
#include <algorithm>
#include <vector>
#include <iterator>
#include "interop/util/assert.h"
#include "interop/util/exception.h"
#include "interop/constants/enums.h"
#include "interop/model/model_exceptions.h"

namespace illumina { namespace interop { namespace logic { namespace utils
{

    /** Normalize a channel name by making it lower case
     *
     * @param channel channel name
     * @return lowercase channel name
     */
    inline std::string normalize(const std::string &channel)
    {
        std::string channel_normalized=channel;
        std::transform(channel.begin(), channel.end(), channel_normalized.begin(), ::tolower);
        return channel_normalized;
    }
    /** Normalize a collection of channel names
     *
     * @param beg iterator to start of collection
     * @param end iterator to end of collection
     * @param out output iterator
     */
    template<class I, class O>
    void normalize(I beg, I end, O out)
    {
        for(;beg != end;++beg, ++out) *out = normalize(*beg);
    }
    /** Join a collection of strings separated by a token into a string
     *
     * @param beg iterator to start of collection
     * @param end iterator to end of collection
     * @param token string separator token
     * @return string of joined values
     */
    template<class I>
    std::string join(I beg, I end, const std::string& token)
    {
        std::string joined;
        if(beg != end)
        {
            joined = *beg;
            ++beg;
        }
        for(;beg != end;++beg) joined += token + *beg;
        return joined;
    }
    /** Join a vector of strings separated by a token into a string
     *
     * @param values vector of strings
     * @param token string separator token
     * @return string of joined values
     */
    inline std::string join(const std::vector<std::string>& values, const std::string& token)
    {
        return join(values.begin(), values.end(), token);
    }
    /** Expected channel order
     *
     * @param beg iterator to start of collection
     * @param end iterator to end of collection
     * @return string vector of expected channels
     */
    template<class I>
    std::vector<std::string> expected_order(I beg, I end) throw( model::invalid_channel_exception )
    {
        std::vector<std::string> expected;
        expected.reserve(std::distance(beg, end));
        normalize(beg, end, std::back_inserter(expected));
        std::stable_sort(expected.begin(), expected.end());
        std::string norm = join(expected.begin(), expected.end(), ",");
        if(norm == "a,c,g,t") return expected;
        if(norm == "green,red")
        {
            std::swap(expected[0], expected[1]);
            return expected;
        }
        INTEROP_THROW( model::invalid_channel_exception, "Invalid channel names: " << norm);
    }

    /** Expected channel order
     *
     * @param channels vector of string channel names
     * @return string vector of expected channels
     */
    inline std::vector<std::string> expected_order(const std::vector<std::string>& channels)
    {
        return expected_order(channels.begin(), channels.end());
    }
    /** Create a mapping of indexes from the expected order to the actual channel order
     *
     * @param channels vector of string channel names
     * @param map vector of indexes mapping the expected to actual order
     */
    inline void expected2actual(const std::vector<std::string>& channels, std::vector<size_t>& map)
    {
        std::vector<std::string> normed;
        normed.reserve(channels.size());
        normalize(channels.begin(), channels.end(), std::back_inserter(normed));
        std::vector<std::string> expected = expected_order(normed);
        map.resize(normed.size());
        INTEROP_ASSERTMSG(expected.size() == normed.size(), expected.size() << " == " << normed.size());
        for(size_t i=0;i<normed.size();++i)
        {
            INTEROP_ASSERT(i < map.size());
            map[i] = std::distance(normed.begin(), std::find(normed.begin(), normed.end(), expected[i]));
        }
    }

    /** Create a mapping of indexes from the actual order to the expected channel order
     *
     * @param channels vector of string channel names
     * @param map vector of indexes mapping the actual to expected order
     */
    inline void actual2expected(const std::vector<std::string>& channels, std::vector<size_t>& map)
    {
        std::vector<std::string> normed;
        normed.reserve(channels.size());
        normalize(channels.begin(), channels.end(), std::back_inserter(normed));
        std::vector<std::string> expected = expected_order(normed);
        map.resize(normed.size());
        INTEROP_ASSERTMSG(expected.size() == normed.size(), expected.size() << " == " << normed.size());
        for(size_t i=0;i<normed.size();++i)
        {
            map[i] = std::distance(expected.begin(), std::find(expected.begin(), expected.end(), normed[i]));
        }
    }

    /** Create a mapping of indexes from the expected order to the actual channel order
     *
     * @param channels vector of string channel names
     * @return vector of indexes mapping the expected to actual order
     */
    inline std::vector<size_t> expected2actual_map(const std::vector<std::string>& channels)
    {
        std::vector<size_t> map;
        expected2actual(channels, map);
        return map;
    }
    /** Create a mapping of indexes from the actual order to the expected channel order
     *
     * @param channels vector of string channel names
     * @return vector of indexes mapping the actual to expected order
     */
    inline std::vector<size_t> actual2expected_map(const std::vector<std::string>& channels)
    {
        std::vector<size_t> map;
        actual2expected(channels, map);
        return map;
    }
    /** Update channels from instrument type
     *
     * @param instrument enum type of instrument
     * @param channels destination vector for channels
     */
    inline void update_channel_from_instrument_type(const constants::instrument_type instrument,
                                                    std::vector<std::string>& channels)
    {
        switch(instrument)
        {
            case constants::MiniSeq:
            case constants::NextSeq:
                channels.clear();
                channels.reserve(2);
                channels.push_back("Red");
                channels.push_back("Green");
                break;
            case constants::HiSeq:
            case constants::MiSeq:
            case constants::HiScan:
                channels.clear();
                channels.reserve(4);
                channels.push_back("A");
                channels.push_back("C");
                channels.push_back("G");
                channels.push_back("T");
                break;
            default:
                break;
        };
    }
    /** Update channels from instrument type
     *
     * @param instrument enum type of instrument
     * @return vector of channel names
     */
    inline std::vector<std::string> update_channel_from_instrument_type(const constants::instrument_type instrument)
    {
        std::vector<std::string> channels;
        update_channel_from_instrument_type(instrument, channels);
        return channels;
    }


}}}}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

