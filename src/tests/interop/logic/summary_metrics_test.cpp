/** Unit tests for the summary metrics
 *
 *
 *  @file
 *  @date 3/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <fstream>
#include <gtest/gtest.h>
#include "interop/util/math.h"
#include "interop/logic/summary/run_summary.h"
#include "interop/logic/utils/channel.h"
#include "src/tests/interop/metrics/inc/corrected_intensity_metrics_test.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"
#include "src/tests/interop/metrics/inc/extraction_metrics_test.h"
#include "src/tests/interop/metrics/inc/tile_metrics_test.h"
#include "src/tests/interop/metrics/inc/q_metrics_test.h"
#include "src/tests/interop/inc/abstract_regression_test_generator.h"
using namespace illumina::interop::model::summary;
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Setup for tests that compare two run summaries */
struct run_summary_tests : public generic_test_fixture<model::summary::run_summary> {};


/** Check if two stats are nearly the same. If both are NaN, then this check succeeds
 *
 * @todo Use this everywhere
 *
 * @param expected expected stat
 * @param actual actual stat
 * @param tol tolerance
 * @return true if both numbers hold the same value, or their difference is less than tolerance
 */
::testing::AssertionResult AreStatsNear(const metric_stat& expected, const metric_stat& actual, const float tol)
{
    ::testing::Message msg;
    bool test_failed = false;
    if(!is_float_near(expected.mean(), actual.mean(), tol))
    {
        msg << "Mean Expected: " << expected.mean() << " == Actual: " << actual.mean();
        test_failed=true;
    }
    if(!is_float_near(expected.stddev(), actual.stddev(), tol))
    {
        if(test_failed) msg << " | ";
        msg << "StdDev Expected: " << expected.stddev() << " == Actual: " << actual.stddev();
        test_failed=true;
    }
    if(!is_float_near(expected.median(), actual.median(), tol))
    {
        if(test_failed) msg << " | ";
        msg << "Median Expected: " << expected.median() << " == Actual: " << actual.median();
        test_failed=true;
    }
    if(test_failed) return ::testing::AssertionFailure(msg << " Tol: " << tol);
    return ::testing::AssertionSuccess();
}

/** Check if two cycle states are the same.
 *
 * @todo Use this everywhere
 *
 * @param expected expected cycle state
 * @param actual actual cycle state
 * @return true if both are equal
 */
::testing::AssertionResult AreCycleStatesNear(const model::run::cycle_range& expected,
                                              const model::run::cycle_range& actual)
{
    ::testing::Message msg;
    bool test_failed = false;
    if(expected.first_cycle() != actual.first_cycle())
    {
        msg << "First Cycle Expected: " << expected.first_cycle() << " == Actual: " << actual.first_cycle();
        test_failed = true;
    }
    if(expected.last_cycle() != actual.last_cycle())
    {
        if(test_failed) msg << " | ";
        msg << "Last Cycle Expected: " << expected.last_cycle() << " == Actual: " << actual.last_cycle();
        test_failed = true;
    }
    if(test_failed) return ::testing::AssertionFailure(msg);
    return ::testing::AssertionSuccess();
}

#define INTEROP_EXPECT_STAT_NEAR(EXPECTED, ACTUAL, TOL) EXPECT_TRUE(AreStatsNear(EXPECTED, ACTUAL, TOL))
#define INTEROP_EXPECT_CYCLE_EQ(ACTUAL, EXPECTED) EXPECT_TRUE(AreCycleStatesNear(EXPECTED, ACTUAL))

/** Test if calculated run summary matches actual run summary
 *
 * @todo Add more robust testing (e.g. non-index)
 */
TEST_P(run_summary_tests, run_summary)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    const float tol = 1e-7f;
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.lane_count(), expected.lane_count());
    ASSERT_EQ(actual.surface_count(), expected.surface_count());
    EXPECT_EQ(actual.channel_count(), expected.channel_count());


    INTEROP_EXPECT_NEAR(actual.total_summary().error_rate(), expected.total_summary().error_rate(), tol);
    INTEROP_EXPECT_NEAR(actual.total_summary().percent_aligned(), expected.total_summary().percent_aligned(), tol);
    INTEROP_EXPECT_NEAR(actual.total_summary().first_cycle_intensity(), expected.total_summary().first_cycle_intensity(), tol);
    INTEROP_EXPECT_NEAR(actual.total_summary().percent_gt_q30(), expected.total_summary().percent_gt_q30(), tol);
    INTEROP_EXPECT_NEAR(actual.total_summary().yield_g(), expected.total_summary().yield_g(), tol);
    INTEROP_EXPECT_NEAR(actual.total_summary().projected_yield_g(), expected.total_summary().projected_yield_g(), tol);


    INTEROP_EXPECT_NEAR(actual.nonindex_summary().error_rate(), expected.nonindex_summary().error_rate(), tol);
    INTEROP_EXPECT_NEAR(actual.nonindex_summary().percent_aligned(), expected.nonindex_summary().percent_aligned(), tol);
    INTEROP_EXPECT_NEAR(actual.nonindex_summary().first_cycle_intensity(), expected.nonindex_summary().first_cycle_intensity(),
                tol);
    INTEROP_EXPECT_NEAR(actual.nonindex_summary().percent_gt_q30(), expected.nonindex_summary().percent_gt_q30(), tol);
    INTEROP_EXPECT_NEAR(actual.nonindex_summary().yield_g(), expected.nonindex_summary().yield_g(), tol);
    INTEROP_EXPECT_NEAR(actual.nonindex_summary().projected_yield_g(), expected.nonindex_summary().projected_yield_g(), tol);

    const model::summary::cycle_state_summary &actual_cycle_summary = actual.cycle_state();
    const model::summary::cycle_state_summary &expected_cycle_summary = expected.cycle_state();
    INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.extracted_cycle_range(), expected_cycle_summary.extracted_cycle_range());
    INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.called_cycle_range(), expected_cycle_summary.called_cycle_range());
    INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.qscored_cycle_range(), expected_cycle_summary.qscored_cycle_range());
    INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.error_cycle_range(), expected_cycle_summary.error_cycle_range());

}
/** Test if calculated read summary matches actual read summary
 */
TEST_P(run_summary_tests, read_summary)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.lane_count(), expected.lane_count());
    ASSERT_EQ(actual.surface_count(), expected.surface_count());
    const float tol = 1e-7f;
    for (size_t read = 0; read < expected.size(); ++read)
    {
        const model::summary::read_summary &actual_read_summary = actual[read];
        const model::summary::read_summary &expected_read_summary = expected[read];
        ::testing::Message trace_message;
        trace_message << "Read Index: " << read;
        SCOPED_TRACE(trace_message);
        EXPECT_EQ(actual_read_summary.size(), expected_read_summary.size());
        EXPECT_EQ(actual_read_summary.lane_count(), expected_read_summary.lane_count());
        INTEROP_EXPECT_NEAR(actual_read_summary.summary().error_rate(),
                            expected_read_summary.summary().error_rate(),
                            tol);
        INTEROP_EXPECT_NEAR(actual_read_summary.summary().percent_aligned(),
                    expected_read_summary.summary().percent_aligned(), tol);
        INTEROP_EXPECT_NEAR(actual_read_summary.summary().first_cycle_intensity(),
                    expected_read_summary.summary().first_cycle_intensity(), tol);
        INTEROP_EXPECT_NEAR(actual_read_summary.summary().percent_gt_q30(),
                    expected_read_summary.summary().percent_gt_q30(), tol);
        INTEROP_EXPECT_NEAR(actual_read_summary.summary().yield_g(),
                    expected_read_summary.summary().yield_g(), tol);
        INTEROP_EXPECT_NEAR(actual_read_summary.summary().projected_yield_g(),
                    expected_read_summary.summary().projected_yield_g(), tol);
        EXPECT_EQ(actual_read_summary.read().first_cycle(), expected_read_summary.read().first_cycle());
        EXPECT_EQ(actual_read_summary.read().last_cycle(), expected_read_summary.read().last_cycle());
        EXPECT_EQ(actual_read_summary.read().number(), expected_read_summary.read().number());
        EXPECT_EQ(actual_read_summary.read().is_index(), expected_read_summary.read().is_index());

    }
}
/** Test if calculated lane summary matches actual lane summary
 */
TEST_P(run_summary_tests, lane_summary)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
#ifdef _INTEROP_WIN32
    const float density_tol = 0.5f;
    const float tol = 1e-2f; // TODO: fix this unit test on external Windows Builds (appveyor) was 1e-7f
#else
    const float density_tol = 1e-7f;
    const float tol = 1e-7f;
#endif
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.lane_count(), expected.lane_count());
    ASSERT_EQ(actual.surface_count(), expected.surface_count());
    for (size_t read = 0; read < expected.size(); ++read)
    {
        ASSERT_EQ(actual[read].size(), expected[read].size());
        for (size_t lane = 0; lane < expected[read].size(); ++lane)
        {
            ::testing::Message trace_message;
            const model::summary::lane_summary &actual_lane_summary = actual[read][lane];
            const model::summary::lane_summary &expected_lane_summary = expected[read][lane];
            trace_message << "Read Index: " << read
                          << " - Lane Index: " << lane
                          << " - Lane Number: " << expected_lane_summary.lane();
            SCOPED_TRACE(trace_message);
            EXPECT_EQ(actual_lane_summary.lane(), expected_lane_summary.lane());
            EXPECT_GT(actual_lane_summary.lane(), 0u);
            EXPECT_EQ(actual_lane_summary.tile_count(),
                      expected_lane_summary.tile_count());

            INTEROP_EXPECT_NEAR(actual_lane_summary.percent_gt_q30(),
                                      expected_lane_summary.percent_gt_q30(),
                                      tol);
            INTEROP_EXPECT_NEAR(actual_lane_summary.yield_g(),
                        expected_lane_summary.yield_g(), tol);
            INTEROP_EXPECT_NEAR(actual_lane_summary.projected_yield_g(),
                        expected_lane_summary.projected_yield_g(), tol);
            INTEROP_EXPECT_NEAR(actual_lane_summary.reads_pf(),
                        expected_lane_summary.reads_pf(), tol);
            INTEROP_EXPECT_NEAR(actual_lane_summary.reads(),
                        expected_lane_summary.reads(), tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.density(), expected_lane_summary.density(), density_tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.density_pf(),
                                     expected_lane_summary.density_pf(),
                                     density_tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.cluster_count(),
                                     expected_lane_summary.cluster_count(),
                                     density_tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.cluster_count_pf(),
                                     expected_lane_summary.cluster_count_pf(),
                                     density_tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.percent_pf(), expected_lane_summary.percent_pf(), tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.phasing(), expected_lane_summary.phasing(), tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.prephasing(), expected_lane_summary.prephasing(), tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.percent_aligned(),
                                     expected_lane_summary.percent_aligned(),
                                     tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.error_rate(), expected_lane_summary.error_rate(), tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.error_rate_35(), expected_lane_summary.error_rate_35(), tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.error_rate_50(), expected_lane_summary.error_rate_50(), tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.error_rate_75(), expected_lane_summary.error_rate_75(), tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.error_rate_100(),
                                     expected_lane_summary.error_rate_100(),
                                     tol);
            INTEROP_EXPECT_STAT_NEAR(actual_lane_summary.first_cycle_intensity(),
                                     expected_lane_summary.first_cycle_intensity(),
                                     tol);

            const model::summary::cycle_state_summary &actual_cycle_summary = actual_lane_summary.cycle_state();
            const model::summary::cycle_state_summary &expected_cycle_summary = expected_lane_summary.cycle_state();
            INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.called_cycle_range(), expected_cycle_summary.called_cycle_range());
            INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.extracted_cycle_range(),
                            expected_cycle_summary.extracted_cycle_range());
            INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.called_cycle_range(), expected_cycle_summary.called_cycle_range());
            INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.qscored_cycle_range(), expected_cycle_summary.qscored_cycle_range());
            INTEROP_EXPECT_CYCLE_EQ(actual_cycle_summary.error_cycle_range(), expected_cycle_summary.error_cycle_range());

        }
    }
}


/** Test if calculated lane summary matches actual lane summary
 */
TEST_P(run_summary_tests, surface_summary)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
#ifdef _INTEROP_WIN32
    const float density_tol = 0.5f;
    const float tol = 1e-2f; // TODO: fix this unit test on external Windows Builds (appveyor) was 1e-7f
#else
    const float density_tol = 1e-7f;
    const float tol = 1e-7f;
#endif
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.lane_count(), expected.lane_count());
    ASSERT_EQ(actual.surface_count(), expected.surface_count());
    for (size_t read = 0; read < expected.size(); ++read)
    {
        ASSERT_EQ(actual[read].size(), expected[read].size());
        for (size_t lane = 0; lane < expected[read].size(); ++lane)
        {
            ASSERT_EQ(actual[read][lane].size(), expected[read][lane].size());
            for (size_t surface = 0; surface < expected[read][lane].size(); ++surface)
            {
                ::testing::Message trace_message;
                trace_message << "Read Index: " << read << " - Lane Index: " << lane << " - Surface Index: " << surface;
                SCOPED_TRACE(trace_message);
                const model::summary::surface_summary &actual_surface_summary = actual[read][lane][surface];
                const model::summary::surface_summary &expected_surface_summary = expected[read][lane][surface];
                EXPECT_EQ(actual_surface_summary.surface(), expected_surface_summary.surface());
                EXPECT_GT(actual_surface_summary.surface(), 0u);
                EXPECT_EQ(actual_surface_summary.tile_count(), expected_surface_summary.tile_count());
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.percent_gt_q30(),
                                         expected_surface_summary.percent_gt_q30(),
                                         tol);
                INTEROP_EXPECT_NEAR(actual_surface_summary.yield_g(), expected_surface_summary.yield_g(), tol);
                INTEROP_EXPECT_NEAR(actual_surface_summary.projected_yield_g(), expected_surface_summary.projected_yield_g(),
                            tol);
                INTEROP_EXPECT_NEAR(actual_surface_summary.reads_pf(), expected_surface_summary.reads_pf(), tol);
                INTEROP_EXPECT_NEAR(actual_surface_summary.reads(), expected_surface_summary.reads(), tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.density(),
                                         expected_surface_summary.density(),
                                         density_tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.density_pf(),
                                         expected_surface_summary.density_pf(),
                                         density_tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.cluster_count(),
                                         expected_surface_summary.cluster_count(),
                                         density_tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.cluster_count_pf(),
                                         expected_surface_summary.cluster_count_pf(),
                                         density_tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.percent_pf(),
                                         expected_surface_summary.percent_pf(),
                                         tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.phasing(),
                                         expected_surface_summary.phasing(),
                                         tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.prephasing(),
                                         expected_surface_summary.prephasing(),
                                         tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.percent_aligned(),
                                         expected_surface_summary.percent_aligned(),
                                         tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.error_rate(),
                                         expected_surface_summary.error_rate(), tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.error_rate_35(),
                                         expected_surface_summary.error_rate_35(), tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.error_rate_50(),
                                         expected_surface_summary.error_rate_50(), tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.error_rate_75(),
                                         expected_surface_summary.error_rate_75(), tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.error_rate_100(),
                                         expected_surface_summary.error_rate_100(),
                                         tol);
                INTEROP_EXPECT_STAT_NEAR(actual_surface_summary.first_cycle_intensity(),
                                 expected_surface_summary.first_cycle_intensity(), tol);

            }
        }
    }
}

TEST(summary_metrics_test, cycle_35_cycle_34_tile)
{
    const size_t lane_count = 8;
    const size_t surface_count = 2;
    const size_t swath_count = 4;
    const size_t tile_count = 99;
    const size_t sections_per_lane = 1;
    const size_t lanes_per_section = 1;
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<model::run::read_info> reads;
    reads.push_back(model::run::read_info(1, 1, 36));
    model::run::info run_info("XX",
                              "",
                              1,
                              model::run::flowcell_layout(lane_count,
                                                          surface_count,
                                                          swath_count,
                                                          tile_count,
                                                          sections_per_lane,
                                                          lanes_per_section),
                              channels,
                              model::run::image_dimensions(),
                              reads);
    run_info.set_naming_method(constants::FourDigit);

    model::metrics::run_metrics expected_run_metrics(run_info);
    model::metric_base::metric_set<model::metrics::error_metric> &expected_error_metrics =
            expected_run_metrics.get<model::metrics::error_metric>();
    model::metrics::run_metrics actual_run_metrics(run_info);
    model::metric_base::metric_set<model::metrics::error_metric> &actual_error_metrics =
            actual_run_metrics.get<model::metrics::error_metric>();
    typedef model::metrics::error_metric::uint_t uint_t;
    for (uint_t cycle_number = 0; cycle_number < 36; ++cycle_number)
    {
        expected_error_metrics.insert(error_metric(1, 1101, 1 + cycle_number, 3.0f));
        actual_error_metrics.insert(error_metric(1, 1101, 1 + cycle_number, 3.0f));
    }
    for (uint_t cycle_number = 0; cycle_number < 34; ++cycle_number)
    {
        actual_error_metrics.insert(error_metric(1, 1102, 1 + cycle_number, 1.0f));
    }

    model::summary::run_summary expected;
    logic::summary::summarize_run_metrics(expected_run_metrics, expected);
    model::summary::run_summary actual;
    logic::summary::summarize_run_metrics(actual_run_metrics, actual);
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual[0].size(), expected[0].size());
    ASSERT_EQ(actual.size(), 1u);
    ASSERT_EQ(actual[0].size(), 1u);

    const float tol = 1e-7f;
    const model::summary::lane_summary &actual_lane_summary = actual[0][0];
    const model::summary::lane_summary &expected_lane_summary = expected[0][0];
    model::summary::metric_stat expected_stat(3.0f, 0.0f, 3.0f);
    INTEROP_EXPECT_STAT_NEAR(expected_lane_summary.error_rate_35(), expected_stat, tol);
    INTEROP_EXPECT_NEAR(actual_lane_summary.error_rate_35().mean(), expected_lane_summary.error_rate_35().mean(), tol);
    expected_stat = model::summary::metric_stat();
    INTEROP_EXPECT_STAT_NEAR(expected_lane_summary.error_rate_50(), expected_stat, tol);
    INTEROP_EXPECT_NEAR(actual_lane_summary.error_rate_50().mean(), expected_lane_summary.error_rate_50().mean(), tol);

}

TEST(summary_metrics_test, clear_run_metrics) // TODO Expand to catch everything: probably use a fixture and the methods above
{
    const float tol = 1e-9f;
    const size_t lane_count = 8;
    const size_t surface_count = 2;
    const size_t swath_count = 4;
    const size_t tile_count = 99;
    const size_t sections_per_lane = 1;
    const size_t lanes_per_section = 1;
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<model::run::read_info> reads;
    reads.push_back(model::run::read_info(1, 1, 36));
    reads.push_back(model::run::read_info(2, 37, 42));
    reads.push_back(model::run::read_info(3, 43, 80));
    model::run::info run_info("XX",
                              "",
                              1,
                              model::run::flowcell_layout(lane_count,
                                                          surface_count,
                                                          swath_count,
                                                          tile_count,
                                                          sections_per_lane,
                                                          lanes_per_section),
                              channels,
                              model::run::image_dimensions(),
                              reads);
    run_info.set_naming_method(constants::FourDigit);

    model::metrics::run_metrics full_metrics(run_info);
    tile_metric_v2::create_expected(full_metrics.get<tile_metric>(), run_info);
    model::summary::run_summary summary;
    logic::summary::summarize_run_metrics(full_metrics, summary);
    INTEROP_EXPECT_NEAR(summary.total_summary().percent_aligned(), 2.5863409042358398f, tol);


    model::metrics::run_metrics partial_metrics(run_info);
    extraction_metric_v2::create_expected(partial_metrics.get<extraction_metric>());
    logic::summary::summarize_run_metrics(partial_metrics, summary);
    INTEROP_EXPECT_NEAR(summary.total_summary().percent_aligned(), 0, tol);

    model::metrics::run_metrics empty_metrics;
    logic::summary::summarize_run_metrics(empty_metrics, summary);
    INTEROP_EXPECT_NEAR(summary.total_summary().percent_aligned(), 0, tol);
    EXPECT_EQ(summary.size(), 0u);
}

TEST(summary_metrics_test, empty_run_metrics)
{
    const float tol = 1e-9f;
    model::metrics::run_metrics metrics;
    model::summary::run_summary summary;
    logic::summary::summarize_run_metrics(metrics, summary);
    INTEROP_EXPECT_NEAR(summary.total_summary().percent_aligned(), 0, tol);
    EXPECT_EQ(summary.size(), 0u);
}

//---------------------------------------------------------------------------------------------------------------------
// Unit test section
//---------------------------------------------------------------------------------------------------------------------

/** Run the summary logic */
struct summary_logic
{
    /** Run the summary logic
     *
     * @param metrics
     * @param summary
     */
    void operator()(model::metrics::run_metrics& metrics,
                    model::summary::run_summary& summary)
    {
        logic::summary::summarize_run_metrics(metrics, summary);
    }
    /** Get name of the logic
     *
     * @return name of the logic
     */
    static const char* name()
    {
        return "Summary";
    }
};


/** Generate the actual metric set by reading in from hardcoded binary buffer
 *
 * The expected metric set is provided by the generator.
 */
template<class Gen, class SummaryLogic>
class run_summary_generator : public abstract_generator<model::summary::run_summary>
{
    typedef typename Gen::metric_set_t metric_set_t;
    typedef abstract_generator<model::summary::run_summary>::parent_type base_t;
public:
    /** Generate the expected and actual metric sets
     *
     * @param expected expected metric set
     * @param actual actual metric set
     */
    ::testing::AssertionResult generate(model::summary::run_summary &expected,
                                        model::summary::run_summary &actual,
                                        bool*) const
    {
        typedef model::run::flowcell_layout::uint_t  uint_t;
        expected.clear();
        actual.clear();
        const uint_t swath_count = 4;
        const uint_t tile_count = 99;
        const uint_t sections_per_lane = 1;
        const uint_t lanes_per_section = 1;
        Gen::create_summary(expected);
        const uint_t lane_count = 8;//expected.max_lane();
        const uint_t surface_count = static_cast<uint_t>(expected.surface_count());
        std::vector<model::run::read_info> reads;
        expected.copy_reads(reads);
        std::vector<std::string> channels;
        logic::utils::update_channel_from_instrument_type(
                expected.channel_count() == 2 ? constants::NextSeq : constants::HiSeq, channels );
        actual = model::summary::run_summary(reads.begin(), reads.end(), lane_count, surface_count, channels.size());
        model::run::info run_info("XX",
                                  "",
                                  1,
                                  model::run::flowcell_layout(lane_count,
                                                              surface_count,
                                                              swath_count,
                                                              tile_count,
                                                              sections_per_lane,
                                                              lanes_per_section),
                                  channels,
                                  model::run::image_dimensions(),
                                  reads);
        run_info.set_naming_method(constants::FourDigit); // TODO: Set from metrics?
        model::metrics::run_metrics metrics(run_info);
        Gen::create_expected(metrics.get<typename Gen::metric_t>(), run_info);
        metrics.finalize_after_load();
        SummaryLogic summary_logic;
        summary_logic(metrics, actual);
        return ::testing::AssertionSuccess();
    }

    /** Create a copy of this object
     *
     * @return pointer to an abstract_generator
     */
    base_t clone() const
    {
        return new run_summary_generator<Gen, SummaryLogic>;
    }

    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream &out) const
    {
        out << "run_summary_generator<" << Gen::name() << "_" << SummaryLogic::name() << ">";
    }
};

/** Generator to test writing and reading an index summary */
class summary_write_read_generator
{
public:
    /** Define parameter type */
    typedef int parameter_type;
public:
    /** Constructor */
    summary_write_read_generator(const int){}
    /** Generate the expected and actual metric sets
     *
     * @param expected expected plot data
     * @param actual actual plot data
     */
    ::testing::AssertionResult generate(model::summary::run_summary& expected,
                                        model::summary::run_summary &actual) const
    {
        error_metric_v3::create_summary(expected);
        extraction_metric_v2::create_summary(expected);
        q_metric_v4::create_summary(expected);
        tile_metric_v2::create_summary(expected);
        corrected_intensity_metric_v2::create_summary(expected);
        std::ostringstream oss;
        oss << expected;

        std::istringstream iss(oss.str());
        iss >> actual;
        return ::testing::AssertionSuccess();
    }
    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream &out) const
    {
        out << "summary_write_read_generator";
    }
};

run_summary_tests::generator_type run_summary_unit_test_generators[] = {
        new run_summary_generator<error_metric_v3, summary_logic>(),
        new run_summary_generator<extraction_metric_v2, summary_logic>(),
        new run_summary_generator<q_metric_v4, summary_logic>(),
        new run_summary_generator<q_metric_v5, summary_logic>(),
        new run_summary_generator<q_metric_v6, summary_logic>(),
        new run_summary_generator<tile_metric_v2, summary_logic>(),
        new run_summary_generator<corrected_intensity_metric_v2, summary_logic>(),
        new run_summary_generator<corrected_intensity_metric_v3, summary_logic>(),

        // Requirements testing
        new run_summary_generator<q_metric_requirements, summary_logic>(),
        new run_summary_generator<error_metric_requirements, summary_logic>(),

        // Write/read
        wrap(new standard_parameter_generator<model::summary::run_summary, summary_write_read_generator>(0))
};


// Setup unit tests for run summary tests
INSTANTIATE_TEST_CASE_P(run_summary_unit_test,
                        run_summary_tests,
                        ::testing::ValuesIn(run_summary_unit_test_generators));


//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------
/** Generate the actual run summary writing out the expected and reading it back in again
 */
template<class Logic>
class regression_test_summary_generator : public abstract_regression_test_generator<model::summary::run_summary>
{
    typedef abstract_regression_test_generator<model::summary::run_summary> parent_t;
public:
    /** Constructor
     *
     * @param test_dir sub folder where tests are stored
     */
    regression_test_summary_generator(const std::string &test_dir) : parent_t(test_dir)
    {}

    /** Constructor
     *
     * @param run_folder run folder with data
     * @param test_dir sub folder where tests are stored
     */
    regression_test_summary_generator(const std::string &run_folder, const std::string &test_dir) : parent_t(run_folder,
                                                                                                             test_dir)
    {}

protected:
    /** Read the expected data from the baseline file into the model
     *
     * @param baseline_file baseline file
     * @param expected expected model data
     * @return true if the file was found, and the read completed without failure
     */
    bool read_expected(const std::string &baseline_file, model::summary::run_summary &expected) const
    {
        std::ifstream fin(baseline_file.c_str(), std::ifstream::binary);
        if( !fin.good() ) return false;
        fin >> expected;
        if( fin.eof() ) return true;
        return !fin.fail();
    }

    /** Read the actual data from the run folder
     *
     * @param run_folder run folder
     * @param actual actual model data
     * @return true if data was generated
     */
    bool generate_actual(const std::string &run_folder, model::summary::run_summary &actual) const
    {
        model::metrics::run_metrics actual_metrics;
        actual_metrics.read(run_folder);
        if( actual_metrics.empty() ) return false;
        Logic logic;
        logic(actual_metrics, actual);
        return actual.size() > 0;
    }

    /** Write the actual data to the run folder
     *
     * @param baseline_file baseline file
     * @param actual actual model data
     */
    bool write_actual(const std::string &baseline_file, const model::summary::run_summary &actual) const
    {
        std::ofstream fout(baseline_file.c_str(), std::ofstream::binary);
        fout << actual;
        return fout.good();
    }
    /** Create a copy of the current object with the given run folder
     *
     * @param run_folder run folder
     * @return pointer to new copy
     */
    base_t clone(const std::string& run_folder)const
    {
        return new regression_test_summary_generator(run_folder, m_test_dir);
    }

    /** Create a copy of the current object
     *
     * @return pointer to new copy
     */
    base_t clone() const
    {
        return new regression_test_summary_generator(*this);
    }

    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream &out) const
    {
        out << "regression_test_summary_generator - " << io::basename(m_run_folder);
    }
};

regression_test_summary_generator<summary_logic> run_summary_regression_gen("summary");

INSTANTIATE_TEST_CASE_P(run_summary_regression_test,
                        run_summary_tests,
                        ProxyValuesIn(run_summary_regression_gen, regression_test_data::instance().files()));


