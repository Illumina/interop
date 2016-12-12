/** Unit tests for the error metrics
 *
 *
 *  @file
 *  @date 3/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "src/tests/interop/metrics/inc/metric_test.h"
#include "interop/model/metrics/error_metric.h"
#include "interop/model/summary/run_summary.h"
#include "interop/util/length_of.h"


namespace illumina { namespace interop { namespace unittest
{

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::error_metric
     * @note Version 3
     */
    struct error_metric_v3 : metric_test<model::metrics::error_metric, 3>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t &metrics, const model::run::info=model::run::info())
        {
            metrics = metric_set_t(VERSION);
            metrics.insert(metric_t(7, 1114, 1, 0.450100899f));
            metrics.insert(metric_t(7, 1114, 2, 0.900201797f));
            metrics.insert(metric_t(7, 1114, 3, 0.465621591f));
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
                    3, 30, 7, 0, 90, 4, 1, 0, -96, 115, -26, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 7, 0, 90, 4, 2, 0, -96, 115, 102, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 90, 4, 3, 0, -12, 101, -18, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }

        /** Get the summary for these metrics
         *
         * @return run summary
         */
        static void create_summary(model::summary::run_summary &summary)
        {
            const size_t lane_count = 1;
            const size_t surface_count = 2;
            const size_t channel_count = 2;
            const model::run::read_info reads[] = {
                    model::run::read_info(1, 1, 3, false)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);
            summary[0][0].lane(7);
            summary[0][0].error_rate(model::summary::metric_stat(0.67515134811401367f, 0, 0.67515134811401367f));
            summary[0][0][0].error_rate(model::summary::metric_stat(0.67515134811401367f, 0, 0.67515134811401367f));
            summary[0][0][0].tile_count(1);
            summary[0][0].error_rate_35(model::summary::metric_stat());
            summary[0][0].error_rate_50(model::summary::metric_stat());
            summary[0][0].error_rate_75(model::summary::metric_stat());
            summary[0][0].error_rate_100(model::summary::metric_stat());
            summary[0][0].cycle_state().error_cycle_range(model::run::cycle_range(3, 3));
            summary[0].summary().error_rate(0.67515134811401367f);
            summary.total_summary().error_rate(0.67515134811401367f);
            summary.nonindex_summary().error_rate(0.67515134811401367f);
            summary[0][0].tile_count(1);
            summary.cycle_state().error_cycle_range(model::run::cycle_range(3, 3));
        }
    };


}}}

