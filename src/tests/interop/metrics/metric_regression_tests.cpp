/** Regression tests for the run metrics
 *
 *
 *  @file
 *  @date  2/24/17
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#include <gtest/gtest.h>
#include "interop/model/run_metrics.h"
#include "interop/logic/utils/enums.h"
#include "src/tests/interop/inc/abstract_regression_test_generator.h"

using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Setup for tests that use a single, actual run_metrics object */
struct run_metrics_tests : public generic_test_fixture<model::metrics::run_metrics> {};


/** Test if we can compute buffer size for every non-empty metric
 */
TEST_P(run_metrics_tests, compute_buffer_size)
{
    if (skip_test) return;
    ASSERT_TRUE(fixture_test_result);

    for(size_t i=0;i<static_cast<size_t>(constants::MetricCount);++i)
    {
        const constants::metric_group group = static_cast<constants::metric_group>(i);
        if(group == constants::DynamicPhasing) continue; // This does not have a format
        if(actual.is_group_empty(group)) continue;
        EXPECT_NO_THROW(actual.calculate_buffer_size(group)) << constants::to_string(group);
    }
}

/** Read in the actual run metrics, leave expected empty
 */
class regression_test_metric_generator : public abstract_regression_test_generator<model::metrics::run_metrics>
{
    typedef abstract_regression_test_generator<model::metrics::run_metrics> parent_t;
public:
    /** Constructor
     *
     * @param test_dir sub folder where tests are stored
     */
    regression_test_metric_generator(const std::string &test_dir) : parent_t(test_dir)
    {}

    /** Constructor
     *
     * @param run_folder run folder with data
     * @param test_dir sub folder where tests are stored
     */
    regression_test_metric_generator(const std::string &run_folder, const std::string &test_dir) : parent_t(run_folder,
                                                                                                             test_dir)
    {}

protected:
    /** This function does nothing
     *
     * @return true
     */
    bool read_expected(const std::string &, model::metrics::run_metrics &) const
    {
        return true;
    }

    /** Read the actual data from the run folder
     *
     * @param run_folder run folder
     * @param actual actual model data
     * @return true
     */
    bool generate_actual(const std::string &run_folder, model::metrics::run_metrics &actual) const
    {
        read_metrics_safe(actual, run_folder);
        return true;
    }

    /** This function does nothing
     */
    bool write_actual(const std::string &, const model::metrics::run_metrics &) const
    {
        return true;
    }
    /** Create a copy of the current object with the given run folder
     *
     * @param run_folder run folder
     * @return pointer to new copy
     */
    base_t clone(const std::string& run_folder)const
    {
        return new regression_test_metric_generator(run_folder, m_test_dir);
    }

    /** Create a copy of the current object
     *
     * @return pointer to new copy
     */
    base_t clone() const
    {
        return new regression_test_metric_generator(*this);
    }

    /** Write generator info to output stream
     *
     * @param out output stream
     */
    void write(std::ostream &out) const
    {
        out << "regression_test_metric_generator - " << io::basename(m_run_folder);
    }
};

regression_test_metric_generator run_metrics_regression_gen("empty");

INSTANTIATE_TEST_SUITE_P(run_metrics_regression_test,
        run_metrics_tests,
        ProxyValuesIn(run_metrics_regression_gen, regression_test_data::instance().files()));
