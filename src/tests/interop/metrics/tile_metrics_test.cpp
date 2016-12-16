/** Unit tests for the tile metrics
 *
 *
 *  @file
 *  @date 9/6/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <limits>
#include <set>
#include <gtest/gtest.h>
#include "interop/util/math.h"
#include "interop/util/statistics.h"
#include "interop/util/type_traits.h"
#include "interop/model/run_metrics.h"
#include "src/tests/interop/metrics/inc/tile_metrics_test.h"
#include "src/tests/interop/inc/generic_fixture.h"
#include "src/tests/interop/inc/proxy_parameter_generator.h"
#include "src/tests/interop/metrics/inc/metric_generator.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::io;
using namespace illumina::interop;
using namespace illumina::interop::unittest;
using namespace illumina;


/** Generate the actual metric set by reading in from hardcoded binary buffer
 *
 * The expected metric set is provided by the generator.
 */
template<class Gen>
class write_metric_generator : public abstract_generator< typename Gen::metric_set_t >
{
    typedef typename Gen::metric_set_t metric_set_t;
    typedef typename abstract_generator<metric_set_t>::parent_type parent_t;
public:
    /** Generate the expected and actual metric sets
     *
     * @param expected expected metric set
     * @param actual actual metric set
     */
    ::testing::AssertionResult generate(metric_set_t& expected, metric_set_t& actual, bool*)const
    {
        expected.clear();
        tile_metric expected_metric(1,1101);
        expected.set_version(Gen::VERSION);
        expected.insert(expected_metric);

        std::ostringstream fout;
        illumina::interop::io::write_metrics(fout, expected);
        actual.clear();

        try{
            io::read_interop_from_string(fout.str(), actual);
        }catch(const io::incomplete_file_exception&){}
        return ::testing::AssertionSuccess();
    }
    /** Create a copy of this object
     *
     * @return pointer to copy
     */
    parent_t clone()const{return new write_metric_generator<Gen>;}
    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream& out)const
    {
        out << "write_metric_generator<" << Gen::name() << ">";
    }

};

typedef metric_set< tile_metric > tile_metric_set;
/** Setup for tests that compare two tile metric sets */
struct tile_metrics_tests : public generic_test_fixture< tile_metric_set > {};


tile_metrics_tests::generator_type tile_unit_test_generators[] = {
        wrap(new hardcoded_metric_generator< tile_metric_v2 >) ,
        wrap(new write_read_metric_generator< tile_metric_v2 >),
        wrap(new write_metric_generator< tile_metric_v2 >)
};

// Setup unit tests for tile_metrics_tests
INSTANTIATE_TEST_CASE_P(tile_metric_unit_test,
                        tile_metrics_tests,
                        ::testing::ValuesIn(tile_unit_test_generators));

/**
 * @class illumina::interop::model::metrics::tile_metric
 * @test Confirm version 2 of the metric can be written to and read from a stream
 * @test Confirm version 2 of the metric matches known binary file
 */
TEST_P(tile_metrics_tests, test_read_write)
{
    typedef tile_metric_set::const_iterator const_iterator;
    typedef tile_metric_set::metric_type metric_t;
    if(skip_test) return;
    ASSERT_TRUE(fixture_test_result);
    const float scale = (test_modifier==2) ? 0.01f : ( (test_modifier==1) ? 100.0f : 1.0f);
    ASSERT_EQ(actual.version(), expected.version());
    ASSERT_EQ(actual.size(), expected.size());

    const float tol = 1e-7f / 0.01f;
    for(const_iterator it_expected=expected.begin(), it_actual = actual.begin();
        it_expected != expected.end() && it_actual != actual.end();
        it_expected++,it_actual++)
    {
        EXPECT_EQ(it_expected->lane(), it_actual->lane());
        EXPECT_EQ(it_expected->tile(), it_actual->tile());

        INTEROP_EXPECT_NEAR(it_expected->cluster_density(), it_actual->cluster_density(), tol);
        INTEROP_EXPECT_NEAR(it_expected->cluster_density_pf(), it_actual->cluster_density_pf(), tol);
        INTEROP_EXPECT_NEAR(it_expected->cluster_count(), it_actual->cluster_count(), tol);
        INTEROP_EXPECT_NEAR(it_expected->cluster_count_pf(), it_actual->cluster_count_pf(), tol);
        EXPECT_EQ(it_expected->read_metrics().size(), it_actual->read_metrics().size());
        for(metric_t::read_metric_vector::const_iterator it_read_expected = it_expected->read_metrics().begin(),
                        it_read_actual = it_actual->read_metrics().begin();
                        it_read_expected != it_expected->read_metrics().end() &&
                        it_read_actual != it_actual->read_metrics().end(); it_read_expected++, it_read_actual++)
        {
            EXPECT_EQ(it_read_expected->read(), it_read_actual->read());
            INTEROP_ASSERT_NEAR(it_read_expected->percent_aligned(), it_read_actual->percent_aligned(), tol);
            INTEROP_ASSERT_NEAR(it_read_expected->percent_phasing()*scale, it_read_actual->percent_phasing(), tol);
            INTEROP_ASSERT_NEAR(it_read_expected->percent_prephasing()*scale, it_read_actual->percent_prephasing(), tol);
        }
    }
}


TEST(tile_metrics_test, test_unique_id_four_digit)
{
    typedef metric_set<tile_metric>::uint_t uint_t;
    typedef metric_set<tile_metric>::id_t id_t;
    std::set<id_t> ids;
    metric_set<tile_metric> metrics;
    for(uint_t lane=1;lane<=8;++lane)
    {
        for(uint_t surface=1;surface<=2;++surface)
        {
            for (uint_t swath = 1; swath <= 4; ++swath)
            {
                for (uint_t tile = 1; tile <= 36; ++tile)
                {
                    tile_metric metric(lane, surface*1000+swath*100+tile, 0, 0, 0, 0);
                    metrics.insert(metric.id(), metric);
                }
            }
        }
    }
    for(size_t i=0;i<metrics.size();i++)
    {
        EXPECT_TRUE(ids.find(metrics[i].id())==ids.end());
        ids.insert(metrics[i].id());
    }
}

TEST(tile_metrics_test, test_unique_id_five_digit)
{
    typedef metric_set<tile_metric>::uint_t uint_t;
    typedef metric_set<tile_metric>::id_t id_t;
    std::set<id_t> ids;
    metric_set<tile_metric> metrics;
    for(uint_t lane=1;lane<=8;++lane)
    {
        for(uint_t surface=1;surface<=2;++surface)
        {
            for (uint_t swath = 1; swath <= 2; ++swath)
            {
                for(uint_t section=1;section <=4;++section)
                {
                    for (uint_t tile = 1; tile <= 36; ++tile)
                    {
                        tile_metric metric(lane, surface * 10000 + swath * 1000 + section*100 + tile, 0, 0, 0, 0);
                        metrics.insert(metric);
                    }
                }
            }
        }
    }
    for(size_t i=0;i<metrics.size();i++)
    {
        EXPECT_TRUE(ids.find(metrics[i].id())==ids.end());
        ids.insert(metrics[i].id());
    }
}

TEST(tile_metrics_test, test_tile_metric_for_lane)
{
    metric_set<tile_metric> metrics;
    tile_metric expected_metric(7, 1114, 2355119.25f,1158081.50f,6470949,3181956,
                tile_metric::read_metric_vector(1,
                tile_metric::read_metric_type(3, 2.61630869f, 0.0797112584f/100, 0.119908921f/100)));
    metrics.insert(expected_metric.id(), expected_metric);
    metric_set<tile_metric>::metric_array_t tile_lane_metrics = metrics.metrics_for_lane(7);
    tile_metric& actual_metric = tile_lane_metrics[0];

    EXPECT_EQ(expected_metric.lane(), actual_metric.lane());
    EXPECT_EQ(expected_metric.tile(), actual_metric.tile());

    const float tol = 1e-7f / 0.01f;
    EXPECT_NEAR(expected_metric.cluster_density(), actual_metric.cluster_density(), tol);
    EXPECT_NEAR(expected_metric.cluster_density_pf(), actual_metric.cluster_density_pf(), tol);
    EXPECT_NEAR(expected_metric.cluster_count(), actual_metric.cluster_count(), tol);
    EXPECT_NEAR(expected_metric.cluster_count_pf(), actual_metric.cluster_count_pf(), tol);
    EXPECT_EQ(expected_metric.read_metrics().size(), actual_metric.read_metrics().size());
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Setup regression test
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
regression_test_metric_generator<tile_metric_set> tile_regression_gen;
INSTANTIATE_TEST_CASE_P(tile_metric_regression_test,
                        tile_metrics_tests,
                        ProxyValuesIn(tile_regression_gen, regression_test_data::instance().files()));


