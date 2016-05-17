/** Unit tests for the q-metrics
 *
 *
 *  @file
 *  @date 5/18/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <gtest/gtest.h>
#include "metric_test.h"
#include "interop/model/metrics/q_collapsed_metric.h"


namespace illumina{ namespace interop { namespace unittest {
    /** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
     * to the values displayed in SAV.
     *
     * Regression set: 1947950_117213Bin2R0I
     *
     * @note Version 2
     */
    struct q_collapsed_v2 : metric_test<model::metrics::q_collapsed_metric, 2>
    {
        /** Build the expected metric set
         *
         * @return vector of metrics
         */
        static std::vector<metric_t> metrics()
        {
            std::vector<metric_t> expected_metrics;
            expected_metrics.push_back(metric_t(1,1105,1,2447414,2334829,2566750,33));
            expected_metrics.push_back(metric_t(1,1103,1,2436317,2327796,2543605,33));
            expected_metrics.push_back(metric_t(1,1106,1,2474217,2366046,2583629,33));

            return expected_metrics;
        }
        /** Get the expected metric set header
         *
         * @return expected metric set header
         */
        static header_t header()
        {
            typedef header_t::qscore_bin_vector_type qscore_bin_vector_type;
            qscore_bin_vector_type headervec;
            return header_t(headervec);
        }
        /** Get the expected binary data
         *
         * @return binary data string
         */
        static std::string binary_data()
        {
            const int tmp[] = {
                    2,22
                    ,1,0,81,4,1,0,54,88,37,0,109,-96,35,0,94,42,39,0,0,0,4,66
                    ,1,0,79,4,1,0,-35,44,37,0,-12,-124,35,0,-11,-49,38,0,0,0,4,66
                    ,1,0,82,4,1,0,-23,-64,37,0,94,26,36,0,77,108,39,0,0,0,4,66
            };
            return to_string(tmp);
        }
    };

    /** This test writes three records of an InterOp files, then reads them back in and compares
     * each value to ensure they did not change.
     *
     * @note Version 6
     */
    struct q_collapsed_v6 : metric_test<model::metrics::q_collapsed_metric, 6>
    {
        enum{
            /** Do not check the expected binary data */
            disable_binary_data=true,
            /** Do not check the expected binary data size */
            disable_binary_data_size=true
        };
        /** Build the expected metric set
         *
         * @return vector of metrics
         */
        static std::vector<metric_t> metrics()
        {
            std::vector<metric_t> expected_metrics;
            expected_metrics.push_back(metric_t(1,1105,1,2447414,2334829,2566750,33));
            expected_metrics.push_back(metric_t(1,1103,1,2436317,2327796,2543605,33));
            expected_metrics.push_back(metric_t(1,1106,1,2474217,2366046,2583629,33));
            return expected_metrics;
        }
        /** Get the expected metric set header
         *
         * @return expected metric set header
         */
        static header_t header()
        {
            typedef header_t::qscore_bin_vector_type qscore_bin_vector_type;
            typedef header_t::bin_t bin_t;
            typedef bin_t::bin_type ushort_t;
            typedef metric_t::uint_t uint_t;
            const uint_t bin_count = 7;

            const ushort_t lower[] = {2, 10, 20, 25, 30, 35, 40};
            const ushort_t upper[] = {9, 19, 24, 29, 34, 39, 40};
            const ushort_t value[] = {2, 14, 21, 27, 32, 36, 40};
            qscore_bin_vector_type headervec;
            for(uint_t i=0;i<bin_count;i++)
                headervec.push_back(bin_t(lower[i], upper[i], value[i]));
            return header_t(headervec);
        }
        /** Get the expected binary data
         *
         * @return binary data string
         */
        static std::string binary_data()
        {
            const int tmp[] = {
                    6,22,1,7,2,10,20,25,30,35,40,9,19,24,29,34,39,40,2,14,21,27,32,36,40
                    ,1,0,81,4,1,0,54,88,37,0,109,-96,35,0,94,42,39,0,0,0,4,66
                    ,1,0,79,4,1,0,-35,44,37,0,-12,-124,35,0,-11,-49,38,0,0,0,4,66
                    ,1,0,82,4,1,0,-23,-64,37,0,94,26,36,0,77,108,39,0,0,0,4,66
            };
            return to_string(tmp);
        }
    };

    /** Interface between fixtures and Google Test */
    template<typename TestSetup>
    struct q_collapsed_metrics_test : public ::testing::Test, public TestSetup { };
}}}
