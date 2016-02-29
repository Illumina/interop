
/** Unit tests for various failure conditions handled by exceptions.
 *
 *
 *  @file
 *
 *  @date 10/6/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127) // MSVC warns about using constants in conditional statements, for template constants
#endif

/** Wrapper for X-Macro FIXTURE
 *
 */
#define WRAPPED_TEST(FIXTURE, FUNC) TYPED_TEST(FIXTURE, FUNC)

/**
 * Confirm binary write matches expected binary data
 */
WRAPPED_TEST(FIXTURE, test_expected_get_metric)
{
    typename TypeParam::metrics_type::key_vector keys = TypeParam::actual_metric_set.keys();
    for(size_t i=0;i<keys.size();i++)
    {
        EXPECT_EQ(keys[i], TypeParam::actual_metric_set.get_metric(keys[i]).id());
    }
}

/**
 * Confirm binary write matches expected binary data
 */
WRAPPED_TEST(FIXTURE, test_write_read_binary_data)
{
    if(TypeParam::disable_binary_data_size) return;
    EXPECT_EQ(TypeParam::expected_binary_data.size(), TypeParam::actual_binary_data.size());
    if(TypeParam::disable_binary_data) return;
    for(::uint32_t i=0;i<std::min(TypeParam::expected_binary_data.size(), TypeParam::actual_binary_data.size());i++)
    {
        //if(TypeParam::expected_binary_data[i] != TypeParam::actual_binary_data[i])
        //    std::cerr << i << " " << int(TypeParam::expected_binary_data[i]) << " != " <<  int(TypeParam::actual_binary_data[i]) << std::endl;
        EXPECT_EQ(int(TypeParam::expected_binary_data[i]), int(TypeParam::actual_binary_data[i]));
        //BOOST_CHECK_MESSAGE(TypeParam::expected_binary_data[i] == TypeParam::actual_binary_data[i], "i=" << i << " - " << int(TypeParam::expected_binary_data[i]) << " == " << int(TypeParam::actual_binary_data[i]) << " - " << TypeParam::VERSION);
    }
}

/** Confirm bad_format_exception is thrown when version is unsupported
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_bad_format_exception)
{
    std::string tmp = std::string(TypeParam::expected_binary_data);
    tmp[0] = 34;
    std::istringstream fin(tmp);
    typename TypeParam::metrics_type metrics;
    EXPECT_THROW(illumina::interop::io::read_metrics(fin, metrics), bad_format_exception);
}

/** Confirm incomplete_file_exception is thrown for a small partial record
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_incomplete_file_exception)
{
    ::uint32_t incomplete = 0;
    for(::uint32_t i=2;i<25;i++)
    {
        std::istringstream fin(TypeParam::expected_binary_data.substr(0, i));
        typename TypeParam::metrics_type metrics;
        try{
            illumina::interop::io::read_metrics(fin, metrics);
        }
        catch(const incomplete_file_exception&){incomplete++;}
        catch(const std::exception& ex){ std::cerr << i << " " << ex.what() << std::endl;throw;}
    }
    EXPECT_TRUE(incomplete>10);
}
/** Confirm incomplete_file_exception is thrown for a mostly complete file
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_incomplete_file_exception_last_metric)
{
    std::istringstream fin(TypeParam::expected_binary_data.substr(0,TypeParam::expected_binary_data.length()-4));
    typename TypeParam::metrics_type metrics;
    EXPECT_THROW(illumina::interop::io::read_metrics(fin, metrics), incomplete_file_exception);
}
/** Confirm bad_format_exception is thrown when record size is incorrect
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_incorrect_record_size)
{
    if(TypeParam::disable_check_record_size) return;
    std::string tmp = std::string(TypeParam::expected_binary_data);
    tmp[1] = 0;
    std::istringstream fin(tmp);
    typename TypeParam::metrics_type metrics;
    EXPECT_THROW(illumina::interop::io::read_metrics(fin, metrics), bad_format_exception);
}
/** Confirm file_not_found_exception is thrown when a file is not found
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_file_not_found)
{
    typename TypeParam::metrics_type metrics;
    EXPECT_THROW(read_interop("/NO/FILE/EXISTS", metrics), file_not_found_exception);
}
/** Confirm reading from good data does not throw an exception
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_read)
{
    std::string tmp = std::string(TypeParam::expected_binary_data);
    std::istringstream fin(tmp);
    typename TypeParam::metrics_type metrics;
    EXPECT_NO_THROW(illumina::interop::io::read_metrics(fin, metrics));
}
/** Confirm the clear function works
 */
WRAPPED_TEST(FIXTURE, test_clear)
{
    std::string tmp = std::string(TypeParam::expected_binary_data);
    std::istringstream fin(tmp);
    typename TypeParam::metrics_type metrics;
    EXPECT_NO_THROW(illumina::interop::io::read_metrics(fin, metrics));
    size_t cnt = metrics.size();
    metrics.clear();
    std::istringstream fin2(tmp);
    EXPECT_NO_THROW(illumina::interop::io::read_metrics(fin2, metrics));
    EXPECT_EQ(cnt, metrics.size());

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif