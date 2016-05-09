/** Unit tests for the tile metrics
 *
 *
 *  @file
 *  @date 3/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <gtest/gtest.h>
#include "metric_test.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/summary/run_summary.h"


namespace illumina{ namespace interop { namespace unittest {

    /** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
     * to the values displayed in SAV.
     *
     * @note Version 2
     */
    struct tile_v2 : metric_test<model::metrics::tile_metric, 2>
    {
        enum{
            /** Do not check the expected binary data */
            disable_binary_data=true,
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


            metric_t::read_metric_vector reads1;
            reads1.push_back(metric_t::read_metric_type(1, 2.61630869f, 0.0797112584f, 0.119908921f));
            reads1.push_back(metric_t::read_metric_type(2, 2.61630869f, 0.0797112584f, 0.119908921f));

            expected_metrics.push_back(metric_t(7, 1114, 2355119.25f,1158081.50f,6470949,3181956,reads1));
            expected_metrics.push_back(metric_t(7, 1214, 2355119.25f,1174757.75f,6470949,3227776,
                                                metric_t::read_metric_vector(1, metric_t::read_metric_type(1, 2.62243795f, 0.129267812f, 0.135128692f))));
            expected_metrics.push_back(metric_t(7, 2114, 2355119.25f,1211592.38f,6470949,3328983,
                                                metric_t::read_metric_vector(1, metric_t::read_metric_type(1, 2.490309f, 0.11908555f, 0.092706576f))));

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
            const int tmp[] = {
                    2,10
                    ,7,0,90,4,100,0,-67,-66,15,74
                    ,7,0,90,4,102,0,74,122,-59,74
                    ,7,0,90,4,101,0,12,94,-115,73
                    ,7,0,90,4,103,0,16,54,66,74
                    ,7,0,90,4,-56,0,82,-11,80,58
                    ,7,0,90,4,-55,0,-62,42,-99,58
                    ,7,0,90,4,44,1,-102,113,39,64
                    ,7,0,90,4,-54,0,82,-11,80,58
                    ,7,0,90,4,-53,0,-62,42,-99,58
                    ,7,0,90,4,45,1,-102,113,39,64
                    ,7,0,90,4,-56,0,82,-11,80,58
                    ,7,0,90,4,-55,0,-62,42,-99,58
                    ,7,0,90,4,44,1,-102,113,39,64
                    ,7,0,-66,4,100,0,-67,-66,15,74
                    ,7,0,-66,4,102,0,74,122,-59,74
                    ,7,0,-66,4,101,0,46,103,-113,73
                    ,7,0,-66,4,103,0,0,2,69,74
                    ,7,0,-66,4,-56,0,21,111,-87,58
                    ,7,0,-66,4,-55,0,-86,29,-79,58
                    ,7,0,-66,4,44,1,6,-42,39,64
                    ,7,0,66,8,100,0,-67,-66,15,74
                    ,7,0,66,8,102,0,74,122,-59,74
                    ,7,0,66,8,101,0,67,-26,-109,73
                    ,7,0,66,8,103,0,92,47,75,74
                    ,7,0,66,8,-56,0,123,22,-100,58
                    ,7,0,66,8,-55,0,85,6,115,58
                    ,7,0,66,8,44,1,57,97,31,64
                    ,7,0,66,8,144,1,0,0,0,0   // Test whether control lane accidentally clears data
                    ,6,0,66,8,144,1,0,0,0,0   // Test whether control lane for empty tile shows up
            };
            return to_string(tmp);
        }
        /** Get number lanes in data
         *
         * @return 8 lanes
         */
        static model::metric_base::base_metric::uint_t lane_count(){return 8;}
        /** Get reads describing data
         *
         * @return reads vector
         */
        static std::vector<model::run::read_info> reads()
        {
            std::vector<model::run::read_info> reads;
            reads.reserve(2);
            reads.push_back(model::run::read_info(1, 1, 3, false));
            reads.push_back(model::run::read_info(2, 1, 3, false));
            return reads;
        }
        /** Get the summary for these metrics
         *
         * @return run summary
         */
        static model::summary::run_summary summary()
        {
            std::vector<model::run::read_info> read_infos = reads();
            model::summary::run_summary summary(read_infos.begin(), read_infos.end(), lane_count());
            for(size_t read=0;read<read_infos.size();++read)
            {
                summary[read][6].tile_count(3);
                summary[read][6].reads_pf(9738715);
                summary[read][6].reads(19412848);
                summary[read][6].density() = model::summary::metric_stat(2355119.25f, 0, 2355119.25f);
                summary[read][6].density_pf() = model::summary::metric_stat(1181477.125f, 27380.955078125f, 1174757.75f);
                summary[read][6].cluster_count() = model::summary::metric_stat(6470949.5f, 0, 6470949);
                summary[read][6].cluster_count_pf() = model::summary::metric_stat(3246238.25f, 75232.1640625f, 3227776);
                summary[read][6].percent_pf() = model::summary::metric_stat(50.166339874267578f, 1.1626163721084595f, 49.881031036376953f);
            }
            summary[0][6].phasing() = model::summary::metric_stat(0.10935487598180771f, 0.026172075420618057f, 0.11908555030822754f);
            summary[0][6].prephasing() = model::summary::metric_stat(0.1159147247672081f, 0.021491257473826408f, 0.11990892142057419f);
            summary[0][6].percent_aligned() = model::summary::metric_stat(2.5763518810272217f, 0.074578315019607544f, 2.6163086891174316f);

            summary[1][6].phasing() = model::summary::metric_stat(0.079711258411407471f, 0, 0.079711258411407471f);
            summary[1][6].prephasing() = model::summary::metric_stat(0.11990892142057419f, 0, 0.11990892142057419f);
            summary[1][6].percent_aligned() = model::summary::metric_stat(2.6163086891174316, 0, 2.6163086891174316f);

            summary[0].summary().percent_aligned(2.5763518810272217f);
            summary[1].summary().percent_aligned(2.6163086891174316f);
            summary.total_summary().percent_aligned(2.5863409042358398f);
            summary.nonindex_summary().percent_aligned(2.5863409042358398f);

            return summary;
        }
    };

    /** Interface between fixtures and Google Test */
    template<typename TestSetup>
    struct tile_metrics_test : public ::testing::Test, public TestSetup { };
}}}

