/** Unit tests for the extraction metrics
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
#include "interop/model/metrics/extraction_metric.h"
#include "interop/model/summary/run_summary.h"
#include "interop/util/length_of.h"


namespace illumina{ namespace interop { namespace unittest 
{

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::extraction_metric
     * @note Version 2
     */
    struct extraction_metric_v2 : metric_test<model::metrics::extraction_metric, 2>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics)
        {
            metrics = metric_set_t(VERSION);

            typedef metric_t::ushort_t ushort_t;
            const float focus1[] = {2.24664021f, 2.1896739f, 0, 0};
            const ushort_t p90_1[]  = {302, 273, 0, 0};
            metrics.insert(metric_t(7, 1114, 1, interop::util::csharp_date_time(9859129975844165472ul), to_vector(p90_1), to_vector(focus1)));
            const float focus2[] = {2.23177338f, 2.20616174f, 0, 0};
            const ushort_t p90_2[]  = {312, 273, 0, 0};
            metrics.insert(metric_t(7, 1214, 1, interop::util::csharp_date_time(9859129975872781680ul), to_vector(p90_2), to_vector(focus2)));
            const float focus3[] = {2.10524225f, 2.14023066f, 0, 0};
            const ushort_t p90_3[]  = {349, 302, 0, 0};
            metrics.insert(metric_t(7, 2114, 1, interop::util::csharp_date_time(9859129975901427921ul), to_vector(p90_3), to_vector(focus3)));

        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection& buffer)
        {
            static const int tmp[] =
            {
                2,38,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
                ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
                ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120
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
                    model::run::read_info(1, 1, 2, false)
            };
            summary.initialize(to_vector(reads), lane_count);
            summary[0][0].lane(7);
            summary[0][0].first_cycle_intensity(model::summary::metric_stat(321, 24.75883674621582f, 312));
            summary[0][0].cycle_state().extracted_cycle_range(model::run::cycle_range(1, 1));
            summary[0].summary().first_cycle_intensity(321);
            summary.total_summary().first_cycle_intensity(321);
            summary.nonindex_summary().first_cycle_intensity(321);
            summary[0][0].tile_count(3);
            summary.cycle_state().extracted_cycle_range(model::run::cycle_range(1, 1));
        }
    };
    
    /** This test compares byte values taken from an InterOp file for three records produced by RTA 2.7.x
     * to the values displayed in SAV.
     *
     * @note Version 2
     */
    struct extraction_v2 : metric_test<model::metrics::extraction_metric, 2>
    {
        /** Build the expected metric set
         *
         * @return vector of metrics
         */
        static std::vector<metric_t> metrics()
        {
            std::vector<metric_t> expected_metrics;


            typedef metric_t::ushort_t ushort_t;
            const float focus1[] = {2.24664021f, 2.1896739f, 0, 0};
            const ushort_t p90_1[]  = {302, 273, 0, 0};
            expected_metrics.push_back(metric_t(7, 1114, 1, interop::util::csharp_date_time(9859129975844165472ul), to_vector(p90_1), to_vector(focus1)));
            const float focus2[] = {2.23177338f, 2.20616174f, 0, 0};
            const ushort_t p90_2[]  = {312, 273, 0, 0};
            expected_metrics.push_back(metric_t(7, 1214, 1, interop::util::csharp_date_time(9859129975872781680ul), to_vector(p90_2), to_vector(focus2)));
            const float focus3[] = {2.10524225f, 2.14023066f, 0, 0};
            const ushort_t p90_3[]  = {349, 302, 0, 0};
            expected_metrics.push_back(metric_t(7, 2114, 1, interop::util::csharp_date_time(9859129975901427921ul), to_vector(p90_3), to_vector(focus3)));

            return expected_metrics;
        }
        /** Get the expected metric set header
         *
         * @return expected metric set header
         */
        static header_t header()
        {
            return header_t::default_header();
        }
        /** Get the expected binary data
         *
         * @return binary data string
         */
        static std::string binary_data()
        {
            const int tmp[] = {2,38,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
                    ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
                    ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120
            };
            return to_string(tmp);
        }
        /** Get reads describing data
         *
         * @return reads vector
         */
        static std::vector<model::run::read_info> reads()
        {
            std::vector<model::run::read_info> reads(1, model::run::read_info(1, 1, 2, false));
            return reads;
        }
        /** Get the summary for these metrics
         *
         * @return run summary
         */
        static model::summary::run_summary summary()
        {
            const size_t lane_count=1;
            std::vector<model::run::read_info> read_infos = reads();
            model::summary::run_summary summary(read_infos.begin(), read_infos.end(), lane_count);
            summary[0][0].lane(7);
            summary[0][0].first_cycle_intensity(model::summary::metric_stat(321, 24.75883674621582f, 312));
            summary[0][0].cycle_state().extracted_cycle_range(model::run::cycle_range(1, 1));
            summary[0].summary().first_cycle_intensity(321);
            summary.total_summary().first_cycle_intensity(321);
            summary.nonindex_summary().first_cycle_intensity(321);
            summary[0][0].tile_count(3);
            summary.cycle_state().extracted_cycle_range(model::run::cycle_range(1, 1));
            return summary;
        }
    };

    /** Interface between fixtures and Google Test */
    template<typename TestSetup>
    struct extraction_metrics_test : public ::testing::Test, public TestSetup { };
}}}
