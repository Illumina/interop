/** Unit tests for the image metrics
 *
 *
 *  @file
 *  @date 3/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "src/tests/interop/metrics/inc/metric_test.h"
#include "interop/model/metrics/image_metric.h"
#include "interop/util/length_of.h"


namespace illumina{ namespace interop { namespace unittest 
{

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::image_metric
     * @note Version 1
     */
    struct image_metric_v1 : metric_test<model::metrics::image_metric, 1>
    {
        enum{
            /** Do not check the expected binary data */
            disable_binary_data=true // The order here could change
        };
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(VERSION);
            typedef metric_t::ushort_t ushort_t;
            const ushort_t channel_count = metric_t::MAX_CHANNELS;
            const ushort_t min_contrast1[]  = {896, 1725,738,812};
            const ushort_t min_contrast2[]  = {908, 1770,739,806};
            const ushort_t min_contrast3[]  = {923, 1829,734,802};

            const ushort_t max_contrast1[]  = {4840, 8144,3308,4974};
            const ushort_t max_contrast2[]  = {4829, 8159,3302,4985};
            const ushort_t max_contrast3[]  = {4829, 8236,3304,4947};
            metrics.insert(metric_t(1, 1101, 1, channel_count, to_vector(min_contrast1), to_vector(max_contrast1)));
            metrics.insert(metric_t(1, 1102, 1, channel_count, to_vector(min_contrast2), to_vector(max_contrast2)));
            metrics.insert(metric_t(1, 1103, 1, channel_count, to_vector(min_contrast3), to_vector(max_contrast3)));
        }
        /** Get the expected binary data
         *
         * @param buffer binary data string
         */
        template<class Collection>
        static void create_binary_data(Collection& buffer)
        {
            const int tmp[] =
            {1,12,
             1,0,77,4,1,0,0,0,128,3,232,18,
             1,0,77,4,1,0,1,0,189,6,208,31,
             1,0,77,4,1,0,2,0,226,2,236,12,
             1,0,77,4,1,0,3,0,44,3,110,19,
             1,0,78,4,1,0,0,0,140,3,221,18,
             1,0,78,4,1,0,1,0,234,6,223,31,
             1,0,78,4,1,0,2,0,227,2,230,12,
             1,0,78,4,1,0,3,0,38,3,121,19,
             1,0,79,4,1,0,0,0,155,3,221,18,
             1,0,79,4,1,0,1,0,37,7,44,32,
             1,0,79,4,1,0,2,0,222,2,232,12,
             1,0,79,4,1,0,3,0,34,3,83,19
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };

    /** This generator creates an expected metric set and the corresponding binary data
     *
     * @see model::metrics::image_metric
     * @note Version 2
     */
    struct image_metric_v2 : metric_test<model::metrics::image_metric, 2>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         */
        static void create_expected(metric_set_t& metrics, const model::run::info& =model::run::info())
        {
            metrics = metric_set_t(header_t(2), VERSION);
            typedef metric_t::ushort_t ushort_t;
            const ushort_t channel_count = metrics.channel_count();
            const ushort_t min_contrast1[]  = {231, 207};
            const ushort_t min_contrast2[]  = {229, 205};
            const ushort_t min_contrast3[]  = {231, 222};

            const ushort_t max_contrast1[]  = {462, 387};
            const ushort_t max_contrast2[]  = {457, 387};
            const ushort_t max_contrast3[]  = {473, 416};
            metrics.insert(metric_t(7, 1114, 1, channel_count, to_vector(min_contrast1), to_vector(max_contrast1)));
            metrics.insert(metric_t(7, 1214, 1, channel_count, to_vector(min_contrast2), to_vector(max_contrast2)));
            metrics.insert(metric_t(7, 2114, 1, channel_count, to_vector(min_contrast3), to_vector(max_contrast3)));
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
                2,14,2
                ,7,0,90,4,1,0,-25,0,-49,0,-50,1,-125,1
                ,7,0,-66,4,1,0,-27,0,-51,0,-55,1,-125,1
                ,7,0,66,8,1,0,-25,0,-34,0,-39,1,-96,1
            };
            buffer.assign(tmp, tmp+util::length_of(tmp));
        }
    };

}}}

