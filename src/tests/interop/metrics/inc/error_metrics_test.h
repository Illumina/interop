/** Unit tests for the error metrics
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
#include "interop/util/length_of.h"


namespace illumina { namespace interop { namespace unittest
{

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @note Version 3
     */
    struct error_metric_v3 : metric_test<model::metrics::error_metric, 3>
    {
        //typedef model::metrics::error_metric metric_t;
        //typedef model::metric_base::metric_set< metric_t > metric_set_t;
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics)
        {
            metrics = metric_set_t(3);
            metrics.insert(metric_t(7, 1114, 1, 0.450100899f));
            metrics.insert(metric_t(7, 1114, 2, 0.900201797f));
            metrics.insert(metric_t(7, 1114, 3, 0.465621591f));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data vector
         */
        static void create_binary_data(std::vector< ::uint8_t >& buffer)
        {
            create_binary_data_t(buffer);
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        static void create_binary_data(std::string& buffer)
        {
            create_binary_data_t(buffer);
        }

    private:
        /** Get the expected binary data
         *
         * @return binary data string
         */
        template<class T>
        static void create_binary_data_t(T& buffer)
        {
            static const int tmp[] = {3, 30,
                                      7, 0, 90, 4, 1, 0, -96, 115, -26, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0,
                                      7, 0, 90, 4, 2, 0, -96, 115, 102, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0,
                                      7, 0, 90, 4, 3, 0, -12, 101, -18, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };

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
            const int tmp[] = {3, 30, 7, 0, 90, 4, 1, 0, -96, 115, -26, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 7, 0, 90, 4, 2, 0, -96, 115, 102, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 90, 4, 3, 0, -12, 101, -18, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };
            return to_string(tmp);
        }

        /** Get number lanes in data
         *
         * @return 8 lanes
         */
        static model::metric_base::base_metric::uint_t lane_count()
        { return 8; }

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
            model::summary::run_summary summary(read_infos.begin(), read_infos.end(), 1);
            summary[0][0].lane(7);
            summary[0][0].error_rate(model::summary::metric_stat(0.67515134811401367f, 0, 0.67515134811401367f));
            summary[0][0].error_rate_35(model::summary::metric_stat(0.0f, 0, 0.0f));
            summary[0][0].error_rate_50(model::summary::metric_stat(00.0f, 0, 0.0f));
            summary[0][0].error_rate_75(model::summary::metric_stat(0.0f, 0, 0.0f));
            summary[0][0].error_rate_100(model::summary::metric_stat(0.0f, 0, 0.0f));
            summary[0][0].cycle_state().error_cycle_range(model::run::cycle_range(3, 3));
            summary[0].summary().error_rate(0.67515134811401367f);
            summary.total_summary().error_rate(0.67515134811401367f);
            summary.nonindex_summary().error_rate(0.67515134811401367f);
            summary[0][0].tile_count(1);
            summary.cycle_state().error_cycle_range(model::run::cycle_range(3, 3));
            return summary;
        }
    };

/** Interface between fixtures and Google Test */
    template<typename TestSetup>
    struct error_metrics_test : public ::testing::Test, public TestSetup
    {
    };
}}}
