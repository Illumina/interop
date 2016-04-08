/** Unit tests for the corrected intensity metrics
 *
 *
 *  @file
 *  @date 3/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <gtest/gtest.h>
#include "metric_test.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/summary/run_summary.h"


namespace illumina{ namespace interop { namespace unittest {

/** This test writes three records of an InterOp files, then reads them back in and compares
 * each value to ensure they did not change.
 *
 * @note Version 3
 */
struct error_v3 : metric_test<model::metrics::error_metric, 3>
{
    /** Build the expected metric set
     *
     * @return vector of metrics
     */
    static std::vector<metric_t> metrics()
    {
        std::vector<metric_t> expected_metrics;


        expected_metrics.push_back(metric_t(7, 1114, 1, 0.450100899f));
        expected_metrics.push_back(metric_t(7, 1114, 2, 0.900201797f));
        expected_metrics.push_back(metric_t(7, 1114, 3, 0.465621591f));

        return expected_metrics;
    }
    /** Get the expected metric set header
     *
     * @return expected metric set header
     */
    static header_t header()
    {
        return header_t();
    }
    /** Get the expected binary data
     *
     * @return binary data string
     */
    static std::string binary_data()
    {
        const int tmp[] = {3,30,7,0,90,4,1,0,-96,115,-26,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,2,0,-96,115,102,63,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,3,0,-12,101,-18,62,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
        };
        return to_string(tmp);
    }
    /** Get number lanes in data
     *
     * @return 8 lanes
     */
    static size_t lane_count(){return 8;}
    /** Get reads describing data
     *
     * @return reads vector
     */
    static std::vector<model::run::read_info> reads()
    {
        std::vector<model::run::read_info> reads(1, model::run::read_info(1, 1, 3, false));
        return reads;
    }
    /** Get the summary for these metrics
     *
     * @return run summary
     */
    static model::summary::run_summary summary()
    {
        std::vector<model::run::read_info> read_infos = reads();
        model::summary::run_summary summary(read_infos.begin(), read_infos.end(), lane_count());
        summary[0][6].error_rate()=model::summary::metric_stat(0.67515134811401367f, 0, 0.67515134811401367f);
        summary[0][6].error_rate_35()=model::summary::metric_stat(0.67515134811401367f, 0, 0.67515134811401367f);
        summary[0][6].error_rate_50()=model::summary::metric_stat(0.67515134811401367f, 0, 0.67515134811401367f);
        summary[0][6].error_rate_75()=model::summary::metric_stat(0.67515134811401367f, 0, 0.67515134811401367f);
        summary[0][6].error_rate_100()=model::summary::metric_stat(0.67515134811401367f, 0, 0.67515134811401367f);
        summary[0][6].cycle_state().error_cycle_range(model::run::cycle_range(2, 2));
        summary[0].summary().error_rate(0.67515134811401367f);
        summary.total_summary().error_rate(0.67515134811401367f);
        summary.nonindex_summary().error_rate(0.67515134811401367f);
        summary[0][6].tile_count(1);
        summary.cycle_state().error_cycle_range(model::run::cycle_range(2, 2));
        return summary;
    }
};

/** Interface between fixtures and Google Test */
template<typename TestSetup>
struct error_metrics_test : public ::testing::Test, public TestSetup{};
}}}
