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
#include "interop/logic/summary/index_summary.h"
#include "src/tests/interop/metrics/inc/error_metrics_test.h"
#include "src/tests/interop/metrics/inc/extraction_metrics_test.h"
#include "src/tests/interop/metrics/inc/tile_metrics_test.h"
#include "src/tests/interop/metrics/inc/q_metrics_test.h"
#include "src/tests/interop/metrics/inc/index_metrics_test.h"
#include "src/tests/interop/inc/regression_fixture.h"
#include "src/tests/interop/inc/abstract_regression_test_generator.h"
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Generate the actual metric set by reading in from hardcoded binary buffer
 *
 * The expected metric set is provided by the generator.
 */
template<class Gen>
class run_summary_generator : public abstract_generator< model::summary::run_summary >
{
    typedef typename Gen::metric_set_t metric_set_t;
public:
    /** Generate the expected and actual metric sets
     *
     * @param expected expected metric set
     * @param actual actual metric set
     */
    bool generate(model::summary::run_summary& expected, model::summary::run_summary& actual)const
    {
        expected = Gen::summary();
        std::vector<model::run::read_info> reads;
        expected.copy_reads(reads);
        actual = model::summary::run_summary(reads.begin(), reads.end(), 8);
        std::vector<std::string> channels;
        channels.push_back("Red");
        channels.push_back("Green");
        model::run::info run_info("XX",
                                  "",
                                  1,
                                  model::run::flowcell_layout(8, 2, 4, 99, 6, 6),
                                  channels,
                                  model::run::image_dimensions(),
                                  reads);
        run_info.set_naming_method(constants::FourDigit);
        model::metrics::run_metrics metrics(run_info);
        try
        {
            io::read_interop_from_string(Gen::binary_data(),
                                         metrics.get_set<typename Gen::metric_t>());
        }
        catch (const std::exception &) { }
        metrics.finalize_after_load();
        logic::summary::summarize_run_metrics(metrics, actual);
        return true;
    }
    abstract_generator< model::summary::run_summary >* clone()const
    {
        return new run_summary_generator<Gen>;
    }
};

/** Setup for tests that compare two run summaries */
struct run_summary_tests : public generic_test_fixture< model::summary::run_summary > {};

run_summary_tests::generator_type run_summary_unit_test_generators[] = {
        new run_summary_generator< error_v3 >(),
        new run_summary_generator< extraction_v2 >(),
        new run_summary_generator< q_v4 >(),
        new run_summary_generator< q_v5 >(),
        new run_summary_generator< q_v6 >(),
        new run_summary_generator< tile_v2 >()
};


// Setup unit tests for run summary tests
INSTANTIATE_TEST_CASE_P(run_summary_unit_test,
                        run_summary_tests,
                        ::testing::ValuesIn(run_summary_unit_test_generators));

#define EXPECT_STAT_NEAR(ACTUAL, EXPECTED, TOL) \
    EXPECT_NEAR(ACTUAL.mean(), EXPECTED.mean(), TOL); \
    EXPECT_NEAR(ACTUAL.stddev(), EXPECTED.stddev(), TOL); \
    if(!std::isnan(ACTUAL.median()) && !std::isnan(EXPECTED.median())) \
        EXPECT_NEAR(ACTUAL.median(), EXPECTED.median(), TOL)

#define EXPECT_CYCLE_EQ(ACTUAL, EXPECTED) \
    EXPECT_EQ(ACTUAL.first_cycle(), EXPECTED.first_cycle()); \
    EXPECT_EQ(ACTUAL.last_cycle(), EXPECTED.last_cycle())

#define EXPECT_READ_EQ(ACTUAL, EXPECTED) \
    EXPECT_EQ(ACTUAL.first_cycle(), EXPECTED.first_cycle()); \
    EXPECT_EQ(ACTUAL.last_cycle(), EXPECTED.last_cycle()); \
    EXPECT_EQ(ACTUAL.number(), EXPECTED.number()); \
    EXPECT_EQ(ACTUAL.is_index(), EXPECTED.is_index())



/** Test if calculated run summary matches actual run summary
 *
 * @todo Add more robust testing (e.g. non-index)
 */
TEST_P(run_summary_tests, run_summary)
{
    if(!test) return;// Disable test for rebaseline
    const float tol = 1e-7f;
    ASSERT_EQ(actual.size(), expected.size());
    ASSERT_EQ(actual.lane_count(), expected.lane_count());
    EXPECT_NEAR(actual.total_summary().error_rate(), expected.total_summary().error_rate(), tol);
    EXPECT_NEAR(actual.total_summary().percent_aligned(), expected.total_summary().percent_aligned(), tol);
    EXPECT_NEAR(actual.total_summary().first_cycle_intensity(), expected.total_summary().first_cycle_intensity(), tol);
    EXPECT_NEAR(actual.total_summary().percent_gt_q30(), expected.total_summary().percent_gt_q30(), tol);
    EXPECT_NEAR(actual.total_summary().yield_g(), expected.total_summary().yield_g(), tol);
    EXPECT_NEAR(actual.total_summary().projected_yield_g(), expected.total_summary().projected_yield_g(), tol);


    EXPECT_NEAR(actual.nonindex_summary().error_rate(), expected.nonindex_summary().error_rate(), tol);
    EXPECT_NEAR(actual.nonindex_summary().percent_aligned(), expected.nonindex_summary().percent_aligned(), tol);
    EXPECT_NEAR(actual.nonindex_summary().first_cycle_intensity(), expected.nonindex_summary().first_cycle_intensity(), tol);
    EXPECT_NEAR(actual.nonindex_summary().percent_gt_q30(), expected.nonindex_summary().percent_gt_q30(), tol);
    EXPECT_NEAR(actual.nonindex_summary().yield_g(), expected.nonindex_summary().yield_g(), tol);
    EXPECT_NEAR(actual.nonindex_summary().projected_yield_g(), expected.nonindex_summary().projected_yield_g(), tol);

    const model::summary::cycle_state_summary& actual_cycle_summary = actual.cycle_state();
    const model::summary::cycle_state_summary& expected_cycle_summary = expected.cycle_state();
    EXPECT_CYCLE_EQ(actual_cycle_summary.extracted_cycle_range(), expected_cycle_summary.extracted_cycle_range());
    EXPECT_CYCLE_EQ(actual_cycle_summary.called_cycle_range(), expected_cycle_summary.called_cycle_range());
    EXPECT_CYCLE_EQ(actual_cycle_summary.qscored_cycle_range(), expected_cycle_summary.qscored_cycle_range());
    EXPECT_CYCLE_EQ(actual_cycle_summary.error_cycle_range(), expected_cycle_summary.error_cycle_range());
}
/** Test if calculated read summary matches actual read summary
 */
TEST_P(run_summary_tests, read_summary)
{
    if(!test) return;// Disable test for rebaseline
    ASSERT_EQ(actual.size(), expected.size());
    const float tol = 1e-7f;
    for(size_t read=0;read<expected.size();++read)
    {
        const model::summary::read_summary &actual_read_summary = actual[read];
        const model::summary::read_summary &expected_read_summary = expected[read];
        EXPECT_EQ(actual_read_summary.size(), expected_read_summary.size());
        EXPECT_EQ(actual_read_summary.lane_count(), expected_read_summary.lane_count());
        EXPECT_NEAR(actual_read_summary.summary().error_rate(), expected_read_summary.summary().error_rate(), tol);
        EXPECT_NEAR(actual_read_summary.summary().percent_aligned(), expected_read_summary.summary().percent_aligned(), tol);
        EXPECT_NEAR(actual_read_summary.summary().first_cycle_intensity(), expected_read_summary.summary().first_cycle_intensity(), tol);
        EXPECT_NEAR(actual_read_summary.summary().percent_gt_q30(), expected_read_summary.summary().percent_gt_q30(), tol)  << "Failed read: " << read;
        EXPECT_NEAR(actual_read_summary.summary().yield_g(), expected_read_summary.summary().yield_g(), tol)  << "Failed read: " << read;
        EXPECT_NEAR(actual_read_summary.summary().projected_yield_g(), expected_read_summary.summary().projected_yield_g(), tol)  << "Failed read: " << read;
        EXPECT_READ_EQ(actual_read_summary.read(), expected_read_summary.read());
    }
}
/** Test if calculated lane summary matches actual lane summary
 */
TEST_P(run_summary_tests, lane_summary)
{
    if(!test) return;// Disable test for rebaseline
#ifdef _INTEROP_WIN32
    const float density_tol = 0.5f;
    const float tol = 1e-2f; // TODO: fix this unit test on external Windows Builds (appveyor) was 1e-7f
#else
    const float density_tol = 1e-7f;
    const float tol = 1e-7f;
#endif
    ASSERT_EQ(actual.size(), expected.size());
    for(size_t read=0;read<expected.size();++read)
    {
        ASSERT_EQ(actual[read].size(), expected[read].size());
        for(size_t lane=0;lane<expected[read].size();++lane)
        {
            const model::summary::lane_summary& actual_lane_summary = actual[read][lane];
            const model::summary::lane_summary& expected_lane_summary = expected[read][lane];
            EXPECT_EQ(actual_lane_summary.lane(), expected_lane_summary.lane());
            EXPECT_GT(actual_lane_summary.lane(), 0u);
            EXPECT_EQ(actual_lane_summary.tile_count(), expected_lane_summary.tile_count()) << "Failed read: " << read << ", lane: " << lane;
            if(!std::isnan(actual_lane_summary.percent_gt_q30()) && !std::isnan(expected_lane_summary.percent_gt_q30()))
                EXPECT_NEAR(actual_lane_summary.percent_gt_q30(), expected_lane_summary.percent_gt_q30(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_NEAR(actual_lane_summary.yield_g(), expected_lane_summary.yield_g(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_NEAR(actual_lane_summary.projected_yield_g(), expected_lane_summary.projected_yield_g(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_NEAR(actual_lane_summary.reads_pf(), expected_lane_summary.reads_pf(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_NEAR(actual_lane_summary.reads(), expected_lane_summary.reads(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_STAT_NEAR(actual_lane_summary.density(), expected_lane_summary.density(), density_tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_STAT_NEAR(actual_lane_summary.density_pf(), expected_lane_summary.density_pf(), density_tol);
            EXPECT_STAT_NEAR(actual_lane_summary.cluster_count(), expected_lane_summary.cluster_count(), density_tol);
            EXPECT_STAT_NEAR(actual_lane_summary.cluster_count_pf(), expected_lane_summary.cluster_count_pf(), density_tol);
            EXPECT_STAT_NEAR(actual_lane_summary.percent_pf(), expected_lane_summary.percent_pf(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_STAT_NEAR(actual_lane_summary.phasing(), expected_lane_summary.phasing(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_STAT_NEAR(actual_lane_summary.prephasing(), expected_lane_summary.prephasing(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_STAT_NEAR(actual_lane_summary.percent_aligned(), expected_lane_summary.percent_aligned(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_STAT_NEAR(actual_lane_summary.error_rate(), expected_lane_summary.error_rate(), tol);
            EXPECT_STAT_NEAR(actual_lane_summary.error_rate_35(), expected_lane_summary.error_rate_35(), tol);
            EXPECT_STAT_NEAR(actual_lane_summary.error_rate_50(), expected_lane_summary.error_rate_50(), tol);
            EXPECT_STAT_NEAR(actual_lane_summary.error_rate_75(), expected_lane_summary.error_rate_75(), tol);
            EXPECT_STAT_NEAR(actual_lane_summary.error_rate_100(), expected_lane_summary.error_rate_100(), tol) << "Failed read: " << read << ", lane: " << lane;
            EXPECT_STAT_NEAR(actual_lane_summary.first_cycle_intensity(), expected_lane_summary.first_cycle_intensity(), tol);

            const model::summary::cycle_state_summary& actual_cycle_summary = actual_lane_summary.cycle_state();
            const model::summary::cycle_state_summary& expected_cycle_summary = expected_lane_summary.cycle_state();
            EXPECT_CYCLE_EQ(actual_cycle_summary.called_cycle_range(), expected_cycle_summary.called_cycle_range());
            EXPECT_CYCLE_EQ(actual_cycle_summary.extracted_cycle_range(), expected_cycle_summary.extracted_cycle_range());
            EXPECT_CYCLE_EQ(actual_cycle_summary.called_cycle_range(), expected_cycle_summary.called_cycle_range());
            EXPECT_CYCLE_EQ(actual_cycle_summary.qscored_cycle_range(), expected_cycle_summary.qscored_cycle_range());
            EXPECT_CYCLE_EQ(actual_cycle_summary.error_cycle_range(), expected_cycle_summary.error_cycle_range()) << "Failed read: " << read << ", lane: " << lane;
        }
    }
}

TEST(summary_metrics_test, cycle_35_cycle_34_tile)
{
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    std::vector<model::run::read_info> reads;
    reads.push_back(model::run::read_info(1, 1, 36));
    model::run::info run_info("XX",
                              "",
                              1,
                              model::run::flowcell_layout(8, 2, 4, 99, 6, 6),
                              channels,
                              model::run::image_dimensions(),
                              reads);
    run_info.set_naming_method(constants::FourDigit);

    model::metrics::run_metrics expected_run_metrics(run_info);
    model::metric_base::metric_set<model::metrics::error_metric>& expected_error_metrics=
            expected_run_metrics.get_set<model::metrics::error_metric>();
    model::metrics::run_metrics actual_run_metrics(run_info);
    model::metric_base::metric_set<model::metrics::error_metric>& actual_error_metrics=
            actual_run_metrics.get_set<model::metrics::error_metric>();
    typedef model::metrics::error_metric::uint_t  uint_t;
    for(uint_t cycle_number=0;cycle_number<36;++cycle_number)
    {
        expected_error_metrics.insert(error_metric(1, 1101, 1 + cycle_number, 3.0f));
        actual_error_metrics.insert(error_metric(1, 1101, 1 + cycle_number, 3.0f));
    }
    for(uint_t cycle_number=0;cycle_number<34;++cycle_number)
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
    const model::summary::lane_summary& actual_lane_summary = actual[0][0];
    const model::summary::lane_summary& expected_lane_summary = expected[0][0];
    model::summary::metric_stat expected_stat(3.0f, 0.0f, 3.0f);
    EXPECT_STAT_NEAR(expected_lane_summary.error_rate_35(), expected_stat, tol);
    EXPECT_NEAR(actual_lane_summary.error_rate_35().mean(), expected_lane_summary.error_rate_35().mean(), tol);
    expected_stat=model::summary::metric_stat(0.0f, 0.0f, 0.0f);
    EXPECT_STAT_NEAR(expected_lane_summary.error_rate_50(), expected_stat, tol);
    EXPECT_NEAR(actual_lane_summary.error_rate_50().mean(), expected_lane_summary.error_rate_50().mean(), tol);

}

TEST(summary_metrics_test, empty_run_metrics)
{
    model::metrics::run_metrics metrics;
    model::summary::run_summary summary;
    logic::summary::summarize_run_metrics(metrics, summary);
}

/** TODO take tile metrics and index metrics from the same run */
TEST(index_summary_test, lane_summary)
{
    model::summary::index_flowcell_summary expected(index_v1::summary());
    model::summary::index_flowcell_summary actual;
    model::metrics::run_metrics metrics(index_v1::run_info());
    try
    {
        io::read_interop_from_string(index_v1::binary_data(),
                                     metrics.get_set<index_metric>());
        io::read_interop_from_string(tile_v2::binary_data(),
                                     metrics.get_set<tile_metric>());
    }
    catch (const std::exception &) { }
    logic::summary::summarize_index_metrics(metrics, actual);

    const float tol = 1e-7f;
    ASSERT_EQ(expected.size(), actual.size());
    for(size_t lane=0;lane < expected.size();++lane)
    {
        const model::summary::index_lane_summary& expected_lane = expected[lane];
        const model::summary::index_lane_summary& actual_lane = actual[lane];
        EXPECT_EQ(expected_lane.total_reads(), actual_lane.total_reads());
        EXPECT_EQ(expected_lane.total_pf_reads(), actual_lane.total_pf_reads());
        EXPECT_NEAR(expected_lane.total_fraction_mapped_reads(), actual_lane.total_fraction_mapped_reads(), tol);
        if(!std::isnan(expected_lane.mapped_reads_cv()) && !std::isnan(actual_lane.mapped_reads_cv()))
            EXPECT_NEAR(expected_lane.mapped_reads_cv(), actual_lane.mapped_reads_cv(), tol);
        EXPECT_NEAR(expected_lane.min_mapped_reads(), actual_lane.min_mapped_reads(), tol);
        EXPECT_NEAR(expected_lane.max_mapped_reads(), actual_lane.max_mapped_reads(), tol);
        ASSERT_EQ(expected_lane.size(), actual_lane.size());
        for(size_t index=0;index < expected_lane.size();++index)
        {
            const model::summary::index_count_summary& expected_count = expected_lane[index];
            const model::summary::index_count_summary& actual_count = actual_lane[index];
            EXPECT_EQ(expected_count.id(), actual_count.id());
            EXPECT_EQ(expected_count.index1(), actual_count.index1());
            EXPECT_EQ(expected_count.index2(), actual_count.index2());
            EXPECT_EQ(expected_count.count(), actual_count.count());
            EXPECT_EQ(expected_count.sample_id(), actual_count.sample_id());
            EXPECT_EQ(expected_count.project_name(), actual_count.project_name());
            EXPECT_NEAR(expected_count.fraction_mapped(), actual_count.fraction_mapped(), tol);
        }
    }
};

//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------
/** Generate the actual run summary writing out the expected and reading it back in again
 */
class regression_test_summary_generator : public abstract_regression_test_generator< model::summary::run_summary >
{
    typedef abstract_regression_test_generator< model::summary::run_summary > parent_t;
public:
    regression_test_summary_generator(const std::string& test_dir) : parent_t(test_dir){}
    regression_test_summary_generator(const std::string& run_folder, const std::string& test_dir) : parent_t(run_folder, test_dir){}

protected:
    /** Read the expected data from the baseline file into the model
     *
     * @param baseline_file baseline file
     * @param expected expected model data
     */
    void read_expected(const std::string& baseline_file,  model::summary::run_summary& expected)const
    {
        std::ifstream fin(baseline_file.c_str());
        fin >> expected;
    }
    /** Read the actual data from the run folder
     *
     * @param run_folder run folder
     * @param actual actual model data
     */
    void generate_actual(const std::string& run_folder,  model::summary::run_summary& actual)const
    {
        model::metrics::run_metrics actual_metrics;
        actual_metrics.read(run_folder);
        logic::summary::summarize_run_metrics(actual_metrics, actual);
    }
    /** Write the actual data to the run folder
     *
     * @param baseline_file baseline file
     * @param actual actual model data
     */
    bool write_actual(const std::string& baseline_file,  const model::summary::run_summary& actual)const
    {
        std::ofstream fout(baseline_file.c_str());
        fout << actual;
        return fout.good();
    }
    /** Create a copy of the current object
     *
     * @return pointer to new copy
     */
    base_type clone()const
    {
        return new regression_test_summary_generator(m_run_folder, m_test_dir);
    }
};

regression_test_summary_generator run_summary_regression_gen("summary");
INSTANTIATE_TEST_CASE_P(run_summary_regression_test,
                        run_summary_tests,
                        ProxyValuesIn(run_summary_regression_gen, regression_test_data::instance().files()));

