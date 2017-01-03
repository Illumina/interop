/** Unit tests for the q-metrics
 *
 *
 *  @file
 *  @date 5/18/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "src/tests/interop/metrics/inc/metric_test.h"
#include "interop/model/metrics/q_collapsed_metric.h"
#include "interop/util/length_of.h"


namespace illumina{ namespace interop { namespace unittest 
{

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_collapsed_metric
     */
    template<int Version>
    struct q_collapsed_metric_v_2_4 : metric_test<model::metrics::q_collapsed_metric, Version>
    {
        /** Define a parent type */
        typedef metric_test<model::metrics::q_collapsed_metric, Version> parent_t;
        /** Define a metric set type */
        typedef typename parent_t::metric_set_t metric_set_t;
        /** Define a metric type */
        typedef typename parent_t::metric_t metric_t;
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(parent_t::VERSION);
            metrics.insert(metric_t(1,1105,1,2447414,2334829,2566750,33));
            metrics.insert(metric_t(1,1103,1,2436317,2327796,2543605,33));
            metrics.insert(metric_t(1,1106,1,2474217,2366046,2583629,33));
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
                    Version,22
                    ,1,0,81,4,1,0,54,88,37,0,109,-96,35,0,94,42,39,0,0,0,4,66
                    ,1,0,79,4,1,0,-35,44,37,0,-12,-124,35,0,-11,-49,38,0,0,0,4,66
                    ,1,0,82,4,1,0,-23,-64,37,0,94,26,36,0,77,108,39,0,0,0,4,66
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };
    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_collapsed_metric
     * @note Version 2
     */
    struct q_collapsed_metric_v2 : q_collapsed_metric_v_2_4<2>{};
    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_collapsed_metric
     * @note Version 3
     */
    struct q_collapsed_metric_v3 : q_collapsed_metric_v_2_4<3>{};
    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_collapsed_metric
     * @note Version 4
     */
    struct q_collapsed_metric_v4 : q_collapsed_metric_v_2_4<4>{};


    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_collapsed_metric
     * @note Version 6
     */
    template<int Version>
    struct q_collapsed_metric_v5_6 : metric_test<model::metrics::q_collapsed_metric, Version>
    {
        /** Define a parent type */
        typedef metric_test<model::metrics::q_collapsed_metric, Version> parent_t;
        /** Define a metric set type */
        typedef typename parent_t::metric_set_t metric_set_t;
        /** Define a metric type */
        typedef typename parent_t::metric_t metric_t;
        enum{
            /** Do not check the expected binary data */
            disable_binary_data=true,
            /** Do not check the expected binary data size */
            disable_binary_data_size=true
        };
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            typedef typename metric_set_t::header_type header_t;
            typedef typename header_t::qscore_bin_vector_type qscore_bin_vector_type;
            typedef typename header_t::bin_t bin_t;
            typedef typename bin_t::bin_type ushort_t;
            typedef typename metric_t::uint_t uint_t;
            const uint_t bin_count = 7;

            const ushort_t lower[] = {2, 10, 20, 25, 30, 35, 40};
            const ushort_t upper[] = {9, 19, 24, 29, 34, 39, 40};
            const ushort_t value[] = {2, 14, 21, 27, 32, 36, 40};
            qscore_bin_vector_type headervec;
            for(uint_t i=0;i<bin_count;i++)
                headervec.push_back(bin_t(lower[i], upper[i], value[i]));

            metrics = metric_set_t(header_t(headervec), parent_t::VERSION);
            metrics.insert(metric_t(1,1105,1,2447414,2334829,2566750,33));
            metrics.insert(metric_t(1,1103,1,2436317,2327796,2543605,33));
            metrics.insert(metric_t(1,1106,1,2474217,2366046,2583629,33));
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
                    Version,22,1,7,2,10,20,25,30,35,40,9,19,24,29,34,39,40,2,14,21,27,32,36,40
                    ,1,0,81,4,1,0,54,88,37,0,109,-96,35,0,94,42,39,0,0,0,4,66
                    ,1,0,79,4,1,0,-35,44,37,0,-12,-124,35,0,-11,-49,38,0,0,0,4,66
                    ,1,0,82,4,1,0,-23,-64,37,0,94,26,36,0,77,108,39,0,0,0,4,66
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };
    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_collapsed_metric
     * @note Version 5
     */
    struct q_collapsed_metric_v5 : q_collapsed_metric_v5_6<5>{};
    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::q_collapsed_metric
     * @note Version 6
     */
    struct q_collapsed_metric_v6 : q_collapsed_metric_v5_6<6>{};


}}}

