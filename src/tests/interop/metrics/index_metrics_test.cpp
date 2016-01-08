/** Unit tests for the index metrics
 *
 *
 *  @file
 *
 *  @date 11/4/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <fstream>
#include <gtest/gtest.h>
#include "interop/model/metric_sets/index_metric_set.h"
#include "src/tests/interop/metrics/metric_test_utils.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;


namespace illumina{ namespace interop { namespace unittest {
/** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
 * to the values displayed in SAV.
 *
 * This metric was artificially changed to give it three unique records. The lane was changed from 1 to 2 and 3
 * for the second and third records.
 *
 * @note Version 1
 */
struct index_metrics_hardcoded_fixture_v1 : util::fixture_helper<index_metrics, 1>
{
    enum{
        /** Do not check record size (does not have it)*/
        disable_check_record_size=true,
        /** Do not check the expected binary data size */
        disable_binary_data_size=true
    };
    /** Setup fixture */
    index_metrics_hardcoded_fixture_v1()
    {
        index_metric::index_array_t indices1;
        indices1.push_back(index_info("ATCACGAC-AAGGTTCA", "1", "TSCAIndexes", 4570));
        expected_metrics.push_back(metric_type(1, 12106, 3, indices1));
        index_metric::index_array_t indices2;
        indices2.push_back(index_info("ACAGTGGT-AAGGTTCA", "2", "TSCAIndexes", 4477));
        expected_metrics.push_back(metric_type(2, 12106, 3, indices2));
        index_metric::index_array_t indices3;
        indices3.push_back(index_info("CAGATCCA-AAGGTTCA", "3", "TSCAIndexes", 4578));
        expected_metrics.push_back(metric_type(3, 12106, 3, indices3));
        int tmp[] = {1
                ,1,0,74,47,3,0
                ,17,0,65,84,67,65,67,71,65,67,45,65,65,71,71,84,84,67,65,218,17,0,0,1,0,49,11,0,84,83,67,65,73,110
                ,100,101,120,101,115
                ,2,0,74,47,3,0 // changed to lane 2
                ,17,0,65,67,65,71,84,71,71,84,45,65,65,71,71,84,84,67,65,125,17,0,0,1,0,50,11,0,84,83,67,65,73,110
                ,100,101,120,101,115
                ,3,0,74,47,3,0 // changed to lane 3
                ,17,0,67,65,71,65,84,67,67,65,45,65,65,71,71,84,84,67,65,226,17,0,0,1,0,51,11,0,84,83,67,65,73,110
                ,100,101,120,101,115
        };
        setup_hardcoded_binary(tmp, header_type());
    }
};


/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 1
 */
struct index_metrics_write_read_fixture_v1 : util::fixture_helper<index_metrics, 1>
{
    enum{
        /** Do not check record size (does not have it)*/
        disable_check_record_size=true
    };
    /** Setup fixture */
    index_metrics_write_read_fixture_v1()
    {
        index_metric::index_array_t indices1;
        indices1.push_back(index_info("ATCACGAC-AAGGTTCA", "1", "TSCAIndexes", 4570));
        expected_metrics.push_back(metric_type(1, 12106, 3, indices1));
        index_metric::index_array_t indices2;
        indices2.push_back(index_info("ACAGTGGT-AAGGTTCA", "2", "TSCAIndexes", 4477));
        expected_metrics.push_back(metric_type(2, 12106, 3, indices2));
        index_metric::index_array_t indices3;
        indices3.push_back(index_info("CAGATCCA-AAGGTTCA", "3", "TSCAIndexes", 4578));
        expected_metrics.push_back(metric_type(3, 12106, 3, indices3));
        setup_write_read();
    }
};
/** Interface between fixtures and Google Test */
template<typename TestSetup>
struct index_metrics_test : public ::testing::Test, public TestSetup { };
}}}
using namespace illumina::interop::unittest;




typedef ::testing::Types<
index_metrics_hardcoded_fixture_v1
,index_metrics_write_read_fixture_v1
> Formats;
TYPED_TEST_CASE(index_metrics_test, Formats);

/**
 * @class illumina::interop::model::metrics::index_metric
 * @test Confirm version 1 of the metric can be written to and read from a stream
 * @test Confirm version 1 of the metric matches known binary file
 */
TYPED_TEST(index_metrics_test, test_read_write)
{
    EXPECT_EQ(TypeParam::actual_metric_set.version(), TypeParam::VERSION);
    EXPECT_EQ(TypeParam::actual_metrics.size(), TypeParam::expected_metrics.size());

    for(typename TypeParam::const_iterator itExpected=TypeParam::expected_metrics.begin(), itActual = TypeParam::actual_metrics.begin();
        itExpected != TypeParam::expected_metrics.end() && itActual != TypeParam::actual_metrics.end();
        itExpected++,itActual++)
    {
        EXPECT_EQ(itExpected->lane(), itActual->lane());
        EXPECT_EQ(itExpected->tile(), itActual->tile());
        EXPECT_EQ(itExpected->read(), itActual->read());
        EXPECT_EQ(itExpected->size(), itActual->size());
        for(size_t i=0;i<std::min(itExpected->size(), itActual->size());i++)
        {
            EXPECT_EQ(itExpected->indices(i).index_seq(), itActual->indices(i).index_seq());
            EXPECT_EQ(itExpected->indices(i).sample_id(), itActual->indices(i).sample_id());
            EXPECT_EQ(itExpected->indices(i).sample_proj(), itActual->indices(i).sample_proj());
            EXPECT_EQ(itExpected->indices(i).count(), itActual->indices(i).count());
        }
    }
}

#define FIXTURE index_metrics_test
/**
 * @class illumina::interop::model::metrics::index_metric
 * @test Confirm bad_format_exception is thrown when version is unsupported
 * @test Confirm incomplete_file_exception is thrown for a small partial record
 * @test Confirm incomplete_file_exception is thrown for a mostly complete file
 * @test Confirm bad_format_exception is thrown when record size is incorrect
 * @test Confirm file_not_found_exception is thrown when a file is not found
 * @test Confirm reading from good data does not throw an exception
 */
#include "src/tests/interop/metrics/stream_exception_tests.hpp"


