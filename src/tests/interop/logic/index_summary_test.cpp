/** Unit tests for the index summary
 *
 *
 *  @file
 *  @date 11/14/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/logic/summary/index_summary.h"
#include "src/tests/interop/metrics/inc/index_metrics_test.h"
#include "src/tests/interop/metrics/inc/tile_metrics_test.h"
#include "src/tests/interop/inc/abstract_regression_test_generator.h"


using namespace illumina::interop::model::summary;
using namespace illumina::interop;
using namespace illumina::interop::unittest;
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;


/** Setup for tests that compare two index summaries */
struct index_summary_tests : public generic_test_fixture<model::summary::index_flowcell_summary> {};

/** Test if calculated expected index flowcell summary matches the actual index flowcell summary
 */
TEST_P(index_summary_tests, index_lane_summary)
{
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    const float tol = 1e-7f;
    ASSERT_EQ(actual.size(), expected.size());
    for(size_t lane=0;lane < actual.size();++lane)
    {
        ::testing::Message trace_message_lane;
        trace_message_lane << "Lane Index: " << lane << " of " << actual.size();
        SCOPED_TRACE(trace_message_lane);
        const index_lane_summary& actual_lane_summary = actual[lane];
        const index_lane_summary& expected_lane_summary = expected[lane];
        EXPECT_EQ(expected_lane_summary.total_reads(), actual_lane_summary.total_reads());
        EXPECT_EQ(expected_lane_summary.total_pf_reads(), actual_lane_summary.total_pf_reads());
        INTEROP_EXPECT_NEAR(expected_lane_summary.total_fraction_mapped_reads(),
                            actual_lane_summary.total_fraction_mapped_reads(),
                            tol);
        INTEROP_EXPECT_NEAR(expected_lane_summary.mapped_reads_cv(), actual_lane_summary.mapped_reads_cv(), tol);
        INTEROP_EXPECT_NEAR(expected_lane_summary.min_mapped_reads(), actual_lane_summary.min_mapped_reads(), tol);
        INTEROP_EXPECT_NEAR(expected_lane_summary.max_mapped_reads(), actual_lane_summary.max_mapped_reads(), tol);
        ASSERT_EQ(expected_lane_summary.size(), actual_lane_summary.size());
        for(size_t index=0;index<actual_lane_summary.size();++index)
        {
            ::testing::Message trace_message_index;
            trace_message_index << "Index Index: " << index << " of " << actual_lane_summary.size();
            SCOPED_TRACE(trace_message_index);
            const index_count_summary& actual_count_summary = actual_lane_summary[index];
            const index_count_summary& expected_count_summary = expected_lane_summary[index];
            ASSERT_EQ(expected_count_summary.id(), actual_count_summary.id());
            EXPECT_EQ(expected_count_summary.index1(), actual_count_summary.index1());
            EXPECT_EQ(expected_count_summary.index2(), actual_count_summary.index2());
            EXPECT_EQ(expected_count_summary.sample_id(), actual_count_summary.sample_id());
            EXPECT_EQ(expected_count_summary.project_name(), actual_count_summary.project_name());
            EXPECT_EQ(expected_count_summary.cluster_count(), actual_count_summary.cluster_count());
            INTEROP_EXPECT_NEAR(expected_count_summary.fraction_mapped(), actual_count_summary.fraction_mapped(), tol);
        }
    }
}

/** TODO take tile metrics and index metrics from the same run */
TEST(index_summary_test, lane_summary)
{
    const size_t lane_count = 8;
    std::vector<model::run::read_info> reads(1, model::run::read_info(1, 1, 3, false));
    std::vector<std::string> channels;
    channels.push_back("Red");
    channels.push_back("Green");
    model::run::info run_info("XX",
                              "",
                              1,
                              model::run::flowcell_layout(lane_count),
                              channels,
                              model::run::image_dimensions(),
                              reads);

    model::summary::index_flowcell_summary expected;
    index_metric_v1::create_summary(expected);
    model::summary::index_flowcell_summary actual;
    model::metrics::run_metrics metrics(run_info);
    try
    {
        std::string data;
        index_metric_v1::create_binary_data(data);
        io::read_interop_from_string(data,
                                     metrics.get<index_metric>());
        tile_metric_v2::create_binary_data(data);
        io::read_interop_from_string(data,
                                     metrics.get<tile_metric>());
    }
    catch (const std::exception &)
    {}
    logic::summary::summarize_index_metrics(metrics, actual);

    const float tol = 1e-7f;
    ASSERT_EQ(expected.size(), actual.size());
    for (size_t lane = 0; lane < expected.size(); ++lane)
    {
        const model::summary::index_lane_summary &expected_lane = expected[lane];
        const model::summary::index_lane_summary &actual_lane = actual[lane];
        EXPECT_EQ(expected_lane.total_reads(), actual_lane.total_reads());
        EXPECT_EQ(expected_lane.total_pf_reads(), actual_lane.total_pf_reads());
        INTEROP_EXPECT_NEAR(expected_lane.total_fraction_mapped_reads(), actual_lane.total_fraction_mapped_reads(), tol);
        INTEROP_EXPECT_NEAR(expected_lane.mapped_reads_cv(),
                            actual_lane.mapped_reads_cv(),
                            tol);
        INTEROP_EXPECT_NEAR(expected_lane.min_mapped_reads(), actual_lane.min_mapped_reads(), tol);
        INTEROP_EXPECT_NEAR(expected_lane.max_mapped_reads(), actual_lane.max_mapped_reads(), tol);
        ASSERT_EQ(expected_lane.size(), actual_lane.size());
        for (size_t index = 0; index < expected_lane.size(); ++index)
        {
            const model::summary::index_count_summary &expected_count = expected_lane[index];
            const model::summary::index_count_summary &actual_count = actual_lane[index];
            EXPECT_EQ(expected_count.id(), actual_count.id());
            EXPECT_EQ(expected_count.index1(), actual_count.index1());
            EXPECT_EQ(expected_count.index2(), actual_count.index2());
            EXPECT_EQ(expected_count.cluster_count(), actual_count.cluster_count());
            EXPECT_EQ(expected_count.sample_id(), actual_count.sample_id());
            EXPECT_EQ(expected_count.project_name(), actual_count.project_name());
            INTEROP_EXPECT_NEAR(expected_count.fraction_mapped(), actual_count.fraction_mapped(), tol);
        }
    }
}

TEST(index_summary_test, lane_summary_cluster_count)
{
    const ::uint64_t cluster_count = static_cast< ::uint64_t >(std::numeric_limits< ::int64_t >::max() );
    model::metrics::run_metrics metrics;
    std::vector< model::metrics::index_info > indices;
    indices.reserve(2);
    indices.push_back(model::metrics::index_info("TTGC", "Unknown", "Unknown", cluster_count));
    indices.push_back(model::metrics::index_info("AATG", "Unknown", "Unknown", cluster_count));

    metrics.get<model::metrics::index_metric>().insert(model::metrics::index_metric(7, 1114, 1, indices));
    metrics.get<model::metrics::index_metric>().insert(model::metrics::index_metric(7, 1115, 1, indices));

    metrics.get<model::metrics::tile_metric>().insert(model::metrics::tile_metric(7, 1114, 10000, 10000, 10000, 10000));
    metrics.get<model::metrics::tile_metric>().insert(model::metrics::tile_metric(7, 1115, 10000, 10000, 10000, 10000));

    index_lane_summary summary;
    logic::summary::summarize_index_metrics(metrics, 7, summary);
    ASSERT_EQ(2u, summary.size());
    EXPECT_EQ(cluster_count*2u, summary[0].cluster_count());
}

//---------------------------------------------------------------------------------------------------------------------
// Unit test section
//---------------------------------------------------------------------------------------------------------------------

/** Generate the actual metric set by reading in from hardcoded binary buffer
 *
 * The expected metric set is provided by the generator.
 */
template<class IndexGen, class TileGen>
class index_summary_generator : public abstract_generator<model::summary::index_flowcell_summary>
{
    typedef typename IndexGen::metric_set_t index_metric_set_t;
    typedef typename TileGen::metric_set_t tile_metric_set_t;
    typedef abstract_generator<model::summary::index_flowcell_summary>::parent_type base_t;
public:
    /** Generate the expected and actual metric sets
     *
     * @param expected expected metric set
     * @param actual actual metric set
     */
    ::testing::AssertionResult generate(model::summary::index_flowcell_summary &expected,
                                        model::summary::index_flowcell_summary &actual,
                                        bool*) const
    {
        expected.clear();
        actual.clear();
        const size_t lane_count = 8;
        const size_t surface_count = 2;
        const size_t swath_count = 4;
        const size_t tile_count = 99;
        const size_t sections_per_lane = 6;
        const size_t lanes_per_section = 6;
        IndexGen::create_summary(expected);
        std::vector<std::string> channels;
        channels.push_back("Red");
        channels.push_back("Green");

        std::vector<model::run::read_info> reads;
        reads.reserve(3);
        reads.push_back(model::run::read_info(1, 1, 3, false));
        reads.push_back(model::run::read_info(2, 4, 10, false));
        reads.push_back(model::run::read_info(3, 11, 15, false));
        actual = model::summary::index_flowcell_summary();
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
        model::metrics::run_metrics metrics(run_info);
        IndexGen::create_expected(metrics.get<index_metric>());
        TileGen::create_expected(metrics.get<tile_metric>());
        metrics.finalize_after_load();
        logic::summary::summarize_index_metrics(metrics, actual);
        return ::testing::AssertionSuccess();
    }

    /** Create a copy of this object
     *
     * @return pointer to an abstract_generator
     */
    base_t clone() const
    {
        return new index_summary_generator<IndexGen, TileGen>;
    }

    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream &out) const
    {
        out << "index_summary_generator<" << IndexGen::name() << "_" << TileGen::name() << ">";
    }
};

/** Generator to test writing and reading an index summary */
class index_write_read_generator
{
public:
    /** Define parameter type */
    typedef int parameter_type;
public:
    /** Constructor */
    index_write_read_generator(const int){}
    /** Generate the expected and actual metric sets
     *
     * @param expected expected plot data
     * @param actual actual plot data
     */
    ::testing::AssertionResult generate(model::summary::index_flowcell_summary& expected,
                                        model::summary::index_flowcell_summary &actual) const
    {
        index_metric_v1::create_summary(expected);
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
        out << "index_write_read_generator";
    }
};

index_summary_tests::generator_type index_summary_unit_test_generators[] = {
        wrap(new index_summary_generator<index_metric_v1, tile_metric_v2>()),
        wrap(new standard_parameter_generator<model::summary::index_flowcell_summary, index_write_read_generator>(0))
};


// Setup unit tests for run summary tests
INSTANTIATE_TEST_CASE_P(index_summary_unit_test,
                        index_summary_tests,
                        ::testing::ValuesIn(index_summary_unit_test_generators));

//---------------------------------------------------------------------------------------------------------------------
// Regression test section
//---------------------------------------------------------------------------------------------------------------------

/** Generate the actual run summary writing out the expected and reading it back in again
 */
class regression_test_index_summary_generator : public abstract_regression_test_generator<model::summary::index_flowcell_summary>
{
    typedef abstract_regression_test_generator<model::summary::index_flowcell_summary> parent_t;
public:
    /** Constructor
     *
     * @param test_dir sub folder where tests are stored
     */
    regression_test_index_summary_generator(const std::string &test_dir) : parent_t(test_dir)
    {}

    /** Constructor
     *
     * @param run_folder run folder with data
     * @param test_dir sub folder where tests are stored
     */
    regression_test_index_summary_generator(const std::string &run_folder, const std::string &test_dir) :
            parent_t(run_folder,test_dir)
    {}

protected:
    /** Read the expected data from the baseline file into the model
     *
     * @param baseline_file baseline file
     * @param expected expected model data
     * @return true if the file was found, and the read completed without failure
     */
    bool read_expected(const std::string &baseline_file, model::summary::index_flowcell_summary &expected) const
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
    bool generate_actual(const std::string &run_folder, model::summary::index_flowcell_summary &actual) const
    {
        model::metrics::run_metrics actual_metrics;
        actual_metrics.read(run_folder);
        if( actual_metrics.empty() ) return false;
        logic::summary::summarize_index_metrics(actual_metrics, actual);
        return actual.size() > 0;
    }

    /** Write the actual data to the run folder
     *
     * @param baseline_file baseline file
     * @param actual actual model data
     */
    bool write_actual(const std::string &baseline_file, const model::summary::index_flowcell_summary &actual) const
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
        return new regression_test_index_summary_generator(run_folder, m_test_dir);
    }

    /** Create a copy of the current object
     *
     * @return pointer to new copy
     */
    base_t clone() const
    {
        return new regression_test_index_summary_generator(*this);
    }

    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream &out) const
    {
        out << "regression_test_index_summary_generator - " << io::basename(m_run_folder);
    }
};

regression_test_index_summary_generator index_summary_regression_gen("index_summary");

INSTANTIATE_TEST_CASE_P(index_summary_regression_test,
                        index_summary_tests,
                        ProxyValuesIn(index_summary_regression_gen, regression_test_data::instance().files()));


