/** Unit tests for the tile metrics
 *
 *
 *  @file
 *  @date 3/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "src/tests/interop/metrics/inc/metric_test.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/model/summary/run_summary.h"
#include "interop/util/length_of.h"


namespace illumina{ namespace interop { namespace unittest 
{

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::tile_metric
     * @note Version 2
     */
    struct tile_metric_v2 : metric_test<model::metrics::tile_metric, 2>
    {
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
        static void create_metric_set(metric_set_t& metrics)
        {
            metrics = metric_set_t(VERSION);

            metric_t::read_metric_vector reads1;
            reads1.push_back(metric_t::read_metric_type(1, 2.61630869f, 0.0797112584f, 0.119908921f));
            reads1.push_back(metric_t::read_metric_type(2, 2.61630869f, 0.0797112584f, 0.119908921f));

            metrics.insert(metric_t(7, 1114, 2355119.25f,1158081.50f,6470949,3181956,reads1));
            metrics.insert(metric_t(7, 1214, 2355119.25f,1174757.75f,6470949,3227776,
                                    metric_t::read_metric_vector(1, metric_t::read_metric_type(1, 2.62243795f, 0.129267812f, 0.135128692f))));
            metrics.insert(metric_t(7, 2114, 2355119.25f,1211592.38f,6470949,3328983,
                                    metric_t::read_metric_vector(1, metric_t::read_metric_type(1, 2.490309f, 0.11908555f, 0.092706576f))));
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
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }/** Get the summary for these metrics
         *
         * @return run summary
         */
        static void create_summary(model::summary::run_summary& summary)
        {
            const size_t lane_count = 1;
            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, 3, false),
                    model::run::read_info(2, 1, 3, false)
            };
            summary.initialize(to_vector(reads), lane_count);
            for(size_t read=0;read<summary.size();++read)
            {
                summary[read][0].lane(7);
                summary[read][0].tile_count(3);
                summary[read][0].reads_pf(9738715);
                summary[read][0].reads(19412848);
                summary[read][0].density(model::summary::metric_stat(2355119.25f, 0, 2355119.25f));
                summary[read][0].density_pf(model::summary::metric_stat(1181477.125f, 27380.955078125f, 1174757.75f));
                summary[read][0].cluster_count(model::summary::metric_stat(6470949.5f, 0, 6470949));
                summary[read][0].cluster_count_pf(model::summary::metric_stat(3246238.25f, 75232.1640625f, 3227776));
                summary[read][0].percent_pf(model::summary::metric_stat(50.166339874267578f, 1.1626163721084595f, 49.881031036376953f));
            }
            summary[0][0].phasing(model::summary::metric_stat(0.10935487598180771f, 0.026172075420618057f, 0.11908555030822754f));
            summary[0][0].prephasing(model::summary::metric_stat(0.1159147247672081f, 0.021491257473826408f, 0.11990892142057419f));
            summary[0][0].percent_aligned(model::summary::metric_stat(2.5763518810272217f, 0.074578315019607544f, 2.6163086891174316f));

            summary[1][0].phasing(model::summary::metric_stat(0.079711258411407471f, 0, 0.079711258411407471f));
            summary[1][0].prephasing(model::summary::metric_stat(0.11990892142057419f, 0, 0.11990892142057419f));
            summary[1][0].percent_aligned(model::summary::metric_stat(2.6163086891174316, 0, 2.6163086891174316f));

            summary[0].summary().percent_aligned(2.5763518810272217f);
            summary[1].summary().percent_aligned(2.6163086891174316f);
            summary.total_summary().percent_aligned(2.5863409042358398f);
            summary.nonindex_summary().percent_aligned(2.5863409042358398f);
        }
    };


}}}

