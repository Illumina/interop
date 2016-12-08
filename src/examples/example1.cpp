/** Example 1: Reading from an InterOp file
 *
 *
 *  @file
 *  @date 11/6/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */

// @ [Reading a binary InterOp file]
#include <iostream>
#include "interop/model/metric_base/metric_set.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/io/metric_file_stream.h"

using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;

int check_args(int argc);

int main(int argc, char** argv)
{
    int ret;
    if((ret = check_args(argc)) != 0) return ret;
    metric_set<tile_metric> tile_metric_set;

    try {
        read_interop(argv[1], tile_metric_set);
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

    std::cout << "Tile metric set contains " << tile_metric_set.size() << " metrics" << std::endl;

    return 0;
}

// @ [Reading a binary InterOp file]

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


