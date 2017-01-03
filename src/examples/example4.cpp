/** Example 4: Calculating simple statistics
 *
 *
 *  @file
 *  @date 3/1/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

// @ [Calculating simple statistics]
#include <iostream>
#include "interop/model/metric_base/metric_set.h"
#include "interop/model/metrics/corrected_intensity_metric.h"
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
    metric_set<corrected_intensity_metric> corrected_intensity_metric_set;

    try {
        read_interop(argv[1], corrected_intensity_metric_set);
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

    // Member function that takes a single argument
    float avg = mean<float>(corrected_intensity_metric_set.begin(),
                            corrected_intensity_metric_set.end(),
                            op::const_member_function(constants::A, &corrected_intensity_metric::percentBase));
    float standard_dev = std::sqrt(variance<float>(corrected_intensity_metric_set.begin(),
                            corrected_intensity_metric_set.end(),
                            op::const_member_function(constants::A, &corrected_intensity_metric::percentBase)));

    // Member function that takes no arguments
    float avg_no_call = mean<float>(corrected_intensity_metric_set.begin(),
                            corrected_intensity_metric_set.end(),
                            op::const_member_function(&corrected_intensity_metric::no_calls));
    std::cout << "Mean of percent base for A " << avg  << std::endl;
    std::cout << "Standard Deviation of percent base for A " << standard_dev  << std::endl;
    std::cout << "Mean of no call" << avg_no_call  << std::endl;

    return 0;
}

// @ [Calculating simple statistics]

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


