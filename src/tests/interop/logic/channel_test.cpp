/** Unit tests for channel logic
 *
 *  @file
 *  @date  9/10/2016
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/logic/utils/channel.h"
#include "interop/logic/utils/metric_type_ext.h"


using namespace illumina::interop;

/** Convert an array to a vector
 *
 * Determines the length of the stack array automatically.
 *
 * @param vals array pointer
 * @return vector of values
 */
template<typename T, size_t N>
inline std::vector<T> to_vector(const T (&vals)[N])
{
    return std::vector<T>(vals, vals + N);
}

TEST(channel_test, normalize)
{
    EXPECT_EQ(logic::utils::normalize("A"), "a");
    EXPECT_EQ(logic::utils::normalize("a"), "a");
    EXPECT_EQ(logic::utils::normalize("C"), "c");
    EXPECT_EQ(logic::utils::normalize("c"), "c");
    EXPECT_EQ(logic::utils::normalize("G"), "g");
    EXPECT_EQ(logic::utils::normalize("g"), "g");
    EXPECT_EQ(logic::utils::normalize("T"), "t");
    EXPECT_EQ(logic::utils::normalize("t"), "t");
    EXPECT_EQ(logic::utils::normalize("1"), "1");
    EXPECT_EQ(logic::utils::normalize("2"), "2");
    EXPECT_EQ(logic::utils::normalize(" "), " ");
    EXPECT_EQ(logic::utils::normalize(""), "");
    EXPECT_EQ(logic::utils::normalize("RED"), "red");
    EXPECT_EQ(logic::utils::normalize("gReeN"), "green");
    EXPECT_NE(logic::utils::normalize("A"), "c");
}

//Standard test, 4 channel system in different order
TEST(channel_test, expected2actual_4channel_valid)
{
    const std::string channel_array[] = {"C", "G", "T", "A"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    logic::utils::expected2actual(channels, map);

    EXPECT_EQ((size_t) map.size(), (size_t) 4);
    EXPECT_EQ(map[0], (size_t) 3);
    EXPECT_EQ(map[1], (size_t )0);
    EXPECT_EQ(map[2], (size_t )1);
    EXPECT_EQ(map[3], (size_t )2);
}

//Standard test, 2 channel system in different order
TEST(channel_test, expected2actual_2channel_valid)
{
    const std::string channel_array[] = {"Red", "Green"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    logic::utils::expected2actual(channels, map);

    EXPECT_EQ((size_t) map.size(), (size_t) 2);
    EXPECT_EQ(map[0], (size_t )0);
    EXPECT_EQ(map[1], (size_t )1);
}


//Test 4 - check full normalization of all letters in channel name
TEST(channel_test, expected2actual_valid_4)
{
    const std::string channel_array[] = {"rEd", "GReEn"};
    const std::vector<std::string> channels(to_vector(channel_array));

    std::vector<size_t> map;

    logic::utils::expected2actual(channels, map);

    EXPECT_EQ((size_t) map.size(), (size_t) 2);
    EXPECT_EQ(map[0], (size_t )0);
    EXPECT_EQ(map[1], (size_t )1);
}

//INVALID - bogus channel names
TEST(channel_test, expected2actual_invalid_channel_1)
{
    const std::string channel_array[] = {"Boy", "Girl"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - valid channel names, but invalid channel count
TEST(channel_test, expected2actual_invalid_channel_2)
{
    const std::string channel_array[] = {"A", "C", "G"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - space string instead of valid channel name
TEST(channel_test, expected2actual_invalid_channel_3)
{
    const std::string channel_array[] = {"A", "C", "G", " "};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - empty string for channel
TEST(channel_test, expected2actual_invalid_channel_4)
{
    const std::string channel_array[] = {"A", "C", "G", ""};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - mixing ACGT system with 1/2 system
TEST(channel_test, expected2actual_invalid_channel_5)
{
    const std::string channel_array[] = {"A", "C", "G", "1"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - invalid channel name that starts with valid channel name (T -> TT)
TEST(channel_test, expected2actual_invalid_channel_6)
{
    const std::string channel_array[] = {"A", "C", "G", "TT"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - valid channel names but these two cannot exist by themselves
TEST(channel_test, expected2actual_invalid_channel_7)
{
    const std::string channel_array[] = {"A", "t"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - duplicate channel name with original duplicate not quite being duplicate
TEST(channel_test, expected2actual_invalid_channel_8)
{
    const std::string channel_array[] = {"red", "green", "Red"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - no channels
TEST(channel_test, expected2actual_invalid_channel_9)
{
    const std::vector<std::string> channels;
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - 1 channel
TEST(channel_test, expected2actual_invalid_channel_10)
{
    const std::string channel_array[] = {"1"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//INVALID - 1 valid channel with a duplicate making it invalid
TEST(channel_test, expected2actual_invalid_channel_11)
{
    const std::string channel_array[] = {"1", "1"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::expected2actual(channels, map), model::invalid_channel_exception);
}

//Standard test, 4 channel system in different order
TEST(channel_test, actual2expected_4channel_valid)
{
    const std::string channel_array[] = {"C", "G", "T", "A"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    logic::utils::actual2expected(channels, map);

    EXPECT_EQ((size_t) map.size(), (size_t) 4);
    EXPECT_EQ(map[0], (size_t) 1);
    EXPECT_EQ(map[1], (size_t )2);
    EXPECT_EQ(map[2], (size_t )3);
    EXPECT_EQ(map[3], (size_t )0);
}

//Standard test, 2 channel system in different order
TEST(channel_test, actual2expected_2channel_valid)
{
    const std::string channel_array[] = {"red", "green"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    logic::utils::actual2expected(channels, map);

    EXPECT_EQ((size_t) map.size(), (size_t) 2);
    EXPECT_EQ(map[0], (size_t )0);
    EXPECT_EQ(map[1], (size_t )1);
}



//Test 4 - check full normalization of all letters in channel name
TEST(channel_test, actual2expected_valid_4)
{
    const std::string channel_array[] = {"rEd", "GReEn"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    logic::utils::actual2expected(channels, map);

    EXPECT_EQ((size_t) map.size(), (size_t) 2);
    EXPECT_EQ(map[0], (size_t )0);
    EXPECT_EQ(map[1], (size_t )1);
}

//INVALID - bogus channel names
TEST(channel_test, actual2expected_invalid_channel_1)
{
    const std::string channel_array[] = {"Boy", "Girl"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - valid channel names, but invalid channel count
TEST(channel_test, actual2expected_invalid_channel_2)
{
    const std::string channel_array[] = {"A", "C", "G"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - space string instead of valid channel name
TEST(channel_test, actual2expected_invalid_channel_3)
{
    const std::string channel_array[] = {"A", "C", "G", " "};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - empty string for channel
TEST(channel_test, actual2expected_invalid_channel_4)
{
    const std::string channel_array[] = {"A", "C", "G", ""};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - mixing ACGT system with 1/2 system
TEST(channel_test, actual2expected_invalid_channel_5)
{
    const std::string channel_array[] = {"A", "C", "G", "1"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - invalid channel name that starts with valid channel name (T -> TT)
TEST(channel_test, actual2expected_invalid_channel_6)
{
    const std::string channel_array[] = {"A", "C", "G", "TT"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - valid channel names but these two cannot exist by themselves
TEST(channel_test, actual2expected_invalid_channel_7)
{
    const std::string channel_array[] = {"A", "t"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - duplicate channel name with original duplicate not quite being duplicate
TEST(channel_test, actual2expected_invalid_channel_8)
{
    const std::string channel_array[] = {"rEd", "GreEn", "Red"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - no channels
TEST(channel_test, actual2expected_invalid_channel_9)
{
    const std::vector<std::string> channels;
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - 1 channel
TEST(channel_test, actual2expected_invalid_channel_10)
{
    const std::string channel_array[] = {"1"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//INVALID - 1 valid channel with a duplicate making it invalid
TEST(channel_test, actual2expected_invalid_channel_11)
{
    const std::string channel_array[] = {"1", "1"};
    const std::vector<std::string> channels(to_vector(channel_array));
    std::vector<size_t> map;

    EXPECT_THROW(logic::utils::actual2expected(channels, map), model::invalid_channel_exception);
}

//Test that getting different channels, including edge case channel names, gives the right size for channel array
TEST(channel_test, update_channel_from_instrument)
{
    std::vector<std::string> channels;
    channels = logic::utils::update_channel_from_instrument_type(constants::NextSeq);
    EXPECT_EQ(channels.size(), (size_t) 2);

    std::vector<std::string> channels_2;
    channels_2 = logic::utils::update_channel_from_instrument_type(constants::UnknownInstrument);
    EXPECT_EQ(channels_2.size(), (size_t) 0);

    std::vector<std::string> channels_3;
    channels_3 = logic::utils::update_channel_from_instrument_type(constants::InstrumentCount);
    EXPECT_EQ(channels_3.size(), (size_t) 0);

    std::vector<std::string> channels_4;
    channels_4 = logic::utils::update_channel_from_instrument_type(constants::HiSeq);
    EXPECT_EQ(channels_4.size(), (size_t) 4);

    std::vector<std::string> channels_5;
    channels_5 = logic::utils::update_channel_from_instrument_type(constants::HiScan);
    EXPECT_EQ(channels_5.size(), (size_t) 4);
}
