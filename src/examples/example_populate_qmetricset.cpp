/** Populate and use a Q-metric Set
 *
 *
 *  @file
 *  @date 4/6/17
 *  @version 1.0
 *  @copyright GNU Public License
 */

#include <iostream>
#include "interop/model/run_metrics.h"

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
    const char* filename = argv[1];


// @ [Reading a single metric set using run_metrics]
    std::vector<unsigned char> valid_to_load(constants::MetricCount, 0);
    valid_to_load[constants::Q]=1;
    run_metrics metrics;
    try
    {
        metrics.read(filename, valid_to_load);
    }
    catch(const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    metric_set<q_metric>& q_metric_set = metrics.get<model::metrics::q_metric>();
// @ [Reading a single metric set using run_metrics]


    if(q_metric_set.size()==0)
    {
        std::cerr << "Empty q-metrics" << std::endl;
        return 1;
    }

    try
    {
// @ [Calculating Total >= Q30]

        q_metric &metric0 = q_metric_set[0];
        std::cout << "Total >= Q30: " << metric0.total_over_qscore(q_metric_set.index_for_q_value(30)) << std::endl;

// @ [Calculating Total >= Q30]

// @ [Calculating Percent >= Q30]

        q_metric &metric1 = q_metric_set[0];
        std::cout << "Percent >= Q30: " << metric1.percent_over_qscore(q_metric_set.index_for_q_value(30)) << std::endl;

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


