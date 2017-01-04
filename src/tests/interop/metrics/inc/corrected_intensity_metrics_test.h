/** Unit tests for the corrected intensity metrics
 *
 *
 *  @file
 *  @date 3/10/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "src/tests/interop/metrics/inc/metric_test.h"
#include "interop/model/metrics/corrected_intensity_metric.h"
#include "interop/model/summary/run_summary.h"
#include "interop/util/length_of.h"

namespace illumina{ namespace interop { namespace unittest
{
    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::corrected_intensity_metric
     * @note Version 2
     */
    struct corrected_intensity_metric_v2 : metric_test<model::metrics::corrected_intensity_metric, 2>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            typedef metric_t::uint_t uint_t;
            typedef metric_t::ushort_t ushort_t;
            metrics = metric_set_t(VERSION);
            const ushort_t corrected_int_all1[] = {1213, 966, 960, 1095};
            const ushort_t corrected_int_called1[] = {4070, 4074, 4029, 3972};
            const uint_t called_counts1[] = {0, 698433, 548189, 548712, 646638};
            metrics.insert(
                    metric_t(1, 1103, 25, 1063, 11.9458876f, to_vector(corrected_int_called1), to_vector(corrected_int_all1),
                             to_vector(called_counts1)));
            const ushort_t corrected_int_all2[] = {1558, 1151, 1158, 1293};
            const uint_t called_counts2[] = {10938, 733661, 537957, 543912, 615504};
            const ushort_t corrected_int_called2[] = {5013, 4983, 4915, 4932};
            metrics.insert(
                    metric_t(1, 1104, 1, 1295, 13.3051805f, to_vector(corrected_int_called2), to_vector(corrected_int_all2),
                             to_vector(called_counts2)));
            const ushort_t corrected_int_all3[] = {1171, 932, 912, 1069};
            const uint_t called_counts3[] = {0, 706987, 556441, 556067, 653959};
            const ushort_t corrected_int_called3[] = {3931, 3931, 3923, 3878};
            metrics.insert(
                    metric_t(1, 1105, 25, 1025, 11.7396259f, to_vector(corrected_int_called3), to_vector(corrected_int_all3),
                             to_vector(called_counts3)));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection& buffer)
        {
            const int tmp[] =
            {
                 2, 48, 1, 0, 79, 4, 25, 0, 39, 4, 189, 4, 198, 3, 192, 3, 71, 4, 230, 15, 234, 15, 189, 15, 132,
                 15, 0, 0, 0, 0, 65, 168, 10, 0, 93, 93, 8, 0, 104, 95, 8, 0, 238, 221, 9, 0, 91, 34, 63, 65, 1, 0,
                 80, 4, 1, 0, 15, 5, 22, 6, 127, 4, 134, 4, 13, 5, 149, 19, 119, 19, 51, 19, 68, 19, 186, 42, 0, 0,
                 221, 49, 11, 0, 101, 53, 8, 0, 168, 76, 8, 0, 80, 100, 9, 0, 5, 226, 84, 65, 1, 0, 81, 4, 25, 0, 1,
                 4, 147, 4, 164, 3, 144, 3, 45, 4, 91, 15, 91, 15, 83, 15, 38, 15, 0, 0, 0, 0, 171, 201, 10, 0, 153,
                 125, 8, 0, 35, 124, 8, 0, 135, 250, 9, 0, 130, 213, 59, 65
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }/** Get the summary for these metrics
         *
         * @return run summary
         */
        static void create_summary(model::summary::run_summary& summary)
        {
            const size_t lane_count = 0;
            const size_t surface_count = 2;
            const size_t channel_count = 2;
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 27, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary.cycle_state().called_cycle_range(model::run::cycle_range(1, 25));
        }
    };


    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::corrected_intensity_metric
     * @note Version 3
     */
    struct corrected_intensity_metric_v3 : metric_test<model::metrics::corrected_intensity_metric, 3>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            typedef metric_t::uint_t uint_t;
            typedef metric_t::ushort_t ushort_t;
            metrics = metric_set_t(VERSION);
            const uint_t called_counts1[] = {52, 1049523, 654071, 500476, 982989};
            const ushort_t corrected_int_called1[] = {245, 252, 61, 235};
            metrics.insert(metric_t(7, 1114, 1, to_vector(corrected_int_called1), to_vector(called_counts1)));
            const uint_t called_counts2[] = {0, 1063708, 582243, 588028, 953132};
            const ushort_t corrected_int_called2[] = {232, 257, 68, 228};
            metrics.insert(metric_t(7, 1114, 2, to_vector(corrected_int_called2), to_vector(called_counts2)));
            const uint_t called_counts3[] = {0, 1022928, 617523, 594836, 951825};
            const ushort_t corrected_int_called3[] = {227, 268, 68, 229};
            metrics.insert(metric_t(7, 1114, 3, to_vector(corrected_int_called3), to_vector(called_counts3)));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection& buffer)
        {
            const int tmp[] =
            {
                3, 34, 7, 0, 90, 4, 1, 0, -11, 0, -4, 0, 61, 0, -21, 0, 52, 0, 0, 0, -77, 3, 16, 0, -9, -6, 9, 0,
                -4, -94, 7, 0, -51, -1, 14, 0, 7, 0, 90, 4, 2, 0, -24, 0, 1, 1, 68, 0, -28, 0, 0, 0, 0, 0, 28, 59,
                16, 0, 99, -30, 8, 0, -4, -8, 8, 0, 44, -117, 14, 0, 7, 0, 90, 4, 3, 0, -29, 0, 12, 1, 68, 0, -27,
                0, 0, 0, 0, 0, -48, -101, 15, 0, 51, 108, 9, 0, -108, 19, 9, 0, 17, -122, 14, 0
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
        /** Get the summary for these metrics
         *
         * @return run summary
         */
        static void create_summary(model::summary::run_summary& summary)
        {
            const size_t lane_count = 0;
            const size_t surface_count = 2;
            const size_t channel_count = 2;
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 27, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary.cycle_state().called_cycle_range(model::run::cycle_range(3, 3));
        }
    };

}}}

