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
            const float corrected_int_called1[] = {4070, 4074, 4029, 3972};
            const uint_t called_counts1[] = {0, 698433, 548189, 548712, 646638};
            metrics.insert(
                    metric_t(1, 1103, 25, 1063, 11.9458876f, to_vector(corrected_int_called1), to_vector(corrected_int_all1),
                             to_vector(called_counts1)));
            const ushort_t corrected_int_all2[] = {1558, 1151, 1158, 1293};
            const uint_t called_counts2[] = {10938, 733661, 537957, 543912, 615504};
            const float corrected_int_called2[] = {5013, 4983, 4915, 4932};
            metrics.insert(
                    metric_t(1, 1104, 1, 1295, 13.3051805f, to_vector(corrected_int_called2), to_vector(corrected_int_all2),
                             to_vector(called_counts2)));
            const ushort_t corrected_int_all3[] = {1171, 932, 912, 1069};
            const uint_t called_counts3[] = {0, 706987, 556441, 556067, 653959};
            const float corrected_int_called3[] = {3931, 3931, 3923, 3878};
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
            const signed char tmp[] =
            {
                2
                ,48,1,0,79,4,25,0,39,4,-67,4,-58,3,-64,3,71,4,-26,15,-22
                ,15,-67,15,-124,15,0,0,0,0,65,-88,10,0,93,93,8,0,104,95,8
                ,0,-18,-35,9,0,91,34,63,65,1,0,80,4,1,0,15,5,22,6,127
                ,4,-122,4,13,5,-107,19,119,19,51,19,68,19,-70,42,0,0,-35,49,11
                ,0,101,53,8,0,-88,76,8,0,80,100,9,0,5,-30,84,65,1,0,81
                ,4,25,0,1,4,-109,4,-92,3,-112,3,45,4,91,15,91,15,83,15,38
                ,15,0,0,0,0,-85,-55,10,0,-103,125,8,0,35,124,8,0,-121,-6,9
                ,0,-126,-43,59,65
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }/** Get the summary for these metrics
         *
         * @return run summary
         */
        static void create_summary(model::summary::run_summary& summary)
        {
            const size_t lane_count = 1;
            const size_t surface_count = 2;
            const size_t channel_count = 2;
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 27, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary.cycle_state().called_cycle_range(model::run::cycle_range(1, 25));
            summary[0][0].cycle_state().called_cycle_range(model::run::cycle_range(1, 25));
            summary[0][0][0].tile_count(3);
            summary[0][0].tile_count(3);
            summary[0][0].lane(1);
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
            metrics = metric_set_t(VERSION);
            const uint_t called_counts1[] = {52, 1049523, 654071, 500476, 982989};
            const float corrected_int_called1[] = {245, 252, 61, 235};
            metrics.insert(metric_t(7, 1114, 1, to_vector(corrected_int_called1), to_vector(called_counts1)));
            const uint_t called_counts2[] = {0, 1063708, 582243, 588028, 953132};
            const float corrected_int_called2[] = {232, 257, 68, 228};
            metrics.insert(metric_t(7, 1114, 2, to_vector(corrected_int_called2), to_vector(called_counts2)));
            const uint_t called_counts3[] = {0, 1022928, 617523, 594836, 951825};
            const float corrected_int_called3[] = {227, 268, 68, 229};
            metrics.insert(metric_t(7, 1114, 3, to_vector(corrected_int_called3), to_vector(called_counts3)));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection& buffer)
        {
            const signed char tmp[] =
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
            const size_t lane_count = 1;
            const size_t surface_count = 2;
            const size_t channel_count = 2;
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 27, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary.cycle_state().called_cycle_range(model::run::cycle_range(3, 3));
            summary[0][0].lane(7);
            summary[0][0].tile_count(1);
            summary[0][0][0].tile_count(1);
            summary[0][0].cycle_state().called_cycle_range(model::run::cycle_range(3, 3));
        }
    };

    /** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
     * to the values displayed in SAV.
     *
     * Regression Test: Regression_Win_150925_SF-0128_0466_AH23HLCFXX_Indexed-Six-Digit
     * @see model::metrics::corrected_intensity_metric
     * @note Version 4
     */
    struct corrected_intensity_metric_v4 : metric_test<model::metrics::corrected_intensity_metric, 4>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            typedef metric_t::uint_t uint_t;
            metrics = metric_set_t(VERSION);
            uint_t called_counts1[] = {0,111617,71352,57947,104195};
            uint_t called_counts2[] = {0,109387,64466,66598,104660};
            uint_t called_counts3[] = {0,106933,68530,65092,104556};

            metrics.insert(metric_t(3, 211011, 1, to_vector(called_counts1)));
            metrics.insert(metric_t(3, 211011, 2, to_vector(called_counts2)));
            metrics.insert(metric_t(3, 211011, 3, to_vector(called_counts3)));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection& buffer)
        {
            const signed char tmp[] =
            {
                    4,28,3,0,67,56,3,0,1,0,0,0,0,0,1,-76,1,0,-72,22,1,0,91,-30
                    ,0,0,3,-105,1,0,3,0,67,56,3,0,2,0,0,0,0,0,75,-85,1,0,-46
                    ,-5,0,0,38,4,1,0,-44,-104,1,0,3,0,67,56,3,0,3,0,0,0,0,0
                    ,-75,-95,1,0,-78,11,1,0,68,-2,0,0,108,-104,1,0
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };
}}}

