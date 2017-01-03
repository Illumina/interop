/** Example: Calculating >= Q30
 *
 *
 *  @file
 *  @date 4/4/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <iostream>
#include "interop/model/metric_base/metric_set.h"
#include "interop/model/metrics/q_metric.h"
#include "interop/io/metric_file_stream.h"
#include "interop/util/statistics.h"

using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;
using namespace illumina::interop::util;
using namespace illumina::interop;

int check_args(int argc);

int main(int argc, char** argv)
{
    int ret;
    if((ret = check_args(argc)) != 0) return ret;
    metric_set<q_metric> q_metric_set;

    try {
        read_interop(argv[1], q_metric_set);
    }
    catch(const incomplete_file_exception&){}// Ignore incomplete files
    catch(const bad_format_exception& ex) // Something catastrophic happened to the file
    {
        std::cerr << "InterOp did not have the expected format: " << ex.what() << std::endl;
        return 1;
    }
    catch(const file_not_found_exception& ex)
    {
        std::cerr << "Count not find InterOp file: " << ex.what() << std::endl;
        return 1;
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    if(q_metric_set.size()==0)
    {
        std::cerr << "Empty q-metrics" << std::endl;
        return 1;
    }

    try
    {
// @ [Calculating Total >= Q30]

        q_metric &metric0 = q_metric_set[0];
        std::cout << "Total >= Q30: " << metric0.total_over_qscore(30, q_metric_set.bins()) << std::endl;

// @ [Calculating Total >= Q30]

// @ [Calculating Percent >= Q30]

        q_metric &metric1 = q_metric_set[0];
        std::cout << "Percent >= Q30: " << metric1.percent_over_qscore(30, q_metric_set.bins()) << std::endl;

// @ [Calculating Percent >= Q30]
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}


int check_args(int argc)
{
    if (argc != 2)
    {
        if (argc < 2) std::cerr << "No run folder" << std::endl;
        else std::cerr << "Too many arguments" << std::endl;
        return 1;
    }
    return 0;
}


