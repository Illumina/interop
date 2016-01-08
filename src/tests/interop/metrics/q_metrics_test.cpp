/** Unit tests for the q-score metrics
 *
 *
 *  @file
 *
 *  @date 8/25/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <limits>
#include <fstream>
#include <gtest/gtest.h>
#include "interop/model/metric_sets/q_metric_set.h"
#include "src/tests/interop/metrics/metric_test_utils.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;


namespace illumina{ namespace interop { namespace unittest {
/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * Regression set: 1947950_117213Bin2R0I
 *
 * @note Version 4
 */
struct q_metrics_hardcoded_fixture_v4 : util::fixture_helper<q_metrics, 4>
{
    /** Setup fixture */
    q_metrics_hardcoded_fixture_v4()
    {
        typedef metric_type::uint_t uint_t;
        typedef sparse_value<uint_t, q_metrics::MAX_Q_BINS> q_val;
        typedef header_type::qscore_bin_vector_type qscore_bin_vector_type;

        qscore_bin_vector_type headervec;
        header_type header(headervec);

        q_val hist1[] = {q_val(14,21208), q_val(21,8227), q_val(26,73051), q_val(32,2339486)};
        q_val hist2[] = {q_val(14,22647), q_val(21,9570), q_val(26,81839), q_val(32,2413227)};
        q_val hist3[] = {q_val(14,18878), q_val(21,8168), q_val(26,72634), q_val(32,2342292)};
        //std::vector<uint_t> hist_tmp(50, 0);

        expected_metrics.push_back(metric_type(1, 1104, 1, to_vector(hist1)));
        expected_metrics.push_back(metric_type(1, 1106, 1, to_vector(hist2)));
        expected_metrics.push_back(metric_type(1, 1104, 2, to_vector(hist3)));

        int tmp[] = {
                4,206,
                1,0,80,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,216,82,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,91
                ,29,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,158,178,35,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                1,0,82,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,119,88,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,98,37,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,175,63,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,171,210,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
                1,0,80,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,190,73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,232,31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,186,27,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,148,189,35,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0
        };
        setup_hardcoded_binary(tmp, header);
    }
};

/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * Regression set: 8612619_11864Bin1R2I
 *
 * @note Version 5
 */
struct q_metrics_hardcoded_fixture_v5 : util::fixture_helper<q_metrics, 5>
{
    /** Setup fixture */
    q_metrics_hardcoded_fixture_v5()
    {
        typedef q_score_bin::bin_type ushort_t;
        typedef metric_type::uint_t uint_t;
        typedef sparse_value<uint_t, 7> q_val;
        typedef header_type::qscore_bin_vector_type qscore_bin_vector_type;
        const uint_t bin_count = 7;

        ushort_t lower[] = {1, 10, 20, 25, 30, 35, 40};
        ushort_t upper[] = {9, 19, 24, 29, 34, 39, 41};
        ushort_t value[] = {1, 14, 22, 27, 33, 37, 40};
        qscore_bin_vector_type headervec;
        for(uint_t i=0;i<bin_count;i++)
            headervec.push_back(q_score_bin(lower[i], upper[i], value[i]));
        header_type header(headervec);

        q_val hist1[] = {q_val(1,45272), q_val(3,33369), q_val(4,1784241)};
        q_val hist2[] = {q_val(1,45229), q_val(3,34304), q_val(4,1792186)};
        q_val hist3[] = {q_val(1,49152), q_val(3,37440), q_val(4,1806479)};

        expected_metrics.push_back(metric_type(1, 1103, 1, to_vector(hist1)));
        expected_metrics.push_back(metric_type(1, 1104, 1, to_vector(hist2)));
        expected_metrics.push_back(metric_type(1, 1108, 1, to_vector(hist3)));

        int tmp[] = {
                5,206,
                1,7,
                1,10,20,25,30,35,40,9,19,24,29,34,39,41,1,14,22,27,33,37,40,
                1,0,79,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,216,176,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,89,130,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,177,57,27,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,
                1,0,80,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,173,176,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,134,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,186,88,27,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,
                1,0,84,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,192,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,64,146,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,143,144,27,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };
        setup_hardcoded_binary(tmp, header);
    }
};

/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * @note Version 6
 */
struct q_metrics_hardcoded_fixture_v6 : util::fixture_helper<q_metrics, 6>
{
    /** Setup fixture */
    q_metrics_hardcoded_fixture_v6()
    {
        typedef q_score_bin::bin_type ushort_t;
        typedef metric_type::uint_t uint_t;
        typedef header_type::qscore_bin_vector_type qscore_bin_vector_type;
        const uint_t bin_count = 7;

        ushort_t lower[] = {2, 10, 20, 25, 30, 35, 40};
        ushort_t upper[] = {9, 19, 24, 29, 34, 39, 40};
        ushort_t value[] = {2, 14, 21, 27, 32, 36, 40};
        qscore_bin_vector_type headervec;
        for(uint_t i=0;i<bin_count;i++)
            headervec.push_back(q_score_bin(lower[i], upper[i], value[i]));
        header_type header(headervec);

        uint_t hist_all1[] = {0, 267962, 118703, 4284, 2796110, 0, 0};
        uint_t hist_all2[] = {0,241483, 44960, 1100, 2899568, 0 ,0};
        uint_t hist_all3[] = {0,212144, 53942, 427, 2920598, 0, 0};
        std::vector<uint_t> hist_tmp(50, 0);

        expected_metrics.push_back(metric_type(7, 1114, 1, to_vector(hist_all1)));
        expected_metrics.push_back(metric_type(7, 1114, 2, to_vector(hist_all2)));
        expected_metrics.push_back(metric_type(7, 1114, 3,to_vector(hist_all3)));

        int tmp[] = {
                6,34,1,7,2,10,20,25,30,35,40,9,19,24,29,34,39,40,2,14,21,27,32,36,40
                ,7,0,90,4,1,0,0,0,0,0,-70,22,4,0,-81,-49,1,0,-68,16,0,0,78,-86,42,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,2,0,0,0,0,0,75,-81,3,0,-96,-81,0,0,76,4,0,0,112,62,44,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,3,0,0,0,0,0,-80,60,3,0,-74,-46,0,0,-85,1,0,0,-106,-112,44,0,0,0,0,0,0,0,0,0
        };
        setup_hardcoded_binary(tmp, header);
    }
};

/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 4
 */
struct q_metrics_write_read_fixture_v4 : util::fixture_helper<q_metrics, 4>
{
    /** Setup fixture */
    q_metrics_write_read_fixture_v4()
    {
        typedef metric_type::uint_t uint_t;
        typedef sparse_value<uint_t, q_metrics::MAX_Q_BINS> q_val;
        typedef header_type::qscore_bin_vector_type qscore_bin_vector_type;

        qscore_bin_vector_type headervec;
        header_type header(headervec);

        q_val hist1[] = {q_val(14,21208), q_val(21,8227), q_val(26,73051), q_val(32,2339486)};
        q_val hist2[] = {q_val(14,22647), q_val(21,9570), q_val(26,81839), q_val(32,2413227)};
        q_val hist3[] = {q_val(14,18878), q_val(21,8168), q_val(26,72634), q_val(32,2342292)};
        //std::vector<uint_t> hist_tmp(50, 0);

        expected_metrics.push_back(metric_type(1, 1104, 1, to_vector(hist1)));
        expected_metrics.push_back(metric_type(1, 1106, 1, to_vector(hist2)));
        expected_metrics.push_back(metric_type(1, 1104, 2, to_vector(hist3)));

        setup_write_read(header);
    }
};

/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 5
 */
struct q_metrics_write_read_fixture_v5 : util::fixture_helper<q_metrics, 5>
{
    /** Setup fixture */
    q_metrics_write_read_fixture_v5()
    {
        typedef q_score_bin::bin_type ushort_t;
        typedef metric_type::uint_t uint_t;
        typedef sparse_value<uint_t, 7> q_val;
        typedef header_type::qscore_bin_vector_type qscore_bin_vector_type;
        const uint_t bin_count = 7;

        ushort_t lower[] = {1, 10, 20, 25, 30, 35, 40};
        ushort_t upper[] = {9, 19, 24, 29, 34, 39, 41};
        ushort_t value[] = {1, 14, 22, 27, 33, 37, 40};
        qscore_bin_vector_type headervec;
        for(uint_t i=0;i<bin_count;i++)
            headervec.push_back(q_score_bin(lower[i], upper[i], value[i]));
        header_type header(headervec);

        q_val hist1[] = {q_val(1,45272), q_val(3,33369), q_val(4,1784241)};
        q_val hist2[] = {q_val(1,45229), q_val(3,34304), q_val(4,1792186)};
        q_val hist3[] = {q_val(1,49152), q_val(3,37440), q_val(4,1806479)};

        expected_metrics.push_back(metric_type(1, 1103, 1, to_vector(hist1)));
        expected_metrics.push_back(metric_type(1, 1104, 1, to_vector(hist2)));
        expected_metrics.push_back(metric_type(1, 1108, 1, to_vector(hist3)));

        setup_write_read(header);
    }
};
/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 6
 */
struct q_metrics_write_read_fixture_v6 : util::fixture_helper<q_metrics, 6>
{
    /** Setup fixture */
    q_metrics_write_read_fixture_v6()
    {
        typedef q_score_bin::bin_type ushort_t;
        typedef metric_type::uint_t uint_t;
        typedef header_type::qscore_bin_vector_type qscore_bin_vector_type;
        const uint_t bin_count = 7;

        ushort_t lower[] = {2, 10, 20, 25, 30, 35, 40};
        ushort_t upper[] = {9, 19, 24, 29, 34, 39, 40};
        ushort_t value[] = {2, 14, 21, 27, 32, 36, 40};
        qscore_bin_vector_type headervec;
        for(uint_t i=0;i<bin_count;i++)
            headervec.push_back(q_score_bin(lower[i], upper[i], value[i]));
        header_type header(headervec);

        uint_t hist_all1[] = {0, 267962, 118703, 4284, 2796110, 0, 0};
        uint_t hist_all2[] = {0,241483, 44960, 1100, 2899568, 0 ,0};
        uint_t hist_all3[] = {0,212144, 53942, 427, 2920598, 0, 0};

        expected_metrics.push_back(metric_type(7, 1114, 1, to_vector(hist_all1)));
        expected_metrics.push_back(metric_type(7, 1114, 2, to_vector(hist_all2)));
        expected_metrics.push_back(metric_type(7, 1114, 3, to_vector(hist_all3)));

        setup_write_read(header);
    }
};
/** Interface between fixtures and Google Test */
template<typename TestSetup>
struct q_metrics_test : public ::testing::Test, public TestSetup { };
}}}

using namespace illumina::interop::unittest;


typedef ::testing::Types<
q_metrics_hardcoded_fixture_v4,
q_metrics_hardcoded_fixture_v5,
q_metrics_hardcoded_fixture_v6,
q_metrics_write_read_fixture_v4,
q_metrics_write_read_fixture_v5,
q_metrics_write_read_fixture_v6
> Formats;
TYPED_TEST_CASE(q_metrics_test, Formats);

/**
 * @class illumina::interop::model::metrics::q_metric
 * @test Confirm version 4 of the metric can be written to and read from a stream
 * @test Confirm version 4 of the metric matches known binary file
 * @test Confirm version 5 of the metric can be written to and read from a stream
 * @test Confirm version 5 of the metric matches known binary file
 * @test Confirm version 6 of the metric can be written to and read from a stream
 * @test Confirm version 6 of the metric matches known binary file
 */
TYPED_TEST(q_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metrics.size(), TypeParam::expected_metrics.size());

    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metrics.begin(), itActual = TypeParam::actual_metrics.begin();
        itExpected != TypeParam::expected_metrics.end() && itActual != TypeParam::actual_metrics.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->cycle(), itActual->cycle());
        EXPECT_EQ(itExpected->size(), itActual->size());
        for(size_t i=0;i<std::min(itExpected->size(), itActual->size());i++)
        {
            EXPECT_EQ(itExpected->qscoreHist(i), itActual->qscoreHist(i));
        }
    }
    EXPECT_EQ(TypeParam::actual_metric_set.histBinCount(), TypeParam::expected_metric_set.histBinCount());
    EXPECT_EQ(TypeParam::actual_metric_set.binCount(), TypeParam::expected_metric_set.binCount());
    for(size_t i=0;i<std::min(TypeParam::actual_metric_set.binCount(), TypeParam::expected_metric_set.binCount());i++)
    {
        EXPECT_EQ(TypeParam::actual_metric_set.binAt(i).lower(), TypeParam::expected_metric_set.binAt(i).lower());
        EXPECT_EQ(TypeParam::actual_metric_set.binAt(i).upper(), TypeParam::expected_metric_set.binAt(i).upper());
        EXPECT_EQ(TypeParam::actual_metric_set.binAt(i).value(), TypeParam::expected_metric_set.binAt(i).value());
    }
}
/**
 * @class illumina::interop::model::metrics::q_metrics
 * @test Confirm populate does not crash on empty q-metrics
 */
TYPED_TEST(q_metrics_test, test_populate_cumulative_on_empty)
{
    TypeParam::actual_metric_set.populateCumulativeDistributions();
    for(typename TypeParam::const_iterator cur=TypeParam::actual_metric_set.metrics().begin();cur != TypeParam::actual_metric_set.metrics().end();++cur)
        EXPECT_TRUE(!cur->is_cumulative_empty());


    q_metrics empty_metrics;
    empty_metrics.populateCumulativeDistributions();
}


#define FIXTURE q_metrics_test
/**
 * @class illumina::interop::model::metrics::q_metric
 * @test Confirm binary write matches expected binary data
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "src/tests/interop/metrics/stream_exception_tests.hpp"


