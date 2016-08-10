/** Unit tests for various failure conditions handled by exceptions.
 *
 *
 *  @file
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
#define WRAPPED_TEST(FIXTURE, FUNC) TEST_P(FIXTURE, FUNC)


/**
 * Confirm binary write matches expected binary data
 */
WRAPPED_TEST(FIXTURE, test_write_read_binary_data)
{
#   ifdef INTEROP_BINARY_DATA_SIZE_CHECK
        return;
#   endif
    EXPECT_EQ(expected.size(), actual.size());
#   ifdef INTEROP_BINARY_DATA_CHECK
        return;
#   endif
    for(::uint32_t i=0;i<std::min(expected.size(), actual.size());i++)
    {
        EXPECT_EQ(int(expected[i]), int(actual[i]));
    }
}

/** Confirm bad_format_exception is thrown when version is unsupported
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_bad_format_exception)
{
    std::string tmp = std::string(expected);
    tmp[0] = 34;
    std::istringstream fin(tmp);
    metric_set_t metrics;
    EXPECT_THROW(illumina::interop::io::read_metrics(fin, metrics), bad_format_exception);
}

/** Confirm incomplete_file_exception is thrown for a small partial record
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_incomplete_file_exception)
{
    ::uint32_t incomplete = 0;
    for(::uint32_t i=2;i<25;i++)
    {
        std::istringstream fin(expected.substr(0, i));
        metric_set_t metrics;
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
    std::istringstream fin(expected.substr(0,expected.length()-4));
    metric_set_t metrics;
    EXPECT_THROW(illumina::interop::io::read_metrics(fin, metrics), incomplete_file_exception);
}
/** Confirm bad_format_exception is thrown when record size is incorrect
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_incorrect_record_size)
{
#   ifdef INTEROP_SKIP_RECORD_SIZE_CHECK
        return;
#   endif
    std::string tmp = std::string(expected);
    tmp[1] = 0;
    std::istringstream fin(tmp);
    metric_set_t metrics;
    EXPECT_THROW(illumina::interop::io::read_metrics(fin, metrics), bad_format_exception);
}
/** Confirm file_not_found_exception is thrown when a file is not found
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_file_not_found)
{
    metric_set_t metrics;
    EXPECT_THROW(illumina::interop::io::read_interop("/NO/FILE/EXISTS", metrics), file_not_found_exception);
}
/** Confirm reading from good data does not throw an exception
 */
WRAPPED_TEST(FIXTURE, test_hardcoded_read)
{
    std::string tmp = std::string(expected);
    std::istringstream fin(tmp);
    metric_set_t metrics;
    EXPECT_NO_THROW(illumina::interop::io::read_metrics(fin, metrics));
}
/** Confirm the clear function works
 */
WRAPPED_TEST(FIXTURE, test_clear)
{
    std::string tmp = std::string(expected);
    std::istringstream fin(tmp);
    metric_set_t metrics;
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
