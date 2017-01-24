/** Fixtures for all the metric formats
 *
 *
 *  @file
 *  @date 9/21/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/corrected_intensity_metrics_test.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"
#include "src/tests/interop/metrics/inc/extraction_metrics_test.h"
#include "src/tests/interop/metrics/inc/image_metrics_test.h"
#include "src/tests/interop/metrics/inc/index_metrics_test.h"
#include "src/tests/interop/metrics/inc/phasing_metrics_test.h"
#include "src/tests/interop/metrics/inc/q_collapsed_metrics_test.h"
#include "src/tests/interop/metrics/inc/q_metrics_test.h"
#include "src/tests/interop/metrics/inc/tile_metrics_test.h"

namespace illumina{ namespace interop { namespace unittest
{


    typedef ::testing::Types
            <
                    corrected_intensity_metric_v2,
                    corrected_intensity_metric_v3,
                    corrected_intensity_metric_v4,
                    error_metric_v3,
                    error_metric_v4,
                    extraction_metric_v2,
                    extraction_metric_v3,
                    image_metric_v1,
                    image_metric_v2,
                    image_metric_v3,
                    index_metric_v1,
                    index_metric_v2,
                    phasing_metric_v1,
                    phasing_metric_v2,
                    q_collapsed_metric_v2,
                    q_collapsed_metric_v3,
                    q_collapsed_metric_v4,
                    q_collapsed_metric_v5,
                    q_collapsed_metric_v6,
                    q_collapsed_metric_no_median_v2,
                    q_collapsed_metric_no_median_v5,
                    q_metric_v4,
                    q_metric_v5,
                    q_metric_v6,
                    q_metric_v6_unbinned,
                    q_metric_v7,
                    tile_metric_v2,
                    tile_metric_v3
            > PublicFormats;


}}}

