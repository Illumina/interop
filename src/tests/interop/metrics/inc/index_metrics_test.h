/** Unit tests for the index metrics
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
#include "interop/model/metrics/index_metric.h"
#include "interop/model/run/info.h"
#include "interop/model/summary/index_flowcell_summary.h"


namespace illumina{ namespace interop { namespace unittest {

    /** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
     * to the values displayed in SAV.
     *
     * This metric was artificially changed to give it three unique records. The lane was changed from 1 to 2 and 3
     * for the second and third records.
     *
     * @note Version 1
     */
    struct index_v1 : metric_test<model::metrics::index_metric, 1>
    {
        enum{
            /** Do not check record size (does not have it)*/
            disable_check_record_size=true,
            /** Do not check the expected binary data size */
            disable_binary_data_size=true
        };
        /** Build the expected metric set
         *
         * @return vector of metrics
         */
        static std::vector<metric_t> metrics()
        {
            std::vector<metric_t> expected_metrics;
            typedef metric_t::index_info_t index_info_t;


            metric_t::index_array_t indices1;
            indices1.push_back(index_info_t("ATCACGAC-AAGGTTCA", "1", "TSCAIndexes", 4570));
            expected_metrics.push_back(metric_t(1, 12106, 3, indices1));
            metric_t::index_array_t indices2;
            indices2.push_back(index_info_t("ACAGTGGT-AAGGTTCA", "2", "TSCAIndexes", 4477));
            expected_metrics.push_back(metric_t(2, 12106, 3, indices2));
            metric_t::index_array_t indices3;
            indices3.push_back(index_info_t("CAGATCCA-AAGGTTCA", "3", "TSCAIndexes", 4578));
            expected_metrics.push_back(metric_t(3, 12106, 3, indices3));

            return expected_metrics;
        }
        /** Get the expected metric set header
         *
         * @return expected metric set header
         */
        static header_t header()
        {
            return header_t();
        }
        /** Get the expected binary data
         *
         * @return binary data string
         */
        static std::string binary_data()
        {
            const int tmp[] = {1
                    ,1,0,74,47,3,0
                    ,17,0,65,84,67,65,67,71,65,67,45,65,65,71,71,84,84,67,65,218,17,0,0,1,0,49,11,0,84,83,67,65,73,110
                    ,100,101,120,101,115
                    ,2,0,74,47,3,0 // changed to lane 2
                    ,17,0,65,67,65,71,84,71,71,84,45,65,65,71,71,84,84,67,65,125,17,0,0,1,0,50,11,0,84,83,67,65,73,110
                    ,100,101,120,101,115
                    ,3,0,74,47,3,0 // changed to lane 3
                    ,17,0,67,65,71,65,84,67,67,65,45,65,65,71,71,84,84,67,65,226,17,0,0,1,0,51,11,0,84,83,67,65,73,110
                    ,100,101,120,101,115
            };
            return to_string(tmp);
        }
        /** Get the expected run info
         *
         * @return expected run info
         */
        static model::run::info run_info()
        {
            std::vector<model::run::read_info> reads(1, model::run::read_info(1, 1, 3, false));
            std::vector<std::string> channels;
            channels.push_back("Red");
            channels.push_back("Green");
            model::run::info run_info("XX",
                                      "",
                                      1,
                                      model::run::flowcell_layout(8),
                                      channels,
                                      model::run::image_dimensions(),
                                      reads);
            return run_info;
        }
        /** Get the expected index summary
         *
         * @return expected index summary
         */
        static model::summary::index_flowcell_summary summary()
        {
            const float NaN = std::numeric_limits<float>::quiet_NaN();
            model::summary::index_flowcell_summary index_summary(8);
            for(size_t lane = 0;lane<index_summary.size();++lane)
                index_summary[lane] = model::summary::index_lane_summary(0, 0, 0, NaN);
            return index_summary;
        }

    };

    /** Interface between fixtures and Google Test */
    template<typename TestSetup>
    struct index_metrics_test : public ::testing::Test, public TestSetup { };
}}}
