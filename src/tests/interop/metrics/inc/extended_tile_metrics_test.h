/** Unit tests for the extended tile metrics
 *
 *
 *  @file
 *  @date 3/11/16
 *  @version 1.0
 *  @copyright GNU Public License
 */
#pragma once
#include <gtest/gtest.h>
#include "metric_test.h"
#include "interop/model/metrics/extended_tile_metric.h"
#include "interop/model/summary/run_summary.h"


namespace illumina{ namespace interop { namespace unittest 
{

    /** This test writes three records of an InterOp files, then reads them back in and compares
     * each value to ensure they did not change.
     *
     * @see model::metrics::extended_tile_metric
     * @note Version 1
     */
    struct extended_tile_metric_v1 : metric_test<model::metrics::extended_tile_metric, 1>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t &metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(VERSION);
            metrics.insert(metric_t(7, 1114, 6309736));
            metrics.insert(metric_t(7, 1214, 6260919));
            metrics.insert(metric_t(7, 2114, 6232864));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection &buffer)
        {
            const int tmp[] =
            {
                1,10,
                7,0,90,4,0,0,208,142,192,74,
                7,0,190,4,0,0,110,17,191,74,
                7,0,66,8,0,0,64,54,190,74
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
        /** Get the summary for these metrics
         *
         * @return run summary
         */
        static void create_summary(model::summary::run_summary& summary, const bool =false)
        {
            const size_t lane_count = 1;
            const size_t surface_count = 2;
            const size_t channel_count = 2;
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 3, false),
                    model::run::read_info(2, 1, 3, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary[0][0].lane(7);
            summary[0][0][0].tile_count(2);
            summary[0][0][1].tile_count(1);
            summary[0][0].tile_count(3);

            const float occupied_mean = 96.8612060546875f;
            summary.total_summary().percent_occupied(occupied_mean);
            summary.nonindex_summary().percent_occupied(occupied_mean);
            for(size_t read_index=0;read_index<2;++read_index)
            {
                summary[read_index].summary().percent_occupied(occupied_mean);
                summary[read_index][0].percent_occupied(model::summary::metric_stat(96.8612060546875f, 0.60115677118301392f, 96.754257202148438f));
                summary[read_index][0][0].percent_occupied(model::summary::metric_stat(97.131454467773438f, 0.53344285488128662f, 97.131454467773438f));
                summary[read_index][0][1].percent_occupied(model::summary::metric_stat(96.320701599121094f, 0.0f, 96.320701599121094f));
            }
        }
    };
    /** This test writes three records of an InterOp files, then reads them back in and compares
     * each value to ensure they did not change.
     * 
     * @see model::metrics::extended_tile_metric
     * @note Version 2
     */
    struct extended_tile_metric_v2 : metric_test<model::metrics::extended_tile_metric, 2>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t &metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(VERSION);
            metrics.insert(metric_t(7, 111014, 6309736));
            metrics.insert(metric_t(7, 121014, 6260919));
            metrics.insert(metric_t(7, 211014, 6232864));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection &buffer)
        {
            const int tmp[] =
            {
                2,10,
                7,0,-90,-79,1,0,-48,-114,-64,74,7,0,-74,-40,1,0,110,17
                ,-65,74,7,0,70,56,3,0,64,54,-66,74
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };
    /** This test writes three records of an InterOp files, then reads them back in and compares
     * each value to ensure they did not change.
     *
     * @see model::metrics::extended_tile_metric
     * @note Version 3
     */
    struct extended_tile_metric_v3 : metric_test<model::metrics::extended_tile_metric, 3>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t &metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(VERSION);
            metrics.insert(metric_t(7, 111014, 6309736, model::metrics::point2d(1,2)));
            metrics.insert(metric_t(7, 121014, 6260919, model::metrics::point2d(3,4)));
            metrics.insert(metric_t(7, 211014, 6232864, model::metrics::point2d(5,6)));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection &buffer)
        {
            const int tmp[] =
            {
                3
                ,18,7,0,-90,-79,1,0,-48,-114,-64,74,0,0,-128,63,0,0,0,64,7
                ,0,-74,-40,1,0,110,17,-65,74,0,0,64,64,0,0,-128,64,7,0,70
                ,56,3,0,64,54,-66,74,0,0,-96,64,0,0,-64,64
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };
    
}}}

