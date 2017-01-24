/** Unit tests for the phasing metrics
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
#include "interop/model/metrics/phasing_metric.h"
#include "interop/model/summary/run_summary.h"


namespace illumina{ namespace interop { namespace unittest
{

    /** This test writes three records of an InterOp files, then reads them back in and compares
     * each value to ensure they did not change.
     *
     * @see model::metrics::phasing_metric
     */
    struct phasing_metric_v1 : metric_test<model::metrics::phasing_metric, 1>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t &metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(VERSION);
            metrics.insert(metric_t(4, 1116, 2, 1.16406322f, 1.92952883f));
            metrics.insert(metric_t(4, 1116, 3, 0.582031608f, 2.12364316f));
            metrics.insert(metric_t(4, 1116, 4, 0.291015804f, 2.20452404f)); // TODO add 25 cycles of data
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
                1,14,
                4,0,92,4,2,0,6,0,149,63,205,250,246,63,
                4,0,92,4,3,0,6,0,21,63,197,233,7,64,
                4,0,92,4,4,0,6,0,149,62,236,22,13,64
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
                    model::run::read_info(1, 1, 4, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary[0][0].lane(4);
            summary[0][0][0].tile_count(1);
            summary[0][0].tile_count(1);
        }
    };

    /** This test writes three records of an InterOp files, then reads them back in and compares
     * each value to ensure they did not change.
     *
     * @see model::metrics::phasing_metric
     */
    struct phasing_metric_v2 : metric_test<model::metrics::phasing_metric, 2>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t &metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(VERSION);
            metrics.insert(metric_t(4, 1116, 2, 1.16406322f, 1.92952883f));
            metrics.insert(metric_t(4, 1116, 3, 0.582031608f, 2.12364316f));
            metrics.insert(metric_t(4, 1116, 4, 0.291015804f, 2.20452404f));
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
                2,16,4,0,92,4,0,0,2,0,6,0,-107,63,-51,-6,-10,63,4,0,92,4,0
                ,0,3,0,6,0,21,63,-59,-23,7,64,4,0,92,4,0,0,4,0,6,0,-107,62
                ,-20,22,13,64
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };

}}}

