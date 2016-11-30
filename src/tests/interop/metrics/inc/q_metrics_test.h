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
        static void create_metric_set(metric_set_t& metrics)
        {
            metrics = metric_set_t(VERSION);

            typedef metric_t::uint_t uint_t;
            typedef sparse_value<uint_t, metric_t::MAX_Q_BINS> q_val;

            const q_val hist1[] = {q_val(14,21208), q_val(21,8227), q_val(26,73051), q_val(32,2339486)};
            const q_val hist2[] = {q_val(14,22647), q_val(21,9570), q_val(26,81839), q_val(32,2413227)};
            const q_val hist3[] = {q_val(14,18878), q_val(21,8168), q_val(26,72634), q_val(32,2342292)};

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
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }/** Get the summary for these metrics
         *
         * @return run summary
         */
        static void create_summary(model::summary::run_summary& summary)
        {
            const size_t lane_count = 1;
            const size_t surface_count = 2;
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 3, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count);
            summary[0][0].tile_count(2);
            summary[0][0].projected_yield_g(0.0098816361278295517);
            summary[0][0].yield_g(0.0074112270958721638f);
            summary[0][0].percent_gt_q30(95.733200073242188f);
            summary[0][0][0].tile_count(2);
            summary[0][0][0].projected_yield_g(0.0098816361278295517);
            summary[0][0][0].yield_g(0.0074112270958721638f);
            summary[0][0][0].percent_gt_q30(95.733200073242188f);
            summary[0][0].cycle_state().qscored_cycle_range(model::run::cycle_range(1, 2));
            summary[0].summary().projected_yield_g(0.0098816361278295517);
            summary[0].summary().yield_g(0.0074112270958721638f);
            summary[0].summary().percent_gt_q30(95.733200073242188f);
            summary.total_summary().percent_gt_q30(95.733200073242188f);
            summary.total_summary().projected_yield_g(0.0098816361278295517);
            summary.total_summary().yield_g(0.0074112270958721638f);
            summary.nonindex_summary().percent_gt_q30(95.733200073242188f);
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
        static void create_metric_set(metric_set_t& metrics)
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
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 2, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count);
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
        static void create_metric_set(metric_set_t& metrics)
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
            std::vector<uint_t> hist_tmp(50, 0);

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
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 4, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count);
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
        static void create_metric_set(metric_set_t& metrics)
        {
            metrics = metric_set_t(VERSION);
            typedef metric_t::uint_t uint_t;

            std::vector<uint_t> hist_tmp(50, 0);

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

}}}

