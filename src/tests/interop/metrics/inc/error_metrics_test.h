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
        static void create_expected(metric_set_t &metrics, const model::run::info& =model::run::info())
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

    /**
     * @page error_metrics_requirement Error Metrics
     *
     * The ErrorMetricsOut.bin InterOp file contains a the error in Phi X alignment for each tile on each cycle.
     *
     * @sa illumina::interop::unittest::error_metric_requirements
     *
     * @section error_metrics_requirement_35_lane % Error After 35 Cycles Per Lane
     *
     * The average (+/- the standard deviation) of the Phi X error rate for all tiles in a lane for the first 35
     * cycles of a given read. If an error rate value is not present for any of the first 35 cycles in the read for
     * a given tile, it is excluded from the averaging. If no tiles have any error statistics for a given read, a
     * value of 0 is returned. Note that the base unit for this is the error rate for a single tile - in other words,
     * we average the per-cycle error rate across all the relevant cycles for a tile before calculating these
     * statistics.
     *
     * @section error_metrics_requirement_50_lane % Error After 50 Cycles Per Lane
     *
     * The average (+/- the standard deviation) of the Phi X error rate for all tiles in a lane for the first 50
     * cycles of a given read. If an error rate value is not present for any of the first 50 cycles in the read for
     * a given tile, it is excluded from the averaging. If no tiles have any error statistics for a given read, a
     * value of 0 is returned. Note that the base unit for this is the error rate for a single tile - in other words,
     * we average the per-cycle error rate across all the relevant cycles for a tile before calculating these
     * statistics.
     *
     * @section error_metrics_requirement_75_lane % Error After 75 Cycles Per Lane
     *
     * The average (+/- the standard deviation) of the Phi X error rate for all tiles in a lane for the first 75
     * cycles of a given read. If an error rate value is not present for any of the first 75 cycles in the read for
     * a given tile, it is excluded from the averaging. If no tiles have any error statistics for a given read, a
     * value of 0 is returned. Note that the base unit for this is the error rate for a single tile - in other words,
     * we average the per-cycle error rate across all the relevant cycles for a tile before calculating these
     * statistics.
     *
     * @section error_metrics_requirement_100_lane % Error After 100 Cycles Per Lane
     *
     * The average (+/- the standard deviation) of the Phi X error rate for all tiles in a lane for the first 100
     * cycles of a given read. If an error rate value is not present for any of the first 100 cycles in the read for
     * a given tile, it is excluded from the averaging. If no tiles have any error statistics for a given read, a
     * value of 0 is returned. Note that the base unit for this is the error rate for a single tile - in other words,
     * we average the per-cycle error rate across all the relevant cycles for a tile before calculating these
     * statistics.
     *
     * @section error_metrics_requirement_error % Error
     *
     * @subsection error_metrics_requirement_error_lane Lane
     *
     * The average (+/- the standard deviation) of the Phi X error rate for all tiles in a lane for a given read. If
     * an error rate value is not present for any cycles in the read for a given tile, it is excluded from the
     * averaging. If no tiles have any error statistics for a given read, a value of 0 is returned. Note that the
     * base unit for this is the error rate for a single tile - in other words, we average the per-cycle error
     * rate across all the relevant cycles for a tile before calculating these statistics
     *
     * @subsection error_metrics_requirement_error_read Read
     *
     * The average of the Phi X error rate for all tiles over all lanes for a given read. If an error rate value is
     * not present for any cycles in the read for a given tile, it is excluded from the averaging. If no tiles have
     * any error statistics for a given read, a value of 0 is returned. Note that the base unit for this is the error
     * rate for a single tile - in other words, we average the per-cycle error rate across all the relevant cycles for
     * a tile before calculating these statistics
     *
     * @subsection error_metrics_requirement_error_non_index Non-Indexed Total
     *
     * The average of the Phi X error rate for all tiles over all non-indexed reads. If an error rate value is not
     * present for any cycles in the read for a given tile, it is excluded from the averaging. If no tiles have any
     * error statistics for any non-indexed read, a value of 0 is returned. Note that the base unit for this is the
     * error rate for a single tile - in other words, we average the per-cycle error rate across all the relevant
     * cycles for a tile before calculating these statistics
     *
     * @subsection error_metrics_requirement_error_total Total
     *
     * The average of the Phi X error rate for all tiles over all reads. If an error rate value is not present for
     * any cycles in the read for a given tile, it is excluded from the averaging. If no tiles have any error
     * statistics for any read, a value of 0 is returned. Note that the base unit for this is the error rate for
     * a single tile - in other words, we average the per-cycle error rate across all the relevant cycles for a
     * tile before calculating these statistics
     *
     */

    /** This generator creates a contrived run info and error-metrics to test specific requirements
     *
     * @see model::metrics::error_metric
     * @note Version 3
     */
    struct error_metric_requirements : metric_test<model::metrics::error_metric, 3>
    {
        /** Create the expected metric set
         *
         * @param metrics destination metric set
         * @param run_info run info
         */
        static void create_expected(metric_set_t &metrics, const model::run::info& run_info)
        {
            metrics = metric_set_t(VERSION);

            const float kMissingValue = std::numeric_limits<float>::quiet_NaN();
            const float kAverageError35 = 1.0f;
            const float kAverageError50 = 4.0f;
            const float kAverageError75 = 16.0f;
            const float kAverageError100 = 26.0f;
            const float kAverageErrorAfter = 36.0f;

            float sum = 0.0f;
            const size_t cycle_count = run_info.total_cycles();
            for (size_t cycle = 1; cycle <= cycle_count; ++cycle)
            {
                const size_t cycle_within_read = run_info.cycle_within_read(cycle);
                const metric_t::uint_t cycle_id = static_cast<metric_t::uint_t >(cycle);
                metrics.insert(metric_t(7, 1113, cycle_id, kMissingValue));
                if( cycle_within_read <= 5 )
                {
                    //Doing nothing is the same as adding a record with the missing value sentinel
                    //metrics.insert(metric_t(7, 1114, cycle, kMissingValue));
                }
                else if( cycle_within_read <= 35 )
                {
                    metrics.insert(metric_t(7, 1114, cycle_id, kAverageError35));
                    sum += kAverageError35;
                }
                else if( cycle_within_read <= 50 )
                {
                    metrics.insert(metric_t(7, 1114, cycle_id, kAverageError50));
                    sum += kAverageError35;
                }
                else if( cycle_within_read <= 75 )
                {
                    metrics.insert(metric_t(7, 1114, cycle_id, kAverageError75));
                }
                else if( cycle_within_read <= 100 )
                {
                    metrics.insert(metric_t(7, 1114, cycle_id, kAverageError100));
                }
                else
                {
                    metrics.insert(metric_t(7, 1114, cycle_id, kAverageErrorAfter));
                }
            }
        }

        /** Create the expected run_summary to test the error metrics requirements
         *
         * @param summary run summary
         */
        static void create_summary(model::summary::run_summary &summary)
        {
            const size_t lane_count = 1;
            const size_t surface_count = 2;
            const size_t channel_count = 2;
            const size_t cycles_per_read = 126;

            const model::run::read_info reads[]={
                    model::run::read_info(1, 1, cycles_per_read),
                    model::run::read_info(2, cycles_per_read+1, cycles_per_read*2)
            };
            summary.initialize(to_vector(reads), lane_count, surface_count, channel_count);

            const size_t read_count = util::length_of(reads);
            const size_t total_cycles = cycles_per_read*read_count;
            const size_t tile_count = 2;
            const size_t lane_number = 7;
            const float kReadError35 = 1.0f;
            const float kReadError50 = 2.0f;
            const float kReadError75 = 7.0f;
            const float kReadError100 = 12.0f;
            const float kReadError = 17.0f;
            const float kSigma = 0.0f;

            for(size_t read_index=0;read_index < read_count;++read_index)
            {
                summary[read_index][0].lane(lane_number);
                summary[read_index][0].tile_count(tile_count);
                summary[read_index][0][0].tile_count(tile_count);
                summary[read_index][0].cycle_state().error_cycle_range(
                        model::run::cycle_range(cycles_per_read, cycles_per_read));

                // Lane Error rate
                summary[read_index][0].error_rate_35(model::summary::metric_stat(kReadError35,kSigma,kReadError35));
                summary[read_index][0][0].error_rate_35(model::summary::metric_stat(kReadError35,kSigma,kReadError35));

                summary[read_index][0].error_rate_50(model::summary::metric_stat(kReadError50,kSigma,kReadError50));
                summary[read_index][0][0].error_rate_50(model::summary::metric_stat(kReadError50,kSigma,kReadError50));

                summary[read_index][0].error_rate_75(model::summary::metric_stat(kReadError75,kSigma,kReadError75));
                summary[read_index][0][0].error_rate_75(model::summary::metric_stat(kReadError75,kSigma,kReadError75));

                summary[read_index][0].error_rate_100(model::summary::metric_stat(kReadError100,kSigma,kReadError100));
                summary[read_index][0][0].error_rate_100(model::summary::metric_stat(kReadError100,kSigma,kReadError100));

                summary[read_index][0].error_rate(model::summary::metric_stat(kReadError,kSigma,kReadError));
                summary[read_index][0][0].error_rate(model::summary::metric_stat(kReadError,kSigma,kReadError));

                // Read Error Rate
                summary[read_index].summary().error_rate(kReadError);
            }
            summary.nonindex_summary().error_rate(kReadError);
            summary.total_summary().error_rate(kReadError);
            summary.cycle_state().error_cycle_range(model::run::cycle_range(total_cycles, total_cycles));
        }
    };

}}}

