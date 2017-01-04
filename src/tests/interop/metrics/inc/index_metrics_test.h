/** Unit tests for the index metrics
 *
 *
 *  @file
 *  @date 3/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "src/tests/interop/metrics/inc/metric_test.h"
#include "interop/model/metrics/index_metric.h"
#include "interop/model/run/info.h"
#include "interop/model/summary/index_flowcell_summary.h"
#include "interop/util/length_of.h"


namespace illumina{ namespace interop { namespace unittest 
{

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::index_metric
     * @note Version 1
     */
    struct index_metric_v1 : metric_test<model::metrics::index_metric, 1>
    {
        enum{
            /** Do not check record size (does not have it)*/
            disable_check_record_size=true,
            /** Do not check the expected binary data size */
            disable_binary_data_size=true
        };
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            typedef metric_t::index_info_t index_info_t;
            metrics = metric_set_t(VERSION);
            metric_t::index_array_t indices1;
            indices1.push_back(index_info_t("ATCACGAC-AAGGTTCA", "1", "TSCAIndexes", 4570));
            metrics.insert(metric_t(7, 1114, 3, indices1));
            metric_t::index_array_t indices2;
            indices2.push_back(index_info_t("ACAGTGGT-AAGGTTCA", "2", "TSCAIndexes", 4477));
            metrics.insert(metric_t(7, 1214, 3, indices2));
            metric_t::index_array_t indices3;
            indices3.push_back(index_info_t("CAGATCCA-AAGGTTCA", "3", "TSCAIndexes", 4578));
            metrics.insert(metric_t(7, 2114, 3, indices3));
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
                1
                ,7,0,90,4,3,0
                ,17,0,65,84,67,65,67,71,65,67,45,65,65,71,71,84,84,67,65,218,17,0,0,1,0,49,11,0,84,83,67,65,73,110
                ,100,101,120,101,115
                ,7,0,-66,4,3,0
                ,17,0,65,67,65,71,84,71,71,84,45,65,65,71,71,84,84,67,65,125,17,0,0,1,0,50,11,0,84,83,67,65,73,110
                ,100,101,120,101,115
                ,7,0,66,8,3,0
                ,17,0,67,65,71,65,84,67,67,65,45,65,65,71,71,84,84,67,65,226,17,0,0,1,0,51,11,0,84,83,67,65,73,110
                ,100,101,120,101,115
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
        /** Get the summary for these metrics
         *
         * @return index_summary index summary
         */
        static void create_summary(model::summary::index_flowcell_summary& index_summary)
        {
            const float NaN = std::numeric_limits<float>::quiet_NaN();
            index_summary.resize(8);
            for(size_t lane = 0;lane<index_summary.size();++lane)
            {
                if(lane != 6)
                {
                    index_summary[lane] = model::summary::index_lane_summary(0, 0, 0, NaN);
                }
                else
                {
                    index_summary[lane] = model::summary::index_lane_summary(19412847, 9738715, 0.1399f, 0.0118f, 0.046f, 0.047f);
                    index_summary[lane].push_back(model::summary::index_count_summary(1, "ACAGTGGT", "AAGGTTCA", "2", "TSCAIndexes", 4477, 0.046f));
                    index_summary[lane].push_back(model::summary::index_count_summary(2, "ATCACGAC", "AAGGTTCA", "1", "TSCAIndexes", 4570, 0.0469f));
                    index_summary[lane].push_back(model::summary::index_count_summary(3, "CAGATCCA", "AAGGTTCA", "3", "TSCAIndexes", 4578, 0.047f));
                }
            }

        }
    };


}}}

