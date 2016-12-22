/** Unit tests for the q-metrics
 *
 *
 *  @file
 *  @date 3/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "src/tests/interop/metrics/inc/metric_test.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/model/summary/run_summary.h"
#include "interop/util/length_of.h"


namespace illumina{ namespace interop { namespace unittest 
{

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_metric
     * @note Version 4
     */
    struct q_metric_v4 : metric_test<model::metrics::q_metric, 4>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(VERSION);

            typedef metric_t::uint_t uint_t;
            typedef sparse_value<uint_t, metric_t::MAX_Q_BINS> q_val;
            const q_val hist1[] = {q_val(6,21208), q_val(19,8227), q_val(32,73051), q_val(37,2339486)};
            const q_val hist2[] = {q_val(6,22647), q_val(19,9570), q_val(32,81839), q_val(37,2413227)};
            const q_val hist3[] = {q_val(6,18878), q_val(19,8168), q_val(32,72634), q_val(37,2342292)};

            metrics.insert(metric_t(1, 1104, 1, to_vector(hist1)));
            metrics.insert(metric_t(1, 1106, 1, to_vector(hist2)));
            metrics.insert(metric_t(1, 1104, 2, to_vector(hist3)));
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
                4
                ,-50,1,0,80,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,-40,82,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,35,32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,91,29,1,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-98,-78,35,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,1,0,82,4,1,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,119,88,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,98,37,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,-81,63,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,-85,-46,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,80,4,2,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,-66,73,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-24,31,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,-70,27,1,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,-108,-67,35,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
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
                    model::run::read_info(1, 1, 3, false)
            };
            const float q30 = 98.803192138671875f;
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary[0][0].tile_count(2);
            summary[0][0].projected_yield_g(0.0098816361278295517);
            summary[0][0].yield_g(0.0074112270958721638f);
            summary[0][0].percent_gt_q30(q30);
            summary[0][0][0].tile_count(2);
            summary[0][0][0].projected_yield_g(0.0098816361278295517);
            summary[0][0][0].yield_g(0.0074112270958721638f);
            summary[0][0][0].percent_gt_q30(q30);
            summary[0][0].cycle_state().qscored_cycle_range(model::run::cycle_range(1, 2));
            summary[0].summary().projected_yield_g(0.0098816361278295517);
            summary[0].summary().yield_g(0.0074112270958721638f);
            summary[0].summary().percent_gt_q30(q30);
            summary.total_summary().percent_gt_q30(q30);
            summary.total_summary().projected_yield_g(0.0098816361278295517);
            summary.total_summary().yield_g(0.0074112270958721638f);
            summary.nonindex_summary().percent_gt_q30(q30);
            summary.nonindex_summary().projected_yield_g(0.0098816361278295517);
            summary.nonindex_summary().yield_g(0.0074112270958721638f);
            summary.cycle_state().qscored_cycle_range(model::run::cycle_range(1, 2));
        }
    };

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_metric
     * @note Version 5
     */
    struct q_metric_v5 : metric_test<model::metrics::q_metric, 5>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            typedef header_t::qscore_bin_vector_type qscore_bin_vector_type;
            typedef header_t::bin_t bin_t;
            typedef bin_t::bin_type ushort_t;
            typedef metric_t::uint_t uint_t;
            const uint_t bin_count = 7;
            ushort_t lower[] = {1, 10, 20, 25, 30, 35, 40};
            ushort_t upper[] = {9, 19, 24, 29, 34, 39, 41};
            ushort_t value[] = {1, 14, 22, 27, 33, 37, 40};
            qscore_bin_vector_type headervec;
            for(uint_t i=0;i<bin_count;i++)
                headervec.push_back(bin_t(lower[i], upper[i], value[i]));
            metrics = metric_set_t(header_t(headervec), VERSION);
            typedef metric_t::uint_t uint_t;
            typedef sparse_value<uint_t, 7> q_val;

            const q_val hist1[] = {q_val(1,45272), q_val(3,33369), q_val(4,1784241)};
            const q_val hist2[] = {q_val(1,45229), q_val(3,34304), q_val(4,1792186)};
            const q_val hist3[] = {q_val(1,49152), q_val(3,37440), q_val(4,1806479)};

            metrics.insert(metric_t(1, 1103, 1, to_vector(hist1)));
            metrics.insert(metric_t(1, 1104, 1, to_vector(hist2)));
            metrics.insert(metric_t(1, 1108, 1, to_vector(hist3)));
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
                    model::run::read_info(1, 1, 2, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary[0][0].tile_count(3);
            summary[0][0].projected_yield_g(0.0056276721879839897f);
            summary[0][0].yield_g(0.0056276721879839897f);
            summary[0][0].percent_gt_q30(95.650672912597656f);
            summary[0][0][0].tile_count(3);
            summary[0][0][0].projected_yield_g(0.0056276721879839897f);
            summary[0][0][0].yield_g(0.0056276721879839897f);
            summary[0][0][0].percent_gt_q30(95.650672912597656f);
            summary[0][0].cycle_state().qscored_cycle_range(model::run::cycle_range(1, 1));
            summary[0].summary().projected_yield_g(0.0056276721879839897f);
            summary[0].summary().yield_g(0.0056276721879839897f);
            summary[0].summary().percent_gt_q30(95.650672912597656f);
            summary.total_summary().percent_gt_q30(95.650672912597656f);
            summary.total_summary().projected_yield_g(0.0056276721879839897f);
            summary.total_summary().yield_g(0.0056276721879839897);
            summary.nonindex_summary().percent_gt_q30(95.650672912597656f);
            summary.nonindex_summary().projected_yield_g(0.0056276721879839897f);
            summary.nonindex_summary().yield_g(0.0056276721879839897);
            summary.cycle_state().qscored_cycle_range(model::run::cycle_range(1, 1));
        }
    };

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_metric
     * @note Version 6
     */
    struct q_metric_v6 : metric_test<model::metrics::q_metric, 6>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
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
            metrics = metric_set_t(header_t(headervec), VERSION);
            typedef metric_t::uint_t uint_t;

            const uint_t hist_all1[] = {0, 267962, 118703, 4284, 2796110, 0, 0};
            const uint_t hist_all2[] = {0,241483, 44960, 1100, 2899568, 0 ,0};
            const uint_t hist_all3[] = {0,212144, 53942, 427, 2920598, 0, 0};

            metrics.insert(metric_t(7, 1114, 1, to_vector(hist_all1)));
            metrics.insert(metric_t(7, 1114, 2, to_vector(hist_all2)));
            metrics.insert(metric_t(7, 1114, 3,to_vector(hist_all3)));
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
                6,34,1,7,2,10,20,25,30,35,40,9,19,24,29,34,39,40,2,14,21,27,32,36,40
                ,7,0,90,4,1,0,0,0,0,0,-70,22,4,0,-81,-49,1,0,-68,16,0,0,78,-86,42,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,2,0,0,0,0,0,75,-81,3,0,-96,-81,0,0,76,4,0,0,112,62,44,0,0,0,0,0,0,0,0,0
                ,7,0,90,4,3,0,0,0,0,0,-80,60,3,0,-74,-46,0,0,-85,1,0,0,-106,-112,44,0,0,0,0,0,0,0,0,0
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
                    model::run::read_info(1, 1, 4, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary[0][0].lane(7);
            summary[0][0].tile_count(1);
            summary[0][0].projected_yield_g(0.0095612816512584686f);
            summary[0][0].yield_g(0.009561280719935894f);
            summary[0][0].percent_gt_q30(90.1163330078125f);
            summary[0][0][0].tile_count(1);
            summary[0][0][0].projected_yield_g(0.0095612816512584686f);
            summary[0][0][0].yield_g(0.009561280719935894f);
            summary[0][0][0].percent_gt_q30(90.1163330078125f);
            summary[0][0].cycle_state().qscored_cycle_range(model::run::cycle_range(3, 3));
            summary[0].summary().projected_yield_g(0.0095612816512584686f);
            summary[0].summary().yield_g(0.0095612816512584686f);
            summary[0].summary().percent_gt_q30(90.1163330078125f);
            summary.total_summary().percent_gt_q30(90.1163330078125f);
            summary.total_summary().projected_yield_g(0.0095612816512584686f);
            summary.total_summary().yield_g(0.0095612816512584686f);
            summary.nonindex_summary().percent_gt_q30(90.1163330078125f);
            summary.nonindex_summary().projected_yield_g(0.0095612816512584686f);
            summary.nonindex_summary().yield_g(0.0095612816512584686f);
            summary.cycle_state().qscored_cycle_range(model::run::cycle_range(3, 3));
        }
    };
    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_metric
     * @note Version 6 (unbinned)
     */
    struct q_metric_v6_unbinned : metric_test<model::metrics::q_metric, 6>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(VERSION);
            typedef metric_t::uint_t uint_t;

            std::vector<uint_t> hist_tmp(model::metrics::q_metric::MAX_Q_BINS, 0);

            metrics.insert(metric_t(1, 1110, 1, hist_tmp));
            metrics.insert(metric_t(1, 1110, 2, hist_tmp));
            metrics.insert(metric_t(1, 1110, 3, hist_tmp));
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
                    6,-50,0,1,0,86,4,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,86,4,2
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,1,0,86,4,3,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
                    ,0,0,0
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };

    /**
     * @page q_metrics_requirement_q30 % >= Q30
     *
     * The QMetricsOut.bin InterOp file contains a histogram of the counts of PF clusters at each quality value ranging
     * from 1 to 50 for each lane, tile, and cycle.  % >= Q30 is calculated as the sum of the populations in bins with
     * a quality value of 30 or greater divided by the total non-N basecalls (sum of the population over all bins times
     * 100). Note that we perform these calculations on usable cycles only (in other words, we exclude the last cycle
     * of each read).
     *
     * @sa illumina::interop::unittest::q_metric_requirements
     *
     * @section q_metrics_requirement_q30_lane Lane
     *
     * For all tiles over all lanes for a given read, for all usable cycles (i.e. not counting the last
     * cycle of the read), the sum of the populations in bins with a quality value of 30 or greater divided
     * by the sum of the population over all bins times 100.
     *
     * @section q_metrics_requirement_q30_read Read
     *
     * For all tiles in a lane for a given read, for all usable cycles (i.e. not counting the last
     * cycle of the read), the sum of the populations in bins with a quality value of 30 or greater
     * divided by the sum of the population over all bins times 100.
     *
     * @section q_metrics_requirement_q30_non_index Non-Indexed Total
     *
     * For all tiles over all non-indexed reads, for all usable cycles (i.e. not counting the last cycle of
     * each read), the sum of the populations in bins with a quality value of 30 or greater divided by the sum
     * of the population over all bins times 100.
     *
     * @section q_metrics_requirement_q30_total Total
     *
     * For all tiles over all reads, for all usable cycles (i.e. not counting the last cycle of each read), the
     * sum of the populations in bins with a quality value of 30 or greater divided by the sum of the
     * population over all bins times 100.
     *
     *
     * @page q_metrics_requirement_yield Yield (G)
     *
     * @sa illumina::interop::unittest::q_metric_requirements
     *
     * @section q_metrics_requirement_yield_lane Lane
     *
     * The sum of bases from passing filter clusters that are not N over all cycles, except the last cycle,
     * divided by 1E9 to put in units of gigabases.
     *
     * @section q_metrics_requirement_yield_read Read
     *
     * The sum of bases from passing filter clusters that are not N over all cycles, except the last cycle,
     * divided by 1E9 to put in units of gigabases.
     *
     * @section q_metrics_requirement_yield_non_index Non-Indexed Total
     *
     * The sum of the read yields over all non-indexed reads.
     *
     * @section q_metrics_requirement_yield_total Total
     *
     * The sum of the read yields over all reads.
     *
     *
     * @page q_metrics_requirement_projected_yield Projected Yield (G)
     *
     * @sa illumina::interop::unittest::q_metric_requirements
     *
     * @section q_metrics_requirement_projected_yield_read Read
     *
     * The sum of bases from passing filter clusters that are not N over all cycles, except the last cycle,
     * scaled to the total read length, divided by 1E9 to put in units of gigabases. The scaling factor is
     * defined, per lane, as the final number of entries (tiles in the lane x usable cycles) divided by the
     * current number of entries for the read/lane in the QMetrics file.
     *
     * @section q_metrics_requirement_projected_yield_non_index Non-Indexed Total
     *
     * The sum of the projected total yield over all reads, excluding indexed reads.
     *
     * @section q_metrics_requirement_projected_yield_total Total
     *
     * The sum of the projected total yield over all reads, including indexed reads.
     */

    /** This generator creates a contrived run metrics and q-metrics to test specific requirements
     *
     *
     * @see model::metrics::q_metric
     * @note Version 6
     *
     */
    struct q_metric_requirements : metric_test<model::metrics::q_metric, 6>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         * @param run_info run info
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& run_info)
        {
            typedef model::metrics::q_metric::uint_t uint_t;
            model::metrics::q_score_bin bins[] =
            {
                    model::metrics::q_score_bin(2,9,2),
                    model::metrics::q_score_bin(10,19,14),
                    model::metrics::q_score_bin(20,24,21),
                    model::metrics::q_score_bin(25,29,27),
                    model::metrics::q_score_bin(30,34,32),
                    model::metrics::q_score_bin(35,39,36),
                    model::metrics::q_score_bin(40,40,40)
             };
             metrics = model::metrics::q_metric::header_type(util::to_vector(bins));

            const ::uint32_t kValue1 = static_cast< ::uint32_t>(1e7);
            const ::uint32_t kValue2 = static_cast< ::uint32_t>(2e7);
            // >= Q30 -> 50.0
            const ::uint32_t hist[] = {kValue2, kValue2, kValue1, 0, kValue1, kValue2, kValue2};
            // >= Q30 -> 42.85 // 3/7
            const ::uint32_t hist_last_cycle_of_read[] = {kValue2, kValue2, kValue2, kValue1, kValue1, kValue1, kValue1};
            for(size_t cycle=1;cycle<=run_info.total_cycles();++cycle)
            {
                if(run_info.is_last_cycle_of_read(cycle))
                {
                    metrics.insert(
                            model::metrics::q_metric(1, 1101, static_cast<uint_t>(cycle), util::to_vector(hist_last_cycle_of_read)));
                    metrics.insert( model::metrics::q_metric(1, 1102, static_cast<uint_t>(cycle), util::to_vector(hist_last_cycle_of_read)));
                }
                else
                {
                    metrics.insert( model::metrics::q_metric(1, 1101, static_cast<uint_t>(cycle), util::to_vector(hist)));
                    metrics.insert( model::metrics::q_metric(1, 1102, static_cast<uint_t>(cycle), util::to_vector(hist)));
                }
            }
        }
        /** Create the expected run_summary to test the Q-metrics requirements
         *
         * @param summary run summary
         */
        static void create_summary(model::summary::run_summary& summary)
        {
            const size_t tile_count = 2;
            const size_t lane_count = 1;
            const size_t lane_number = 1;
            const size_t surface_count = 2;
            const size_t channel_count = 2;
            const size_t cycles_per_read = 6;
            const float kExpectedPercentGreaterThanOrEqualToQ30 = 50.0f;// 7 bins were half the data >= Q30
            const float kExpectedReadYield = 1.0f; // 1e9 / 1e9


            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, cycles_per_read),
                    model::run::read_info(2, cycles_per_read+1, cycles_per_read*2)
            };
            const size_t read_count = util::length_of(reads);
            const size_t total_cycles = cycles_per_read*read_count;
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);

            for(size_t read_index=0;read_index < read_count;++read_index)
            {
                summary[read_index][0].lane(lane_number);
                summary[read_index][0].tile_count(tile_count);
                summary[read_index][0][0].tile_count(tile_count);
                // TODO: Describe?
                summary[read_index][0].cycle_state().qscored_cycle_range(
                        model::run::cycle_range(cycles_per_read, cycles_per_read));
                summary[read_index].summary().percent_gt_q30(kExpectedPercentGreaterThanOrEqualToQ30);
                summary[read_index][0].percent_gt_q30(kExpectedPercentGreaterThanOrEqualToQ30);
                summary[read_index][0][0].percent_gt_q30(kExpectedPercentGreaterThanOrEqualToQ30);
                summary[read_index][0].yield_g(kExpectedReadYield);
                summary[read_index][0][0].yield_g(kExpectedReadYield);
                summary[read_index].summary().yield_g(kExpectedReadYield);
                summary[read_index].summary().projected_yield_g(kExpectedReadYield);

                // TODO: Describe?
                summary[read_index][0].projected_yield_g(kExpectedReadYield);
                summary[read_index][0][0].projected_yield_g(kExpectedReadYield);
            }
            summary.nonindex_summary().percent_gt_q30(kExpectedPercentGreaterThanOrEqualToQ30);
            summary.total_summary().percent_gt_q30(kExpectedPercentGreaterThanOrEqualToQ30);
            summary.nonindex_summary().yield_g(kExpectedReadYield*read_count);
            summary.total_summary().yield_g(kExpectedReadYield*read_count);
            summary.total_summary().projected_yield_g(kExpectedReadYield*read_count);

            summary.nonindex_summary().projected_yield_g(kExpectedReadYield*read_count);
            // TODO: Describe?
            summary.cycle_state().qscored_cycle_range(model::run::cycle_range(total_cycles, total_cycles));
        }
    };

}}}

