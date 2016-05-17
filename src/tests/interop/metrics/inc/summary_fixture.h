/** Run summary test fixture
 *
 *  @file
 *  @date 3/11/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <sstream>
#include "interop/model/summary/index_flowcell_summary.h"
#include "interop/logic/summary/run_summary.h"
#include "interop/logic/summary/index_summary.h"

namespace illumina{ namespace interop { namespace unittest {
    /** Fixture for the run summary*/
    template<class Gen>
    struct summary_fixture
    {
        /** Constructor */
        summary_fixture(std::vector<model::run::read_info> reads = Gen::reads()) :
                expected(Gen::summary()),
                actual(reads.begin(), reads.end(), Gen::lane_count())
        {
            std::vector<std::string> channels;
            channels.push_back("Red");
            channels.push_back("Green");
            model::run::info run_info("XX",
                                      "",
                                      1,
                                      model::run::flowcell_layout(Gen::lane_count()),
                                      channels,
                                      model::run::image_dimensions(),
                                      reads);
            model::metrics::run_metrics metrics(run_info);
            try
            {
                std::istringstream fin(Gen::binary_data());
                illumina::interop::io::read_metrics(fin, metrics.get_set<typename Gen::metric_t>());
            }
            catch (const std::exception &) { }
            metrics.finalize_after_load();
            logic::summary::summarize_run_metrics(metrics, actual);
        }
        /** Expected run summary */
        model::summary::run_summary expected;
        /** Actual run summary derived from hardcoded binary data */
        model::summary::run_summary actual;
    };
}}}
