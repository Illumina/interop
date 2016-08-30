/** Unit tests for the corrected intensity metrics
 *
 *
 *  @file
 *  @date 3/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <gtest/gtest.h>
#include "metric_test.h"
#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/model/summary/run_summary.h"


namespace illumina{ namespace interop { namespace unittest {

    /** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
     * to the values displayed in SAV.
     *
     * Regression Test: 1947950_117213Bin2R0I
     *
     * @note Version 2
     */
    struct corrected_intensity_v2 : metric_test<model::metrics::corrected_intensity_metric, 2>
    {
        /** Build the expected metric set
         *
         * @return vector of metrics
         */
        static std::vector<metric_t> metrics()
        {
            typedef metric_t::uint_t uint_t;
            typedef metric_t::ushort_t ushort_t;
            std::vector<metric_t> metrics;
            ushort_t corrected_int_all1[] = {1213, 966, 960, 1095};
            ushort_t corrected_int_called1[] = {4070, 4074, 4029, 3972};
            uint_t called_counts1[] = {0, 698433, 548189, 548712, 646638};
            metrics.push_back(
                    metric_t(1, 1104, 25, 1063, 11.9458876f, to_vector(corrected_int_called1), to_vector(corrected_int_all1),
                             to_vector(called_counts1)));
            ushort_t corrected_int_all2[] = {1558, 1151, 1158, 1293};
            uint_t called_counts2[] = {10938, 733661, 537957, 543912, 615504};
            ushort_t corrected_int_called2[] = {5013, 4983, 4915, 4932};
            metrics.push_back(
                    metric_t(1, 1104, 1, 1295, 13.3051805f, to_vector(corrected_int_called2), to_vector(corrected_int_all2),
                             to_vector(called_counts2)));
            ushort_t corrected_int_all3[] = {1171, 932, 912, 1069};
            uint_t called_counts3[] = {0, 706987, 556441, 556067, 653959};
            ushort_t corrected_int_called3[] = {3931, 3931, 3923, 3878};
            metrics.push_back(
                    metric_t(1, 1105, 25, 1025, 11.7396259f, to_vector(corrected_int_called3), to_vector(corrected_int_all3),
                             to_vector(called_counts3)));
            return metrics;
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
            int tmp[] = {2, 48, 1, 0, 80, 4, 25, 0, 39, 4, 189, 4, 198, 3, 192, 3, 71, 4, 230, 15, 234, 15, 189, 15, 132,
                      15, 0, 0, 0, 0, 65, 168, 10, 0, 93, 93, 8, 0, 104, 95, 8, 0, 238, 221, 9, 0, 91, 34, 63, 65, 1, 0,
                      80, 4, 1, 0, 15, 5, 22, 6, 127, 4, 134, 4, 13, 5, 149, 19, 119, 19, 51, 19, 68, 19, 186, 42, 0, 0,
                      221, 49, 11, 0, 101, 53, 8, 0, 168, 76, 8, 0, 80, 100, 9, 0, 5, 226, 84, 65, 1, 0, 81, 4, 25, 0, 1,
                      4, 147, 4, 164, 3, 144, 3, 45, 4, 91, 15, 91, 15, 83, 15, 38, 15, 0, 0, 0, 0, 171, 201, 10, 0, 153,
                      125, 8, 0, 35, 124, 8, 0, 135, 250, 9, 0, 130, 213, 59, 65
            };
            return to_string(tmp);
        }
        /** Get number lanes in data
         *
         * @return 8 lanes
         */
        static model::metric_base::base_metric::uint_t lane_count(){return 8;}
        /** Get reads describing data
         *
         * @return reads vector
         */
        static std::vector<model::run::read_info> reads()
        {
            std::vector<model::run::read_info> reads(1, model::run::read_info(1, 1, 27, false));
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
            summary[0][0].cycle_state().called_cycle_range(model::run::cycle_range(1, 25));
            summary.cycle_state().called_cycle_range(model::run::cycle_range(1, 25));
            return summary;
        }
    };

    /** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
     * to the values displayed in SAV.
     *
     * @note Version 3
     */
    struct corrected_intensity_v3 : metric_test<model::metrics::corrected_intensity_metric, 3>
    {
        /** Build the expected metric set
         *
         * @return vector of metrics
         */
        static std::vector<metric_t> metrics()
        {
            typedef metric_t::uint_t uint_t;
            typedef metric_t::ushort_t ushort_t;
            std::vector<metric_t> metrics;
            uint_t called_counts1[] = {52, 1049523, 654071, 500476, 982989};
            ushort_t corrected_int_called1[] = {245, 252, 61, 235};
            //expected_metrics.push_back(metric_type(7, 1114, 1, to_vector(corrected_int_called1), to_vector(called_counts1)));
            metrics.push_back(metric_t(7, 1114, 1, (corrected_int_called1), (called_counts1)));
            uint_t called_counts2[] = {0, 1063708, 582243, 588028, 953132};
            ushort_t corrected_int_called2[] = {232, 257, 68, 228};
            //expected_metrics.push_back(metric_type(7, 1114, 2, to_vector(corrected_int_called2), to_vector(called_counts2)));
            metrics.push_back(metric_t(7, 1114, 2, (corrected_int_called2), (called_counts2)));
            uint_t called_counts3[] = {0, 1022928, 617523, 594836, 951825};
            ushort_t corrected_int_called3[] = {227, 268, 68, 229};
            metrics.push_back(metric_t(7, 1114, 3, (corrected_int_called3), (called_counts3)));
            //expected_metrics.push_back(metric_type(7, 1114, 3, to_vector(corrected_int_called3), to_vector(called_counts3)));
            return metrics;
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
            const int tmp[] = {3, 34, 7, 0, 90, 4, 1, 0, -11, 0, -4, 0, 61, 0, -21, 0, 52, 0, 0, 0, -77, 3, 16, 0, -9, -6, 9, 0,
                         -4, -94, 7, 0, -51, -1, 14, 0, 7, 0, 90, 4, 2, 0, -24, 0, 1, 1, 68, 0, -28, 0, 0, 0, 0, 0, 28, 59,
                         16, 0, 99, -30, 8, 0, -4, -8, 8, 0, 44, -117, 14, 0, 7, 0, 90, 4, 3, 0, -29, 0, 12, 1, 68, 0, -27,
                         0, 0, 0, 0, 0, -48, -101, 15, 0, 51, 108, 9, 0, -108, 19, 9, 0, 17, -122, 14, 0
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
            std::vector<model::run::read_info> reads(1, model::run::read_info(1, 1, 27, false));
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
            summary[0][0].cycle_state().called_cycle_range(model::run::cycle_range(3, 3));
            summary.cycle_state().called_cycle_range(model::run::cycle_range(3, 3));
            return summary;
        }
    };
    /** Interface between fixtures and Google Test */
    template<typename TestSetup>
    struct corrected_intensity_metrics_test : public ::testing::Test, public TestSetup{};
}}}
