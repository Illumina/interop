/** Unit tests for the summary run metrics
 *
 *  @file
 *  @date 7/8/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <gtest/gtest.h>
#include "metric_test.h"
#include "interop/model/metrics/summary_run_metric.h"
#include "interop/model/summary/run_summary.h"

namespace illumina{ namespace interop { namespace unittest 
{

    /** This test writes three records of an InterOp files, then reads them back in and compares
     * each value to ensure they did not change.
     * 
     * @see model::metrics::summary_run_metric
     * @note Version 1
     */
    struct summary_run_v1 : metric_test<model::metrics::summary_run_metric, 1>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t &metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(header_t(), VERSION);
            metrics.insert(metric_t(1.0f, 2.0f, 3.0f, 4.0f));// Only supports single metric!
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
                    1
                    ,34,0,0,0,0,0,0,0,0,0,0,0,-16,63,0,0,0,0,0,0
                    ,0,64,0,0,0,0,0,0,8,64,0,0,0,0,0,0,16,64
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };

}}}

